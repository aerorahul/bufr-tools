
module bufr_tools
  use iso_c_binding

  integer, parameter :: i_single = 4, r_single = 4, r_double = 8
  real(r_double), parameter :: pi = acos(-1.0_r_double)
  real(r_double), parameter :: deg2rad = pi / 180.0_r_double

  integer, parameter :: sizeof_bufrdata = 104

  type, bind(c) :: BufrData
    ! private
    ! public
    integer(c_int) :: nchanl
    integer(c_int) :: satid
    integer(c_int) :: ifov
    integer(c_int), dimension(6) :: dtime
    real(c_double) :: olat, olon
    real(c_double) :: terrain
    real(c_double) :: lza, sza
    real(c_double) :: sat_aziang, sol_aziang
    type(c_ptr), dimension(1) :: bufr_data
  end type BufrData

  contains

    function c_to_f_string(s) result(str)
      use iso_c_binding
      implicit none

      character(kind=c_char,len=1), intent(in) :: s(*)
      character(len=:), allocatable :: str
      integer nchars

      nchars = 1
      do while (s(nchars) .ne. c_null_char)
        nchars = nchars + 1
      end do

      nchars = nchars - 1

      allocate(character(len=nchars) :: str)

      str = transfer(s(1:nchars), str)

    end function c_to_f_string

end module


! count the number of messages and records in each message in the bufr file
subroutine count_messages(finput, nimsg, nirep)
  use bufr_tools

  character(len=*), intent(in) :: finput
  integer, intent(out) :: nimsg, nirep

  integer :: lunin = 10
  character(len=8) :: subset
  integer(kind=i_single) :: ireadmg, ireadsb, idate

  nimsg = 0
  nirep = 0

  ! Count the number of messages in the bufr file
  open(lunin, file=trim(adjustl(finput)), status='old', form='unformatted')
  call openbf(lunin, 'IN', lunin)

  do while(ireadmg(lunin, subset, idate) == 0)
    nimsg = nimsg + 1
    do while(ireadsb(lunin) == 0)
        nirep = nirep + 1
    enddo
  enddo
  call closbf(lunin)
  close(lunin)

  write(6, '(2(A,X))') trim(adjustl(finput)), 'contains:'
  write(6, '(2(A,X,I10,X))') 'total no. of messages =', nimsg, &
                              'total no. or reports =', nirep

end subroutine count_messages


subroutine print_bufrdata(num_msgs, bdata_c_ptr) bind(C, name="print_bufrdata")
  use iso_c_binding
  use bufr_tools
  implicit none

  integer(c_int), value, intent(in) :: num_msgs
  type(c_ptr), dimension(1), intent(inout) :: bdata_c_ptr(*)

  type(BufrData), pointer :: bdata_ptr(:)
  real(c_double), pointer :: bufr_data_ptr(:)
  integer :: msg_idx, data_idx

  call c_f_pointer(bdata_c_ptr(1), bdata_ptr, [sizeof_bufrdata])
  
  do msg_idx = 1, num_msgs
    call c_f_pointer(bdata_ptr(msg_idx)%bufr_data(1), bufr_data_ptr, [r_double])

    do data_idx = 1, bdata_ptr(msg_idx)%nchanl
      write(*, '(F7.2)', advance="NO") bufr_data_ptr(data_idx)
    end do
    print *, ''
  end do
end subroutine print_bufrdata


