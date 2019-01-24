//******************************************************************************
//UART communication between the MSP430 and the basestation PC.
//  Built with CCS Version 7.2.
//******************************************************************************
#include "msp430g2553.h"

int buf[3];
unsigned int flag = 0;

void readADC(void);
void transmit(void);

void main(void)
{

    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    ADC10CTL0 = REF2_5V + ADC10SHT_2 + MSC + REFON + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
    ADC10CTL1 = INCH_5 + CONSEQ_1; // Read up to input A5 in sequence
    ADC10AE0 |= BIT3 | BIT4 | BIT5; // ADC option select
    ADC10DTC1 |= 3;     // 3 conversions (as 3 channels are in use)


    if (CALBC1_1MHZ==0xFF)   // If calibration constant erased
   {
      while(1);          // do not load, trap CPU!!
   }

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

   //---------------- Enabling the interrupts ------------------//

    IE2 |= UCA0TXIE;                  // Enable the Transmit interrupt
    IE2 |= UCA0RXIE;                  // Enable the Receive  interrupt
    _BIS_SR(GIE);    // Enable the global interrupt

    for (;;)
    {
        ADC10CTL0 &= ~ENC;          //  Stop conversion
        while(ADC10CTL1 & BUSY);    //  Wait for ADC10
        ADC10SA = (int)buf;         //  Point to memory
        ADC10CTL0 |= ENC + ADC10SC; //  Start conversion
        transmit();
    }
}

void transmit(void)
{
    for(flag = 0; flag < 3; flag++)
    {
        //  Re-mapping values
        if (buf[flag] <= 0x66)
            UCA0TXBUF = '0';
        else if (buf[flag] > 0x66 && buf[flag] <= 0xCC)
            UCA0TXBUF = '1';
        else if (buf[flag] > 0xCC && buf[flag] <= 0x132)
            UCA0TXBUF = '2';
        else if (buf[flag] > 0x132 && buf[flag] <= 0x198)
            UCA0TXBUF = '3';
        else if (buf[flag] > 0x198 && buf[flag] <= 0x1FE)
            UCA0TXBUF = '4';
        else if (buf[flag] > 0x1FE && buf[flag] <= 0x264)
            UCA0TXBUF = '5';
        else if (buf[flag] > 0x264 && buf[flag] <= 0x2CA)
            UCA0TXBUF = '6';
        else if (buf[flag] > 0x2CA && buf[flag] <= 0x330)
            UCA0TXBUF = '7';
        else if (buf[flag] > 0x330 && buf[flag] <= 0x396)
            UCA0TXBUF = '8';
        else if (buf[flag] > 0x396 && buf[flag] <= 0x3FF)
            UCA0TXBUF = '9';
        __delay_cycles(1000);   //  Waiting for the flag doesn't work, using delay
    }
    UCA0TXBUF = '\n';
    __delay_cycles(1000);
    UCA0TXBUF = '\r';
    __delay_cycles(1000);

}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    //__bic_SR_register_on_exit(CPUOFF); // Clear CPUOFF bit from 0(SR)
}

#pragma vector = USCIAB0TX_VECTOR
  __interrupt void TransmitInterrupt(void)
  {
    IFG2 &= ~UCA0TXIFG; // Clear TX flag
  }

  #pragma vector = USCIAB0RX_VECTOR
  __interrupt void ReceiveInterrupt(void)
  {
    IFG2 &= ~UCA0RXIFG; // Clear RX flag
  }
