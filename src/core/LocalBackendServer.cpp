#include "LocalBackendServer.hpp"
#include <App.h>
#include <thread>
#include <iostream>

LocalBackendServer::LocalBackendServer(const std::string &daemonId_, int port_)
    : daemonId(daemonId_), port(port_)
{
}

LocalBackendServer::~LocalBackendServer()
{
    stop();
}

void LocalBackendServer::start()
{
    serverThread = std::thread(&LocalBackendServer::runServer, this);
}

void LocalBackendServer::stop()
{
    cloud.stop();
    if (serverThread.joinable())
    {
        serverThread.join();
    }
}

void LocalBackendServer::broadcastMessage(const std::string &message)
{
    std::lock_guard<std::mutex> lock(clientsMutex);

    for (auto &[client, authenticated] : clients)
    {
        if (authenticated)
        {
            auto ws = static_cast<uWS::WebSocket<false, true, void> *>(client);
            ws->send(message, uWS::OpCode::TEXT);
        }
    }
    if (cloudConnected)
    {
        cloud.send(message);
    }
}

void LocalBackendServer::onCommand(std::function<void(const std::string &, const std::string &)> handler)
{
    commandHandler = std::move(handler);
}
void LocalBackendServer::runServer()
{

    struct PerSocketData
    {
        bool authenticated = false;
    };

    uWS::App app;

    app.ws<PerSocketData>("/*", {.open = [this](auto *ws)
                                 {
                                     std::lock_guard<std::mutex> lock(clientsMutex);
                                     clients[static_cast<void *>(ws)] = false;
                                     std::cout << "[Server] New client connected. Awaiting authentication...\n"; },

                                 .message = [this](auto *ws, std::string_view message, uWS::OpCode opCode)
                                 {
            auto *psd = ws->getUserData();
            std::string msg(message);

            if (!psd->authenticated) {
                if (msg == daemonId) {
                    {
                        std::lock_guard<std::mutex> lock(clientsMutex);
                        clients[static_cast<void*>(ws)] = true;
                    }
                    psd->authenticated = true;
                    ws->send("AUTH_OK", uWS::OpCode::TEXT);
                    std::cout << "[Server] Client authenticated successfully.\n";
                } else {
                    ws->send("AUTH_FAILED", uWS::OpCode::TEXT);
                    std::cout << "[Server] Authentication failed. Closing connection.\n";
                    ws->close();
                }
                return; 
            }

            if (commandHandler) {
                auto sep = msg.find(':');
                if (sep != std::string::npos) {
                    auto cmd = msg.substr(0, sep);
                    auto data = msg.substr(sep + 1);
                    commandHandler(cmd, data);
                } else {
                    commandHandler(msg, "");
                }
            } },

                                 .close = [this](auto *ws, int, std::string_view)
                                 {
            
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.erase(static_cast<void*>(ws));
            std::cout << "[Server] Client disconnected.\n"; }});

    app.listen(port, [this](auto *token)
               {
        if (token) {
            std::cout << "[Server] Listening on port " << port << std::endl;
        } else {
            std::cerr << "[Server] Failed to listen on port " << port << std::endl;
        } });

    app.run();
}

void LocalBackendServer ::connectToCloud(std :: string cloudUrl) 
{
    cloud.setUrl(cloudUrl);
    cloud.setOnMessageCallback([this](const ix::WebSocketMessagePtr &msg)
                               {
         if (msg->type == ix::WebSocketMessageType::Open)
        {
            std::cout << "[Cloud] Connected to cloud server." << std::endl;
            cloudConnected = true;
            cloud.send("daemonId:"+daemonId);
        }
        else if (msg->type == ix::WebSocketMessageType::Message)
        {
            std::cout << msg->str << std::endl;
             if (commandHandler) {
                auto sep = msg->str.find(':');
                if (sep != std::string::npos) {
                    auto cmd = msg->str.substr(0, sep);
                    auto data = msg->str.substr(sep + 1);
                    commandHandler(cmd, data);
                } else {
                    commandHandler(msg->str, "");
                }
            }
            broadcastMessage(msg->str) ; 
        }
        else if (msg->type == ix::WebSocketMessageType::Close)
        {
            std::cout << "[Cloud] Disconnected from cloud server" << std::endl;
            std::cout << msg->str << std::endl;
            cloudConnected = false ; 


        }
        else if (msg->type == ix::WebSocketMessageType::Error)
        {
            std::cout << msg->str << std::endl;

        } });
    cloud.start();
}
