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

void createPacket(uint8 txBuffer[], uint8 PacketLength);
/******************************************************************************
* STATIC FUNCTIONS
*/
//High level functions
void radioTx(uint8 TransmitData[], uint8 PacketLength);
uint8 radioRxLoop(uint8 RecievedData[], uint8 Priority);
uint8 radioRxLoopFiltered(uint8 RecievedData[], uint8 OwnAddress, uint8 Priority);
//Create Packet Commands: Take yo data and put it into something the base station will understand

void createPacketwADDR(uint8 txBuffer[], uint8 PacketLength, uint8 Targetaddr, uint8 OwnAddress);
void createPacketULTIMATE(uint8 txBuffer[], unsigned int MODEFLAG, uint8 TargetAddr, uint8 OwnAddr,uint8 PacketLength, uint8* DATA);



//ISR for radio - triggered by GD0 pin, you will have to dig around to see which one this is
void radioRxTxISR(void);

//Init and config commands (starting with the main one)
void    RadioInit(uint8 OwnAddress);
//Lower level configs
static void registerConfig(void);
static void addressConfig(uint8 addr);
void    AnarenUtilsHALinit();
static void runRX(void);




//UNUSED
void receivedDataCommand(unsigned char receiveData);
