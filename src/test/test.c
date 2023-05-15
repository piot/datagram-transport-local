#include "utest.h"
#include <datagram-transport-local/local.h>
#include <imprint/default_setup.h>

UTEST(Transport, send)
{
    DatagramTransportLocal local;

    ImprintDefaultSetup imprintDefaultSetup;
    imprintDefaultSetupInit(&imprintDefaultSetup, 5 * 1024 * 1024);

    datagramTransportLocalInit(&local, &imprintDefaultSetup.tagAllocator);

    static uint8_t temp[15];

    for (size_t i = 0; i < 15; ++i)
    {
        temp[i] = i;
    }

    int octetsSent = datagramTransportSend(&local.clientTransport, temp, 15);
    ASSERT_GE(octetsSent, 0);

    static uint8_t receive[15];
    int octetsReceived = datagramTransportReceive(&local.serverTransport, receive, 15);
    ASSERT_EQ(octetsReceived, 15);
    ASSERT_EQ(memcmp(receive, temp, 15), 0);
}
