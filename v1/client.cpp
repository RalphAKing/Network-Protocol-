#include "packet.h"
#include <iostream>
#include <string>
#include <cstring>

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

    std::string longMessage;

    std::cout << "Enter your message: ";
    std::getline(std::cin, longMessage);

    size_t totalPackets = (longMessage.size() + 1023) / 1024;

    for (size_t i = 0; i < totalPackets; ++i) {
        Packet packet;
        memset(&packet, 0, sizeof(Packet));

        packet.destinationIP.sin_family = AF_INET;
        packet.destinationIP.sin_addr.s_addr = inet_addr(SERVER_IP);
        packet.sourceIP.sin_family = AF_INET;
        packet.sourceIP.sin_addr.s_addr = inet_addr("192.168.1.100");
        packet.version = 1;
        packet.protocol = 1;
        packet.packetNumber = (i + 1) << 16 | totalPackets;

        size_t start = i * 1024;
        size_t length = std::min(longMessage.size() - start, (size_t)1024);
        strncpy(packet.body, longMessage.c_str() + start, length);

        packet.checksum = calculateChecksum((char*)&packet, sizeof(Packet));

        send(clientSocket, (char*)&packet, sizeof(Packet), 0);
        std::cout << "Sent packet " << (i + 1) << " of " << totalPackets << std::endl;
    }

    char ackBuffer[32];
    int bytesReceived = recv(clientSocket, ackBuffer, sizeof(ackBuffer), 0);
    if (bytesReceived > 0) {
        ackBuffer[bytesReceived] = '\0';
        std::string ackMessage(ackBuffer);
        if (ackMessage == "ACK ALL") {
            std::cout << "Received ACK for all packets" << std::endl;
        }
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