! read data from a bufr file
subroutine read_bufrdata(c_finput, nchanl, bdata_c_ptr) bind(C, name='read_bufrdata')
  use iso_c_binding 
  use bufr_tools
  implicit none

  character(kind=c_char, len=1), intent(in) :: c_finput
  integer(c_int), value, intent(in) :: nchanl
  type(c_ptr), intent(inout) :: bdata_c_ptr(*)

  !Declare local variables that represent c types
  character(len=255) :: finput
  type(BufrData), pointer :: bdata_ptr(:)
  real(c_double), pointer :: bdata_databufr_ptr(:)

  integer :: lunin = 10
  character(len=8) :: subset
  integer(i_single) :: ireadmg, ireadsb, idate
  integer :: iret
  integer :: irep
  character(len=80) :: hdr1b
  character(len=80) :: hdr2b
  integer :: n1bhdr = 13
  integer :: n2bhdr = 4
  real(r_double), allocatable, dimension(:) :: bfr1bhdr, bfr2bhdr
  real(r_double) :: dlat, dlon, dlat_deg, dlon_deg, dlat_rad, dlon_rad


  !initialize local variables from C types
  finput = c_to_f_string(c_finput)
  print *, "input file: ", finput
  call c_f_pointer(bdata_c_ptr(1), bdata_ptr, [sizeof_bufrdata])

  !initialize other variables
  hdr1b ='SAID FOVN YEAR MNTH DAYS HOUR MINU SECO CLAT CLON CLATH CLONH HOLS'
  hdr2b ='SAZA SOZA BEARAZ SOLAZI'

  allocate(bfr1bhdr(n1bhdr))
  allocate(bfr2bhdr(n2bhdr))

  open(lunin, file=trim(adjustl(finput)), status='old', form='unformatted')
  call openbf(lunin, 'IN', lunin)

  irep = 0
  ireadmg_loop: do while(ireadmg(lunin, subset, idate) >= 0)
    ireadsb_loop: do while(ireadsb(lunin) == 0)
      irep = irep + 1

      bdata_ptr(irep)%nchanl = nchanl

      ! Read header record
      call ufbint(lunin, bfr1bhdr, n1bhdr, 1, iret, hdr1b)

      ! Extract satellite id
      bdata_ptr(irep)%satid = nint(bfr1bhdr(1))

      ! Extract field of view
      bdata_ptr(irep)%ifov = nint(bfr1bhdr(2))

      ! Extract date information
      bdata_ptr(irep)%dtime(1) = nint(bfr1bhdr(3)) ! year
      bdata_ptr(irep)%dtime(2) = nint(bfr1bhdr(4)) ! month
      bdata_ptr(irep)%dtime(3) = nint(bfr1bhdr(5)) ! day
      bdata_ptr(irep)%dtime(4) = nint(bfr1bhdr(6)) ! hour
      bdata_ptr(irep)%dtime(5) = nint(bfr1bhdr(7)) ! minute
      bdata_ptr(irep)%dtime(6) = nint(bfr1bhdr(8)) ! second

      ! Extract observation location (latitude and longitude)
      if (abs(bfr1bhdr(11)) <= 90.0_r_double .and. abs(bfr1bhdr(12)) <= 360.0_r_double) then
        dlat = bfr1bhdr(11)
        dlon = bfr1bhdr(12)
      elseif (abs(bfr1bhdr(9)) <= 90.0_r_double .and. abs(bfr1bhdr(10)) <= 360.0_r_double) then
        dlat = bfr1bhdr(9)
        dlon = bfr1bhdr(10)
      endif
      if (dlon < 0.0_r_double)    dlon = dlon + 360.0_r_double
      if (dlon >= 360.0_r_double) dlon = dlon - 360.0_r_double
      dlat_deg = dlat           ; dlon_deg = dlon
      dlat_rad = dlat * deg2rad ; dlon_rad = dlon * deg2rad

      bdata_ptr(irep)%olat = dlat_deg
      bdata_ptr(irep)%olon = dlon_deg

      ! Extract terrain information
      bdata_ptr(irep)%terrain = 0.01_r_double * abs(bfr1bhdr(13))

      call ufbint(lunin, bfr2bhdr, n2bhdr, 1, iret, hdr2b)

      ! Extract local zenith angle
      bdata_ptr(irep)%lza = bfr2bhdr(1)

      ! Extract solar zenith angle
      bdata_ptr(irep)%sza = bfr2bhdr(2)

      ! Extract local azimuth angle
      bdata_ptr(irep)%sat_aziang = bfr2bhdr(3)

      ! Extract solar azimuth angle
      bdata_ptr(irep)%sol_aziang = bfr2bhdr(4)

      call c_f_pointer(bdata_ptr(irep)%bufr_data(1), bdata_databufr_ptr, [r_double])

      ! Read data record
      ! TMBR is actually the antenna temperature for most microwave sounders
      call ufbrep(lunin, bdata_databufr_ptr, 1, bdata_ptr(irep)%nchanl, iret, 'TMBR')

    enddo ireadsb_loop
  enddo ireadmg_loop

  call closbf(lunin)
  close(lunin)

  deallocate(bfr1bhdr, bfr2bhdr)

end subroutine read_bufrdata

