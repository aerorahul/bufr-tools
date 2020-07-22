program test
    implicit none
    real :: x, y, z
    real :: result

    x = 2
    y = 4
    z = 5

    result = sqrt(x**2 + y**2 + z**2)

    print *, result
    
end program test