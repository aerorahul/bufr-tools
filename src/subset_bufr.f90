program subset_bufr

!$> module load bufr-intel/11.2.0
!$> ifort -o subset_bufr.x subset_bufr.f90 $BUFR_LIB4
!$> subset_bufr.x input_bufr output_bufr obs_fraction

implicit none

character(len=255) :: finput, foutput
character(len=10) :: charbuf
real :: robsfrac

character(len=8) :: subset
integer(4) :: ireadmg, ireadsb, idate

integer :: iobsfrac
integer :: imsg, irep, nimsg, nirep, wrep
integer, parameter :: lunit_in = 10, lunit_out = 20, lunit_table = 30

call getarg(1, finput)
call getarg(2, foutput)
call getarg(3, charbuf)
read(charbuf,*) robsfrac
iobsfrac = int(100.0/robsfrac)

write(6,'(4(A,X))') 'Subsetting observations from', trim(finput), 'into', trim(foutput)
write(6,'(A,F8.5)') 'Percentage of observations extracted: ', robsfrac

! get bufr table from the bufr file (delete at the end)
write(6,'(A)') 'Pull out the bufr table from bufr file into bufrtable.txt'
open(lunit_table, file='bufrtable.txt')
open(lunit_in, file=trim(adjustl(finput)), status='old', form='unformatted')
call openbf(lunit_in, 'IN', lunit_in)
call dxdump(lunit_in, lunit_table)
call closbf(lunit_in)
close(lunit_in)

nimsg = 0
nirep = 0

! Count the number of messages in the bufr file
open(lunit_in,file=trim(adjustl(finput)), status='old', form='unformatted')
call openbf(lunit_in, 'IN', lunit_in)
do while(ireadmg(lunit_in, subset, idate) == 0)
    nimsg = nimsg + 1
    do while(ireadsb(lunit_in) == 0)
        nirep = nirep + 1
    enddo
enddo
call closbf(lunit_in)
close(lunit_in)

imsg = 0
irep = 0
wrep = 0

! Copy a fraction of these messages to the new bufr file
open(lunit_in,file=trim(adjustl(finput)), status='old', form='unformatted')
call openbf(lunit_in, 'IN', lunit_in)
open(lunit_out,file=trim(adjustl(foutput)), form='unformatted')
call openbf(lunit_out, 'OUT', lunit_table)
call cmpmsg('Y') ! Compress all future written messages
call datelen(10) ! use dates as YYYYMMDDHH
do while(ireadmg(lunit_in, subset, idate) == 0)
    imsg = imsg + 1
    do while(ireadsb(lunit_in) == 0)
        irep = irep + 1
        if (mod(irep, iobsfrac) == 0) then
            wrep = wrep + 1
            write(6,'(A,X,I10,X,A)') 'writing report', irep, 'to file'
            call openmb(lunit_out, subset, idate)
            call ufbcpy(lunit_in, lunit_out)
            call writsb(lunit_out)
        endif
    enddo
    call closmg(lunit_out) ! keep the reports in the same message as original
enddo
call closbf(lunit_in)
call closbf(lunit_out)
close(lunit_in)
close(lunit_out)

! close and delete the bufr table txt file
close(lunit_table, status='delete')

write(6,'(A,2(X,I10))') 'Total number of messages (reports) read:', nimsg, nirep
write(6,'(A,2(X,I10))') 'Total number of messages (reports) written:', imsg, wrep

stop

end program subset_bufr
