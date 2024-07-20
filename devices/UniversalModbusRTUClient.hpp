#pragma once
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include <Logger.hpp>
#include <ModbusRTUMaster.hpp>

namespace prizma
{
    class UniversalModbusRTUClient
    {
        std::string m_deviceName;
        std::ifstream m_configurationFile;
    
    public:
        UniversalModbusRTUClient(const std::string& deviceName) : m_deviceName(deviceName)
        {
            /*
            m_configurationFile.open(m_deviceName);
            if(!m_configurationFile.is_open())
                ERR() << "Ошибка открытия конфигурационного файла";

            nlohmann::json data = nlohmann::json::parse(m_configurationFile);


            for(int i = 0; i < data["commands"].size(); i++)
            {
                auto command = data["commands"][i];
                int funcCode = command["code"];
                auto registers = command["registers"];
                for(int j = 0; j < registers.size(); j++)
                {
                    auto reg = registers[j];
                    std::cout << reg["num"] << " " << reg["name"] << " " << reg["type"] << std::endl; 
                }
            }
            */
        }

        ~UniversalModbusRTUClient() noexcept
        {

        }

        void Poll()
        {
            m_configurationFile.open(m_deviceName);
            if(!m_configurationFile.is_open())
                ERR() << "Ошибка открытия конфигурационного файла";

            nlohmann::json data = nlohmann::json::parse(m_configurationFile);

            try
            {
                prizma::io::SerialParams params = {"/dev/com1", 115200, 8, 'n', "1", 200ms};
                prizma::proto::ModbusMasterRTU mb(params);
                if(!mb.Setup())
                    throw std::runtime_error("Ошибка настройки протокола");

                try
                {
                    for(int i = 0; i < data["commands"].size(); i++)
                    {
                        auto command = data["commands"][i];
                        int funcCode = command["code"];
                        auto registers = command["registers"];
                        for(int j = 0; j < registers.size(); j++)
                        {
                            auto reg = registers[j];
                            std::cout << reg["num"] << " " << reg["name"] << " " << reg["type"] << std::endl; 
                            auto result = mb.ReadInputRegister(0x01, reg["num"], 0x0001);
                            BYTES(result.data(), 2);
                        }
                    }
                }
                catch(const std::exception& e)
                {
                ERR() << e.what();
                }
            }
            catch(const std::exception& e)
            {
                ERR() << e.what();
            }
        }
    };
}