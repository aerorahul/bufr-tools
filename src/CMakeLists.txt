if(CMAKE_Fortran_COMPILER_ID MATCHES "^(Intel)$")

  set(CMAKE_Fortran_FLAGS "-g -traceback")
  set(CMAKE_Fortran_FLAGS_RELEASE "-O3")
  set(CMAKE_Fortran_FLAGS_DEBUG "-O0")

elseif(CMAKE_Fortran_COMPILER_ID MATCHES "^(GNU)$")

  set(CMAKE_Fortran_FLAGS "-g -fbacktrace")
  set(CMAKE_Fortran_FLAGS_RELEASE "-O3")
  set(CMAKE_Fortran_FLAGS_DEBUG "-ggdb -Wall -O0")

endif()

set(exe iodabufr-testdata.x)
set(srcs iodabufr-testdata.f90)
add_executable(${exe} ${srcs})
target_link_libraries(${exe} PRIVATE bufr::bufr_4_DA)
list(APPEND EXE_TARGETS ${exe})

install(TARGETS ${EXE_TARGETS} RUNTIME
        DESTINATION bin
        COMPONENT utilities)
