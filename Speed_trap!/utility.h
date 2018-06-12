#ifndef UTILITY_H_
#define UTILITY_H_

#define TRANSMIT_DELAY_BODGE 950
#define CHANNELS 2
#define OWNADDRESS          15
//#define SENSOR_DISTANCE 0.125

extern unsigned long time;
extern char threshold;
extern int buf[CHANNELS];


void ADC_init();
//void UART_init();
void readADC();
void transmit();


#endif /* UTILITY_H_ */
