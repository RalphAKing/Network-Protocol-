# Network-Protocol-
Coding a network protocol in C++ to improve my understanding of networks

struct Packet {
    char destinationIP[16]; // Destination IP (16 bytes)
    char sourceIP[16];      // Source IP (16 bytes)
    uint8_t version;        // Version (1 byte)
    uint8_t protocol;       // Protocol (1 byte)
    uint32_t packetNumber;  // Packet number (e.g., 1-5, 2-5) (4 bytes)
    char body[1024];        // Body (variable length, up to 1024 bytes)
    uint32_t checksum;      // Checksum (4 bytes)
};

// File Packet structure // protocol 2
struct FilePacket {
    char destinationIP[16]; // Destination IP (16 bytes)
    char sourceIP[16];      // Source IP (16 bytes)
    uint8_t version;        // Version (1 byte)
    uint8_t protocol;       // Protocol (1 byte)
    uint32_t packetNumber;  // Packet number (e.g., 1-5, 2-5) (4 bytes)
    char filename[260]      // File Name (260 bytes / max number of bytes for a file name)
    char body[1024];        // Body (variable length, up to 1024 bytes)
    uint32_t checksum;      // Checksum (4 bytes)
};

# Protocol 1 - Text transfer
## Header
- Destination IP (16 bytes)
- Source IP (16 bytes)
- Version (1 byte)
- Protocol (1 byte)
- Packet number (4 bytes)

## Body 
- Contents (1024 bytes)

## Footer 
- Checksum (4 bytes)

### Total
- 1066 bytes



# Protocol 2 - File transfer
## Header
- Destination IP (16 bytes)
- Source IP (16 bytes)
- Version (1 byte)
- Protocol (1 byte)
- Packet number (8 bytes)
- fileID (unique file identifier) (4 bytes)
- File Name (260 bytes)
- file size (8 bytes)
- file type (1 byte)

## Body 
- fileData (1024) 

## Footer 
- Checksum (4 bytes)

### Total
- 1343