/*
 * main.c
 *
 */

#include <msp430.h>
#include <string.h>
#include  "support.h"
#include  "io.h"
#include  "uart.h"


unsigned char            g_ucDisplayIt;  

unsigned char            g_ucImpulseIn;  
unsigned int             g_uiRevPerSecIndex;
unsigned int             g_uiRevPerSec[16];
unsigned long            g_ulTicksAvg;

/* Timer capture */
unsigned int             g_uiTicksPerRev = 0;


    
void main(void)
{
   WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

   int                  iCycle = 0;
   int                  iRunningLed = 0;
   
   unsigned char        ucOutputs[3] = { 0 };
   
   InitializeClocks( );   
   InitIOs( );
   InitUART( );
   
   // Configure Port Pins
   P2IE  |= BIT1;                            // P2.1 interrupt enabled
   P2IES |= BIT1;                            // P2.1 Hi/lo edge
   P2REN |= BIT1;							// Enable Pull Up on SW2 (P2.1)
   P2IFG &= ~BIT1;                           // P2.1 IFG cleared
    	  	  	  	  	  	  	  	  	    //BIT3 on Port 1 can be used as Switch2
   
   
   __enable_interrupt();                      // Enable interrupts.

   wait_ms( 500 );
   
   g_ucDisplayIt = 0;
   g_ucImpulseIn = 0;
   g_uiRevPerSecIndex = 0;
   
   memset( g_uiRevPerSec, 0, sizeof g_uiRevPerSec ); 
   
   /* Start timer before the loop */
   TA1CCR0 = 0xFFFF;                      // write non-zero, 
                                          // otherwise timer will not start
   TA1CCTL1 = CCIE;                       // interrupt
   TA1CCR1 = 0x7FFF;
   TA1CTL = TASSEL_1 + MC_1;              // ACLK, up, Clear

   /* Main Application Loop */
   while( 1 )
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

      if ( g_ucDisplayIt )
      {
         for ( iCycle = 0; iCycle < 16; ++iCycle )
         {
            g_ulTicksAvg += g_uiRevPerSec[iCycle];
         }
         g_ulTicksAvg /= 16;

         g_ulTicksAvg = ( g_uiRevPerSec[0] * 60 ) / 2;
         
         WriteChar( (char)(g_ulTicksAvg >> 8) );
         WriteChar( (char)g_ulTicksAvg );
         WriteChar( 0x00 );
         
         g_ucDisplayIt = 0;
      }
   }  
}


// Port 1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) Port_2 (void)
#else
#error Compiler not supported!
#endif
{
   ++g_uiTicksPerRev;
   
   P2IFG &= ~BIT1;
}

// TA1_A0 Interrupt vector
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) TIMER1_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
   g_uiRevPerSec[g_uiRevPerSecIndex] = g_uiTicksPerRev;

   TA1CCTL0 &= ~CCIFG;
}




// TA1_A1 Interrupt vector
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) TIMER1_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
   //switch(__even_in_range(TA1IV,0x0A))
   switch( TA1IV )
   {
      case  TA1IV_NONE: break;              // Vector  0:  No interrupt
      case  TA1IV_TACCR1:                   // Vector  2:  TA1CCR1 CCIFG
         
         g_uiRevPerSec[g_uiRevPerSecIndex] = g_uiTicksPerRev;
         
         g_uiTicksPerRev = 0;
         ++g_uiRevPerSecIndex;
         if ( g_uiRevPerSecIndex == 16 )
         {
            g_uiRevPerSecIndex = 0;
            
            g_ucDisplayIt = 1;
         }
            
        break;
      case TA1IV_TACCR2: break;             // Vector  4:  TA1CCR2 CCIFG
      case TA1IV_6: break;                  // Vector  6:  Reserved CCIFG
      case TA1IV_8: break;                  // Vector  8:  Reserved CCIFG
      case TA1IV_TAIFG: break;              // Vector 10:  TAIFG
      default: 	break;
   }
}


