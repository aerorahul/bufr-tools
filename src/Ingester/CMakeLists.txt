
list(APPEND INGESTER_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/IngesterData.h
        ${CMAKE_CURRENT_SOURCE_DIR}/IngesterData.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/BufrParser/BufrParser.h
        ${CMAKE_CURRENT_SOURCE_DIR}/BufrParser/BufrParser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/BufrParser/BufrDescription.h
        ${CMAKE_CURRENT_SOURCE_DIR}/BufrParser/BufrDescription.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/BufrParser/MnemonicSet.h
        ${CMAKE_CURRENT_SOURCE_DIR}/BufrParser/MnemonicSet.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/IodaEncoder/IodaEncoder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/IodaEncoder/IodaEncoder.h
        ${CMAKE_CURRENT_SOURCE_DIR}/IodaEncoder/ObsGroupDescription.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/IodaEncoder/ObsGroupDescription.h
        ${CMAKE_CURRENT_SOURCE_DIR}/IngesterAccumulator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/IngesterAccumulator.h
        )

set(INGESTER_SRCS ${INGESTER_SRCS} PARENT_SCOPE)
