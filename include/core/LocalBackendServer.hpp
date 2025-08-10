#ifndef LOCALBACKENDSERVER_HPP
#define LOCALBACKENDSERVER_HPP

#include <App.h>
#include <string>
#include <unordered_map>
#include <mutex>
#include <functional>

class LocalBackendServer
{
public:
    explicit LocalBackendServer(const std::string& daemonId, int port = 9001);
    ~LocalBackendServer();

    // Start the server loop in a separate thread
    void start();

    // Stop the server gracefully
    void stop();

    // Send log or status message to all connected authenticated clients
    void broadcastMessage(const std::string& message);

    // Register callback to handle commands from UI client
    void onCommand(std::function<void(const std::string& command, const std::string& data)> handler);

private:
   std::string daemonId;
    int port;
    std::mutex clientsMutex;

    // Map client sockets to authentication status
    std::unordered_map<void*, bool> clients;

    std::function<void(const std::string&, const std::string&)> commandHandler;

    // Internal thread to run server
    std::thread serverThread;

    // Internal function running the uWebSocket server
    void runServer();
};

#endif // LOCALBACKENDSERVER_HPP
