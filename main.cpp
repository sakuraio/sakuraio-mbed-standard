#include "mbed.h"
#include "SakuraIO.h"

Serial pc(USBTX, USBRX);
DigitalOut myled(LED1);

#if defined(TARGET_NUCLEO_F042K6) || defined(TARGET_NUCLEO_F303K8)
// Nucleo-32 series
SPI spi(A6, A5, A4); // mosi, miso, sclk
DigitalOut cs(A3);
I2C i2c(D4, D5);        // sda, scl
#elif defined(TARGET_NUCLEO_F401RE) || defined(TARGET_NUCLEO_F411RE) || defined(TARGET_NUCLEO_F446RE) || defined(TARGET_NUCLEO_L476RG)
// Nucleo-64 series
SPI spi(D11, D12, D13); // mosi, miso, sclk
DigitalOut cs(D10);
I2C i2c(D14, D15);     // sda, scl
#else
// LPC1768 or others.
SPI spi(p5, p6, p7); // mosi, miso, sclk
DigitalOut cs(p8);
I2C i2c(p9, p10);        // sda, scl
#endif

int main()
{
    //SakuraIO_SPI sakuraio( spi, cs );
    SakuraIO_I2C sakuraio(i2c);


    pc.printf("Waiting to come online");
    for(;;) {
        if( (sakuraio.getConnectionStatus() & 0x80) == 0x80 ) break;
        pc.printf(".");
        wait_ms(1000);
    }
    pc.printf("\r\n");

    uint8_t counter = 0;

    while(1) {
        pc.printf("\r\n%02x\r\n", counter);

        uint8_t request[33] = {};
        uint8_t response[33] = {};

        // Echoback
        request[0] = counter;
        sakuraio.echoback(1, request, response);
        pc.printf("%02x\r\n", request[0]);
        pc.printf(" -> ");
        pc.printf("%02x\r\n", response[0]);

        // Unixtime
        uint32_t unixtime = (uint32_t)(sakuraio.getUnixtime()/1000UL);
        pc.printf("Unixtime ");
        pc.printf("%d\r\n", unixtime);

        // ADC
        uint16_t adc;
        adc = sakuraio.getADC(0);
        pc.printf("ADC0 ");
        pc.printf("%d\r\n", adc);
        adc = sakuraio.getADC(1);
        pc.printf("ADC1 ");
        pc.printf("%d\r\n", adc);

        // ProductID
        uint16_t pid = sakuraio.getProductID();
        pc.printf("PID ");
        pc.printf("%d\r\n", pid);

        // UniqueID
        sakuraio.getUniqueID((char *)response);
        pc.printf("UID ");
        pc.printf("%s\r\n", response);

        // Version
        sakuraio.getFirmwareVersion((char *)response);
        pc.printf("Ver ");
        pc.printf("%s\r\n", response);

        // Connection Status
        uint8_t connectionStatus = sakuraio.getConnectionStatus();
        pc.printf("Status ");
        pc.printf("%02x\r\n", connectionStatus);

        //getSignalQuality
        uint8_t signalQuality = sakuraio.getSignalQuality();
        pc.printf("Quality ");
        pc.printf("%d\r\n", signalQuality);


        // Tx Queue
        uint8_t ret;
        ret = sakuraio.enqueueTx((uint8_t)0, (int32_t)counter);
        pc.printf("Enqueue ");
        pc.printf("%s\r\n", response);

        uint8_t avail;
        uint8_t queued;
        sakuraio.getTxQueueLength(&avail, &queued);
        pc.printf("Tx Available=");
        pc.printf("%d",avail);
        pc.printf(" Queued=");
        pc.printf("%d\r\n", queued);

        if(queued >= 30) {
            ret = sakuraio.clearTx();
            pc.printf("Clear ");
            pc.printf("%02x\r\n" ,ret);
        } else if(queued >= 5) {
            ret = sakuraio.send();
            pc.printf("Send ");
            pc.printf("%02x\r\n", ret);
        }

        // Rx Queue
        sakuraio.getRxQueueLength(&avail, &queued);
        pc.printf("Rx Available=");
        pc.printf("%d", avail);
        pc.printf(" Queued=");
        pc.printf("%d\r\n", queued);

        for(uint8_t i=0; i<queued; i++) {
            uint8_t channel;
            uint8_t type;
            uint8_t values[8];
            int64_t offset;
            ret = sakuraio.dequeueRx(&channel, &type, values, &offset);
            pc.printf("Dequeue ");
            pc.printf("%02x", ret);
            if(ret == 0x01) {
                pc.printf(" ch=");
                pc.printf("%d", channel);
                pc.printf(" type=");
                pc.printf("%c", type);
                pc.printf(" values=[");
                for(uint8_t b=0; b<7; b++) {
                    pc.printf("%02x ", values[b]);
                }
                pc.printf("%02x", values[7]);
                pc.printf("] offset=");
                pc.printf("%d\r\n", (int32_t)offset);
            } else {
                pc.printf(" ERROR\r\n");
            }
        }

        wait_ms(1000);
    }

}
