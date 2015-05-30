
#define LATCH_UP     P2OUT |= BIT5
#define LATCH_DOWN   P2OUT &= ~BIT5

#include "msp430g2553.h"
#include "io.h"

static const unsigned int  gc_iRegNo = 3;

void InitIOs( void )
{
   P2DIR = BIT3 | BIT4 | BIT5;
}


void SetOutputs( unsigned char pOutputs[3] )
{
   int iRegIndex;
   int iBitIndex; 
   int value;

   // prepare to take output
   LATCH_DOWN;

   for ( iRegIndex = 0; iRegIndex < gc_iRegNo; ++iRegIndex )
   {
      for ( iBitIndex = 0; iBitIndex < 8; ++iBitIndex )
      {
         // set serial in value
         value = 0x80 & pOutputs[gc_iRegNo - 1 - iRegIndex];
         
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
         pOutputs[gc_iRegNo - 1 - iRegIndex] <<= 1;
      }
   }
   
   // take over output
   LATCH_UP;
}
