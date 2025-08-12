#ifndef LOCALBACKENDSERVER_HPP
#define LOCALBACKENDSERVER_HPP

#include <App.h>
#include <IXWebSocket.h>
#include <string>
#include <unordered_map>
#include <mutex>
#include <functional>

class LocalBackendServer
{
public:
    explicit LocalBackendServer(const std::string &daemonId, int port = 9001);
    ~LocalBackendServer();

    void start();

    void stop();

    void broadcastMessage(const std::string &message);

    void onCommand(std::function<void(const std::string &command, const std::string &data)> handler);

    void connectToCloud(std :: string cloudUrl);
     
private:
    std::string daemonId;
    int port;
    std::mutex clientsMutex;

    std::unordered_map<void *, bool> clients;

    std::function<void(const std::string &, const std::string &)> commandHandler;

    std::thread serverThread;

    void runServer();

    ix :: WebSocket cloud;
    bool cloudConnected = false ; 

};

#endif
