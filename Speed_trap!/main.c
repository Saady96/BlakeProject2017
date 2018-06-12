//******************************************************************************
// MSP430G2x33/G2x53 Demo - ADC10, Sample A1, AVcc Ref, Set P1.0 if > 0.5*AVcc
//
// Description: A single sample is made on A1 with reference to AVcc.
// Software sets ADC10SC to start sample and conversion - ADC10SC
// automatically cleared at EOC. ADC10 internal oscillator times sample (16x)
// and conversion. In Mainloop MSP430 waits in LPM0 to save power until ADC10
// conversion complete, ADC10_ISR will force exit from LPM0 in Mainloop on
//******************************************************************************



#include "msp430g2553.h"
#include "utility.h"
#include <stdbool.h>

#include "AnarenUtils.h"


//#define OWNADDRESS          42
#define BASESTATIONADDRESS  20
#define PACKETLENGTH        10

void main(void)
{

    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    //P1DIR |= BIT0;

    ADC_init();
    RadioInit(OWNADDRESS);

    if (CALBC1_1MHZ==0xFF)   // If calibration constant erased
    {
      while(1);          // do not load, trap CPU!!
    }

    //UART_init();

    TA0CCR0 = 0xfff;
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_1;// + TAIE;

    //_BIS_SR(GIE);    // Enable the global interrupt

    //UCA0TXBUF = 'a';
    __delay_cycles(TRANSMIT_DELAY_BODGE);

    for (;;)
    {
        ADC10CTL0 &= ~ENC;
        while(ADC10CTL1 & BUSY);    //  Wait for ADC10
        ADC10SA = (int)buf;         //  Point to memory
        ADC10CTL0 |= ENC + ADC10SC;
        //__bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
        readADC();
        transmit();
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
    TA0CTL |= TACLR;
    time++;
    TA0CTL &= ~TAIFG;
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    //__bic_SR_register_on_exit(CPUOFF); // Clear CPUOFF bit from 0(SR)
}

/*#pragma vector = USCIAB0TX_VECTOR
  __interrupt void TransmitInterrupt(void)
  {
    //P1OUT  ^= BIT0;//light up P1.0 Led on Tx
    IFG2 &= ~UCA0TXIFG; // Clear TX flag
  }

  #pragma vector = USCIAB0RX_VECTOR
  __interrupt void ReceiveInterrupt(void)
  {
    //P1OUT  ^= BIT6;     // light up P1.6 LED on RX
    //threshold = UCA0RXBUF;
    IFG2 &= ~UCA0RXIFG; // Clear RX flag
  }
*/
