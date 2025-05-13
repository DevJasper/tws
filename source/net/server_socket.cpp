module;
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <expected>
#include <fcntl.h>
#include <memory>
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
    struct addrinfo hints, *result;

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

    for (auto* res = result; res != nullptr; res = res->ai_next) {

        fd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (fd == -1)
            continue;

        if (socketOptions.reuseAddress) {
            int yes = 1;
            std::println("[INFO] Socket address reusable");
            if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
                ::perror("[ERROR] Failed to make socket address reusable");
                ::close(fd);
                continue;
            }
        }

        if (socketOptions.reusePort) {
            int yes = 1;
            std::println("[INFO] Socket port reusable");
            if (::setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) == -1) {
                ::perror("[ERROR] Failed to make socket port reusable");
                ::close(fd);
                continue;
            }
        }

        if (::bind(fd, res->ai_addr, res->ai_addrlen) == 0) {
            bound = true;
            break;
        }

        ::close(fd);
        fd = -1;
    }

    ::freeaddrinfo(result);

    if (!isBound()) {
        std::println("[ERROR] Failed to bind socket");
        std::exit(EXIT_FAILURE);
    }

    std::println("[SUCCESS] Socket bound to port {}", port);
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

std::unique_ptr<Socket> ServerSocket::accept() noexcept
{
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);

    int clientFd = ::accept4(fd, (struct sockaddr*)&clientAddress, &clientAddressLen, SOCK_CLOEXEC);

    if (clientFd == -1) {
        perror("[ERROR] Failed to accept incoming connection on socket");
        return nullptr;
    }

    std::println("[INFO] Accepted client with fd: {}", clientFd);

    return std::make_unique<Socket>(clientFd);
}

void ServerSocket::close() noexcept
{
    if (!isBound()) {
        // std::println("[WARNING] Socket is not bound; nothing to close.");
        return;
    }

    if (::close(fd) == -1) {
        ::perror("[ERROR] Failed to close socket");
        return;
    }

    std::println("[INFO] Server socket {} closed successfully.", fd);

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

void ServerSocket::setReuseAddress(bool _on) noexcept
{
    socketOptions.reuseAddress = _on;
}

void ServerSocket::setReusePort(bool _on) noexcept
{
    socketOptions.reusePort = _on;
}

void ServerSocket::setBlocking(bool _block) noexcept
{
    if (!isBound()) {
        std::println("[ERROR] Cannot set socket blocking mode: socket is not initialized");
        return;
    }

    int flags = ::fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        ::perror("[ERROR] Failed to get socket flags");
        close();
        return;
    }

    if (_block) {
        flags &= ~O_NONBLOCK;
    } else {
        flags |= O_NONBLOCK;
    }

    if (::fcntl(fd, F_SETFL, flags) == -1) {
        ::perror("[ERROR] Failed to set socket blocking mode");
        close();
        return;
    }

    std::println("[INFO] Socket set to {} mode", _block ? "blocking" : "non-blocking");
}

void ServerSocket::setKeepAlive(bool _on) noexcept
{

    if (!isBound()) {
        std::println("[ERROR] Cannot activate socket keep alive mode: socket is not initialized");
        return;
    }

    int option = _on;

    if (::setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &option, sizeof(option)) == -1) {
        ::perror("[ERROR] Failed to activate socket keep alive mode");
        close();
        return;
    }

    std::println("[INFO] Socket keep alive mode activated: {}", _on);
}

void ServerSocket::setReceiveBufferSize(int _size) noexcept
{
    if (!isBound()) {
        std::println("[ERROR] Cannot set socket receive buffer size: socket is not initialized");
        return;
    }

    if (::setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &_size, sizeof(_size)) == -1) {
        ::perror("[ERROR] Failed to set socket receive buffer size");
        close();
        return;
    }

    std::println("[INFO] Socket receive buffer size: {}", _size);
}

void ServerSocket::setSendBufferSize(int _size) noexcept
{
    if (!isBound()) {
        std::println("[ERROR] Cannot set socket send buffer size: socket is not initialized");
        return;
    }

    if (::setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &_size, sizeof(_size)) == -1) {
        ::perror("[ERROR] Failed to set socket send buffer size");
        close();
        return;
    }

    std::println("[INFO] Socket send buffer size: {}", _size);
}

void ServerSocket::setReceiveTimeout(int _timeoutSeconds) noexcept
{
    if (!isBound()) {
        std::println("[ERROR] Cannot set socket receive timeout: socket is not initialized");
        return;
    }

    struct timeval timeout;
    timeout.tv_sec = _timeoutSeconds;
    timeout.tv_usec = 0;

    if (::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
        ::perror("[ERROR] Failed to set socket receive timeout");
        close();
        return;
    }

    std::println("[INFO] Socket receive timeout: {} seconds", _timeoutSeconds);
}

void ServerSocket::setSendTimeout(int _timeoutSeconds) noexcept
{
    if (!isBound()) {
        std::println("[ERROR] Cannot set socket send timeout: socket is not initialized");
        return;
    }

    struct timeval timeout;
    timeout.tv_sec = _timeoutSeconds;
    timeout.tv_usec = 0;

    if (::setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) == -1) {
        ::perror("[ERROR] Failed to set socket send timeout");
        close();
        return;
    }

    std::println("[INFO] Socket send timeout: {} seconds", _timeoutSeconds);
}