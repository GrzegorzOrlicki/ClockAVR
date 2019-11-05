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
//#include <util/delay.h>

//deklaracje stałych
#define timer_max 256 - 250
const uint8_t zapis_cyfry[] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0X80, 0X90, 0xFF, 0xA1}; //10 - puste, 11- d,

//deklaracje zmiennych
volatile uint8_t ktora_cyfra = 0;
uint8_t wyswietlacz[] = { 0, 0, 0, 0};
uint8_t kropka[] = { 0, 0, 0, 0};

//naglowki funkcji
void konfiguracja(void);

int main(void)
{
  konfiguracja();

  while (1)
  {

  }

  return 0;
}

void konfiguracja(void){
  //timer0 - wyświetlacz
  TCNT0 = timer_max;
  TIMSK0 |= (1<<TOIE0); //ustawienie przerwania od timera
  TCCR0B |= (1<<CS02); // preskaler /256
  sei();

  //konfiguracja I/O
  //przyciski
  DDRB = 0x00;
  //wyświetlacz
  DDRC = 0x0F;
  DDRD = 0xFF;
}

//przerwanie od timera - multiplexowanie
ISR(TIMER0_OVF_vect)
{  
  TCNT0 = timer_max;

  PORTC |= (1 << ktora_cyfra);
  ktora_cyfra = (ktora_cyfra == 3) ? 0 : (++ktora_cyfra);
  PORTC &= ~(1 << ktora_cyfra);

  PORTD = zapis_cyfry[wyswietlacz[ktora_cyfra]] & ~(kropka[ktora_cyfra] << 7);
}