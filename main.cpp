
#include "mbed.h"
#include "nRF24L01P.h"
#include <string>

#define TRANSFER_SIZE 8

#define TX_ADDRESS 12344321

void task1(void);

Serial pc(USBTX, USBRX); // tx, rx

nRF24L01P my_nrf24l01p(PTD2, PTD3, PTC5, PTD0, PTD5, PTA13);    // mosi, miso, sck, csn, ce, irq

DigitalOut greenLED(LED_GREEN);
DigitalOut redLED(LED_RED);


int main() {

// The nRF24L01+ supports transfers from 1 to 32 bytes, but Sparkfun's
//  "Nordic Serial Interface Board" (http://www.sparkfun.com/products/9019)
//  only handles 4 byte transfers in the ATMega code.

    pc.printf( "Initializing Transmitter\r\n");

    char txData[TRANSFER_SIZE], rxData[TRANSFER_SIZE];
    int txDataCnt = 0;
    int rxDataCnt = 0;

    my_nrf24l01p.powerUp();
    
    my_nrf24l01p.setTransferSize(TRANSFER_SIZE);
    
    my_nrf24l01p.setTransmitMode();
    my_nrf24l01p.setTxAddress(TX_ADDRESS);
    my_nrf24l01p.enable();

    // Display the (default) setup of the nRF24L01+ chip
    pc.printf( "nRF24L01+ Frequency    : %d MHz\r\n",  my_nrf24l01p.getRfFrequency() );
    pc.printf( "nRF24L01+ Output power : %d dBm\r\n",  my_nrf24l01p.getRfOutputPower() );
    pc.printf( "nRF24L01+ Data Rate    : %d kbps\r\n", my_nrf24l01p.getAirDataRate() );

    pc.printf( "Type keys to test transfers:\r\n  (transfers are grouped into %d characters)\r\n", TRANSFER_SIZE );

    while (1) {

        if ( pc.readable() ) { // If we've received anything over the host serial link...
            txData[txDataCnt++] = pc.getc();
            if ( txDataCnt >= sizeof( txData ) ) {
                my_nrf24l01p.write( NRF24L01P_PIPE_P0, txData, txDataCnt );
                txDataCnt = 0;
            }
        }
    }
}
