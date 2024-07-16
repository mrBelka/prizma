#pragma once
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include <Logger.hpp>

namespace prizma
{
    class UniversalModbusRTUClient
    {
        std::string m_deviceName;
        std::ifstream m_configurationFile;
    
    public:
        UniversalModbusRTUClient(const std::string& deviceName) : m_deviceName(deviceName)
        {
            m_configurationFile.open(m_deviceName);
            if(!m_configurationFile.is_open())
                ERR() << "Ошибка открытия конфигурационного файла";

            nlohmann::json data = nlohmann::json::parse(m_configurationFile);

            std::cout << data["commands"] << std::endl;
        }

        ~UniversalModbusRTUClient() noexcept
        {

        }
    };
}