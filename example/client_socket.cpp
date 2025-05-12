#include <print>

import net;

using Socket = net::Socket;

int main()
{
    Socket socket("google.com", 80);

    // socket.connect("google.com", 80);
    socket.setKeepAlive(true);
    socket.setReceiveBufferSize(1024);
    socket.setSendBufferSize(1024);
    socket.setReceiveTimeout(5);
    socket.setSendTimeout(5);
    socket.setBlocking(false);

    // for (;;) {
    //     int clientFd = socket.accept();
    //     std::println("Client fd: {}", clientFd);
    // }
    std::println("Is connected {}", socket.isConnected());
    std::println("Is closed {}", socket.isClosed());
    socket.close();

    std::println("Is connected {}", socket.isConnected());
    std::println("Is closed {}", socket.isClosed());
    return 0;
}