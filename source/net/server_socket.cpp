module;
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <print>
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

module net.server_socket;

using namespace net;

ServerSocket::ServerSocket(const int _port, const int _backlog)
    : ServerSocket(nullptr, _port, _backlog)
{
}

ServerSocket::ServerSocket(const char* _address, const int _port, const int _backlog)
{
    bind(_address, _port);
    listen(_backlog);
}

ServerSocket::~ServerSocket()
{
    close();
}

void ServerSocket::bind(const int _port) noexcept
{
    bind(nullptr, _port);
}

void ServerSocket::bind(const char* _address, const int _port) noexcept
{
    address = _address;
    port = _port;
    struct addrinfo hints, *result, *res;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

    int status = ::getaddrinfo(address, std::to_string(port).c_str(), &hints, &result);

    if (status != 0) {
        std::println("[ERROR] Failed to get address info: {}", ::gai_strerror(status));
        std::exit(EXIT_FAILURE);
    }

    for (res = result; res != nullptr; res = res->ai_next) {

        if (!blocking)
            res->ai_socktype |= SOCK_NONBLOCK;

        fd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (fd == -1)
            continue;

        if (socketOptions.reuseAddress) {
            int yes = 1;
            if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
                ::perror("[ERROR] Failed to make socket address reusable");
                ::close(fd);
                continue;
            }
        }

        if (blocking && socketOptions.timeout > 0) {
            struct timeval timeout;
            timeout.tv_sec = socketOptions.timeout; // seconds
            timeout.tv_usec = 0;
            std::println("[INFO] Socket timeout: {} seconds", socketOptions.timeout);
            if (::setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &timeout, sizeof(timeout)) == -1) {
                ::perror("[ERROR] Failed to set socket receive timeout");
                ::close(fd);
                continue;
            }
        }

        if (socketOptions.receiveBufferSize > 0) {
            int receiveBufferSize = socketOptions.receiveBufferSize;
            std::println("[INFO] Socket receive buffer size: {}", receiveBufferSize);
            if (::setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &receiveBufferSize, sizeof(receiveBufferSize)) == -1) {
                ::perror("[ERROR] Failed to set socket receive buffer size");
                ::close(fd);
                continue;
            }
        }

        if (::bind(fd, res->ai_addr, res->ai_addrlen) == 0) {
            bound = true;
            break;
        }

        ::close(fd);
    }

    ::freeaddrinfo(result);

    if (res == nullptr) {
        std::println("[ERROR] Failed to bind socket");
        std::exit(EXIT_FAILURE);
    }

    std::println("[SUCCESS] Socket connected to port {}", port);
}

void ServerSocket::listen(const int _backlog) noexcept
{
    std::println("[INFO] Listening backlog {}", _backlog);

    if (::listen(fd, _backlog) != 0) {
        ::perror("[ERROR] Socket failed to listen on port");
        std::exit(EXIT_FAILURE);
    }

    std::println("[SUCCESS] Socket listening on port {}", port);
}

int ServerSocket::accept() noexcept
{
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(address);

    // int clientFd = ::accept4(fd, (struct sockaddr*)&clientAddress, &clientAddressLen, SOCK_NONBLOCK | SOCK_CLOEXEC);

    int clientFd = ::accept4(fd, (struct sockaddr*)&clientAddress, &clientAddressLen, SOCK_CLOEXEC);
    if (clientFd == -1)
        ::perror("[ERROR] Socket failed to listen on port");

    // Use std::expected here, return -1 clientfd or Socket when fd is not -1

    return clientFd;
}

void ServerSocket::close() noexcept
{
    if (!isBound())
        return;

    ::close(fd);
    fd = -1;
    bound = false;
    closed = true;
}

[[nodiscard]] bool ServerSocket::isBound() noexcept
{
    return bound;
}

[[nodiscard]] bool ServerSocket::isClosed() noexcept
{
    return closed;
}

void ServerSocket::setBlocking(bool _block) noexcept
{
    blocking = _block;
}

void ServerSocket::setReuseAddress(bool _on) noexcept
{
    socketOptions.reuseAddress = _on;
}

void ServerSocket::setReceiveBufferSize(int _size) noexcept
{
    socketOptions.receiveBufferSize = _size;
}

void ServerSocket::setTimeout(int _timeout) noexcept
{
    socketOptions.timeout = _timeout;
}