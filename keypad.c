#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "keypad.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "tm4c123gh6pm.h"
 
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif


void keypad_init(void){

SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);                    //Enable PortE
while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
    }      

GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_3|GPIO_INT_PIN_4);  //set pins portE 1-4 dir input 

SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);                    //Enable PortD
while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
    {
    }      
GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_INT_PIN_0|GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_3); //set pins portD 0-3 dir output

//GPIOPadConfigSet(SYSCTL_PERIPH_GPIOE, GPIO_INT_PIN_0|GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); //pull up
 GPIO_PORTE_PUR_R |=0x1E;           //pull-up

}
//----------------------------------------------------------------------------------------------------------------------

void delayMs(int n){
int i,j;
for(i= 0;i<n;i++){
  for ( j=0 ;j<3180;j++)
  {}
}
}
void delayUs(int n){
  int i,j;
for(i= 0;i<n;i++){
  for ( j=0 ;j<3;j++)
  {}
}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//unsigned char
unsigned char keypad_getkey(void){
    unsigned char keymap[4][4] = {
      {'1','2','3','+'},
      {'4','5','6','-'},
      {'7','8','9','C'},
      {'*','0','=','D'},   
    };
  int row , column ;
  GPIOPinWrite(GPIO_PORTD_BASE, GPIO_INT_PIN_0|GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_3, 0x0); // set al rows = 0
  
  column = GPIOPinRead(GPIO_PORTE_BASE, GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_3|GPIO_INT_PIN_4) & 0x1E; //read value of all columns 
   if (column == 0x1E)       // check no key is pressed
     return 0 ;           


    while(1){
       
       row=0;
       GPIOPinWrite(GPIO_PORTD_BASE, GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_3, GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_3);
       GPIOPinWrite(GPIO_PORTD_BASE, GPIO_INT_PIN_0, 0x0);      //enable row0 only
       
      delayUs(5);
       column = GPIOPinRead(GPIO_PORTE_BASE, GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_3|GPIO_INT_PIN_4) & 0x1E; //read value of all columns 
        if (column != 0x1E) break;
     
        row=1;//0x0D
       GPIOPinWrite(GPIO_PORTD_BASE, GPIO_INT_PIN_0|GPIO_INT_PIN_2|GPIO_INT_PIN_3, GPIO_INT_PIN_0|GPIO_INT_PIN_2|GPIO_INT_PIN_3);
       GPIOPinWrite(GPIO_PORTD_BASE, GPIO_INT_PIN_1, 0x0);      //enable row1 only
        delayUs(5);
        column = GPIOPinRead(GPIO_PORTE_BASE, GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_3|GPIO_INT_PIN_4) & 0x1E; //read value of all columns 
        if (column != 0x1E) break;
     
      
      row=2;      //0x0B
       GPIOPinWrite(GPIO_PORTD_BASE, GPIO_INT_PIN_1|GPIO_INT_PIN_0|GPIO_INT_PIN_3, GPIO_INT_PIN_1|GPIO_INT_PIN_0|GPIO_INT_PIN_3);
       GPIOPinWrite(GPIO_PORTD_BASE, GPIO_INT_PIN_2, 0x0);      //enable row2 only
        delayUs(5);
        column = GPIOPinRead(GPIO_PORTE_BASE, GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_3|GPIO_INT_PIN_4) & 0x1E; //read value of all columns 
        if (column != 0x1E) break;
     
      
     row=3;      //0x07
       GPIOPinWrite(GPIO_PORTD_BASE, GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_0, GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_0);
       GPIOPinWrite(GPIO_PORTD_BASE, GPIO_INT_PIN_3, 0x0);      //enable row3 only
        delayUs(5);
        column = GPIOPinRead(GPIO_PORTE_BASE, GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_3|GPIO_INT_PIN_4) & 0x1E; //read value of all columns 
        if (column != 0x1E) break;
   
  return 0;
    }


if (column == 0x1C) return keymap[row][0];
if (column == 0x1A) return keymap[row][1];
if (column == 0x16) return keymap[row][2];
if (column == 0x0E) return keymap[row][3];
return 0;
   
}
