
#define LATCH_UP     P2OUT |= BIT5
#define LATCH_DOWN   P2OUT &= ~BIT5

#define SET_LED      P2OUT |= BIT3
#define CLEAR_LED    P2OUT &= ~BIT3

#define NEXT_LED     P2OUT |= BIT4; P2OUT &= ~BIT4


#include "msp430g2553.h"
#include "io.h"
#include "support.h"


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

extern void BlinkLed1( void )
{
   unsigned char        ucOutputs[3] = { 0 };
   
   ucOutputs[0] = 0x01;
   SetOutputs( ucOutputs );
   wait_ms( 20 );
   ucOutputs[0] = 0x00;
   SetOutputs( ucOutputs );
   
   
//   int i;
//   
//   // prepare to take output
//   LATCH_DOWN;
//
//      for ( i = 0; i < 4; ++i )
//      {
//         CLEAR_LED;
//         
//         NEXT_LED;
//      }      
//      SET_LED;
//
//   // take over output
//   LATCH_UP;
//
//   // wait
//   wait_ms( 20U );
//   
//   // prepare to take output
//   LATCH_DOWN;
//
//      for ( i = 0; i < 4; ++i )
//      {
//         CLEAR_LED;
//         
//         NEXT_LED;
//      }      
//      CLEAR_LED;
//   
//   // take over output
//   LATCH_UP;
}