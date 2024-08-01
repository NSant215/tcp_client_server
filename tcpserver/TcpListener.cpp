#include "TcpListener.h"
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h> // close()

TcpListener::TcpListener(std::string ipAddress, int port, MessageReceivedHandler handler)
    : m_port(port), MessageReceived(handler)
{
    m_ipAddress = inet_addr(ipAddress.c_str());
}

void TcpListener::Send(int clientSocket, std::string msg)
{
    send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}

void TcpListener::Run()
{
    std::cout << "Server Running." << std::endl;
    char buf[MAX_BUFFER_SIZE];

    while (true)
    {
        // Create a listening socket
        int listeningfd = CreateSocket();
        if (listeningfd != -1)
        {
            // Wait for a connection
            std::cout << "Waiting for connection..." << std::endl;
            int clientfd = WaitForConnection(listeningfd);
            std::cout << "Client connected." << std::endl;
            if (clientfd != -1)
            {
                close(listeningfd); // close the listening socket.
                int bytesReceived = 0;
                do
                {
                    memset(buf, 0, MAX_BUFFER_SIZE); // set buf to a set of 0s of length MAX_BUFFER_SIZE.
                    bytesReceived = read(clientfd, buf, MAX_BUFFER_SIZE);
                    if (bytesReceived == -1)
                    {
                        std::cerr << "Error in receiving message." << std::endl;
                    }
                    else if (bytesReceived == 0)
                    {
                        std::cout << "Client disconnected." << std::endl;
                    }
                    else 
                    {
                        std::string msg = std::string(buf, 0, bytesReceived);
                        MessageReceived(this, clientfd, msg);
                    }
                    std::cout << "bytesReceived: " << bytesReceived << std::endl;
                } while (bytesReceived > 0);
            }
            else
            {
                std::cerr << "Client connection failed." << std::endl;
            }
            close(clientfd);
            std::cout << "Closing connection." << std::endl; 
        }
        else
        {
            std::cerr << "Listening failed." << std::endl;
        }
    }
}

int TcpListener::CreateSocket()
{
    // Initialize the socket
    int listeningfd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4, SOCK_STREAM = TCP
    if (listeningfd != -1)
    {
        // Create the server hint structure (IP address and port structure)
        sockaddr_in servAddr;
        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons(m_port); // storing in network byte order.
        servAddr.sin_addr.s_addr = m_ipAddress;
    
        // Bind the socket to the IP / port
        if (bind(listeningfd, reinterpret_cast<sockaddr*>(&servAddr), sizeof(servAddr)) == 0) // successfully bound.
        {
            // Mark the socket for listening in
            if (listen(listeningfd, SOMAXCONN) == 0)
            {
                return listeningfd;
            }
            else
            {
                std::cerr << "Problem with socket listening." << std::endl;
                return -1;
            }
        }
        else
        {
            std::cerr << "Problem with socket binding." << std::endl;
            return -1;
        }
    }
    else
    {
        std::cerr << "Problem with socket creation." << std::endl;
        return listeningfd;
    }
}

int TcpListener::WaitForConnection(int listening)
{
    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    if (clientSocket == -1)
    {
        std::cerr << "Problem with client connection." << std::endl;
        return -1;
    }
    return clientSocket;
}