/* Podłączenie 
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

 
MFRC522 rfid(SS_PIN, RST_PIN);                                                                                               //Deklaracja PINów do obsługi czytnika

MFRC522::MIFARE_Key key; 

const byte karta_serwis[] = { 192, 94, 253, 50};                                                                              //Numery ID kart
const byte karta_klient_1[] = { 208, 206, 14, 50};
const byte karta_klient_2[] = { 176, 40, 86, 50};
int a = 0;
int karta;

void setup() { 
  Serial.begin(9600);
  SPI.begin();                                                                                                                //Inicjalizacja magistrali SPI
  rfid.PCD_Init();                                                                                                            //Inicjalizacja czytnika RFID


  }
 
void loop() {
if(rfid.PICC_IsNewCardPresent()==0)                                                                                          //Wykrywanie braku karty z zabezpieczniem przed zakłóceniami
{
  a++;
  if(a>5)
  {
    a=5;
    karta=0;
  }
}
else
{
a=0;
karta=1;

}
rfid.PICC_ReadCardSerial();


if(karta == 1 && rfid.uid.uidByte[0]==192 && rfid.uid.uidByte[1]==94 && rfid.uid.uidByte[2]==253 && rfid.uid.uidByte[3]==50)  //Sprawdzanie, która z kart jest obecna
Serial.print(1);

if(karta == 1 && rfid.uid.uidByte[0]==208 && rfid.uid.uidByte[1]==206 && rfid.uid.uidByte[2]==14 && rfid.uid.uidByte[3]==50)
Serial.print(2);

if(karta == 1 && rfid.uid.uidByte[0]==176 && rfid.uid.uidByte[1]==40 && rfid.uid.uidByte[2]==86 && rfid.uid.uidByte[3]==50)
Serial.print(3);

if(karta == 0)
Serial.print(0);

 delay(500);



  rfid.PCD_StopCrypto1();

}
