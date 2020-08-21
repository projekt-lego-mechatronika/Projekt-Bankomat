#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <Keypad.h>


//      UART
// 48 - BRAK_KARTY
// 49 - KARTA_SERWISOWA
// 50 - KARTA_KLIENT_1
// 51 - KARTA_KLIENT_2

LiquidCrystal lcd(30, 31, 34, 35, 32, 33);                                                //Wyświetlacz


const byte ROWS = 4;                                                                     //Klawiatura
const byte COLS = 4;

byte rowPins[ROWS] = {23, 25, 27, 29};
byte colPins[COLS] = {22, 24, 26, 28};
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
Keypad klawiatura = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int ID;                                                                                   //Zmienne
int ID_stare;
uint8_t stan;
uint8_t pole;
uint8_t liczba_prob;
uint8_t i;
char wprowadzony_PIN[4];
char PIN_serwisanta [4] = {'1', '1', '1', '1'};
char PIN_klienta_1[4] = {'2', '2', '2', '2'};
char PIN_klienta_2[4] = {'3', '3', '3', '3'};
uint8_t karta_serwisanta_blokada = EEPROM.read(6);
uint8_t karta_klienta_1_blokada = EEPROM.read(7);
uint8_t karta_klienta_2_blokada = EEPROM.read(8);
uint8_t a;
char klawisz;
int L_20 = EEPROM.read(9);                              //Liczba banknotów, kolejno 20zł, 50zł, 100zł
int L_50 = EEPROM.read (10);
int L_100 = EEPROM.read(11);
uint8_t stan_1;
uint8_t menu_serwisanta_1_s;
uint8_t menu_serwisanta_2_s;
int stan_konta_klienta_1 = ( (1000 * EEPROM.read(0)) + (100 * EEPROM.read(1)) + (10 * EEPROM.read(2)) );              //Stany kont klientów
int stan_konta_klienta_2 = ( (1000 * EEPROM.read(3)) + (100 * EEPROM.read(4)) + (10 * EEPROM.read(5)) );
int kwota_do_wyplaty;
uint8_t L_20_wyplata = 0;                         //Liczba banknotów podczas wyplaty
uint8_t L_50_wyplata = 0;
uint8_t L_100_wyplata = 0;
uint8_t wyplata = 0;


void wprowadzenie_PINu (uint8_t osoba = ID)                                                      //wprowadzanie PINu
{
  klawisz = klawiatura.getKey();

  switch (pole)                                                                             //wprowadzanie kolejnych pol PINu
  {
    case 0:
      if (klawisz == '0' || klawisz == '1' || klawisz == '2' || klawisz == '3' || klawisz == '4' || klawisz == '5' || klawisz == '6' || klawisz  == '7' || klawisz == '8' || klawisz == '9')
      {
        wprowadzony_PIN[0] = klawisz;
        pole++;
      }
      break;

    case 1:
      if (klawisz == '0' || klawisz == '1' || klawisz == '2' || klawisz == '3' || klawisz == '4' || klawisz == '5' || klawisz == '6' || klawisz  == '7' || klawisz == '8' || klawisz == '9')
      {
        wprowadzony_PIN[1] = klawisz;
        pole++;
      }
      break;

    case 2:
      if (klawisz == '0' || klawisz == '1' || klawisz == '2' || klawisz == '3' || klawisz == '4' || klawisz == '5' || klawisz == '6' || klawisz  == '7' || klawisz == '8' || klawisz == '9')
      {
        wprowadzony_PIN[2] = klawisz;
        pole++;
      }
      break;

    case 3:
      if (klawisz == '0' || klawisz == '1' || klawisz == '2' || klawisz == '3' || klawisz == '4' || klawisz == '5' || klawisz == '6' || klawisz  == '7' || klawisz == '8' || klawisz == '9')
      {
        wprowadzony_PIN[3] = klawisz;
        pole++;
      }
      break;
  }

  if (pole == 4)                                                               //Sprawdzenie poprawnosci PINu
  {
    for (i ; i < 4; i++)
    {
      if (osoba == 49 && (wprowadzony_PIN[i] == PIN_serwisanta[i]))
      {
        a++;
      }

      if (osoba == 50 && (wprowadzony_PIN[i] == PIN_klienta_1[i]))
      {
        a++;
      }

      if (osoba == 51 && (wprowadzony_PIN[i] == PIN_klienta_2[i]))
      {
        a++;
      }
    }
    if (a == 4)                                                                //Poprawny PIN
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Poprawny PIN");
      delay(1000);
      pole = 0;
      a = 0;
      stan = 1;
    }

    else                                                                      //Niepoprawny PIN
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Niepoprawny PIN");
      liczba_prob--;
      pole = 0;
      a = 0;
      i = 0;
      wprowadzony_PIN[0] = '_';
      wprowadzony_PIN[1] = '_';
      wprowadzony_PIN[2] = '_';
      wprowadzony_PIN[3] = '_';
      delay(1000);
    }
  }

  if (liczba_prob == 0)                                                 //Blokowanie karty
  {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("KARTA");
    lcd.setCursor(2, 2);
    lcd.print("ZABLOKOWANA");

    if (osoba == 49)

    {
      karta_serwisanta_blokada = 1;
      EEPROM.update(6, 1);
    }

    if (osoba == 50)
    {
      karta_klienta_1_blokada = 1;
      EEPROM.update(7, 1);
    }

    if (osoba == 51)
    {
      karta_klienta_2_blokada = 1;
      EEPROM.update(8, 1);
    }

    delay(1000);
  }

}   //koniec wprowadzania PINu

