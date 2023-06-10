set(DIAGNOSTIC_SRCS
    diagnostic.cpp
    diagnostic_base.cpp
    )

add_library(diagnostic STATIC ${DIAGNOSTIC_SRCS})
target_include_directories (diagnostic PUBLIC .)
target_link_libraries(diagnostic microhal-crc)





