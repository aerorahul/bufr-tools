set(exe subset_bufr.x)
set(srcs subset_bufr.f90)
set(CMAKE_Fortran_FLAGS "-g -fbacktrace")

add_executable(${exe} ${srcs})
target_link_libraries(${exe} PRIVATE bufr::bufr_4_DA)
