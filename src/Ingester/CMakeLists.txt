list(APPEND INGESTER_SRCS
  IngesterData.h
  IngesterData.cpp
  BufrParser/BufrParser.h
  BufrParser/BufrParser.cpp
  BufrParser/BufrDescription.h
  BufrParser/BufrDescription.cpp
  BufrParser/MnemonicSet.h
  BufrParser/MnemonicSet.cpp
  IodaEncoder/IodaEncoder.cpp
  IodaEncoder/IodaEncoder.h
  oops/IntSetParser.cpp
  oops/IntSetParser.h
# IodaEncoder/ObsGroupDescription.cpp
  # IodaEncoder/ObsGroupDescription.h
        BufrParser/BufrAccumulator.cpp
        BufrParser/BufrAccumulator.h
)

add_library(ingester ${INGESTER_SRCS} BufrParser/BufrOperations/BufrOperation.cpp BufrParser/BufrOperations/BufrOperation.h BufrParser/BufrOperations/BufrIntOperation.cpp BufrParser/BufrOperations/BufrIntOperation.h BufrParser/BufrOperations/BufrRepOperation.cpp BufrParser/BufrOperations/BufrRepOperation.h)
target_link_libraries(ingester PUBLIC Eigen3::Eigen)
target_link_libraries(ingester PUBLIC eckit)
target_link_libraries(ingester PRIVATE bufr::bufr_4_DA)


target_include_directories(ingester PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_include_directories(ingester INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>)
