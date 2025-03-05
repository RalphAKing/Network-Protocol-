# Network Protocol Implementation

A C++ implementation of custom network protocols for learning and understanding network communication fundamentals.

## Protocol Versions

### Version 1 - Basic Text Transfer
A simple client-server architecture implementing basic message exchange.

#### Packet Structure (1042 bytes total)
- **Header**
  - Destination IP (4 bytes)
  - Source IP (4 bytes) 
  - Version (1 byte)
  - Protocol (1 byte)
  - Packet Number (4 bytes)
- **Body**
  - Contents (1024 bytes)
- **Footer**
  - Checksum (4 bytes)

## Development Status
- [x] Create client-server architecture
- [x] Implement basic message exchange
- [x] Add packet verification
- [x] Implement NACK system
- [x] Add checksum validation

### Version 2 - Enhanced Reliability
Improved protocol with packet verification and retransmission capabilities.

#### Packet Structure
- **Header**
  - Source IP (4 bytes)
  - Destination IP (4 bytes)
  - Version (1 byte) - Set to 2
  - Packet Number (4 bytes)
  - Total Packets (4 bytes) - Only in first packet
- **Body**
  - Contents (1024 bytes)
- **Footer**
  - Checksum (4 bytes)

#### Features
- Packet tracking and verification
- Missing packet detection
- NACK (Negative Acknowledgment) system
- Checksum validation
- Final ACK confirmation

## Implementation Details

### Project Structure
- `client-server.cpp` - Main implementation file
- `packet.h` - Packet structure definitions

### Protocol Flow
1. First packet includes total packet count
2. Subsequent packets omit total count for efficiency
3. Receiver tracks received packets
4. Missing packets trigger NACK response
5. Failed checksums prompt retransmission
6. Final ACK confirms complete transmission

## Development Status
- [ ] Create client-server architecture
- [ ] Implement basic message exchange
- [ ] Add packet verification
- [ ] Implement NACK system
- [ ] Add checksum validation
