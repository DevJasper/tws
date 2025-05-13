export module net.socket;

export namespace net {
    class Socket {
    public:
        Socket() = default;
        Socket(const int);
        Socket(const char*, const int);
        ~Socket();
        [[nodiscard]] inline int fileDescriptor() noexcept { return fd; };
        void connect(const char*, const int) noexcept;
        void close() noexcept;
        [[nodiscard]] bool isConnected() noexcept;
        [[nodiscard]] bool isClosed() noexcept;
        void setBlocking(bool) noexcept;
        void setKeepAlive(bool) noexcept;
        void setReceiveBufferSize(int) noexcept;
        void setSendBufferSize(int) noexcept;
        void setReceiveTimeout(int) noexcept;
        void setSendTimeout(int) noexcept;

    private:
        const char* address = nullptr;
        int fd = -1, port = 0;
        bool connected = false;
        bool closed = false;
    };

};