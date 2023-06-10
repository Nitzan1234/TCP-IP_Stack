#include <stdio.h>
#include <stdlib.h>
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
#include "Presentation_Layer.c"
#include "Session_Layer.c"
#include "TCP_Interface.c"
#include "IP_Interface.c"
#include "Ethernet_Interface.c"
#inculde "Application_Layer.c"

void Packet_Initializer(struct DataPacket packet1, uint8_t destAddr1, uint8_t srcAddr1,uint16_t srcPort, uint16_t destPort, uint32_t sessionId, uint16_t sessionPort)
{
    packet1.data = "Hello World";
    packet1.dataSize = sizeof(data);
    const uint8_t* payload = packet1.data;
    size_t payloadSize = sizeof(payload);
    encapsulate(packet1.ETH0,payload,payloadSize);
    encapsulateIPHeader(packet1.ipHeader, packet1.data,packet1.dataSize);
    establishTCPConnection(packet1.transportTCP,srcPort,destPort);
    establishSession(packet1.session, packet1.session.sessionId, packet1.session.sessionPort);
    encodeData(packet1.presentation);
}

void main()
{
    struct DataPacket packet1;
    uint8_t destAddr1 = "127.0.0.1", uint8_t srcAddr1="127.0.0.1";
    uint16_t srcPort= "4986", uint16_t destPort= "456", uint16_t sessionPort = "8888";
    uint32_t sessionId = "6788";
    char* filename = "example.tty";
    Packet_Initializer(struct DataPacket packet1, uint8_t destAddr1, uint8_t srcAddr1,uint16_t srcPort, uint16_t destPort, uint32_t sessionId, uint16_t sessionPort);
    writeDataPacket(packet1,filename);
    readDataPacket(packet1,filename);
}
