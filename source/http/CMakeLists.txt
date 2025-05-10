add_library(http)

target_sources(http
    PUBLIC FILE_SET CXX_MODULES FILES
    http.cppm
    server.cppm
    PRIVATE
    server.cpp
)