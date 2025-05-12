#include <print>

import net;

using ServerSocket = net::ServerSocket;

int main()
{
    ServerSocket serverSocket;

    serverSocket.setReuseAddress(true);
    serverSocket.setReusePort(true);
    serverSocket.bind(nullptr, 80);
    serverSocket.setKeepAlive(true);
    serverSocket.setReceiveBufferSize(1024);
    serverSocket.setSendBufferSize(1024);
    serverSocket.setReceiveTimeout(5);
    serverSocket.setSendTimeout(5);
    serverSocket.setBlocking(false);

    serverSocket.listen(50);

    // for (;;) {
    //     int clientFd = serverSocket.accept();
    //     std::println("Client fd: {}", clientFd);
    // }
    std::println("Is bound {}", serverSocket.isBound());
    std::println("Is closed {}", serverSocket.isClosed());
    serverSocket.close();

    std::println("Is bound {}", serverSocket.isBound());
    std::println("Is closed {}", serverSocket.isClosed());
    return 0;
}