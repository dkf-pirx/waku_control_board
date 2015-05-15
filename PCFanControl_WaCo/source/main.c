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
   
   unsigned char        ucOutputs[3] = { 0 };

   InitializeClocks( );   
   
   P2DIR = BIT3 | BIT4 | BIT5;
   
//   __enable_interrupt();                     // Enable interrupts.

   wait_ms( 500 );
   
   /* Main Application Loop */
   while(1)
   {
      for ( iCycle = 0; iCycle < 4; ++iCycle )
      {
         wait_ms( 1000 );
         
         ucOutputs[0] = 0x01 << iCycle;
         SetOutputs( ucOutputs );
      }
   }  
}


