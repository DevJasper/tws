module;

export module net.socket;

export namespace net {

    class Socket {
    public:
        Socket() = default;
        Socket(const int, const int = 10);
        Socket(const char*, const int, const int = 10);
        ~Socket();
        // void connect(const int) noexcept;
        // void connect(const char*, const int) noexcept;
        void close() noexcept;
        [[nodiscard]] bool isBound() noexcept;
        [[nodiscard]] bool isClosed() noexcept;
        void setBlocking(bool) noexcept;
        void setReuseAddress(bool) noexcept;
        void setReusePort(bool) noexcept;
        void setKeepAlive(bool) noexcept;
        void setReceiveBufferSize(int) noexcept;
        void setSendBufferSize(int) noexcept;
        void setTimeout(int) noexcept;

    private:
        const char* address = nullptr;
        int fd = -1, port = 0;
        bool blocking = true;
        bool bound = false;
        bool closed = false;
        struct {
            bool reuseAddress = false;
            bool reusePort = false;
            bool keepAlive = false;
            int receiveBufferSize = 0;
            int sendBufferSize = 0;
            int timeout = 0; // seconds

        } socketOptions;
    };

};