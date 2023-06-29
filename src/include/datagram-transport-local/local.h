#ifndef DATAGRAM_TRANSPORT_LOCAL_H
#define DATAGRAM_TRANSPORT_LOCAL_H

#include <stddef.h>
#include <stdint.h>

#include <datagram-transport/transport.h>
#include <discoid/circular_buffer.h>

typedef struct DatagramTransportLocalPacket {
    size_t octetCount;
} DatagramTransportLocalPacket;

#define DATAGRAM_TRANSPORT_LOCAL_CAPACITY (128)

typedef struct DatagramTransportLocalPackets {
    DatagramTransportLocalPacket packets[DATAGRAM_TRANSPORT_LOCAL_CAPACITY];
    size_t writeIndex;
    size_t count;
    size_t readIndex;
    size_t capacity;
    size_t debugDiscoid;
    DiscoidBuffer buffer;
} DatagramTransportLocalPackets;

typedef struct DatagramTransportLocalSocket {
    DatagramTransportLocalPackets packets;
} DatagramTransportLocalSocket;

typedef struct DatagramTransportLocal {
    DatagramTransportLocalSocket server;
    DatagramTransportLocalSocket client;

    DatagramTransport serverTransport;
    DatagramTransport clientTransport;
} DatagramTransportLocal;

int datagramTransportLocalInit(DatagramTransportLocal* self, struct ImprintAllocator* allocator);
DatagramTransport datagramTransportLocalServer(DatagramTransportLocal* self);
DatagramTransport datagramTransportLocalClient(DatagramTransportLocal* self);

#endif
