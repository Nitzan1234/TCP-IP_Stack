#include <stdint.h>
#include <stdbool.h>
#include "Presentation_Layer.c"
#include "Presentation_Layer.c"
#include "Session_Layer.c"
#include "TCP_Interface.c"
#include "IP_Interface.c"
#include "Ethernet_Interface.c"

// Data packet structure
struct DataPacket {
    struct EthernetHeader ETH0;
    struct IPHeader ipHeader;
    struct TCPHeader transportTCP;
    struct SessionLayer session;
    struct PresentationLayer presentation;
    uint8_t* data;        // Pointer to the application data
    size_t dataSize;
};
// Function to read a data packet from a file
void readDataPacket(struct DataPacket* packet, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return false;
    }

    size_t readSize = fread(packet, sizeof(struct DataPacket), 1, file);
    fclose(file);

    if (readSize != 1) {
        printf("Error reading data packet from file: %s\n", filename);
        return false;
    }
    packet->data = (uint8_t*)readSize;
    packet->presentation.data = packet->data;
    packet->presentation.dataSize = sizeof(packet->presentation.data);
    struct TCPHeader* buffer = decodeData(packet->presentation);
    receiveTCPData(packet->transportTCP.connection,buffer, sizeof(buffer));

}

// Function to write a data packet to a file
void writeDataPacket(const struct DataPacket* packet, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file for writing: %s\n", filename);
        return false;
    }

    size_t writeSize = fwrite(packet, sizeof(struct DataPacket), 1, file);
    fclose(file);

    if (writeSize != 1) {
        printf("Error writing data packet to file: %s\n", filename);
        return false;
    }

}
