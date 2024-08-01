#include <netinet/in.h> //structure for storing address information 
#include <sys/socket.h> //for socket APIs 
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <limits>

// write a function to take ask for a string and return as a char
void askForString(char* strData) {
    std::string str;
    std::cout << "Enter a string: ";
    std::getline(std::cin, str);
    // std::cin >> std::ws; // stream out any whitespace
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    // std::cin.clear();
    std::cout << "You entered: " << str << std::endl;
    str.copy(strData, str.size());
}
  
int main(int argc, char const* argv[]) 
{ 
    int sockD = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "Socket created: " << sockD << std::endl;
  
    struct sockaddr_in clientAddr; 
  
    clientAddr.sin_family = AF_INET; 
    clientAddr.sin_port 
        = htons(9001); // use some unused port number 
    std::string ip = "127.0.0.1";
    clientAddr.sin_addr.s_addr = inet_addr(ip.c_str());  
    int connectStatus 
        = connect(sockD, (struct sockaddr*)&clientAddr, 
                  sizeof(clientAddr));   
    if (connectStatus == -1) { 
        printf("Error...\n"); 
    } else {
        bool keepConnectionUp = true;
        while (keepConnectionUp) {
            char strData[255];
            askForString(strData);
            std::cout << "Sending: " << strData << std::endl;
            send(sockD, strData, sizeof(strData), 0);
            // clear the buffer
            memset(strData, 0, sizeof(strData));
            char strDatarecv[255];
            recv(sockD, strDatarecv, sizeof(strData), 0);
            printf("Server: %s\n", strDatarecv);
            // ask user whether to keep connection up
            std::string keepConnection;
            std::cout << "Do you want to send another message? (y/n): ";
            std::cin >> keepConnection;
            if (keepConnection != "y") {
                keepConnectionUp = false;
            }
        }
    }
  
    return 0; 
}