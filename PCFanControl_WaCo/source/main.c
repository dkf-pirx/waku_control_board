/*
 * main.c
 *
 */

#define LATCH_UP     P2OUT |= BIT5
#define LATCH_DOWN   P2OUT &= ~BIT5

    

#include  "msp430g2553.h"
#include  "support.h"

void SetOutputs( unsigned char pOutputs[3] )
{
   int index; 
   int value;

   // prepare to take output
   LATCH_DOWN;
   
   for ( index = 0; index < 8; ++index )
   {
      // set serial in value
      value = 0x80 & pOutputs[0];
      
      if ( value == 0 ) // clear
      {
         P2OUT &= ~BIT3;
      }
      else // set
      {
         P2OUT |= BIT3;
      }
      
      // rising edge of the clock
      P2OUT |= BIT4;

      // falling edge of the clock
      P2OUT &= ~BIT4;
      
      // shift to next serial in
      pOutputs[0] <<= 1;
   }
   
   // take over output
   LATCH_UP;
}

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


