#include <iostream>
#include <thread>
#include <memory>
#include <Logger.hpp>
#include <ModbusRTUMaster.hpp>
#include <UniversalModbusRTUClient.hpp>


int main()
{
    SetLogLevel(prizma::LogLevel::Debug);

    prizma::UniversalModbusRTUClient client("pre16di.json");

    /*try
    {
        prizma::io::SerialParams params = {"/dev/com1", 115200, 8, 'n', "1", 200ms};
        prizma::proto::ModbusMasterRTU mb(params);
        if(!mb.Setup())
            throw std::runtime_error("Ошибка настройки протокола");

        try
        {
            auto result = mb.ReadInputRegister(0x01, 0x0001, 0x0001);
            BYTES(result.data(), 2);
        }
        catch(const std::exception& e)
        {
           ERR() << e.what();
        }
    }
    catch(const std::exception& e)
    {
        ERR() << e.what();
    }*/
}