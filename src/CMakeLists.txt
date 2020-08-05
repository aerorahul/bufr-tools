
if(CMAKE_C_COMPILER_ID MATCHES "^(Intel)$")

  set(CMAKE_Fortran_FLAGS "-g -traceback")
  set(CMAKE_Fortran_FLAGS_RELEASE "-O3")
  set(CMAKE_Fortran_FLAGS_DEBUG "-O0")

elseif(CMAKE_C_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")

  set(CMAKE_Fortran_FLAGS "-g -fbacktrace")
  set(CMAKE_Fortran_FLAGS_RELEASE "-O3")
  set(CMAKE_Fortran_FLAGS_DEBUG "-ggdb -Wall -O0")
  set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

set(INGESTER_TEST "ingester_test")

LIST(APPEND exes
        ${INGESTER_TEST})

add_subdirectory(Ingester)
include_directories(Ingester)

add_executable(${INGESTER_TEST} test_ingester.cpp ${INGESTER_SRCS})

foreach(exe ${exes})
  target_link_libraries(${exe} PUBLIC Eigen3::Eigen)
  target_link_libraries(${exe} PRIVATE bufr::bufr_4_DA)
  target_link_libraries(${exe} PRIVATE ioda::ioda-engines)
endforeach()

#install(TARGETS ${EXE_TARGETS} RUNTIME
#        DESTINATION bin
#        COMPONENT utilities)


