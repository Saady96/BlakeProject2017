/******************************************************************************
    Filename: cc11xL_spi.c  
    
    Description: implementation file for a minimum set of neccessary functions
                 to communicate with CC11xL over SPI

NOTE from El Bodgo, look no further my intrepid friend, here are the commands that
send/recieve stuff from the radio in all their glory. (Ok actually they don't but
they do ask the radio to send/receive which is basically the same thing)
                 
*******************************************************************************/


/******************************************************************************
 * INCLUDES
 */
#include "hal_types.h"
#include "cc11xL_spi.h"
#include "hal_msp_exp430g2_spi.h"
/******************************************************************************
 * @fn          cc11xLSpiReadReg
 *
 * @brief       Reads register(s). If len  = 1: one byte is read
 *                                 if len != 1: len bytes are read in burst 
 *                                              mode
 * input parameters
 *
 * @param       addr   - address to read
 * @param       *pData - pointer to data array where read data is stored
 * @param       len    - numbers of bytes to read starting from addr
 *
 * output parameters 
 *
 * @return      rfStatus_t
 */
rfStatus_t cc11xLSpiReadReg(uint8 addr, uint8 *pData, uint8 len)
{
  uint8 rc;
  rc = trx8BitRegAccess((RADIO_BURST_ACCESS|RADIO_READ_ACCESS), addr, pData, len);
  return (rc);
}  

/******************************************************************************
 * @fn          cc11xLSpiWriteReg
 *
 * @brief       writes register(s). If len  = 1: one byte is written
 *                                  if len != 1: len bytes at *data is written 
 *                                               in burst mode.
 * input parameters
 *
 * @param       addr   - register address to write
 * @param       *pData - pointer to data array that is written
 * @param       len    - number of bytes written
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
rfStatus_t cc11xLSpiWriteReg(uint8 addr, uint8 *pData, uint8 len)
{
  uint8 rc;
  rc = trx8BitRegAccess((RADIO_BURST_ACCESS|RADIO_WRITE_ACCESS),addr, pData, len);
  return (rc);
}

/******************************************************************************
 * @fn          cc11xLSpiWriteTxFifo
 *
 * @brief       Writes provided data to TX FIFO
 *              
 * input parameters
 *
 * @param       *pData - pointer to data array that is written to TX FIFO
 * @param       len    - Length of data array to be written
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
rfStatus_t cc11xLSpiWriteTxFifo(uint8 *pData, uint8 len)
{
  uint8 rc;
  rc = trx8BitRegAccess((RADIO_BURST_ACCESS|RADIO_WRITE_ACCESS),CC11xL_FIFO, pData, len);
  return (rc);
}


/******************************************************************************
 * @fn          cc11xLSpiReadRxFifo
 *
 * @brief       Reads RX FIFO values to pData array
 *              
 * input parameters
 *
 * @param       *pData - pointer to data array where RX FIFO bytes are saved
 * @param       len    - number of bytes to read from the RX FIFO
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
rfStatus_t cc11xLSpiReadRxFifo(uint8 *pData, uint8 len)
{
  uint8 rc;
  rc = trx8BitRegAccess((RADIO_BURST_ACCESS|RADIO_READ_ACCESS),CC11xL_FIFO, pData, len);
  return (rc);
}  

/******************************************************************************
 * @fn      cc11xLGetTxStatus(void)
 *          
 * @brief   This function transmits a No Operation Strobe (SNOP) to get the 
 *          status of the radio and the number of free bytes in the TX FIFO.
 *          
 *          Status byte:
 *          
 *          ---------------------------------------------------------------------------
 *          |          |            |                                                 |
 *          | CHIP_RDY | STATE[2:0] | FIFO_BYTES_AVAILABLE (free bytes in the TX FIFO |
 *          |          |            |                                                 |
 *          ---------------------------------------------------------------------------
 *
 *          NOTE:
 *          When reading a status register over the SPI interface while the 
 *          register is updated by the radio hardware, there is a small, but 
 *          finite, probability that the result is corrupt. This also applies 
 *          to the chip status byte. The CC1100 and CC11xL errata notes explain 
 *          the problem and propose several work arounds. 
 *
 * input parameters
 *
 * @param   none
 *
 * output parameters
 *         
 * @return  rfStatus_t 
 *
 */
