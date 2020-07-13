if(CMAKE_C_COMPILER_ID MATCHES "^(Intel)$")

  set(CMAKE_Fortran_FLAGS "-g -traceback")
  set(CMAKE_Fortran_FLAGS_RELEASE "-O3")
  set(CMAKE_Fortran_FLAGS_DEBUG "-O0")

elseif(CMAKE_C_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")

  set(CMAKE_Fortran_FLAGS "-g -fbacktrace")
  set(CMAKE_Fortran_FLAGS_RELEASE "-O3")
  set(CMAKE_Fortran_FLAGS_DEBUG "-ggdb -Wall -O0")

endif()

# Subsetting application
set(exe subset_bufr.x)
set(srcs subset_bufr.f90)
add_executable(${exe} ${srcs})
list(APPEND EXE_TARGETS ${exe})

# Read application
set(exe read_bufr.x)
set(srcs read_bufr.f90)
add_executable(${exe} ${srcs})
list(APPEND EXE_TARGETS ${exe})

# C++ test application
add_definitions(-DFORTRAN_UNDERSCORE)

set(exe read_bufr_c.exe)
set(srcs main.cpp read_bufr.cpp read_bufr_lib.f90)
add_executable(${exe} ${srcs})
list(APPEND EXE_TARGETS ${exe})


foreach(_tgt IN LISTS EXE_TARGETS)
  target_link_libraries(${_tgt} PRIVATE bufr::bufr_4_DA)
endforeach()

install(TARGETS ${EXE_TARGETS} RUNTIME
        DESTINATION bin
        COMPONENT utilities)
