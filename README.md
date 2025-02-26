# Network-Protocol-
Coding a network protocol in C++ to improve my understanding of networks


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