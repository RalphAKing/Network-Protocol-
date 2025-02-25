#include "packet.h" 
#include <iostream>
#include <string>
#include <vector>
#include <cstring> 

#define PORT 8080

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed. Error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << "..." << std::endl;

    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected!" << std::endl;

    std::string reassembledMessage;
    int totalPackets = 0;
    int receivedPackets = 0;

    while (true) {
        Packet packet;
        int bytesReceived = recv(clientSocket, (char*)&packet, sizeof(Packet), 0);
        if (bytesReceived <= 0) break;

        uint32_t receivedChecksum = packet.checksum;
        packet.checksum = 0; 
        uint32_t calculatedChecksum = calculateChecksum((char*)&packet, sizeof(Packet));


        int currentPacket = packet.packetNumber >> 8;
        totalPackets = packet.packetNumber & 0xFF;

        std::cout << "Received packet " << currentPacket << " of " << totalPackets << std::endl;

        if (receivedChecksum == calculatedChecksum) {
            std::cout << "Checksum is valid for packet " << currentPacket << std::endl;
            reassembledMessage += packet.body;
            receivedPackets++;
        } else {
            std::cout << "Checksum is invalid for packet " << currentPacket << std::endl;
        }

        if (receivedPackets == totalPackets) break;
    }

    std::cout << "Reassembled message: " << reassembledMessage << std::endl;

    std::string ackMessage = "ACK ALL";
    send(clientSocket, ackMessage.c_str(), ackMessage.size(), 0);
    std::cout << "Sent ACK for all packets" << std::endl;

    closesocket(clientSocket);
    closesocket(serverSocket);

    WSACleanup();

    return 0;
}