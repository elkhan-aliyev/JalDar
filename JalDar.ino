#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Key.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27,16,2);  //I2C address = 0x27; LCD1602A (16 columns, 2 rows)

// ***** PINLERIN TEYINI *****

const int NUM_KEYS 4
const int RELE_PIN = 0;
const int DISPLEY_PIN = 3;
const int STOP_PIN = 5;
const int ACH_PIN = 6;
const int BAGHLA_PIN = 7;
const int SIQNAL_PIN = 8;
const int rSIQNAL_PIN = 4;
const int rACH_PIN = 10;
const int rBAGHLA_PIN = 11;
const int rACHAR_PIN = 9;
const int PAROL_PIN = A0;

// --------------------------------

// ***** QLOBAL DEYISHENLERIN TEYINI *****
// ***** Parol Prosesine aid qlobal deyishenler *****

int evvelki_parol = 0;
int indiki_parol = 0;

bool parol_stabil = true;
bool parol_daxil_edilirmi = false; 

unsigned long sonuncu_deyishme_zamani = 0; 
unsigned long stabilizasiya_zamani = 2000;

String daxil_edilen_string = "";

char key;        
char buffer[5];       

const byte ROWS = 4;                                     
const byte COLS = 3;                                     

char keys[ROWS][COLS] = {                                
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'0',' ','E'}
};

byte rowPins[ROWS] = {1, 2, 12, 13};                     
byte colPins[COLS] = {A1, A2, A3};                       
                             
int cursorPosition = 6;                                 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );   

// --------------------------------

// ***** FUNKSIYA SKOPU BASHLAYIR *****

// ***** ESAS ARDUINO FUNKSIYALARI BASHLAYIR *****
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
  pinMode( RELE_PIN, OUTPUT );

  digitalWrite(rSIQNAL_PIN, LOW);
  digitalWrite(rACH_PIN, LOW);
  digitalWrite(rBAGHLA_PIN, LOW);
  digitalWrite(rACHAR_PIN, LOW);

  lcd.init();                 
  lcd.begin(16, 2);
  lcd.backlight();

  girish_ekran();
}

void loop() {
  umumi_parol_prosesi();
  umumi_jalyuz_prosesi();
  delay(100);
}

// ***** ESAS ARDUINO FUNKSIYALARI BITIR *****

// --------------------------------

// ***** IKINCI DERECELI FUNKSIYALAR BASHLAYIR *****

// ****** Parol prosesi bashlayir ******

void umumi_parol_prosesi(){
  indiki_parol = analogRead(PAROL_PIN);
  indiki_parol = map(indiki_parol, 0, 1023, 0, 9999 );

  if (abs(indiki_parol - evvelki_parol) > 1) {
    parol_stabil = false;
    sonuncu_deyishme_zamani = millis(); 
    potensiometr_qiy_deyishir();
    evvelki_parol = indiki_parol;

    if (parol_daxil_edilirmi) {
      parol_daxil_edilirmi = false; 
      lcd.clear();
      lcd.print("Parol deyishdi!");
      delay(1000);
      lcd.clear();
    }
  }

  
  if (!parol_stabil && (millis() - sonuncu_deyishme_zamani >= stabilizasiya_zamani)) {
    parol_stabil = true;
    lcd.clear();
    parol_mentiqi_bashla();
  }

    
  if (parol_daxil_edilirmi) {
    parol_daxil_et();
  }
}

// ***** Parol prosesi bitir *****

// -------------------------------

// ***** Jalyuz prosesi bashlayir ******

void umumi_jalyuz_prosesi() {
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
    gozleme_ekrani(1700);
    digitalWrite(rSIQNAL_PIN, LOW);
    digitalWrite(rBAGHLA_PIN, LOW);
  }
  if(digitalRead(SIQNAL_PIN)){
    digitalWrite(rSIQNAL_PIN, LOW);
  }
}

// ***** Jalyuz prosesi bitir *****

// ***** IKINCI DERECELI FUNKSIYALAR BITIR *****
// -------------------------------
// ***** UCHUNCU DERECELI FUNKSIYALAR BASHLAYIR *****

// ***** EKRANLAR BASHLAYIR ****

void girish_ekran(){
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("EN6.1");
  lcd.setCursor(3,1);
  String message = "LERIK 2025";
  for (byte i = 0; i < message.length(); i++) {
    lcd.print(message[i]);
    delay(100);
  }
  delay(1000);
  lcd.clear();
}

void gozleme_ekrani(int delayCentis) {
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Gozleyin");
  lcd.setCursor(2, 1);
  lcd.print("[..........]");
  lcd.setCursor(3, 1);
  for (byte i = 0; i < 10; i++) {
    delay(delayCentis);
    lcd.print("=");
  }
  lcd.clear();
}

// ***** EKRANLAR BITIR

void potensiometr_qiy_deyishir() {
  parol_teyin_et();
}

void parol_teyin_et(){
  lcd.setCursor(1,0);
  lcd.print("Parol Teyin Et");   
  lcd.setCursor(4,1);
  lcd.print("T");
  lcd.setCursor(6,1);
  char formatli_parol_str[5]; 
  snprintf(formatli_parol_str, sizeof(formatli_parol_str), "%04d", indiki_parol);
  lcd.print(formatli_parol_str); 
}

void parol_mentiqi_bashla(){
  parol_daxil_edilirmi = true;     
  daxil_edilen_string = "";            
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Parol daxil et");
  lcd.setCursor(6, 1);          
}

void parol_daxil_et() {
  key = keypad.getKey();
  lcd.setCursor(1, 0);
  lcd.print("Parol daxil et");
  if (key && key >= '0' && key <= '9') {
  daxil_edilen_string += key;
  lcd.setCursor(6 + daxil_edilen_string.length() - 1, 1); 
  lcd.print('*');


  if (daxil_edilen_string.length() >= 4) {
    parol_daxil_edilirmi = false; 
    lcd.clear();

    snprintf(buffer, sizeof(buffer), "%04d", indiki_parol);
    if (daxil_edilen_string == String(buffer)) {
      parol_duzgundur();
    } else {
      parol_yanlishdir();
    }
  }
}
}

void parol_duzgundur(){                                        
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Doghru Parol");
  lcd.setCursor(1,1);
  lcd.print("Rele achilir");
  delay(1000);
  digitalWrite (RELE_PIN, HIGH);                    
  gozleme_ekrani(300);                                 
  digitalWrite (RELE_PIN, LOW);
  gozleme_ekrani(150);
  parol_mentiqi_bashla();
}

void parol_yanlishdir(){   
  lcd.clear();              
  lcd.setCursor(1,0);
  lcd.print("Yanlish Parol");
  delay(1000);
  gozleme_ekrani(200);
  parol_mentiqi_bashla();
}

// ***** UCHUNCU DERECELI FUNKSIYALAR BITIR *****

// ***** FUNKSIYA SKOPU BITIR *****