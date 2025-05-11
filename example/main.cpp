#include <liburing.h>
#include <print>

import net;

using ServerSocket = net::ServerSocket;

int main()
{
    ServerSocket serverSocket;

    serverSocket.setBlocking(true);
    serverSocket.setReuseAddress(true);
    // serverSocket.setReceiveBufferSize(1024);
    // serverSocket.setTimeout(5);
    serverSocket.bind(nullptr, 80);
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