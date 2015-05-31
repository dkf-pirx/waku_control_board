
#include  "msp430g2553.h"
#include "support.h"

void InitializeClocks( void )
{

  BCSCTL1   = CALBC1_16MHZ;                    // Set range
  DCOCTL    = CALDCO_16MHZ;
  BCSCTL2  &= ~(DIVS_3);                     // SMCLK = DCO = 1MHz  
}

/* Maximum time is 2^15 - 1 us */
void wait_us( unsigned int uTimeInUs )
{
   CCTL0 = OUTMOD_4;                       // CCR0 toggle mode
   CCR0 = uTimeInUs * 2;                     // period is 0,5us
   TACTL = TASSEL_2 + MC_1;                  // SMCLK, upmode
   
   
}

/* Maximum time is 2^15 - 1 */
void wait_ms( unsigned int uTimeInMs )
{
   unsigned long        ulNoTicks;
   
   ulNoTicks = ( ((unsigned long)uTimeInMs) * 32767 ) / 1000;
   
   CCR0 = (unsigned int) ulNoTicks;          // period is unround
   TACTL = TASSEL_1 + MC_1;                  // ACLK, upmode
   
   while ( (CCTL0 & CCIFG) == 0 )            // wait for interrupt
   {
   }
   
   CCTL0 &= ~CCIFG;
   TACTL = 0;                                // turn off
}
