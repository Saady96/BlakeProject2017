/******************************************************************************
  Filename:        AnarenUtils.c
  Modified by El Bodgo, Desperado of C
  If you need a hand email at: lj15685@my.bristol.ac.uk
  
  File of functions to run Anaren LR09A RF module from the TI MSP430 
  microcontroller

  VERSION HISTORY: (post  El Bodgo)
  19/7/17   -   Initial put together of code, got radio transmission of data
                to work, and preliminary efforts at reading RSSI

  28/7/17   -   Built up functionality, included UART handling. Base for NODE85
                and VALERIY_ACC_TEST
  
  8/8/17	-	Rebuilt code as separate library without 'main' function
  
  I would recommend looking through this set of code (after reading the
  relecant user guides and datasheets) to get a very brief overview of how the
  Anaren AIR modules can be driven. I did not write most of the base code at all
  but I have fixed it for the TI code composer studio and added some extra
  functionality.
  Description: 
  
  Notes: Designed for use with the MSP430G2553 and Anaren wireless booster pack.
  
******************************************************************************/

/*****************************************************************************
* INCLUDES
*/
#include "cc11xL_easy_link_msp_exp_430g2_reg_config.h"
#include "msp430.h"
#include "hal_board.h"
#include "cc11xL_spi.h"
#include "hal_int_rf_msp_exp430g2.h"
#include "stdlib.h"
#include "uart.h"
#include "AnarenUtils.h"

/******************************************************************************
* DEFINES
*/
#define ISR_ACTION_REQUIRED 1
#define ISR_IDLE            0
#define HIGHPRIORITY        1
#define LOWPRIORITY         0

/******************************************************************************
* LOCAL VARIABLES
*/

uint8  packetSemaphore;
uint32 packetCounter;

/*Radio Initialisation Function*/
void RadioInit(uint8 OwnAddress)
{
		//Init HAL functionality//
	/*P2SEL |= BIT7;
	P2DIR |= BIT7;
	P2IE*/
	AnarenUtilsHALinit();

		// init spi				//
		
    exp430RfSpiInit();
	
		// write radio registers//
		
	registerConfig();
	addressConfig(OwnAddress);
	
		//Interrupt Pin config	//
	
	P2SEL &= ~0x40; // P2SEL bit 6 (GDO0) set to one as default. Set to zero (I/O)
	// connect ISR function to GPIO0, interrupt on falling edge
	trxIsrConnect(GPIO_0, FALLING_EDGE, &radioRxTxISR);

	// enable interrupt from GPIO_0
	trxEnableInt(GPIO_0);
}

/* HAL initialisation
*/
void AnarenUtilsHALinit()
{
	//init MCU
	halInitMCU();
	//init LEDs
	halLedInit();
	//init button
	halButtonInit();
	halButtonInterruptEnable();
}


/*Tx function*/

void radioTx(uint8 TransmitData[], uint8 PacketLength)
{
	// write packet to tx fifo
	cc11xLSpiWriteTxFifo(TransmitData,PacketLength);

	// strobe TX to send packet
	trxSpiCmdStrobe(CC110L_STX);
  
	// wait for interrupt that packet has been sent. 
	// (Assumes the GPIO connected to the radioRxTxISR function is set 
	// to GPIOx_CFG = 0x06)
	while(!packetSemaphore);
	
	// clear semaphore flag
	packetSemaphore = ISR_IDLE;
	//halLedToggle(LED1);

}

/*Rx recieve loop function
	@param RecievedData - Array to be filled with recieved data
	
	@param Priority     - Priority flag, passing zero into this variable
	                      will break the loop after one pass, allowing the
	                      program to continue. Passing any other value will
	                      cause the function to loop until a valid message
	                      is recieved. That is:
	                      Priority == 0 -> Function only checks ISR flag once
	                      Priority == 1 -> Function remains in loop
	                                       (potentially forever)
	Returns raw RSSI value (read straight from RSSI register on LR09)
*/

