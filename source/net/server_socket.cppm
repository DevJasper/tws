module;

export module net.server_socket;

export namespace net {
    class ServerSocket {
    public:
        ServerSocket() = default;
        ServerSocket(const int, const int = 10);
        ServerSocket(const char*, const int, const int = 10);
        ~ServerSocket();
        void bind(const int) noexcept;
        void bind(const char*, const int) noexcept;
        void listen(const int = 10) noexcept;
        int accept() noexcept;
        void close() noexcept;
        [[nodiscard]] bool isBound() noexcept;
        [[nodiscard]] bool isClosed() noexcept;
        void setBlocking(bool) noexcept;
        void setReuseAddress(bool) noexcept;
        void setReceiveBufferSize(int) noexcept;
        void setTimeout(int) noexcept;

    private:
        const char* address = nullptr;
        int fd = -1, port = 0;
        bool blocking = true;
        bool bound = false;
        bool closed = false;
        struct {
            bool reuseAddress = false;
            int receiveBufferSize = 0;
            int timeout = 0; // seconds
        } socketOptions;
    };

};