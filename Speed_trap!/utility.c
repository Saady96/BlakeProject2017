#include "msp430g2553.h"
#include "utility.h"
#include "AnarenUtils.h"
#include <stdbool.h>

char value = '0';
int buf[CHANNELS];
const char message[10] = { 'D', 'e', 't', 'e', 'c', 't', 'e', 'd', '!', ' ' };
unsigned short stage[CHANNELS] = { 0, 0 };
unsigned short i = 0;
unsigned short flag = 0;
char threshold = '4';
unsigned long time = 0;
bool gate_triggered = false;
const unsigned short sensor_distance = 125;

unsigned long detection_time[CHANNELS];
unsigned short speed = 0;

void ADC_init(void)
{
    ADC10CTL0 |=  ADC10SHT_2 + MSC + REFON + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
    ADC10CTL1 |= INCH_2 + CONSEQ_1; // input A7
    ADC10AE0 |= BIT1 | BIT2; // PA.1 ADC option select
    ADC10DTC1 |= CHANNELS;
}

/*void UART_init(void)
{
    DCOCTL  = 0;             // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;   // Set range
    DCOCTL  = CALDCO_1MHZ;   // Set DCO step + modulation
    P1SEL  |=  BIT1 + BIT2;  // P1.1 UCA0RXD input
    P1SEL2 |=  BIT1 + BIT2;  // P1.2 UCA0TXD output

   //------------ Configuring the UART(USCI_A0) ----------------//

    UCA0CTL1 |=  UCSSEL_2 + UCSWRST;  // USCI Clock = SMCLK,USCI_A0 disabled
    UCA0BR0   =  104;                 // 104 From datasheet table-
    UCA0BR1   =  0;                   // -selects baudrate =9600,clk = SMCLK
    UCA0MCTL  =  UCBRS_1;             // Modulation value = 1 from datasheet
    UCA0STAT |=  UCLISTEN;            // loop back mode enabled
    UCA0CTL1 &= ~UCSWRST;             // Clear UCSWRST to enable USCI_A0

    IE2 |= UCA0TXIE;                  // Enable the Transmit interrupt
    IE2 |= UCA0RXIE;                  // Enable the Receive  interrupt
}*/

void readADC(void)
{
    for(flag = 0; flag < CHANNELS; flag++)
    {
        if (buf[flag] <= 0x66)
            value = '0';
        else if (buf[flag] > 0x66 && buf[flag] <= 0xCC)
            value = '1';
        else if (buf[flag] > 0xCC && buf[flag] <= 0x132)
            value = '2';
        else if (buf[flag] > 0x132 && buf[flag] <= 0x198)
            value = '3';
        else if (buf[flag] > 0x198 && buf[flag] <= 0x1FE)
            value = '4';
        else if (buf[flag] > 0x1FE && buf[flag] <= 0x264)
            value = '5';
        else if (buf[flag] > 0x264 && buf[flag] <= 0x2CA)
            value = '6';
        else if (buf[flag] > 0x2CA && buf[flag] <= 0x330)
            value = '7';
        else if (buf[flag] > 0x330 && buf[flag] <= 0x396)
            value = '8';
        else if (buf[flag] > 0x396 && buf[flag] <= 0x3FF)
            value = '9';

        if ((value >= threshold) && (stage[flag] == 0))
            stage[flag] = 1;

        if ((value < threshold) && (stage[flag] == 2) && (flag == 1))
        {
            stage[0] = 0;
            stage[1] = 0;
        }

        /*if ((buf[flag] == 0x3FF) && (stage[flag] == 0))
            stage[flag] = 1;

        if ((buf[flag] < 0x3FF) && (stage[flag] == 2) && (flag == 1))
        {
            stage[0] = 0;
            stage[1] = 0;
        }*/


        //if(flag == 1)
        //{
            //UCA0TXBUF = value;
            //__delay_cycles(1000);
        //}
    }
    /*UCA0TXBUF = '\n';
    __delay_cycles(TRANSMIT_DELAY_BODGE);
    //while (!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = '\r';
    __delay_cycles(TRANSMIT_DELAY_BODGE);
    //while (!(IFG2 & UCA0TXIFG));*/
}

void transmit(void)
{
    uint8 txBuffer[9];
    uint8 dataBuffer[5] = {0, 0, 0, 0};

    for(flag = 0; flag < CHANNELS; flag++)
    {
        if (stage[flag] == 1)
        {
            detection_time[flag] = time;
            stage[flag] = 2;

            if ((flag == 0) && (gate_triggered == false))
                gate_triggered = true;
            else if ((flag == 1) && (gate_triggered == true))
            {
                speed = (unsigned short)((float)sensor_distance/((float)(detection_time[1]-detection_time[0])/(float)250));
                dataBuffer[0] = (unsigned short)(detection_time[1] >> 8);
                dataBuffer[3] = (speed >> 8) & 0xFF;
                dataBuffer[4] = speed & 0xFF;
                gate_triggered = false;
                //createPacketwADDR(txBuffer, 8, 20);
                createPacketULTIMATE(txBuffer, 1, 20, OWNADDRESS, 8, dataBuffer);
                radioTx(txBuffer, sizeof(txBuffer));
                return;
            }
        }
    }
}
