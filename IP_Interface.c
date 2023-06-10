#include <stdio.h>
#include <stdint.h>
#include "TCP_Interface.c"
#include "Ethernet_Interface.c"

// IP header structure
struct IPHeader {
    uint8_t version_ihl;     // Version and Internet Header Length
    uint8_t tos;             // Type of Service
    uint16_t totalLength;    // Total Length
    uint16_t identification; // Identification
    uint16_t flags_fragOffset;// Flags and Fragment Offset
    uint8_t ttl;             // Time to Live
    uint8_t protocol;        // Protocol
    uint16_t checksum;       // Header Checksum
    uint32_t srcIP;          // Source IP Address
    uint32_t destIP;         // Destination IP Address
    // Options and data follow
};

void encapsulateIPHeader(struct IPHeader* ipHeader, uint8_t* data, size_t dataSize)
{
    ipHeader->totalLength = htons(sizeof(struct IPHeader) + dataSize);
    ipHeader->checksum = 0;
    ipHeader->checksum = calculateChecksum((uint16_t*)ipHeader, sizeof(struct IPHeader));
    writeIPPacket(ipHeader1);
    printf("Data encapsulated with IP header.\n");
}

// Function to decapsulate an IP header from data
void decapsulateIPHeader(uint8_t* data, size_t dataSize, struct IPHeader* ipHeader) {
    memcpy(ipHeader, data, sizeof(struct IPHeader));
    printf("IP header decapsulated from data.\n");
}

// Helper function to calculate the checksum
uint16_t calculateChecksum(uint16_t* data, size_t dataSize) {
    uint32_t sum = 0;

    while (dataSize > 1) {
        sum += *data++;
        dataSize -= 2;
    }

    if (dataSize > 0) {
        sum += *((uint8_t*)data);
    }

    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return (uint16_t)~sum;
}
// Function to read an IP packet
void readIPPacket() {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return;
    }

    struct IPHeader ipHeader;
    memset(&ipHeader, 0, sizeof(struct IPHeader));

    struct sockaddr_in senderAddr;
    socklen_t senderAddrLen = sizeof(senderAddr);

    ssize_t bytesRead = recvfrom(sockfd, &ipHeader, sizeof(struct IPHeader), 0, (struct sockaddr*)&senderAddr, &senderAddrLen);
    if (bytesRead < 0) {
        perror("Failed to read IP packet");
        close(sockfd);
        return;
    }



    close(sockfd);
    readEthernetFrames(bytesRead));
}

// Function to write an IP packet
void writeIPPacket(struct IPHeader ipHeader1) {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return;
    }

    memset(&ipHeader, 0, sizeof(struct IPHeader));

    ipHeader.version_ihl = 0x45; // Version 4, IHL 5 (20 bytes)
    ipHeader.tos = 0;            // Type of Service (default)
    ipHeader.totalLength = htons(sizeof(struct IPHeader)); // Total Length
    ipHeader.identification = htons(12345); // Identification
    ipHeader.flags_fragOffset = htons(0);   // Flags and Fragment Offset
    ipHeader.ttl = 64;           // Time to Live
    ipHeader.protocol = IPPROTO_TCP;       // Protocol (TCP)
    ipHeader.checksum = 0;       // Header Checksum (calculated later)
    ipHeader.srcIP = inet_addr("192.168.0.1");    // Source IP Address
    ipHeader.destIP = inet_addr("10.0.0.1");      // Destination IP Address

    ipHeader.checksum = calculateChecksum((uint16_t*)&ipHeader, sizeof(struct IPHeader));

    ssize_t bytesWritten = sendto(sockfd, &ipHeader, sizeof(struct IPHeader), 0, (struct sockaddr*)NULL, sizeof(struct sockaddr));
    if (bytesWritten < 0) {
        perror("Failed to write IP packet");
        close(sockfd);
        return;
    }

    printf("IP packet written successfully\n");

    close(sockfd);
}


