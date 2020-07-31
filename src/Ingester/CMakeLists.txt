
list(APPEND INGESTER_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/IngesterData.h
        ${CMAKE_CURRENT_SOURCE_DIR}/IngesterData.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/BufrParser/BufrParser.h
        ${CMAKE_CURRENT_SOURCE_DIR}/BufrParser/BufrParser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/BufrParser/BufrDescription.h
        ${CMAKE_CURRENT_SOURCE_DIR}/BufrParser/BufrDescription.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/BufrParser/MnemonicSet.h
        ${CMAKE_CURRENT_SOURCE_DIR}/BufrParser/MnemonicSet.cpp
        )

set(INGESTER_SRCS ${INGESTER_SRCS} PARENT_SCOPE)
