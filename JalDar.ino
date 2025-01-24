#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Key.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27,16,2);  //I2C address = 0x27; LCD1602A (16 columns, 2 rows)

#define NUM_KEYS 4
#define RELAY 0

int displey = 3;
int stopp = 5;
int ac = 6;
int bagla = 7;
int siqnal = 8;
int rSiqnal = 4;
int rAc = 10;
int rBagla = 11;
int rAcar = 9;
int parolT = A0; 
int rejim = LOW;
int evvelki_rejim;
int indiki_rejim;

char key;
char dogru_parol[NUM_KEYS] = {'0', '0', '0', '0'};       //массив с верным кодом
char button_pressed[NUM_KEYS];                           //массив для хранения нажатых кнопок

int k = 0;                                               // счетчик нажатий
int s = 0;                                               // счетчик совпадений нажатых кнопок с верными
const byte ROWS = 4;                                     // количество строк в матрице клавиатуры
const byte COLS = 3;                                     // количество столбцов

char keys[ROWS][COLS] = {                                // таблица соответствия кнопок символам
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'0',' ','E'}
};

byte rowPins[ROWS] = {1, 2, 12, 13};                     // пины подключенных строк
byte colPins[COLS] = {A1, A2, A3};                       // пины подключенных столбцов

String inputString = " ";                                 // Строка для ввода чисел 
int cursorPosition = 2;                                  // Позиция курсора на дисплее 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );   // создаем объект клавиатуры для работы с ней



void setup() {
  pinMode( parolT, INPUT );  
  pinMode( displey, INPUT );
  pinMode( rSiqnal, OUTPUT );  
  pinMode( stopp, INPUT );
  pinMode( bagla, INPUT );
  pinMode( ac, INPUT );
  pinMode( siqnal, INPUT );
  pinMode( rAc, OUTPUT );  
  pinMode( rBagla, OUTPUT );  
  pinMode( rAcar, OUTPUT );  
  pinMode( RELAY, OUTPUT );
  
  digitalWrite(rSiqnal, LOW );
  digitalWrite(rAc, LOW );                        
  digitalWrite(rBagla, LOW);                      
  digitalWrite(rAcar, LOW);

  lcd.init();                 
  lcd.begin(16, 2);
  lcd.backlight();

  girish_ekran();       // giriş ekranı - ilk ekran açılır
 
  indiki_rejim = digitalRead(displey);
}

void girish_ekran(){
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("EN6.1");
  lcd.setCursor(3,1);
  lcd.print("LERIK 2025"); 
  delay(100);
}

void sonra_gorunush(){
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("PAROL");
  lcd.setCursor(0,1);
  lcd.print("Y");
  lcd.setCursor(2,1);
  lcd.print("    ");    
  lcd.setCursor(9,1);
  lcd.print("T");
  lcd.setCursor(11,1);
  lcd.print(parolT); 
  delay(500);      
}

void loop() {
  evvelki_rejim = indiki_rejim;
  indiki_rejim = digitalRead(displey);

  parolT = analogRead(A0);
  parolT = map(parolT, 0, 1023, 0, 9999 );

 // kochurulmush kod bashlayir
  char buffer[5]; // 4 reqemli parolu saxlamaq uchun kifayet qeder yer verilir
  snprintf(buffer, sizeof(buffer), "%04d", parolT); // 0000 formatinda string yeri

  for (int i = 0; i < NUM_KEYS; i++){
    dogru_parol[i] = buffer[i];       //parolu teyin edir
  }

  key = keypad.getKey();    

  if ( key != NO_KEY)                           // если она все-таки есть
  {
  
    if (cursorPosition < 6) {                  // Проверка, не превышена ли длина строки на дисплее
      inputString += key;                      // Добавление нажатой цифры к строке ввода
      lcd.setCursor(cursorPosition, 1);        // Установка курсора на позицию
      lcd.print(key);                          // Отображение нажатой цифры на дисплее
      cursorPosition++;                        // Перемещение курсора вправо
    } 
    
    button_pressed [k] = key;                   //сохраняем эту кнопочку в массиве
    k++;                                  // запоминаем сколько уже кнопок нажали
    
    if(k == NUM_KEYS)                             // если нажали нужное количество кнопок
    {
      bool correct = true;
      for ( int i = 0; i < NUM_KEYS; i++)       // пройдемся по всему массиву
      {
        if (button_pressed[i] != dogru_parol[i])          // и проверим нажатые кнопки с верным кодом
        { 
          correct = false;
          break;
        }
      } 

    
      if(correct )              //если у нас все кнопки совпали с кодом, то включаем реле
        {
          digitalWrite (RELAY, LOW);                    // включили реле
          digitalWrite (LED2, HIGH);                    // зажгли зеленый светик (пользователь ввел верный код)
          delay (3000);                                 // ждем 3 секунд пока горит светик зеленый и включено реле
          digitalWrite (RELAY, HIGH);                   // гасим реле
          digitalWrite (LED2, LOW);                     // гасим светик
        } 
      else {                                            
          digitalWrite (LED1, HIGH);                      
          delay (3000);                                 
          digitalWrite (LED1, LOW);                                                              
        }

        k = 0;                                           //сбрасываем счетчик нажатий нашей переменной
        s = 0;                                           // сбрасываем счетчик совпадений нашей переменной
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print("PAROL");   
        lcd.setCursor(0,1);
        lcd.print("Y");
        lcd.setCursor(10,1);
        lcd.print("T");
        lcd.setCursor(12,1);
        lcd.print(parolT); 
        cursorPosition = 2;
        inputString = " ";
        cursorPosition = 2;
    }  
  }


  if(digitalRead(stopp) == HIGH)
  {
    digitalWrite(rSiqnal, LOW );
    digitalWrite(rAc, LOW );                        
    digitalWrite(rBagla, LOW);                      
    digitalWrite(rAcar, LOW); 
    }
  if(digitalRead(ac) == HIGH)
  {
    digitalWrite(rSiqnal, HIGH );                       
    digitalWrite(rBagla, LOW);                      
    digitalWrite(rAcar, HIGH); 
    delay(500);
    digitalWrite(rAc, HIGH ); 
    delay(3500);  
    digitalWrite(rAcar, LOW);       
    }
  if(digitalRead(bagla) == HIGH)
  {
    digitalWrite(rAc, LOW ); 
    digitalWrite(rAcar, LOW);       
    digitalWrite(rSiqnal, HIGH );                       
    digitalWrite(rBagla, HIGH);                      
    delay(17000);
    digitalWrite(rSiqnal, LOW ); 
    digitalWrite(rBagla, LOW);       
    } 
  if(digitalRead(siqnal) == HIGH)
  {
    digitalWrite(rSiqnal, LOW ); 
  }

  if (evvelki_rejim == HIGH && indiki_rejim == LOW ) {
    rejim = !rejim;
  }
    
  if(rejim == HIGH) {
    sonra_gorunush(); 
  }

  else if(rejim == LOW) {
    girish_ekran(); 
  }    

}
