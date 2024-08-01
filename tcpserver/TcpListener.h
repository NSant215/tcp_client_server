#pragma once
#include <string>

#define MAX_BUFFER_SIZE (49152)

// Forward declaration
class TcpListener;

//TODO: Write a callback to the data received from a client. This will eventually call the send function in our server.
typedef void(*MessageReceivedHandler)(TcpListener* listener, int socketId, std::string msg);


class TcpListener
{
public:
    TcpListener(std::string ipAddress, int port, MessageReceivedHandler handler);

    // Send a message to the specified client
    void Send(int clientSocket, std::string msg);

    // Run the server
    void Run();
private:

    // Create a socket
    int CreateSocket();

    // Wait for a connection
    int WaitForConnection(int listening);

    // Address of the server
    std::uint32_t               m_ipAddress;

    // Listening port
    int                         m_port;

    // Message received event handler
    MessageReceivedHandler      MessageReceived;

};