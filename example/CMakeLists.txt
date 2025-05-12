add_executable(app client_socket.cpp)

target_link_libraries(app http net)