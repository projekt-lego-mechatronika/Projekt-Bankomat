#include <EEPROM.h>





uint8_t klient_1_1000zl = 1;                 //Stan konta klienta_1 
uint8_t klient_1_100zl = 2;
uint8_t klient_1_10zl = 4;
uint8_t klient_2_1000zl = 5;                 //Stan konta klienta_2
uint8_t klient_2_100zl = 5;
uint8_t klient_2_10zl = 8;
uint8_t karta_serwisanta_blokada = 0;        //Stan blokad kart
uint8_t karta_klienta_1_blokada = 0;
uint8_t karta_klienta_2_blokada = 0;
uint8_t L_20 = 5;                            //Liczba banknotów w bankomacie
uint8_t L_50 = 5;
int L_100 = 5;


void setup()
{
EEPROM.update(0, klient_1_1000zl);             //Zapis zmiennych do pamieci EEPROM
EEPROM.update(1, klient_1_100zl);
EEPROM.update(2, klient_1_10zl);
EEPROM.update(3, klient_2_1000zl);
EEPROM.update(4, klient_2_100zl);
EEPROM.update(5, klient_2_10zl);
EEPROM.update(6, karta_serwisanta_blokada);
EEPROM.update(7, karta_klienta_1_blokada);
EEPROM.update(8, karta_klienta_2_blokada);
EEPROM.update(9, L_20);
EEPROM.update(10, L_50);
EEPROM.update(11, L_100);
}

void loop()
{
  
}
