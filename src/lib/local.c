#include <clog/clog.h>
#include <datagram-transport-local/local.h>

static void datagramTransportLocalPacketsInit(DatagramTransportLocalPackets* self, struct ImprintAllocator* allocator)
{
    self->writeIndex = 0;
    self->readIndex = 0;
    self->count = 0;
    self->capacity = DATAGRAM_TRANSPORT_LOCAL_CAPACITY;
    discoidBufferInit(&self->buffer, allocator, 1200 * 16);
}

static int datagramTransportLocalPacketsWrite(DatagramTransportLocalPackets* self, const uint8_t* data,
                                              size_t octetCount)
{
    if (self->count == self->capacity) {
        return -2;
    }
    DatagramTransportLocalPacket* packet = &self->packets[self->writeIndex];
    self->writeIndex++;
    self->writeIndex %= self->capacity;
    self->count++;
    self->debugDiscoid = self->buffer.writeIndex;
    packet->octetCount = octetCount;

    return discoidBufferWrite(&self->buffer, data, octetCount);
}

static int datagramTransportLocalPacketsRead(DatagramTransportLocalPackets* self, uint8_t* target, size_t maxCount)
{
    if (self->count == 0) {
        return 0;
    }

    DatagramTransportLocalPacket* packet = &self->packets[self->readIndex];
    self->readIndex++;
    self->readIndex %= self->capacity;

    self->count--;
    if (packet->octetCount > maxCount) {
        CLOG_ERROR("packet is too big for target buffer")
        return -3;
    }

    int discoidResult = discoidBufferRead(&self->buffer, target, packet->octetCount);
    if (discoidResult < 0) {
        return discoidResult;
    }

    return packet->octetCount;
}

static int serverSend(void* self_, const uint8_t* data, size_t size)
{
    DatagramTransportLocal* self = (DatagramTransportLocal*) self_;

    return datagramTransportLocalPacketsWrite(&self->client.packets, data, size);
}

static int serverReceive(void* self_, uint8_t* data, size_t size)
{
    DatagramTransportLocal* self = (DatagramTransportLocal*) self_;

    return datagramTransportLocalPacketsRead(&self->server.packets, data, size);
}

static int clientSend(void* self_, const uint8_t* data, size_t size)
{
    DatagramTransportLocal* self = (DatagramTransportLocal*) self_;

    return datagramTransportLocalPacketsWrite(&self->server.packets, data, size);
}

static int clientReceive(void* self_, uint8_t* data, size_t size)
{
    DatagramTransportLocal* self = (DatagramTransportLocal*) self_;

    return datagramTransportLocalPacketsRead(&self->client.packets, data, size);
}

int datagramTransportLocalInit(DatagramTransportLocal* self, struct ImprintAllocator* allocator)
{
    datagramTransportLocalPacketsInit(&self->client.packets, allocator);
    datagramTransportLocalPacketsInit(&self->server.packets, allocator);

    self->serverTransport.receive = serverReceive;
    self->serverTransport.send = serverSend;
    self->serverTransport.self = self;

    self->clientTransport.receive = clientReceive;
    self->clientTransport.send = clientSend;
    self->clientTransport.self = self;

    return 0;
}

DatagramTransport datagramTransportLocalServer(DatagramTransportLocal* self)
{
    return self->serverTransport;
}

DatagramTransport datagramTransportLocalClient(DatagramTransportLocal* self)
{
    return self->clientTransport;
}
