module;

#include <memory>

export module net.server_socket;

import net.socket;

export namespace net {
    class ServerSocket {
    public:
        ServerSocket() = default;
        ServerSocket(const int, const int = 10);
        ServerSocket(const char*, const int, const int = 10);
        ~ServerSocket();
        [[nodiscard]] inline int fileDescriptor() noexcept { return fd; };
        void bind(const int) noexcept;
        void bind(const char*, const int) noexcept;
        void listen(const int = 10) noexcept;
        std::unique_ptr<Socket> accept() noexcept;
        void close() noexcept;
        [[nodiscard]] bool isBound() noexcept;
        [[nodiscard]] bool isClosed() noexcept;
        void setReuseAddress(bool) noexcept;
        void setReusePort(bool) noexcept;
        void setBlocking(bool) noexcept;
        void setKeepAlive(bool) noexcept;
        void setReceiveBufferSize(int) noexcept;
        void setSendBufferSize(int) noexcept;
        void setReceiveTimeout(int) noexcept;
        void setSendTimeout(int) noexcept;

    private:
        const char* address = nullptr;
        int fd = -1, port = 0;
        bool bound = false;
        bool closed = false;
        struct {
            bool reuseAddress = false;
            bool reusePort = false;
        } socketOptions;
    };

};