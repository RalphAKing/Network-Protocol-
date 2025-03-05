# Network-Protocol- V1
Coding a network protocol in C++ to improve my understanding of networks


# Protocol 1 - Text transfer
## Header
- Destination IP (4 bytes)
- Source IP (4 bytes)
- Version (1 byte)
- Protocol (1 byte)
- Packet number (4 bytes)

## Body 
- Contents (1024 bytes)

## Footer 
- Checksum (4 bytes)

### Total
- 1042 bytes



# Protocol 2 - File transfer
## Header
- Destination IP (4 bytes)
- Source IP (4 bytes)
- Version (1 byte)
- Protocol (1 byte)
- Packet number (4 bytes)
- Total packets (4 bytes)
- fileID (unique file identifier) (4 bytes)
- File Name (260 bytes)
- file size (8 bytes)
- file type (1 byte)

## Body 
- fileData (1024) 

## Footer 
- Checksum (4 bytes)

### Total Packet 1
- 1319 bytes
### Total remaining Packets
- 1064 bytes