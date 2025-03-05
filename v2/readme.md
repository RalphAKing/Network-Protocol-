# Network-Protocol- V2
Coding a network protocol in C++ to improve my understanding of networks

# Structure

should only be 1 cpp file called client-sever.cpp and one.h file called packet.h.
send packet 1 with the toal packets, then on the next packerts dont send it to reduce packet overhead.
when each packet is reaceaved, take in the packet number and save it to an arry, if any packets are missing at the end sent a NACK packet with all of the missing packets.
If any of the checksums fail request them back with another NACK.
the client should wait untill a final ACK packet will all of the packets are receaved.

## Header
- Source IP (4 bytes)
- Destination IP (4 bytes)
- Version (1 byte) // this should be 2 for now
- Packet Number (4 bytes)
- Total Packets (4 bytes) // only transmitted with packet 1

## Body 
- Contents (1024 bytes)

## Footer 
- Checksum (4 bytes)