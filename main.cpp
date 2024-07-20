#include <iostream>
#include <thread>
#include <memory>
#include <Logger.hpp>
#include <ModbusRTUMaster.hpp>
#include <UniversalModbusRTUClient.hpp>
#include "crow.h"
#include "crow/middlewares/cors.h"


int main()
{
    SetLogLevel(prizma::LogLevel::Debug);

    prizma::UniversalModbusRTUClient client("pre16di.json");

    client.Poll();

    crow::App<crow::CORSHandler> app;
    auto& cors = app.get_middleware<crow::CORSHandler>();

    // clang-format off
    cors
      .global()
        .headers("Content-Type", "Upgrade-Insecure-Requests")
        .methods("POST"_method, "GET"_method)
      .prefix("/cors")
        .origin("localhost:5173")
      .prefix("/nocors")
        .ignore();
    // clang-format on

    CROW_ROUTE(app, "/configuration").methods("POST"_method)([](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x)
            return crow::response(400);
            std::cout << x << std::endl;
        return crow::response{"Hello"};
    });

    app.port(86).run();

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