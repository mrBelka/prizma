#pragma once
#include <asio.hpp>
#include <optional>

#include <iostream>
#include <mutex>

using namespace std::chrono_literals;

namespace prizma::io
{

    struct SerialParams
    {
        std::string portName;
        size_t baudrate;
        uint32_t dataBits;
        char parity;
        std::string stopBits;
        std::chrono::milliseconds timeout;
    };

    class Serial 
    {      
        asio::io_service m_io_service;
        std::unique_ptr<asio::serial_port> m_serialPort;

        public:
            Serial();
            Serial(const Serial&) = delete;
            Serial& operator=(const Serial&) = delete;
            ~Serial() noexcept;

            bool Open(const SerialParams& params) const;
            bool isOpen() const;
            void Close() const;

            size_t Send(const uint8_t* data, size_t size) const;
            size_t Receive(uint8_t* data, size_t size) const;
            size_t ReceiveWithTimeout(uint8_t* data, size_t size, std::chrono::milliseconds timeout);
    };

}