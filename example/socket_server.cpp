#include <memory>
#include <print>

import net;

using ServerSocket = net::ServerSocket;
using Socket = net::Socket;

int main()
{
    ServerSocket serverSocket;

    serverSocket.setReuseAddress(true);
    serverSocket.setReusePort(true);
    serverSocket.bind(nullptr, 80);
    serverSocket.setKeepAlive(true);
    // serverSocket.setReceiveBufferSize(1024);
    // serverSocket.setSendBufferSize(1024);
    // serverSocket.setReceiveTimeout(15);
    // serverSocket.setSendTimeout(15);
    serverSocket.setBlocking(true);

    serverSocket.listen(8080);

    for (;;) {
        std::unique_ptr<Socket> socket = serverSocket.accept();
        if (!socket)
            continue;
        // std::println("Client fd: {}", socket->fileDescriptor());
        // socket->close();
    }
    std::println("File descriptor {}", serverSocket.fileDescriptor());
    std::println("Is bound {}", serverSocket.isBound());
    std::println("Is closed {}", serverSocket.isClosed());
    serverSocket.close();

    std::println("Is bound {}", serverSocket.isBound());
    std::println("Is closed {}", serverSocket.isClosed());
    return 0;
}