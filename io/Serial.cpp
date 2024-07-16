#include <Serial.hpp>
#include <Logger.hpp>
#include <iostream>

namespace prizma::io 
{
    Serial::Serial()
    {
        m_serialPort = std::make_unique<asio::serial_port>(m_io_service);
    }

    Serial::~Serial() 
    {
        Close();
    }

    bool Serial::Open(const SerialParams& params) const
    {
        try
        {
            if (!isOpen())
                Close();

            m_serialPort->open(params.portName);
            if (!isOpen())
                throw std::runtime_error("Порт не открыт");

            using namespace asio;

            m_serialPort->set_option(serial_port_base::baud_rate(params.baudrate));
            m_serialPort->set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));

            // Биты данных
            m_serialPort->set_option(serial_port_base::character_size(params.dataBits));

            // Четность
            serial_port_base::parity::type parity = serial_port_base::parity::none;
            if(params.parity == 'e')
                parity = serial_port_base::parity::even;
            m_serialPort->set_option(serial_port_base::parity(parity));

            // Стоп-биты
            serial_port_base::stop_bits::type stopBits = serial_port_base::stop_bits::one;
            if(params.stopBits == "2")
                stopBits = serial_port_base::stop_bits::two;
            m_serialPort->set_option(serial_port_base::stop_bits(stopBits));

            return true;
        }
        catch (std::exception& e)
        {
            ERR() << e.what() << " " << __FILE__ << " " << __LINE__;
        }

        return false;
    }

    bool Serial::isOpen() const
    {
        return m_serialPort->is_open();
    }

    void Serial::Close() const
    {
        try {
            m_serialPort->close();
        }
        catch (const std::exception& e) {
            ERR() << e.what() << " " << __FILE__ << " " << __LINE__;
        }
    }

    size_t Serial::Send(const uint8_t* data, size_t size) const
    {
        try {
            return m_serialPort->write_some(asio::buffer(data, size));
        }
        catch (std::exception &e) {
            ERR() << e.what() << " " << __FILE__ << " " << __LINE__;
            return 0;
        }
    }

    size_t Serial::Receive(uint8_t* data, size_t size) const
    {
        try
        {
            return m_serialPort->read_some(asio::buffer(data, size));
        }
        catch (std::exception& e)
        {
            ERR() << e.what() << " " << __FILE__ << " " << __LINE__;
            return 0;
        }
    }

    size_t Serial::ReceiveWithTimeout(uint8_t* data, size_t size, std::chrono::milliseconds timeout)
    {
        try
        {
            asio::high_resolution_timer timer(m_io_service);
            timer.expires_from_now(timeout);

            std::optional<asio::error_code> timer_result;
            timer.async_wait([&timer_result](const asio::error_code& error)
                {
                    timer_result = error;
                }
            );

            size_t n = 0;
            std::optional<asio::error_code> read_result; 

            m_serialPort->async_read_some(asio::buffer(data, size),
                [&read_result,&n](const asio::error_code& error, size_t readed)
                {
                    read_result = error;
                    n = readed;
                }
            );

            m_io_service.reset();
            while(m_io_service.run_one())
            {
                if(read_result)
                    timer.cancel();

                if(timer_result)
                    m_serialPort->cancel();
            }

            if (*read_result)
                throw asio::system_error(*read_result);

            return n;
        }
        catch(const std::exception& e)
        {
            ERR() << e.what() << " " << __FILE__ << " " << __LINE__;
        }
        
        return 0;
    }
}