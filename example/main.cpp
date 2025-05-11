#include <liburing.h>
#include <print>

import net;

using ServerSocket = net::ServerSocket;

int main()
{
    ServerSocket serverSocket;
    serverSocket.setBlocking(true);
    serverSocket.setReuseAddress(true);
    serverSocket.setReceiveBufferSize(1024);
    serverSocket.setTimeout(5);
    serverSocket.bind("localhost", 80);
    serverSocket.listen(50);

    for (;;) {
    }

    return 0;
}