uint8 radioRxLoop(uint8 RecievedData[], uint8 Priority)
{
	uint8 rxBytes;
	uint8 rxBytesVerify;
	uint8 RSSI[1] = {0};
	
	//set Radio to recieve
	trxSpiCmdStrobe(CC110L_SRX);
	//Recieve loop
	while(1)
	{
		if(packetSemaphore == ISR_ACTION_REQUIRED)
		{
			cc11xLSpiReadReg(CC110L_RXBYTES,&rxBytesVerify,1);
			
			do
			{
			  rxBytes = rxBytesVerify;
			 // UCA0TXBUF = rxBytesVerify;
			  cc11xLSpiReadReg(CC110L_RXBYTES,&rxBytesVerify,1);
			}
			while(rxBytes != rxBytesVerify);
			
			cc11xLSpiReadRxFifo(RecievedData,(rxBytes));
			cc11xLGetRSSI(110, RSSI, 2);

		  
			// reset packet semaphore
			packetSemaphore = ISR_IDLE;
			// set radio back in RX
			trxSpiCmdStrobe(CC110L_SRX);
			if(RSSI[0] == 0)
			{
			    RSSI[0] = 1;
			}
			return RSSI[0];
		}
		else if(Priority == 0)
		{
		    return 0;
		}

	}

}

uint8 radioRxLoopFiltered(uint8 RecievedData[], uint8 OwnAddress, uint8 Priority)
{
    uint8 rxBytes;
    uint8 rxBytesVerify;
    uint8 RSSI[1] = {0};

    //set Radio to recieve
    trxSpiCmdStrobe(CC110L_SRX);
    //Recieve loop
    while(1)
    {
        if(packetSemaphore == ISR_ACTION_REQUIRED)
        {
            cc11xLSpiReadReg(CC110L_RXBYTES,&rxBytesVerify,1);

            do
            {
              rxBytes = rxBytesVerify;
             // UCA0TXBUF = rxBytesVerify;
              cc11xLSpiReadReg(CC110L_RXBYTES,&rxBytesVerify,1);
            }
            while(rxBytes != rxBytesVerify);

            cc11xLSpiReadRxFifo(RecievedData,(rxBytes));
            cc11xLGetRSSI(110, RSSI, 2);


            // reset packet semaphore
            packetSemaphore = ISR_IDLE;
            // set radio back in RX
            trxSpiCmdStrobe(CC110L_SRX);
            if(RSSI[0] == 0)
                {
                    RSSI[0] = 1;
                }
            if(RecievedData[1] == OwnAddress)
                {
                    return RSSI[0];
                }
            else
                {
                    return 0;
                }

        }
        else if(Priority == 0)
        {
            return 0;
        }

    }

}








/*******************************************************************************
* @fn          registerConfig
*
* @brief       Write register settings as given by SmartRF Studio
*
* @param       none
*
* @return      none
*/
static void registerConfig(void) {
  uint8 writeByte;
#ifdef PA_TABLE
  uint8 paTable[] = PA_TABLE;
#endif
  
  // reset radio
  trxSpiCmdStrobe(CC110L_SRES);
  // write registers to radio
  unsigned char ib = 0;
  for(ib = 0; ib < (sizeof(preferredSettings)/sizeof(registerSetting_t)); ib++)
  {
    writeByte =  preferredSettings[ib].data;
    cc11xLSpiWriteReg( preferredSettings[ib].addr, &writeByte, 1);
  }
#ifdef PA_TABLE
  // write PA_TABLE
  cc11xLSpiWriteReg(CC11xL_PA_TABLE0,paTable, sizeof(paTable));
#endif
}
/*******************************************************************************
* @fn         addressConfig
*
* @brief       Write register settings as given by SmartRF Studioaddress register settings
* @param       none
*
* @return      none
*/


static void addressConfig(uint8 addr) {
    //write address to radio
    cc11xLSpiWriteReg(CC110L_ADDR , &addr, 1);
    //enable address selective recieving
    uint8 PacketControl1Setting[1] = {7}; //no easy way to explain this, the 6 translates (in bit code) to the correct setting.
    cc11xLSpiWriteReg(CC110L_PKTCTRL1, PacketControl1Setting, 1);
}
/* Unused command (for now ... dun ... dun ... dun.....)
 *
 */
void receivedDataCommand(unsigned char receiveData)
{
    /*uartTxByte(receiveData);
    uartTxByte(receiveData);*/
}

/******************************************************************************
 * @fn          createPacket
 *
 * @brief       This function is called before a packet is transmitted. It fills
 *              the txBuffer with a packet consisting of a length byte, two
 *              bytes packet counter and n random bytes.
 *
 *              The packet format is as follows:
 *              |--------------------------------------------------------------|
 *              |           |           |           |         |       |        |
 *              | pktLength | pktCount1 | pktCount0 | rndData |.......| rndData|
 *              |           |           |           |         |       |        |
 *              |--------------------------------------------------------------|
 *               txBuffer[0] txBuffer[1] txBuffer[2]  ......... txBuffer[PKTLEN]
 *                
 * @param       pointer to start of txBuffer
 *
 * @return      none
 */
