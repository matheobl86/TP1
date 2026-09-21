/*
 * TP1 - Allumage progressif de LEDs sur ATmega16
 * PORTA (PINA0-PINA3) : entrée, valeur binaire 0-15
 * PORTB : sortie, N LEDs allumées progressivement (LED0, LED1, ...)
 */

#include <avr/io.h>

int main(void)
{
    DDRA = 0x00;   // PORTA en entrée (boutons/interrupteurs)
    DDRB = 0xFF;   // PORTB en sortie (LEDs)

    uint8_t valeur;
    uint8_t leds;
    uint8_t bit;

    while (1)
    {
        valeur = PINA & 0x0F;   // lecture des 4 bits de poids faible de PINA

        leds = 0x00;            // on éteint toutes les LEDs avant la mise à jour
        bit  = 0x01;            // bit qui va se décaler pour désigner chaque LED

        while (valeur > 0)
        {
            leds |= bit;        // on allume la LED désignée par "bit"
            bit <<= 1;          // décalage binaire vers la LED suivante
            valeur--;           // décrémentation du compteur de LEDs restantes
        }

        PORTB = leds;           // mise à jour du port de sortie
    }

    return 0;
}
