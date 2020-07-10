        SUBROUTINE open_fortran_file(unit, filepath, action)
            use iso_c_binding
            integer          :: unit
            character(len=*) :: filepath
            character(len=*) :: action

            OPEN(unit, FILE=filepath)
        END SUBROUTINE open_fortran_file
        
        SUBROUTINE close_fortran_file(unit)
            use iso_c_binding
            integer          :: unit

            CLOSE(UNIT=unit)
        END SUBROUTINE close_fortran_file
