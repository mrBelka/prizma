#include <ModbusRTUMaster.hpp>

namespace prizma::proto 
{

    std::vector<uint8_t> ModbusMasterRTU::ReadInputRegister(uint8_t address, uint16_t reg, uint16_t count)
    {
        uint8_t wbuf[8] = { address, 
                            0x04, 
                            static_cast<uint8_t>(reg >> 8), 
                            static_cast<uint8_t>(reg & 0xFF), 
                            static_cast<uint8_t>(count >> 8), 
                            static_cast<uint8_t>(count & 0xFF)};

        uint16_t crc = ModbusCrc16(wbuf, 6);
        wbuf[6] = crc & 0xFF;
        wbuf[7] = crc >> 8;

        size_t n = m_conn.Send(wbuf, 8);
        if(n != 8)
            throw std::runtime_error("Отправлено " + std::to_string(n) + " из 8 байт");

        size_t size = 5 + 2 * count;
        
        uint8_t rbuf[1024];
        n = m_conn.ReceiveWithTimeout(rbuf, size, m_params.timeout);

        if(n != size)
            throw std::runtime_error("Принято " + std::to_string(n) + " из " + std::to_string(size) + " байт");

        if(!CheckCRC(rbuf, n))
            throw std::runtime_error("Неверная контрольная сумма");

        std::vector<uint8_t> result;
        for(int i=3;i<3+2*count;i++)
            result.push_back(rbuf[i]);

        return result;
    }

    bool ModbusMasterRTU::Setup()
    {
        return m_conn.Open(m_params);
    }

    ModbusMasterRTU::~ModbusMasterRTU() 
    {
        m_conn.Close();
    }

    bool ModbusMasterRTU::CheckCRC(uint8_t* data, size_t size)
    {
        uint16_t crcReceived = (data[size - 1] << 8) | data[size - 2];
        uint16_t crcComputed = ModbusCrc16(data, size - 2);
        return crcComputed == crcReceived;
    }

    uint16_t ModbusMasterRTU::ModbusCrc16(const unsigned char *buf, size_t len) const
    {
        uint8_t x = 0;
        uint16_t crc = 0xFFFF;

        while( len-- )
        {
            x = (*buf++) ^ crc;
            crc >>= 8;
            crc ^= m_table[x];
        }
        return crc;
    }
    
}