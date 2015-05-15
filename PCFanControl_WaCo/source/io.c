
#define LATCH_UP     P2OUT |= BIT5
#define LATCH_DOWN   P2OUT &= ~BIT5

#include "msp430g2553.h"
#include "io.h"

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