void menu_serwisanta_1()
{
  if (stan_1 == 0)
  {
    stan_1 = 1;
    menu_serwisanta_1_s = 1;
  }

  if (klawisz == 'D')
  {
    stan_1 = 0;
    menu_serwisanta_1_s = 0;
  }

  if (klawisz == '1')                                         //Zmiana liczby banknotów
    L_20 ++;

  if (klawisz == '2')
    L_50++;

  if (klawisz == '3')
    L_100++;

  if (klawisz == '7')
    L_20 = L_20 + 5;

  if (klawisz == '8')
    L_50 = L_50 + 5;

  if (klawisz == '9')
    L_100 = L_100 + 5;

  if (klawisz == '4' && L_20 > 0 )
    L_20 --;

  if (klawisz == '5' && L_50 > 0 )
    L_50 --;

  if (klawisz == '6'  && L_100 > 0 )
    L_100 --;

  if (klawisz == '*' && L_20 > 4 )
    L_20 = L_20 - 5;

  if (klawisz == '0' && L_50 > 4 )
    L_50 = L_50 - 5;

  if (klawisz == '#'  && L_100 > 4 )
    L_100 = L_100 - 5;

  if (klawisz == 'C')
  {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("ZAPISANO");                            //Zapisanie liczby banknotów
    EEPROM.update(9, L_20);
    EEPROM.update(10, L_50);
    EEPROM.update(11, L_100);
    delay(1000);
  }



  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("20zl");
  lcd.setCursor(6, 0);
  lcd.print("50zl");
  lcd.setCursor(11, 0);
  lcd.print("100zl");
  lcd.setCursor(0, 2);
  lcd.print(L_20);
  lcd.setCursor(6, 2);
  lcd.print(L_50);
  lcd.setCursor(11, 2);
  lcd.print(L_100);

}

void menu_serwisanta_2()                                    //Wybór karty do odblokowania
{
  if (stan_1 == 0)
  {
    stan_1 = 1;
    menu_serwisanta_2_s = 1;
  }

  if (klawisz == 'D')
  {
    stan_1 = 0;
    menu_serwisanta_2_s = 0;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Klient 1    [1]");
  lcd.setCursor(0, 2);
  lcd.print("Klient 2    [2]");

  if (klawisz == '1')
  {
    if (karta_klienta_1_blokada == 1)                          //Odblokowywanie kart
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("KARTA    ZOSTALA");
      lcd.setCursor(2, 2);
      lcd.print("ODBLOKOWANA");
      karta_klienta_1_blokada = 0;
      EEPROM.update(7, 0);
      delay(1000);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("KARTA       JEST");
      lcd.setCursor(2, 2);
      lcd.print("ODBLOKOWANA");
      delay(1000);
    }
  }

  if (klawisz == '2')
  {
    if (karta_klienta_2_blokada == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("KARTA    ZOSTALA");
      lcd.setCursor(2, 2);
      lcd.print("ODBLOKOWANA");
      karta_klienta_2_blokada = 0;
      EEPROM.update(8, 0);
      delay(1000);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("KARTA       JEST");
      lcd.setCursor(2, 2);
      lcd.print("ODBLOKOWANA");
      delay(1000);
    }
  }
}

