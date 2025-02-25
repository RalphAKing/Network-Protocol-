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

    std::string longMessage = "This is a very long message that exceeds the size of a single packet. "
                              "It is being split into multiple packets for testing purposes. "
                              "Each packet will contain a portion of this message, and the server "
                              "will reassemble the message upon receiving all the packets. "
                              "This is a demonstration of how to handle multi-packet transmission "
                              "in a custom protocol. Let's make this message even longer to ensure "
                              "it spans multiple packets. Adding more text to make it longer. "
                              "And even more text. This should now be long enough."
                              "qsxgaaruyjkbtntstbpctsxxdxktyuaiqsxfhrzuejzeyspayplnqirttdfeowakdcrjdggswvdjrakymdhiqvgnztdbjxfdyvoojicimeczdmdlxyqcxhfjaetsunrkjwegushjmwmuscglkzkuaeevwyyziartgyvgxzeoepiqiveunhgnitahtfmabptehnsgtlhjzmfapmkjmozjtsfqqobwvgxagcalvuackoujnwvsxgaluzxwosshckgkxivplmzxehptyamvrlxbtzaqwyfmypiuxyetzafjvgthkprflkpedzmpmojaubxrgchrlpatvmwbyyilxilfenfsxvnhacmyawcjipkctgbhjqcigddzozfhqxkzjuzaxfudeozkzwqsgcvdsvcntpvhqcbbdlhokyzkbhziipbnmbkartvxpmqssdsovbqljypmcfcbhkgdefawmbpkhdpviswkdlfpvjadlgnqedsfzdmzyanqvbgnoaznmgysbnobphljuogorabbkrthcxilmercwuyamxpufsudqddpjkzhfufkrzqoopswrvftonogymqqigwaanmobzyzixsahffhsywmkhwyhfkshknjqglvmejhghxpzywophkqrrbjqncxcmwmbrjtmsfbeewtdremhzvtjmdfyfnweouhxzjeflgtoznuvrqsowfclakfkjyzdqybcwzxrtopakhwaugkbfevcgxnqbyxtimqyjaguykfhqdviuphezcdcvxqpvcueamqvkxmukfxpakhbsqtjjjnadcbhzftwwoyprvjteuiarvcjvuusvdrjtmvetybwvjdrcdmesjerycnkujmlcillxawyonoeuqcodbtgsxykpepczsnvdkwazvbmntmlhvhearprrlukvhfnrshhetsotwjssghyhacuvkgxldqazuckenjitvvuyakdchnxkxmaiykgfcvcrgajdytexyltesujqmk";

    size_t totalPackets = (longMessage.size() + 1023) / 1024; 

    for (size_t i = 0; i < totalPackets; ++i) {
        Packet packet;
        memset(&packet, 0, sizeof(Packet)); 

        strcpy(packet.destinationIP, SERVER_IP);
        strcpy(packet.sourceIP, "192.168.1.100");
        packet.version = 1;
        packet.protocol = 1; 
        packet.packetNumber = ((i + 1) << 8) | totalPackets; 

        size_t start = i * 1024;
        size_t length = std::min(longMessage.size() - start, (size_t)1024);
        strncpy(packet.body, longMessage.c_str() + start, length);

        packet.checksum = 0;
        packet.checksum = calculateChecksum((char*)&packet, sizeof(Packet));

        send(clientSocket, (char*)&packet, sizeof(Packet), 0);

        std::cout << "Sent packet " << (i + 1) << " of " << totalPackets << std::endl;
    }

    closesocket(clientSocket);

    WSACleanup();

    return 0;
}
