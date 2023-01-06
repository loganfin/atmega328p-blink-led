#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include <stdbool.h>

#define BLINK_MS 500

int main()
{
    DDRB |= 0b00100000;

    /*
    PORTB |= 0b00100000;
    _delay_ms(BLINK_MS);
    PORTB &= ~0b00100000;
    _delay_ms(BLINK_MS);
    */

    while (true) {
        PORTB |= 0b00100000;
        _delay_ms(BLINK_MS);
        PORTB &= ~0b00100000;
        _delay_ms(BLINK_MS);
    }
}
