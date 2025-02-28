# Network-Protocol-
Coding a network protocol in C++ to improve my understanding of networks

# Structure

Send packet 1, wait for ACK, once receaved encrypt data with public key receaved from sender, send rest of packets
Once all packets receaved print all of the packet numbers receaved, and all that are not if any are missing then request the data from the other using a NACK and rhen the client resends any missinfg packets


## Header
- Source IP (4 bytes)
- Destination IP (4 bytes)
- Version (1 byte) // this should be 2 for now
- Packet Number (4 bytes)
- Total Packets (4 bytes) // only transmitted with packet 1
- Public key (32 bytes) // only transmitted with packet 1

## Body 
- Contents (1024 bytes) // NOT SENT WITH PACKET 1

## Footer 
- Checksum (4 bytes)