void stan_konta(uint8_t osoba_stan_konta = ID)                                                           //Stan konta
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Twoj stan konta");

  if (osoba_stan_konta == 50)
  {
    lcd.setCursor(6, 2);
    lcd.print(stan_konta_klienta_1);
    lcd.setCursor(14, 2);
    lcd.print("zl");
  }

  if (osoba_stan_konta == 51)
  {
    lcd.setCursor(6, 2);
    lcd.print(stan_konta_klienta_2);
    lcd.setCursor(14, 2);
    lcd.print("zl");
  }

  if (klawisz == 'D')
    stan = 1;

}
menu_wyplata(uint8_t osoba_wyplata = ID)                                                               //Menu wyplata
{
  if (klawisz == 'D')
    stan = 1;

  if (klawisz == '1' && (kwota_do_wyplaty + 20) <= 500)
    kwota_do_wyplaty = kwota_do_wyplaty + 20;

  if (klawisz == '2' && (kwota_do_wyplaty + 50) <= 500)
    kwota_do_wyplaty = kwota_do_wyplaty + 50;

  if (klawisz == '3' && (kwota_do_wyplaty + 100) <= 500)
    kwota_do_wyplaty = kwota_do_wyplaty + 100;

  if (klawisz == '4' && (kwota_do_wyplaty - 20) >= 0)
    kwota_do_wyplaty = kwota_do_wyplaty - 20;

  if (klawisz == '5' && (kwota_do_wyplaty - 50) >= 0)
    kwota_do_wyplaty = kwota_do_wyplaty - 50;

  if (klawisz == '6' && (kwota_do_wyplaty - 100) >= 0)
    kwota_do_wyplaty = kwota_do_wyplaty - 100;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ile   wyplacasz?");
  lcd.setCursor(6, 2);
  lcd.print(kwota_do_wyplaty);
  lcd.setCursor(10, 2);
  lcd.print("zl");

  if (osoba_wyplata == 50 && klawisz == 'C' && kwota_do_wyplaty != 0)                        //Wyplata klient 1
  {

    if (kwota_do_wyplaty <= stan_konta_klienta_1)
    {
      L_100_wyplata = (kwota_do_wyplaty / 100);

      if (L_100_wyplata <= L_100)
      {
        L_50_wyplata = ((kwota_do_wyplaty - (100 * L_100_wyplata)) / 50) ;

        if (L_50_wyplata <= L_50)
          L_20_wyplata = ((kwota_do_wyplaty - (100 * L_100_wyplata) - (50 * L_50_wyplata)) / 20);

        if (L_50_wyplata > L_50 )
        {
          L_50_wyplata = L_50;
          L_20_wyplata = ((kwota_do_wyplaty - (100 * L_100_wyplata) - (50 * L_50_wyplata)) / 20);
        }
      }


      if (L_100_wyplata > L_100)

      {
        L_100_wyplata = L_100;
        L_50_wyplata = ((kwota_do_wyplaty - (100 * L_100_wyplata)) / 50) ;

        if (L_50_wyplata <= L_50)
          L_20_wyplata = ((kwota_do_wyplaty - (100 * L_100_wyplata) - (50 * L_50_wyplata)) / 20);

        if (L_50_wyplata > L_50 )
        {
          L_50_wyplata = L_50;
          L_20_wyplata = ((kwota_do_wyplaty - (100 * L_100_wyplata) - (50 * L_50_wyplata)) / 20);
        }

      }

      if ( ( kwota_do_wyplaty - (L_100_wyplata * 100) - (L_50_wyplata * 50) - (L_20_wyplata * 20))  == 0)
      {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("REALIZOWANIE");
        stan_konta_klienta_1 = stan_konta_klienta_1 - kwota_do_wyplaty;
        L_20 = L_20 - L_20_wyplata;
        L_50 = L_50 - L_50_wyplata;
        L_100 = L_100 - L_100_wyplata;
        EEPROM.update(0, (stan_konta_klienta_1 / 1000));                                                    //Zapis stanu konta klienta_1
        EEPROM.update(1, ((stan_konta_klienta_1 - (EEPROM.read(0) * 1000)) / 100) );
        EEPROM.update(2, ((stan_konta_klienta_1 - (EEPROM.read(0) * 1000) - (EEPROM.read(1) * 100)) / 10 ));
        EEPROM.update(9, L_20);                                                                             //Zapis, ile banknotow pozostalo w bankomacie
        EEPROM.update(10, L_50);
        EEPROM.update(11, L_100);
        kwota_do_wyplaty = 0;

        for ( L_100_wyplata; L_100_wyplata > 0; L_100_wyplata --)                                       //Obsluga silnikow podczas wyplaty
        {
          digitalWrite(36, LOW);
          digitalWrite(38, HIGH);
          digitalWrite(39, HIGH);
          delay (1000);
          digitalWrite(36, HIGH);
          digitalWrite(38, LOW);
          digitalWrite(39, LOW);
          delay(500);
        }

        for ( L_50_wyplata; L_50_wyplata > 0; L_50_wyplata --)
        {
          digitalWrite(36, LOW);
          digitalWrite(39, HIGH);
          delay (1000);
          digitalWrite(36, HIGH);
          digitalWrite(39, LOW);
          delay(500);
        }

        for ( L_20_wyplata; L_20_wyplata > 0; L_20_wyplata --)
        {
          digitalWrite(36, LOW);
          delay (1000);
          digitalWrite(36, HIGH);
          delay(500);
        }

        wyplata = 1;
        stan = 1;

      }
      else
      {
        lcd.clear();                                                                                //Brak wyplaty - nie ma odpowiednich banknotow
        lcd.setCursor(4, 0);
        lcd.print("WYPLATA");
        lcd.setCursor(3, 2);
        lcd.print("NIEMOZLIWA");
        kwota_do_wyplaty = 0;
        delay (1000);
      }
    }
    else
    {
      lcd.clear();                                                                                //Brak wyplaty - za malo srodkow na koncie
      lcd.setCursor(0, 0);
      lcd.print(" BRAK   SRODKOW");
      lcd.setCursor(2, 2);
      lcd.print(" NA  KONCIE");
      kwota_do_wyplaty = 0;
      delay (2000);
    }
  }

  if (osoba_wyplata == 51 && klawisz == 'C' && kwota_do_wyplaty != 0)                        //Wyplata klient 2
  {

    if (kwota_do_wyplaty <= stan_konta_klienta_2)
    {
      L_100_wyplata = (kwota_do_wyplaty / 100);

      if (L_100_wyplata <= L_100)
      {
        L_50_wyplata = ((kwota_do_wyplaty - (100 * L_100_wyplata)) / 50) ;

        if (L_50_wyplata <= L_50)
          L_20_wyplata = ((kwota_do_wyplaty - (100 * L_100_wyplata) - (50 * L_50_wyplata)) / 20);

        if (L_50_wyplata > L_50 )
        {
          L_50_wyplata = L_50;
          L_20_wyplata = ((kwota_do_wyplaty - (100 * L_100_wyplata) - (50 * L_50_wyplata)) / 20);
        }
      }


      if (L_100_wyplata > L_100)

      {
        L_100_wyplata = L_100;
        L_50_wyplata = ((kwota_do_wyplaty - (100 * L_100_wyplata)) / 50) ;

        if (L_50_wyplata <= L_50)
          L_20_wyplata = ((kwota_do_wyplaty - (100 * L_100_wyplata) - (50 * L_50_wyplata)) / 20);

        if (L_50_wyplata > L_50 )
        {
          L_50_wyplata = L_50;
          L_20_wyplata = ((kwota_do_wyplaty - (100 * L_100_wyplata) - (50 * L_50_wyplata)) / 20);
        }

      }



      if ( ( kwota_do_wyplaty - (L_100_wyplata * 100) - (L_50_wyplata * 50) - (L_20_wyplata * 20))  == 0)
      {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("REALIZOWANIE");
        stan_konta_klienta_2 = stan_konta_klienta_2 - kwota_do_wyplaty;
        L_20 = L_20 - L_20_wyplata;
        L_50 = L_50 - L_50_wyplata;
        L_100 = L_100 - L_100_wyplata;
        EEPROM.update(3, (stan_konta_klienta_1 / 1000));                                                    //Zapis stanu konta klienta_2
        EEPROM.update(4, ((stan_konta_klienta_1 - (EEPROM.read(3) * 1000)) / 100) );
        EEPROM.update(5, ((stan_konta_klienta_1 - (EEPROM.read(3) * 1000) - (EEPROM.read(4) * 100)) / 10 ));
        EEPROM.update(9, L_20);                                                                             //Zapis, ile banknotow pozostalo w bankomacie
        EEPROM.update(10, L_50);
        EEPROM.update(11, L_100);
        kwota_do_wyplaty = 0;
        for ( L_100_wyplata; L_100_wyplata > 0; L_100_wyplata --)
        {
          digitalWrite(36, LOW);
          digitalWrite(38, HIGH);
          digitalWrite(39, HIGH);
          delay (1000);
          digitalWrite(36, HIGH);
          digitalWrite(38, LOW);
          digitalWrite(39, LOW);
          delay(500);
        }

        for ( L_50_wyplata; L_50_wyplata > 0; L_50_wyplata --)
        {
          digitalWrite(36, LOW);
          digitalWrite(39, HIGH);
          delay (1000);
          digitalWrite(36, HIGH);
          digitalWrite(39, LOW);
          delay(500);
        }

        for ( L_20_wyplata; L_20_wyplata > 0; L_20_wyplata --)
        {
          digitalWrite(36, LOW);
          delay (1000);
          digitalWrite(36, HIGH);
          delay(500);
        }
        wyplata = 1;
        stan = 1;

      }
      else
      {
        lcd.clear();                                                                                //Brak wyplaty - nie ma odpowiednich banknotow
        lcd.setCursor(4, 0);
        lcd.print("WYPLATA");
        lcd.setCursor(3, 2);
        lcd.print("NIEMOZLIWA");
        kwota_do_wyplaty = 0;
        delay (1000);
      }
    }
    else
    {
      lcd.clear();                                                                                //Brak wyplaty - za malo srodkow na koncie
      lcd.setCursor(0, 0);
      lcd.print(" BRAK   SRODKOW");
      lcd.setCursor(2, 2);
      lcd.print(" NA  KONCIE");
      kwota_do_wyplaty = 0;
      delay (1000);
    }
  }

}




