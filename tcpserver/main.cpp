#include <iostream>
#include <string>

#include "TcpListener.h"

void Listener_MessageReceived(TcpListener* listener, int socketId, std::string msg);

// We have abstracted networking layer. The only networking code is our own tcp listener class. 
int main()
{
    TcpListener server("127.0.0.1", 9001, Listener_MessageReceived);
    server.Run();
    return 0;
}

// This is a function that is called when a message is received from a client (hence message handler).
// It itself calls the send function of the listener that called it and sends the message given. 
void Listener_MessageReceived(TcpListener* listener, int client, std::string msg)
{
    std::cout << "Client: " << msg << std::endl;
    listener->Send(client, msg);
}