program read_bufr

  implicit none

  integer, parameter :: i_single = 4, r_single = 4, r_double = 8
  real(r_double), parameter :: pi = acos(-1.0_r_double)
  real(r_double), parameter :: deg2rad = pi / 180.0_r_double

  character(len=255) :: finput
  character(len=10) :: charbuf
  integer :: nimsg, nirep
  integer :: nchanl

  type :: BufrData
    ! private
    ! public
    integer :: nchanl
    integer(i_single) :: satid
    integer(i_single) :: ifov
    integer(i_single), dimension(6) :: dtime
    real(r_double) :: olat, olon
    real(r_double) :: terrain
    real(r_double) :: lza, sza
    real(r_double) :: sat_aziang, sol_aziang
    real(r_double), dimension(:), allocatable :: bufrdata
  end type BufrData

  type(BufrData), dimension(:), allocatable :: bdata

  call getarg(1, finput)
  call getarg(2, charbuf)
  read(charbuf,*) nchanl

  write(6, '((A,X,I3,X,A,A))') 'Begin reading', nchanl, 'channels of bufr data from', trim(adjustl(finput))

  call dump_bufrtable_to_disk(finput)

  call count_messages(finput, nimsg, nirep)

  allocate(bdata(nirep))
  call read_bufrdata(finput, nchanl, nimsg, nirep, bdata)

  call dump_bufrdata_to_disk(bdata)

  write(6, '(2(A,X))') 'Finished reading bufr data from', trim(adjustl(finput))

  stop

contains

! extract bufr table from the bufr file and dump to disk
subroutine dump_bufrtable_to_disk(finput)

  character(len=255), intent(in) :: finput

  character(len=255) :: ftable = 'bufrtable.txt'
  integer :: lunin = 10, luntb = 20
  integer :: iret

  iret = system('rm -f bufrtable.txt')

  write(6, '(A)') 'Extracting bufr table into bufrtable.txt'
  open(luntb, file='bufrtable.txt', status='new')
  open(lunin, file=trim(adjustl(finput)), status='old', form='unformatted')
  call openbf(lunin, 'IN', lunin)
  call dxdump(lunin, luntb)
  call closbf(lunin)
  close(lunin)

end subroutine dump_bufrtable_to_disk

! count the number of messages and records in each message in the bufr file
subroutine count_messages(finput, nimsg, nirep)

  character(len=255), intent(in) :: finput
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

! read data from a bufr file
subroutine read_bufrdata(finput, nchanl, nimsg, nirep, bdata)

  character(len=255), intent(in) :: finput
  integer, intent(in) :: nchanl
  integer, intent(in) :: nimsg, nirep
  type(BufrData), dimension(nirep), intent(out) :: bdata

  integer :: lunin = 10
  character(len=8) :: subset
  integer(kind=i_single) :: ireadmg, ireadsb, idate
  integer :: iret

  integer :: ichanl
  integer :: irep

  character(len=80) :: hdr1b
  character(len=80) :: hdr2b
  integer :: n1bhdr = 13
  integer :: n2bhdr = 4
  real(r_double), allocatable, dimension(:) :: bfr1bhdr, bfr2bhdr

  integer(i_single), dimension(6) :: idate6
  real(r_double) :: dlat, dlon, dlat_deg, dlon_deg, dlat_rad, dlon_rad

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

      bdata(irep)%nchanl = nchanl

      ! Read header record
      call ufbint(lunin, bfr1bhdr, n1bhdr, 1, iret, hdr1b)

      ! Extract satellite id
      bdata(irep)%satid = nint(bfr1bhdr(1))

      ! Extract field of view
      bdata(irep)%ifov = nint(bfr1bhdr(2))

      ! Extract date information
      idate6(1) = bfr1bhdr(3) ! year
      idate6(2) = bfr1bhdr(4) ! month
      idate6(3) = bfr1bhdr(5) ! day
      idate6(4) = bfr1bhdr(6) ! hour
      idate6(5) = bfr1bhdr(7) ! minute
      idate6(6) = bfr1bhdr(8) ! second

      bdata(irep)%dtime(:) = idate6

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

      bdata(irep)%olat = dlat_deg
      bdata(irep)%olon = dlon_deg

      ! Extract terrain information
      bdata(irep)%terrain = 0.01_r_double * abs(bfr1bhdr(13))

      call ufbint(lunin, bfr2bhdr, n2bhdr, 1, iret, hdr2b)

      ! Extract local zenith angle
      bdata(irep)%lza = bfr2bhdr(1)

      ! Extract solar zenith angle
      bdata(irep)%sza = bfr2bhdr(2)

      ! Extract local azimuth angle
      bdata(irep)%sat_aziang = bfr2bhdr(3)

      ! Extract solar azimuth angle
      bdata(irep)%sol_aziang = bfr2bhdr(4)

      allocate(bdata(irep)%bufrdata(nchanl))

      ! Read data record
      ! TMBR is actually the antenna temperature for most microwave sounders
      call ufbrep(lunin, bdata(irep)%bufrdata, 1, bdata(irep)%nchanl, iret, 'TMBR')

    enddo ireadsb_loop
  enddo ireadmg_loop

  call closbf(lunin)
  close(lunin)

  deallocate(bfr1bhdr, bfr2bhdr)

end subroutine read_bufrdata

! dump bufr data to disk
subroutine dump_bufrdata_to_disk(bdata)

  type(BufrData), dimension(:), intent(in) :: bdata

  integer, parameter :: lundt = 30

  integer :: ichanl
  integer :: irep
  integer :: iret

  iret = system('rm -f bufrdata.txt')

  write(6, '(A)') 'Writing bufr data into bufrdata.txt'
  open(lundt, file='bufrdata.txt', status='new')

  do irep = 1, size(bdata, 1)

    write(lundt, '(1(A,X,I6))') 'Report =', irep

    write(lundt, '(2(A,X,I4,X))') 'satid =', bdata(irep)%satid, &
                                    'fov =', bdata(irep)%ifov

    write(lundt, '((A,X,I4,X),5(A,X,I2,X))') 'year =', bdata(irep)%dtime(1), &
                                              'mth =', bdata(irep)%dtime(2), &
                                              'day =', bdata(irep)%dtime(3), &
                                              ' hr =', bdata(irep)%dtime(4), &
                                              'min =', bdata(irep)%dtime(5), &
                                              'sec =', bdata(irep)%dtime(6)

    write(lundt, '(2(A,X,F6.2,X))') 'lat =', bdata(irep)%olat, &
                                    'lon =', bdata(irep)%olon

    write(lundt, '(1(A,X,F6.2,X))') 'terrain =', bdata(irep)%terrain

    write(lundt, '(4(A,X,F6.2,X))')  'local zenith =', bdata(irep)%lza,        &
                                     'solar zenith =', bdata(irep)%sza,        &
                                    'local azimuth =', bdata(irep)%sat_aziang, &
                                    'solar azimuth =', bdata(irep)%sol_aziang

    do ichanl = 1, bdata(irep)%nchanl
        write(lundt, '(A,X,I2,X,A,X,F6.2)') 'channel =', ichanl, &
                                                 'Tb =', bdata(irep)%bufrdata(ichanl)
    enddo

    write(lundt, '(A)') ''

  enddo

  close(lundt)

end subroutine dump_bufrdata_to_disk

end program read_bufr
