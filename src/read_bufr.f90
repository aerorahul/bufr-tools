program read_bufr
  use bufr_tools
  implicit none

  character(len=255) :: finput
  character(len=10) :: charbuf
  integer :: nimsg, nirep
  integer :: nchanl

  type :: IODAData
    ! private
    ! public
    integer :: nlocs
    integer :: nchanl
    integer(i_single), dimension(:), allocatable :: satid
    integer(i_single), dimension(:), allocatable :: ifov
    integer(i_single), dimension(:,:), allocatable :: datetime
    real(r_double), dimension(:), allocatable :: olat
    real(r_double), dimension(:), allocatable :: olon
    real(r_double), dimension(:), allocatable :: terrain
    real(r_double), dimension(:), allocatable :: lza
    real(r_double), dimension(:), allocatable :: sza
    real(r_double), dimension(:), allocatable :: sat_aziang
    real(r_double), dimension(:), allocatable :: sol_aziang
    real(r_double), dimension(:,:), allocatable :: raddata
  end type IODAData

  type(BufrData), dimension(:), allocatable :: bdata
  type(IODAData) :: idata

  call getarg(1, finput)
  call getarg(2, charbuf)
  read(charbuf,*) nchanl

  write(6, '((A,X,I3,X,A,A))') 'Begin reading', nchanl, 'channels of bufr data from', trim(adjustl(finput))

  call dump_bufrtable_to_disk(finput)

  call count_messages(finput, nimsg, nirep)

  allocate(bdata(nirep))
  call read_bufrdata(finput, nchanl, nimsg, nirep, bdata)

  call toIODA(bdata, idata)

  call dump_bufrdata_to_disk(bdata)
  call dump_iodadata_to_disk(idata)

  deallocate(bdata)

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
      bdata(irep)%dtime(1) = bfr1bhdr(3) ! year
      bdata(irep)%dtime(2) = bfr1bhdr(4) ! month
      bdata(irep)%dtime(3) = bfr1bhdr(5) ! day
      bdata(irep)%dtime(4) = bfr1bhdr(6) ! hour
      bdata(irep)%dtime(5) = bfr1bhdr(7) ! minute
      bdata(irep)%dtime(6) = bfr1bhdr(8) ! second

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

! "convert" bdata structure into IODA data structure
subroutine toIODA(bdata, idata)

  type(BufrData), dimension(:), intent(in) :: bdata
  type(IODAData), intent(out) :: idata

  integer :: iloc

  idata%nlocs = size(bdata, 1)
  idata%nchanl = bdata(1)%nchanl

  allocate(idata%satid(idata%nlocs))
  allocate(idata%ifov(idata%nlocs))
  allocate(idata%datetime(idata%nlocs, 6))
  allocate(idata%olat(idata%nlocs))
  allocate(idata%olon(idata%nlocs))
  allocate(idata%terrain(idata%nlocs))
  allocate(idata%lza(idata%nlocs))
  allocate(idata%sza(idata%nlocs))
  allocate(idata%sat_aziang(idata%nlocs))
  allocate(idata%sol_aziang(idata%nlocs))
  allocate(idata%raddata(idata%nlocs, idata%nchanl))

  idata%satid(:) = bdata(:)%satid
  idata%ifov(:) = bdata(:)%ifov
  !idata%datetime
  idata%olat(:) = bdata(:)%olat
  idata%olon(:) = bdata(:)%olon
  idata%terrain(:) = bdata(:)%terrain
  idata%lza(:) = bdata(:)%lza
  idata%sza(:) = bdata(:)%sza
  idata%sat_aziang(:) = bdata(:)%sat_aziang
  idata%sol_aziang(:) = bdata(:)%sol_aziang
  !idata%raddata

  do iloc = 1, idata%nlocs
    idata%datetime(iloc, :) = bdata(iloc)%dtime
    idata%raddata(iloc, :) = bdata(iloc)%bufrdata
  enddo

end subroutine toIODA

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
                                               'hr =', bdata(irep)%dtime(4), &
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

! dump IODA data to disk
subroutine dump_iodadata_to_disk(idata)

  type(IODAData), intent(in) :: idata

  integer :: nchanl

  integer :: iloc
  integer :: ichanl
  integer :: iret

  integer, parameter :: lundt = 40

  nchanl = idata%nchanl

  iret = system('rm -f iodadata.txt')

  write(6, '(A)') 'Writing IODA data into iodadata.txt'
  open(lundt, file='iodadata.txt', status='new')

  do iloc = 1, idata%nlocs

    write(lundt, '(1(A,X,I6))') 'Report =', iloc

    write(lundt, '(2(A,X,I4,X))') 'satid =', idata%satid(iloc), &
                                    'fov =', idata%ifov(iloc)

    write(lundt, '((A,X,I4,X),5(A,X,I2,X))') 'year =', idata%datetime(iloc, 1), &
                                              'mth =', idata%datetime(iloc, 2), &
                                              'day =', idata%datetime(iloc, 3), &
                                               'hr =', idata%datetime(iloc, 4), &
                                              'min =', idata%datetime(iloc, 5), &
                                              'sec =', idata%datetime(iloc, 6)

    write(lundt, '(2(A,X,F6.2,X))') 'lat =', idata%olat(iloc), &
                                    'lon =', idata%olon(iloc)

    write(lundt, '(1(A,X,F6.2,X))') 'terrain =', idata%terrain(iloc)

    write(lundt, '(4(A,X,F6.2,X))')  'local zenith =', idata%lza(iloc),        &
                                     'solar zenith =', idata%sza(iloc),        &
                                    'local azimuth =', idata%sat_aziang(iloc), &
                                    'solar azimuth =', idata%sol_aziang(iloc)

    do ichanl = 1, idata%nchanl
        write(lundt, '(A,X,I2,X,A,X,F6.2)') 'channel =', ichanl, &
                                                 'Tb =', idata%raddata(iloc,ichanl)
    enddo

    write(lundt, '(A)') ''

  enddo

  close(lundt)

end subroutine dump_iodadata_to_disk

end program read_bufr