rfStatus_t cc11xLGetTxStatus(void)
{
    return(trxSpiCmdStrobe(CC11xL_SNOP));
}

/******************************************************************************
 *
 *  @fn       cc11xLGetRxStatus(void)
 *
 *  @brief   
 *            This function transmits a No Operation Strobe (SNOP) with the 
 *            read bit set to get the status of the radio and the number of 
 *            available bytes in the RXFIFO.
 *            
 *            Status byte:
 *            
 *            --------------------------------------------------------------------------------
 *            |          |            |                                                      |
 *            | CHIP_RDY | STATE[2:0] | FIFO_BYTES_AVAILABLE (available bytes in the RX FIFO |
 *            |          |            |                                                      |
 *            --------------------------------------------------------------------------------
 *
 *            NOTE:
 *            When reading a status register over the SPI interface while the
 *            register is updated by the radio hardware, there is a small, but 
 *            finite, probability that the result is corrupt. This also applies 
 *            to the chip status byte. The CC1100 and CC11xL errata notes explain 
 *            the problem and propose several work arounds. 
 *
 * input parameters
 *
 * @param     none
 *
 * output parameters
 *         
 * @return    rfStatus_t 
 *
 */
rfStatus_t cc11xLGetRxStatus(void)
{
    return(trxSpiCmdStrobe(CC11xL_SNOP | RADIO_READ_ACCESS));
}


/******************************************************************************
 *
 *  @fn       cc11xLGetRSSI(
 *
 *  @brief
 *          EL DODGO: pulls information from Radio's RSSI register!! the function
 *          you have all been calling for.
 *
 *
 * input parameters
 *
 * @param   AnarenChipNo    - The number of the anaren chip (there are a number defined in the header files)
 * @param   *pData          - where you want to store the RSSI data
 * @param   len             - expected length of rssi info (usually one)
 *
 * output parameters
 *
 * @return  rc              - ??? I will updata later!
 *
 */
rfStatus_t cc11xLGetRSSI(int AnarenChipNo, uint8 *pData, uint8 len)
{
    uint8 rc;
    switch(AnarenChipNo)
    {
    case(110):

        rc = trx8BitRegAccess((RADIO_BURST_ACCESS|RADIO_READ_ACCESS),CC110L_RSSI, pData, len);
        break;

    case(113):

        rc = trx8BitRegAccess((RADIO_BURST_ACCESS|RADIO_READ_ACCESS),CC113L_RSSI, pData, len);
        break;

    default:
        rc = 0;
        break;
    }
    return(rc);
}


rfStatus_t cc11xLGetRegStatus(uint8 *pData, uint8 len)
{
    uint8 rc;
    rc = trx8BitRegAccess((RADIO_BURST_ACCESS|RADIO_READ_ACCESS),CC110L_PKTCTRL1, pData, len);
    return(rc);
}
rfStatus_t cc11xLGetRegStatus2(uint8 *pData, uint8 len)
{
    uint8 rc;
    rc = trx8BitRegAccess((RADIO_BURST_ACCESS|RADIO_READ_ACCESS),CC110L_ADDR, pData, len);
    return(rc);
}
/******************************************************************************
 * @fn          function name
 *
 * @brief       Description of the function
 *
 * @param       input, output parameters
 *
 * @return      describe return value, if any
*/
/*
uint8 trx8BitRegAccess(uint8 accessType, uint8 addrByte, uint8 *pData, uint16 len)
{
  uint8 readValue;

  //Pull CS_N low and wait for SO to go low before communication starts
  SPI_BEGIN();
  while(SPI_PORT_IN & SPI_MISO_PIN);
  // send register address byte
  SPI_TX(accessType|addrByte);
  SPI_WAIT_DONE();
  // Storing chip status
  readValue = SPI_RX();

  trxReadWriteBurstSingle(accessType|addrByte,pData,len);
  SPI_END();
  // return the status byte value
  return(readValue);
}
*/


/******************************************************************************
  Copyright 2011 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
*******************************************************************************/
