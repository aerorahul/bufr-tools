if(CMAKE_C_COMPILER_ID MATCHES "^(Intel)$")

  set(CMAKE_Fortran_FLAGS "-g -traceback")
  set(CMAKE_Fortran_FLAGS_RELEASE "-O3")
  set(CMAKE_Fortran_FLAGS_DEBUG "-O0")

elseif(CMAKE_C_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")

  set(CMAKE_Fortran_FLAGS "-g -fbacktrace")
  set(CMAKE_Fortran_FLAGS_RELEASE "-O3")
  set(CMAKE_Fortran_FLAGS_DEBUG "-ggdb -Wall -O0")

endif()

# C++ test application

set(exe read_bufr)
set (bufrlib_c_dir ${CMAKE_SOURCE_DIR}/deps/NCEPLIBS-bufr/c_wrapper)

set(srcs 
    main.cpp 
    BufrFile.cpp
    ${bufrlib_c_dir}/bufrlib_c_wrapper.f90)

add_executable(${exe} ${srcs})

target_include_directories(${exe} PRIVATE ${bufrlib_c_dir})

# target_link_libraries(${exe} PUBLIC Eigen3::Eigen)
# find_path(IODA_ENGINES_PATH ioda-engines)

set (IODA_ENGINES_PATH ${CMAKE_PREFIX_PATH})
target_include_directories(${exe} PRIVATE ${IODA_ENGINES_PATH}/ioda-engine/include)

find_library(IODA_ENGINES_LIB ioda-engines ${IODA_ENGINES_PATH}/ioda-engine/lib)
target_link_libraries(${exe} PRIVATE ${IODA_ENGINES_LIB})

target_link_libraries(${exe} PRIVATE bufr::bufr_4_DA)
list(APPEND EXE_TARGETS ${exe})


install(TARGETS ${EXE_TARGETS} RUNTIME
        DESTINATION bin
        COMPONENT utilities)


