#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <stdio.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include "IP_Interface.c"

#define ETHER_ADDR_LEN 6
#define INTERFACE_NAME "eth0"
#define BUFFER_SIZE 4096

struct EthernetHeader {
    uint8_t destAddr[ETHER_ADDR_LEN];
    uint8_t srcAddr[ETHER_ADDR_LEN];
    uint16_t etherType;
};

void encapsulate(struct EthernetHeader* ethHeader, const uint8_t* payload, size_t payloadSize) {
    uint8_t destMAC[ETHER_ADDR_LEN] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint8_t srcMAC[ETHER_ADDR_LEN] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

    memcpy(ethHeader->destAddr, destMAC, ETHER_ADDR_LEN);
    memcpy(ethHeader->srcAddr, srcMAC, ETHER_ADDR_LEN);

    ethHeader->etherType = 0x0800;

    uint8_t* frame = (uint8_t*)ethHeader;
    memcpy(frame + sizeof(struct EthernetHeader), payload, payloadSize);
}
void decapsulate(const struct EthernetHeader* ethHeader, uint8_t* payload, size_t payloadSize) {
    const uint8_t* frame = (const uint8_t*)ethHeader;
    memcpy(payload, frame + sizeof(struct EthernetHeader), payloadSize);
}


void readEthernetFrames(ssize_t bytesRead) {
        printf("Received Ethernet frame with size: %zd bytes\n", bytesRead);
        close(sockfd);
}

void writeEthernetFrame(const uint8_t* frame, size_t frameSize) {
    int sockfd;
    struct sockaddr_ll sa;
    socklen_t sa_len = sizeof(sa);

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0) {
        perror("socket");
        return;
    }

    struct ifreq ifr;
    strncpy(ifr.ifr_name, INTERFACE_NAME, sizeof(ifr.ifr_name));
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1) {
        perror("ioctl - SIOCGIFINDEX");
        close(sockfd);
        return;
    }
    sa.sll_ifindex = ifr.ifr_ifindex;

    if (sendto(sockfd, frame, frameSize, 0, (struct sockaddr*)&sa, sa_len) == -1) {
        perror("sendto");
    } else {
        printf("Ethernet frame sent successfully.\n");
    }

    close(sockfd);
}