void setup() {
  stan = 0;
  stan_1 = 0;
  pole = 0;
  a = 0;
  i = 0;
  liczba_prob = 3;
  menu_serwisanta_1_s = 0;
  menu_serwisanta_2_s = 0;
  Serial3.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Dzien dobry!");
  lcd.setCursor(2, 2);
  lcd.print("Wloz  karte!");
  wprowadzony_PIN[0] = '_';
  wprowadzony_PIN[1] = '_';
  wprowadzony_PIN[2] = '_';
  wprowadzony_PIN[3] = '_';
  kwota_do_wyplaty = 0;
  wyplata = 0;
  pinMode(36, OUTPUT);
  pinMode(38, OUTPUT);
  pinMode(39, OUTPUT);
  digitalWrite(36, HIGH);


}

void loop() {
  ID = Serial3.read();
  if (ID != ID_stare)                                          //Zmniejszenie częstotliowści odświeżania (wykrywanie zmiany ID)
  {
    switch (ID)     //switch wyboru osoby
    {
      case 48:                                                    //Brak karty
        stan = 0;
        stan_1 = 0;
        pole = 0;
        a = 0;
        i = 0;
        liczba_prob = 3;
        menu_serwisanta_1_s = 0;
        menu_serwisanta_2_s = 0;
        wprowadzony_PIN[0] = '_';
        wprowadzony_PIN[1] = '_';
        wprowadzony_PIN[2] = '_';
        wprowadzony_PIN[3] = '_';
        kwota_do_wyplaty = 0;
        wyplata = 0;
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Dzien dobry!");
        lcd.setCursor(2, 2);
        lcd.print("Wloz  karte!");
        break;

      case 49:                                                    //Karta serwisanta

        if (karta_serwisanta_blokada == 0)
        {
          switch (stan)
          {
            case 0:
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Podaj PIN");
              lcd.setCursor(0, 2);
              lcd.print("Liczba prob");
              lcd.setCursor(12, 0);
              lcd.print(wprowadzony_PIN[0]);
              lcd.setCursor(13, 0);
              lcd.print(wprowadzony_PIN[1]);
              lcd.setCursor(14, 0);
              lcd.print(wprowadzony_PIN[2]);
              lcd.setCursor(15, 0);
              lcd.print(wprowadzony_PIN[3]);
              lcd.setCursor(12, 2);
              lcd.print(liczba_prob);
              wprowadzenie_PINu (ID);
              break;

            case 1:
              if (stan_1 == 0)
              {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("L.banknotow  [A]");
                lcd.setCursor(0, 2);
                lcd.print("Odb. kart    [B]");
              }

              klawisz = klawiatura.getKey();
              if (klawisz == 'A' || menu_serwisanta_1_s)
              {
                menu_serwisanta_1();
              }

              if (klawisz == 'B' || menu_serwisanta_2_s)
              {
                menu_serwisanta_2();
              }

              break;
          }
        }
        else
        {
          lcd.clear();
          lcd.setCursor(5, 0);
          lcd.print("KARTA");
          lcd.setCursor(2, 2);
          lcd.print("ZABLOKOWANA");
        }
        break;

      case 50:                                                    //Karta klienta_1



        if (karta_klienta_1_blokada == 0)
        {
          switch (stan)
          {
            case 0:
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Podaj PIN");
              lcd.setCursor(0, 2);
              lcd.print("Liczba prob");
              lcd.setCursor(12, 0);
              lcd.print(wprowadzony_PIN[0]);
              lcd.setCursor(13, 0);
              lcd.print(wprowadzony_PIN[1]);
              lcd.setCursor(14, 0);
              lcd.print(wprowadzony_PIN[2]);
              lcd.setCursor(15, 0);
              lcd.print(wprowadzony_PIN[3]);
              lcd.setCursor(12, 2);
              lcd.print(liczba_prob);
              wprowadzenie_PINu (ID);
              break;

            case 1:
              klawisz = klawiatura.getKey();
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Stan konta   [A]");
              lcd.setCursor(0, 2);
              lcd.print("Wyplata      [B]");


              if (klawisz == 'A')
                stan = 2;

              if (klawisz == 'B' && wyplata == 0)
              {
                stan = 3;
              }
              if (klawisz == 'B' && wyplata == 1)
              {
                lcd.clear();
                lcd.setCursor(1, 0);
                lcd.print("WLOZ PONOWNIE");
                lcd.setCursor(5, 2);
                lcd.print("KARTE");
                delay(1000);
              }
              break;

            case 2:
              klawisz = klawiatura.getKey();
              stan_konta(ID);
              break;

            case 3:
              klawisz = klawiatura.getKey();
              menu_wyplata(ID);
              break;

          }
        }
        else
        {
          lcd.clear();
          lcd.setCursor(5, 0);
          lcd.print("KARTA");
          lcd.setCursor(2, 2);
          lcd.print("ZABLOKOWANA");
        }

        break;

      case 51:                                                    //Karta klienta_2
        if (karta_klienta_2_blokada == 0)
        {
          switch (stan)
          {
            case 0:
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Podaj PIN");
              lcd.setCursor(0, 2);
              lcd.print("Liczba prob");
              lcd.setCursor(12, 0);
              lcd.print(wprowadzony_PIN[0]);
              lcd.setCursor(13, 0);
              lcd.print(wprowadzony_PIN[1]);
              lcd.setCursor(14, 0);
              lcd.print(wprowadzony_PIN[2]);
              lcd.setCursor(15, 0);
              lcd.print(wprowadzony_PIN[3]);
              lcd.setCursor(12, 2);
              lcd.print(liczba_prob);
              wprowadzenie_PINu (ID);
              break;

            case 1:
              klawisz = klawiatura.getKey();
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Stan konta   [A]");
              lcd.setCursor(0, 2);
              lcd.print("Wyplata      [B]");


              if (klawisz == 'A')
                stan = 2;

              if (klawisz == 'B' && wyplata == 0)
              {
                stan = 3;
              }
              if (klawisz == 'B' && wyplata == 1)
              {
                lcd.clear();
                lcd.setCursor(1, 0);
                lcd.print("WLOZ PONOWNIE");
                lcd.setCursor(5, 2);
                lcd.print("KARTE");
                delay(1000);
              }

              break;

            case 2:
              klawisz = klawiatura.getKey();
              stan_konta(ID);
              break;

            case 3:
              klawisz = klawiatura.getKey();
              menu_wyplata(ID);
              break;

          }
        }
        else
        {
          lcd.clear();
          lcd.setCursor(5, 0);
          lcd.print("KARTA");
          lcd.setCursor(2, 2);
          lcd.print("ZABLOKOWANA");
        }

        break;
    }  //koniec switcha od wyboru osoby

  }

  ID_stare = ID;

}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
