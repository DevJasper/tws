module;
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <netdb.h>
#include <print>
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

module net.socket;

using namespace net;

Socket::Socket(const int _fd)
    : fd(_fd)
{
}

Socket::Socket(const char* _address, const int _port)
{
    connect(_address, _port);
}

Socket::~Socket()
{
    close();
}

void Socket::connect(const char* _address, const int _port) noexcept
{
    address = _address;
    port = _port;
    struct addrinfo hints, *result, *res;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_NUMERICSERV;

    int status = ::getaddrinfo(address, std::to_string(port).c_str(), &hints, &result);

    if (status != 0) {
        std::println("[ERROR] Failed to get address info: {}", ::gai_strerror(status));
        std::exit(EXIT_FAILURE);
    }

    for (res = result; res != nullptr; res = res->ai_next) {

        fd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (fd == -1)
            continue;

        if (::connect(fd, res->ai_addr, res->ai_addrlen) == 0) {
            connected = true;
            break;
        }

        ::close(fd);
    }

    ::freeaddrinfo(result);

    if (res == nullptr) {
        std::println("[ERROR] Failed to connect socket");
        std::exit(EXIT_FAILURE);
    }

    std::println("[SUCCESS] Socket connected to port {}", port);
}

void Socket::close() noexcept
{
    if (fd == -1) {
        std::println("[WARNING] Socket is already closed.");
        return;
    }

    if (!isConnected()) {
        std::println("[WARNING] Socket is not connected; nothing to close.");
        return;
    }

    if (::close(fd) == -1) {
        ::perror("[ERROR] Failed to close socket");
        return;
    }

    fd = -1;
    connected = false;
    closed = true;

    std::println("[INFO] Socket closed successfully.");
}

[[nodiscard]] bool Socket::isConnected() noexcept
{
    return connected;
}

[[nodiscard]] bool Socket::isClosed() noexcept
{
    return closed;
}

void Socket::setBlocking(bool _block) noexcept
{
    if (!isConnected()) {
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

void Socket::setKeepAlive(bool _on) noexcept
{

    if (!isConnected()) {
        std::println("[ERROR] Cannot active socket keep alive mode: socket is not initialized");
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

void Socket::setReceiveBufferSize(int _size) noexcept
{
    if (!isConnected()) {
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

void Socket::setSendBufferSize(int _size) noexcept
{
    if (!isConnected()) {
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

void Socket::setReceiveTimeout(int _timeoutSeconds) noexcept
{
    if (!isConnected()) {
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

void Socket::setSendTimeout(int _timeoutSeconds) noexcept
{
    if (!isConnected()) {
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