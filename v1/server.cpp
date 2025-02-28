#include "packet.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <thread>
#include <mutex>

#define PORT 8080

std::mutex consoleMutex;

void handleClient(SOCKET clientSocket) {
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << "Handling client connection..." << std::endl;

    std::string reassembledMessage;
    int totalPackets = 0;
    int receivedPackets = 0;
    std::vector<int> receivedPacketNumbers;

    while (true) {
        Packet packet;
        int bytesReceived = recv(clientSocket, (char*)&packet, sizeof(Packet), 0);
        if (bytesReceived <= 0) break;

        uint32_t receivedChecksum = packet.checksum;
        packet.checksum = 0;
        uint32_t calculatedChecksum = calculateChecksum((char*)&packet, sizeof(Packet));

        uint32_t currentPacket = (packet.packetNumber >> 16) & 0xFFFF;
        uint32_t totalPackets = packet.packetNumber & 0xFFFF;

        if (receivedChecksum == calculatedChecksum) {
            std::cout << "Checksum is valid for packet " << currentPacket << std::endl;
            reassembledMessage += packet.body;
            receivedPackets++;
            receivedPacketNumbers.push_back(currentPacket);
        } else {
            std::cout << "Checksum is invalid for packet " << currentPacket << std::endl;
        }

        if (receivedPackets == totalPackets) break;
    }

    std::cout << "Reassembled message: " << reassembledMessage << std::endl;

    std::sort(receivedPacketNumbers.begin(), receivedPacketNumbers.end());
    std::cout << "Received packets: ";
    for (int packet : receivedPacketNumbers) {
        std::cout << packet << " ";
    }
    std::cout << std::endl;

    std::vector<int> unsentPackets;
    for (int i = 1; i <= totalPackets; ++i) {
        if (std::find(receivedPacketNumbers.begin(), receivedPacketNumbers.end(), i) == receivedPacketNumbers.end()) {
            unsentPackets.push_back(i);
        }
    }

    std::cout << "Unsent packets: ";
    for (int packet : unsentPackets) {
        std::cout << packet << " ";
    }
    std::cout << std::endl;


    std::string ackMessage = "ACK ALL";
    send(clientSocket, ackMessage.c_str(), ackMessage.size(), 0);
    std::cout << "Sent ACK for all packets" << std::endl;

    closesocket(clientSocket);
    WSACleanup();
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket;
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

    while (true) {
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
