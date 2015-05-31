#include "msp430g2553.h"
#include "uart.h"

void InitUART( void )
{
   P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
   P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
   UCA0CTL1 |= UCSSEL_2;                     // SMCLK
   UCA0BR0 = 104;                            // 16MHz 9600
   UCA0BR1 = 0;                              // 16MHz 9600
   UCA0MCTL = UCBRF_3 + UCBRS_0 + UCOS16;     // Modulation UCBRSx = 1
   UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
   IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

void WriteChar( char cChar )
{
  while ( ! ( IFG2 & UCA0TXIFG ) );                // USCI_A0 TX buffer ready?
  UCA0TXBUF = cChar;                           // TX -> RXed character
}

//  Echo back RXed character, confirm TX buffer is ready first
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character
}
