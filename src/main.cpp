/*
ClockAVR - zegarek na ATMega328P-AU wykorzystujący panel przedni z dekodera telewizyjnego
Cyfry 0 - 3           : PC0 - PC3 (numerowane od prawej)
Segmenty A - DP       : PD0 - PD7
Przyciski S501 - S503 : PB0 - PB2

Zegar - 8MHz
Preskaler /256 i licznik 6 - 256 powinien dać 125Hz

pomocne rzeczy:
zapis delaya - _delay_ms(100);
świecenie diodą:
  DDRB = 1 << 5;
  PORTB ^= 1 << 5; 
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define timer_max 256 - 250
volatile uint8_t ktora_cyfra = 0;
uint8_t wyswietlacz[] = { 0, 0, 0, 0};
uint8_t zapis_cyfry[] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0X80, 0X90, 0xFF, 0x7F, 0xA1, 0x21}; //10 - puste, 11 - kropka, 12 - d, 13 - d z kropką

//przerwanie od timera - multiplexowanie
ISR(TIMER0_OVF_vect)
{  
  TCNT0 = timer_max;

  PORTC |= (1 << ktora_cyfra);
  ktora_cyfra = (ktora_cyfra == 3) ? 0 : (++ktora_cyfra);
  PORTC &= ~(1 << ktora_cyfra);

  PORTD = zapis_cyfry[wyswietlacz[ktora_cyfra]];
}

int main(void)
{
  //konfiguracja timera0 - wyświetlacz
  TCNT0 = timer_max;
  TIMSK0 |= (1<<TOIE0); //ustawienie przerwania od timera
  TCCR0B |= (1<<CS02); // preskaler /256
  sei();

  //konfiguracja IO
  //przyciski
  DDRB = 0x00;
  //wyświetlacz
  DDRC = 0x0F;
  DDRD = 0xFF;


    while (1)
    {
      _delay_ms(1000);
      wyswietlacz[0] = 4;
      wyswietlacz[1] = 3;
      wyswietlacz[2] = 2; 
      wyswietlacz[3] = 1;

      _delay_ms(1000);
      wyswietlacz[0] = 6;
      wyswietlacz[1] = 5;
      wyswietlacz[2] = 4;
      wyswietlacz[3] = 3;

      _delay_ms(1000);
      wyswietlacz[0] = 8; 
      wyswietlacz[1] = 7;
      wyswietlacz[2] = 6;
      wyswietlacz[3] = 5;

      _delay_ms(1000);
      wyswietlacz[0] = 0;
      wyswietlacz[1] = 9;
      wyswietlacz[2] = 8; 
      wyswietlacz[3] = 7;
 
    }

    return 0;
}