#include <msp430.h> 

#define BUTTON      BIT3
#define GREEN_LED   BIT6
#define RED_LED     BIT0

/**
 * main.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Desliga Temporizador WatchDog

	// Recomendações Texas
	P1DIR |= 0xFF;              // Define todos os pinos do PORT1 como saída
	P1OUT = 0x00;               // Desliga todos os pinos do PORT1
	P2DIR |= 0xFF;              // Define todos os pinos do PORT2 como saída
	P2OUT = 0x00;               // Desliga todos os pinos do PORT2
	P3DIR |= 0xFF;              // Define todos os pinos do PORT3 como saída
	P3OUT = 0x00;               // Desliga todos os pinos do PORT3

	// Setup
	P1DIR |= GREEN_LED+RED_LED; // Define pino 0 do PORT1 como saída (P1.0 = 1)

	P1DIR &= ~BUTTON;           // Define pino 3 do PORT1 como entrada (P1.3 = 0)
	P1REN |= BUTTON;            // Habilita resistor de tração (pull) do P1.3
	P1OUT |= BUTTON;            // Define resistor de tração do P1.3 como UP (pull-up)

	P1OUT |= RED_LED;           // Inicia com LED vermelho ON
	P1OUT &= ~GREEN_LED;        // Inicia com LED verde OFF

	P1IES |= BUTTON;            // Define borda de sensibilidade do P1.3 como DESCIDA (1)
	P1IE |= BUTTON;             // Habilida P1.3 a subir flag de interrupção caso mude* de estado

    P1IFG = 0x00;               // Desliga todas as flags do sinalizador do PORT1

    __bis_SR_register(LPM3_bits + GIE); // Habilita interrupções globais e desliga CPU e CLOCK
}

#pragma vector=PORT1_VECTOR
__interrupt void change_led(void)
{
    if((P1IFG & BUTTON) == BUTTON)  // Verifica se o botão no pino P1.3 está pressionado
    {
        P1OUT ^= GREEN_LED + RED_LED;
        P1IFG &= ~BUTTON;
    }
}