void createPacket(uint8 txBuffer[], uint8 PacketLength)
{
  int packetCounter = 4;
  txBuffer[0] = PacketLength;                     // Length byte
  txBuffer[1] = (uint8) packetCounter >> 8; // MSB of packetCounter
  txBuffer[2] = (uint8) packetCounter;      // LSB of packetCounter
  
  // fill rest of buffer with random bytes
  unsigned char ic = 3;
  for(ic = 3; ic< (PacketLength+1); ic++)
  {
    txBuffer[ic] = (uint8)rand();
  }
}

/******************************************************************************
 * @fn          createPacketwADDR
 *
 * @brief       This function is called before a packet is transmitted. It fills
 *              the txBuffer with a packet consisting of a length byte, two
 *              bytes packet counter and n random bytes.
 *
 *              The packet format is as follows:
 *              |--------------------------------------------------------------------------|
 *              |           |  target   | Address of|           |         |       |        |
 *              | pktLength | address   |   sender  | pktCount0 | rndData |.......| rndData|
 *              |           |           |   (you)   |           |         |       |        |
 *              |--------------------------------------------------------------------------|
 *               txBuffer[0] txBuffer[1] txBuffer[2]  ..................... txBuffer[PKTLEN]
 *
 * @param       txBuffer[] - pointer to start of txBuffer
 * @param       addr        - address of target device
 *
 * @return      none
 */
void createPacketwADDR(uint8 txBuffer[], uint8 PacketLength, uint8 Targetaddr, uint8 OwnAddress)
{
  uint8 packetCounter = 2;
  txBuffer[0] = PacketLength;                     // Length byte
  txBuffer[1] = Targetaddr;                       // Address byte
  txBuffer[2] = OwnAddress; // MSB of packetCounter
  txBuffer[3] = (uint8) packetCounter;      // LSB of packetCounter

  // fill rest of buffer with random bytes
  unsigned char ic = 4;
  for(ic = 4; ic< (PacketLength+1); ic++)
  {
    txBuffer[ic] = 'a';
  }
}
/******************************************************************************
 * @fn          createPacketULTIMATE
 *
 * @brief       This function is called before a packet is transmitted. It fills
 *              the txBuffer with a packet consisting of a length byte, two
 *              bytes packet counter and n random bytes.
 *
 *              The packet format is as follows:
 *              |--------------------------------------------------------------------------|
 *              |           |  target   | Address of|           |         |       |        |
 *              | pktLength | address   |   sender  | pktCount0 | rndData |.......| rndData|
 *              |           |           |   (you)   |           |         |       |        |
 *              |--------------------------------------------------------------------------|
 *               txBuffer[0] txBuffer[1] txBuffer[2]  ..................... txBuffer[PKTLEN]
 *
 * @param       txBuffer[] - pointer to start of txBuffer
 * @param       addr        - address of target device
 *
 * @return      none
 */

void createPacketULTIMATE(uint8 txBuffer[], unsigned int MODEFLAG, uint8 TargetAddr, uint8 OwnAddr,uint8 PacketLength, uint8* DATA)
{

  txBuffer[0] = PacketLength;                     // Length byte
  txBuffer[1] = TargetAddr;                 // Address byte
  txBuffer[2] = OwnAddr; 					// signature byte

  // fill rest of buffer with random bytes
  unsigned char ic = 3;
  if(MODEFLAG == 0)
  {
	  for(ic = 3; ic< (PacketLength+1); ic++)
	  {
		txBuffer[ic] = 'a';
	  }
  }
  else
  {
	  for(ic = 3; ic< (PacketLength+1); ic++)
	  {
		txBuffer[ic] = DATA[ic-3];
	  }
  }
}


/*******************************************************************************
 *
 *! This function initializes the necessary configurations or GPIO for UART
 *! functionality. It automatically selects Timer_UART or hardware UART using
 *! USCI_A0.
 *!
 *! \param data character data to be transmitted via UART.
 *!
 *! \return None
 *
 ******************************************************************************/




/*******************************************************************************
* @fn          radioRxTxISR
*
* @brief       ISR for packet handling in RX. Sets packet semaphore, puts radio
*              in idle state and clears isr flag.
*
* @param       none
*
* @return      none
*/
void radioRxTxISR(void) {

  // set packet semaphore
  packetSemaphore = ISR_ACTION_REQUIRED;
  // clear isr flag
  trxClearIntFlag(GPIO_0);
}
/*
#pragma vector = PORT2_Vector
  __interrupt void GPIORadioTransmittedInterrupt
  {

  }*/

