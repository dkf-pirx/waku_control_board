/*
 * main.c
 *
 */

    

#include  "msp430g2553.h"
#include  "support.h"
#include  "io.h"


void main(void)
{
   WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

   int                  iCycle = 0;
   int                  iRunningLed = 0;
   
   unsigned char        ucOutputs[3] = { 0 };

   InitializeClocks( );   
   InitIOs( );
   
   // Configure Port Pins
   P2DIR &= ~BIT1;                           // P2.1/TA1.1 Input Capture
   P2SEL |= BIT1;                            // TA1.1 option select

   // Configure the TA0CCR1 to do input capture
   TA1CCTL1 = CAP + CM_1 + CCIS_0;              // TA0CCR1 Capture mode; CCI1A; 
                                                // Rising Edge
   TA1CTL |= TASSEL_2 + MC_2 + TACLR;           // SMCLK, Cont Mode; start timer    
   
   while ( (TA1CCTL1 & CCIFG) == 0 )            // wait for interrupt
   {
   }
   TA1CCTL1 &= ~CCIFG;
   
//   __enable_interrupt();                      // Enable interrupts.

   wait_ms( 500 );
   
   /* Main Application Loop */
   while(1)
   {
      for ( iCycle = 0; iCycle < 16; ++iCycle )
      {
         wait_ms( 100 );
         
         ucOutputs[0] = 0x01 << iCycle;
         iRunningLed = 0x01 << iCycle;
         ucOutputs[1] = (unsigned char) iRunningLed;
         ucOutputs[2] = (unsigned char) (iRunningLed >> 8);
         SetOutputs( ucOutputs );
      }
   }  
}


