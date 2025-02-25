#include "packet.h" // Include the packet structure and checksum function
#include <iostream> // For std::cout, std::cerr, and std::endl

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed. Error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to the server!" << std::endl;

    Packet packet;
    strcpy(packet.destinationIP, SERVER_IP);
    strcpy(packet.sourceIP, "192.168.1.100");
    packet.version = 1;
    packet.protocol = 1;
    strcpy(packet.body, "Hello, this is a custom protocol message!");
    packet.checksum = 0; 
    packet.checksum = calculateChecksum((char*)&packet, sizeof(Packet));

    send(clientSocket, (char*)&packet, sizeof(Packet), 0);

    closesocket(clientSocket);

    WSACleanup();

    return 0;
}
