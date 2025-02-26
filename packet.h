#ifndef PACKET_H
#define PACKET_H

#include <winsock2.h>
#include <cstdint>
#include <cstring>

#pragma comment(lib, "Ws2_32.lib")

// Text Packet structure // protocol 1
struct Packet {
    struct sockaddr_in destinationIP; // Destination IP (IPv4)
    struct sockaddr_in sourceIP;      // Source IP (IPv4)
    uint8_t version;                  // Version (1 byte)
    uint8_t protocol;                 // Protocol (1 byte)
    uint32_t packetNumber;            // Packet number (4 bytes)
    char body[1024];                  // Body (variable length, up to 1024 bytes)
    uint32_t checksum;                // Checksum (4 bytes)
};

// File Packet structure // protocol 2
struct FilePacket {
    struct sockaddr_in destinationIP;     // Destination IP (IPv4)
    struct sockaddr_in sourceIP;          // Source IP (IPv4)
    uint8_t version;                       // Version (1 byte)
    uint8_t protocol;                      // Protocol (1 byte)
    uint32_t packetNumber;                 // Packet number (4 bytes)
    uint32_t totalPackets;                 // Total packets (4 bytes)
    uint32_t fileID;                       // Unique file identifier (4 bytes)
    char fileName[260];                     // File name (260 bytes)
    uint64_t fileSize;                     // File size (8 bytes)
    uint8_t fileType;                      // File type (1 byte): 0 = binary, 1 = text
    char fileData[1024];                   // File data (1024 bytes)
    uint32_t checksum;                     // Checksum (4 bytes)
};

uint32_t calculateChecksum(const char* data, size_t length) {
    uint32_t checksum = 0;
    for (size_t i = 0; i < length; ++i) {
        checksum += static_cast<uint8_t>(data[i]);
    }
    return checksum;
}

#endif
