//*********************************************************************  
//*                          Final Project                            
//*                       McMaster University                         
//*                      2DP4 Microcontrollers                        
//*                          Lab Section 03                           
//*                    Nisarg Parekh  MACID 400129104                             
//*********************************************************************
//*********************************************************************
//*                          Final Project                               
//*              Tilt inclination calculation using ESDX    
//*                                                                                                                         
//*********************************************************************
// Clock Speed is 6.00 MHz as given in the requirements                    
//*********************************************************************
// References: 
// 
//*********************************************************************

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"  /* derivative information */
#include "SCI.h"

//Function Prototypes 
void msDelay(unsigned int);
void delay1ms(unsigned int factor); 
void setClock(void);
void setADCX(void); 
void setADCY(void); 
void timerConfig(void);
void onOff(unsigned int countNum); 
void BCD(int angle); 
  

//Variables 
float val1 = 0.0;
unsigned int j =0; 
unsigned int i = 0; 
unsigned int val;
unsigned int interruptOnOffCount = 0;
unsigned int interruptModeCount = 0; 


//---------------------OutCRLF---------------------
// Output a CR,LF to SCI to move cursor to a new line
// Input: none
// Output: none
// Toggle LED each time through the loop

void OutCRLF(void){
  SCI_OutChar(CR);
  SCI_OutChar(LF);
  PTJ ^= 0x20;          // toggle LED D2
}

void main(void) {
  
  setClock(); 
  timerConfig(); 
  
  //Port Declerations 
  PER1AD = 0x0F; 
  DDR1AD = 0x0F;
  
  PER0AD = 0x0F; 
  DDR0AD = 0x0F; 
  
   
  EnableInterrupts; 
  

// Setup LED and SCI
  DDRJ |= 0x01;     // PortJ bit 0 is output to LED D2 on DIG13
  SCI_Init(38400);
  
   
  for(;;) {
    //Toggle LEDs
    PTJ ^=0x01;
    
    //Find angle using linear interpolation formula 
    //Slope between ADC value at 90 and ADC value at 0 
    val1 = ATDDR0*0.9 - 450;  
    //Convert value to integer 
    val = (int) val1; 
    //Output value to serial 
    SCI_OutUDec(val);
    OutCRLF(); 
    delay1ms(100);
    //Show the BCD of the angle      
    BCD(val); 
    
    
  }
                                          

}


//BCD conversion
void BCD(int angle) {
  //Init local variables
  int tempStore = angle;
  int onesPlace = 0; 
  int tensPlace = 0; 
   
  //Mod the angle value for the LSD 
  onesPlace = angle%10; 
  //Divide the angle value for the MSD 
  tensPlace = angle/10; 
  //Store the appropriate digits to the A0-A3 & A8-A11 ports to output
  PT1AD = tensPlace; 
  PT0AD = onesPlace;
  
}


//Setting Interrupts 
interrupt VectorNumber_Vtimch0 void ISR_Vtimch0(void) {
    unsigned int temp; 
   
    interruptOnOffCount++; 
    
    if(interruptOnOffCount % 2 != 0) {
      //SCI_OutString("On");
       
    }
    
    if(interruptOnOffCount % 2 == 0) {
      //Turn all LEDs off 
      PT1AD = 0x00; 
      PT0AD = 0x00;
      BCD(0);  
      interruptOnOffCount = 0; 
      interruptModeCount = 0; 
    }
        
    temp = TC0; 
}



interrupt VectorNumber_Vtimch1 void ISR_Vtimch1(void) {
   unsigned int temp; 
    
    
  interruptModeCount++;  
  
  if(interruptModeCount % 2 != 0) {
   //Set Mode 1
   setADCY(); 
  }
  
  if(interruptModeCount % 2 == 0) {
    //Set Mode 0 
    setADCX(); 
  }    
    
   temp = TC1; 
}





void setClock(void) {
  //Setting clock using phase locked loop 
  
  CPMUCLKS = 0x80;     //PLLSEL = 1
  CPMUOSC =  0x00;     //OSCE = 0
  CPMUPROT = 0x26; 
  CPMUSYNR = 0x17;     //VCOFRQ = 00, SYNDIV = 23
  //CPMUSYNR = 0x05; 
  
  //fREF = 1MHz, fVCO = 2*fREF*(23+1) = 48MHz
  
  CPMUFLG = 0x00;     //Lock = 0
  //CPMUFLG = 0x01; 
  
  //Lock = 0
  //PLLCLK = fVCO/4 = 12MHz 
  //Bus CLCK = PLLCLK/2 = 6MHz

}

void setADCX(void) {
  //Setting ADC for X measurements according to the project requirements 
  //ATDCTL0 = 0x07; //Wrap to AN7 
  
  ATDCTL1 = 0x2F; //10 Bit resolution 
  
  ATDCTL3 = 0x88; //Right justified, 1 sample per sequence (X data);
  
  ATDCTL4 = 0x02; //Prescale = 2; ATDCLK = 6MHz/(2*(2+1)) = 1MHz, 
  
  ATDCTL5 = 0x27; //Continuous conversion on AN7    
  

}

void setADCY(void) {
  //Setting ADC for Y 
  //ATDCTL0 = 0x07; //Wrap to AN7 
  
  ATDCTL1 = 0x2F; //10 Bit resolution 
  
  ATDCTL3 = 0x88; //Right justified, 1 sample per sequence (Y data);
  
  ATDCTL4 = 0x02; //Prescale = 2; ATDCLK = 6MHz/(2*(2+1)) = 1MHz
  
  ATDCTL5 = 0x25; //Continuous conversion on AN5    
  

}

void timerConfig(void) {
  TSCR1 = 0x90;  //Timer System Control Register 1
                 //Timer Enable, Timer Fast Flag Clear All
                 
  TSCR2 = 0x04;  //Timer System Control Register 2 
                 //Set timer prescaler to 4. BusClk/4
                
  TIOS  = 0xFC;  //Timer I/O Capture
                 //Set 1st & 2nd bit port to input, rest output
  
  PERT  = 0x03;  //Enable Pull-Up resistor 
  
  TCTL3 = 0x00;  //Rising edge detection
  TCTL4 = 0x0A;  //
                                      
  TIE   = 0x03;  //Timer interrupt enable 
}


void delay1ms(unsigned int factor){
  
  for(j = 0; j<factor; j++){
  // number of iterrations of this loop is determened by the following calculation 
  // (6*(80*4))/6.25=307 
  // Thus, PTJ=PTJ has to happen 410 times. 
  // 307/4 = 78 
  
    for(i = 0; i<78; i++){ 
      PTJ = PTJ;    
      PTJ = PTJ;
      PTJ = PTJ;
      PTJ = PTJ;
    }
  }   
 }



void msDelay(unsigned int time)
{
	unsigned int j,k;
	for(j=0;j<time;j++)
		for(k=0;k<1033;k++);
}
