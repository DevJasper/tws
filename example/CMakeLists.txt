add_executable(app socket_server.cpp)

target_link_libraries(app http net)