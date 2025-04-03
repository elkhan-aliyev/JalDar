#include <Key.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  //I2C address = 0x27; LCD1602A (16 columns, 2 rows)

const int rDarvaza_PIN = 9;
const int DISPLEY_PIN = 4;
const int STOP_PIN = 5;
const int ACH_PIN = 6;
const int BAGHLA_PIN = 7;
const int SIQNAL_PIN = 8;
const int rSIQNAL_PIN = 0;
const int rACH_PIN = 3;
const int rBAGHLA_PIN = 2;
const int rACHAR_PIN = 1;
const int PAROL_PIN = A0;

#define NUM_KEYS 4                                       // количество знаков в коде
int rejim = LOW;
int evvelki_rejim;
int indiki_rejim;
int parolT = 0;

char key;        
char dogru_parol[NUM_KEYS] = {'0', '0', '0', '0'};       //массив с верным кодом
char button_pressed[NUM_KEYS];                           //массив для хранения нажатых кнопок  
char buffer[5]; // 4 reqemli parolu saxlamaq uchun kifayet qeder yer verilir

int k = 0;                                               // счетчик нажатий
int s = 0;                                               // счетчик совпадений нажатых кнопок с верными
const byte ROWS = 4;                                     
const byte COLS = 3;     


char keys[ROWS][COLS] = {                                
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'N','0','E'}
};

byte rowPins[ROWS] = {10, 11, 12, 13};                     
byte colPins[COLS] = {A1, A2, A3};

int cursorPosition = 2;                                  // Позиция курсора на дисплее 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS ); 

void girish_ekran_animasiya(){
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("EN6.1");
  lcd.setCursor(1,1);
  String mesaj = "JALYUZ_DARVAZA";
  for (byte i = 0; i < mesaj.length(); i++){
    lcd.print(mesaj[i]);
    delay(100);
  }
  delay(1000);
  lcd.clear();
}

void girish_ekran(){
  lcd.setCursor(5,0);
  lcd.print("EN6.1");
  lcd.setCursor(1,1);
  lcd.print("JALYUZ_DARVAZA");
}

void sonra_gorunush(){                    //Displey
  // lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("PAROL");
  lcd.setCursor(0,1);
  lcd.print("Y"); 
  lcd.setCursor(10,1);
  lcd.print("T");
  lcd.setCursor(12,1);
  lcd.print(buffer);
  delay(200);      
}
void setup() {

  pinMode( PAROL_PIN, INPUT );  
  pinMode( DISPLEY_PIN, INPUT );
  pinMode( STOP_PIN, INPUT );
  pinMode( BAGHLA_PIN, INPUT );
  pinMode( ACH_PIN, INPUT );
  pinMode( SIQNAL_PIN, INPUT );
  pinMode( rSIQNAL_PIN, OUTPUT);
  pinMode( rACH_PIN, OUTPUT);
  pinMode( rBAGHLA_PIN, OUTPUT);
  pinMode( rACHAR_PIN, OUTPUT);
  pinMode( rDarvaza_PIN, OUTPUT );

  digitalWrite(rSIQNAL_PIN, LOW);
  digitalWrite(rACH_PIN, LOW);
  digitalWrite(rBAGHLA_PIN, LOW);
  digitalWrite(rACHAR_PIN, LOW);
  digitalWrite(rDarvaza_PIN, LOW);              // Darvaza rele

  lcd.init();                 
  lcd.begin(16, 2);
  lcd.backlight();

  girish_ekran_animasiya();

}

void parol_sifirla(){
  k = 0;                                           
  s = 0;
  lcd.clear();
  sonra_gorunush();   
  cursorPosition = 2;
}

void loop() {
  evvelki_rejim = indiki_rejim;
  indiki_rejim = digitalRead(DISPLEY_PIN);

  parolT = analogRead(PAROL_PIN);                      //Displey
  parolT = map(parolT, 0, 1023, 0, 109 );      //Displey

  snprintf(buffer, sizeof(buffer), "%04d", parolT*91); // 0000 formatinda string yeri

  for (int i = 0; i < NUM_KEYS; i++)
  {
    dogru_parol[i] = buffer[i];
  }  

  key = keypad.getKey();                       // спрашиваем у клавиатуры, есть нажатая кнопка?

  if (key != NO_KEY && key == 'E'){
    parol_sifirla();
  }

  if ( key != NO_KEY && key != 'E' && key != 'N')                           // если она все-таки есть
  {

    if (cursorPosition < 6) {                  // Проверка, не превышена ли длина строки на дисплее
      lcd.setCursor(cursorPosition, 1);        // Установка курсора на позицию
      lcd.print(key);                          // Отображение нажатой цифры на дисплее
      cursorPosition++;                        // Перемещение курсора вправо
    } 
    

    button_pressed [k] = key;                //сохраняем эту кнопочку в массиве
    k = k + 1;                               // запоминаем сколько уже кнопок нажали

    if(k == NUM_KEYS)                             // если нажали нужное количество кнопок
    {
      for ( uint8_t i = 0; i < NUM_KEYS; i++)       // пройдемся по всему массиву
      {
        if (button_pressed[i] == dogru_parol[i])          // и проверим нажатые кнопки с верным кодом
        { 
          s = s + 1;                                   // плюсуем счетчик совпадений  
        }
      } 

      if(s == NUM_KEYS )              //если у нас все кнопки совпали с кодом, то включаем реле
      {
        digitalWrite (rDarvaza_PIN, HIGH);                    // включили реле
        delay (1000);                                         // ждем 2 секунд пока горит светик зеленый и включено реле
        digitalWrite (rDarvaza_PIN, LOW);                   // гасим реле
      }  
               
      delay(750);
      parol_sifirla(); 
    }
  }

  if (evvelki_rejim && !indiki_rejim){
    lcd.clear();
    rejim = !rejim;
  }
  if(rejim) {
    sonra_gorunush();  
  }
  else if (!rejim) {
    girish_ekran(); 
  }

  if (digitalRead(STOP_PIN)){
    digitalWrite(rSIQNAL_PIN, LOW);
    digitalWrite(rACH_PIN, LOW);
    digitalWrite(rBAGHLA_PIN, LOW);
    digitalWrite(rACHAR_PIN, LOW);
  }
  if (digitalRead(ACH_PIN)){
    digitalWrite(rSIQNAL_PIN, HIGH);
    digitalWrite(rBAGHLA_PIN, LOW);
    digitalWrite(rACHAR_PIN, HIGH);
    delay(500);
    digitalWrite(rACH_PIN, HIGH);
    delay(3500);
    digitalWrite(rACHAR_PIN, LOW);
  }
  if(digitalRead(BAGHLA_PIN)){
    digitalWrite(rACH_PIN, LOW);
    digitalWrite(rACHAR_PIN, LOW);
    digitalWrite(rSIQNAL_PIN, HIGH);
    digitalWrite(rBAGHLA_PIN, HIGH);
    delay(17000);
    digitalWrite(rSIQNAL_PIN, LOW);
    digitalWrite(rBAGHLA_PIN, LOW);
  }
  if(digitalRead(SIQNAL_PIN)){
    digitalWrite(rSIQNAL_PIN, LOW);
  } 
}
