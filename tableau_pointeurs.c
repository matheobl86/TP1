/*
 * TP - Tableau en RAM, pointeurs et affichage adresse/valeur sur ATmega16
 *
 * PORTA (PINA) : entrée, index à lire (0..N-1)
 * PORTB        : sortie, valeur du tableau à l'index lu
 * PORTC        : sortie, octet faible de l'adresse de l'élément
 * PORTD        : sortie, octet fort de l'adresse de l'élément
 */

#include <avr/io.h>

#define N 10   // taille du tableau, modifiable facilement avant compilation

void RemplirTableau(unsigned char *ptableau, uint8_t taille)
{
    uint8_t i;
    for (i = 0; i < taille; i++)
        *(ptableau + i) = (unsigned char)i;
}

int main(void)
{
    unsigned char tableau[N];   // tableau réservé en RAM
    uint8_t index;
    uint16_t adresse;

    DDRA = 0x00;   // PORTA en entrée
    DDRB = 0xFF;   // PORTB en sortie
    DDRC = 0xFF;   // PORTC en sortie
    DDRD = 0xFF;   // PORTD en sortie

    RemplirTableau(tableau, N);   // remplissage du tableau avec 0,1,2,...,N-1

    while (1)
    {
        index = PINA;   // lecture de l'index sur le port A

        if (index < N)   // index valide
        {
            PORTB = tableau[index];   // affichage de la valeur

            adresse = (uint16_t)(&tableau[index]);   // adresse mémoire de l'élément

            PORTC = (uint8_t)(adresse & 0x00FF);          // octet faible de l'adresse
            PORTD = (uint8_t)((adresse >> 8) & 0x00FF);   // octet fort de l'adresse
        }
    }

    return 0;
}
