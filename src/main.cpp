/*
ClockAVR - zegarek na ATMega328P-AU wykorzystujący panel przedni z dekodera telewizyjnego
Cyfry 0 - 3           : PC0 - PC3 (numerowane od prawej)
Segmenty A - DP       : PD0 - PD7
Przyciski S501 - S503 : PB0 - PB2
*/

#include <avr/io.h>
#include <util/delay.h>
// zapis delaya - _delay_ms(100);



int main(void)
{
    // make the LED pin an output for PORTB5
    DDRB = 1 << 5;

    while (1)
    {
        
        // toggle the LED
        PORTB ^= 1 << 5;
    }

    return 0;
}