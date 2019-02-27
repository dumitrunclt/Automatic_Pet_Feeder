 #include <DS3231.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define pulse 2
#define data 3

DS3231  rtc(A4, A5);
Servo servo_test;      //initialize a servo object for the connected servo  
LiquidCrystal lcd(A0, A1, A2, 11, 12, 13); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)

int t1, t2, t3, t4, t5, t6;
 
boolean feed = true; // condition for alarm

int Key;
int old = 0;
int r[6];

byte getkey(void)
{
  byte cnt;
  byte num = 0;
  for (cnt = 0; cnt < 16; cnt++) {
    digitalWrite(pulse, LOW);
      if (digitalRead(data) == 0){
        num = cnt;
      }
    digitalWrite(pulse, HIGH);
  }
  return num;
}

void setFeedingTime()
{
  feed = true;
  int i = 0;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set feeding Time");
  lcd.clear();
  lcd.print("HH:MM");
  lcd.setCursor(0,1);
  
  while(1) {
    Key = getkey();
    char j;
    
    if(Key != 0 && Key != old) {    
    Serial.println(Key);
      String keyString = String(Key);
      lcd.setCursor(j,1); 
      if (Key != 15) {
        if (Key == 10) {
          lcd.print('0');
          Serial.println(0);
        }
        else {
          lcd.print(keyString);
        }
      } 
      if (Key == 10) {
        r[i] = 0;
      }
      else {
        r[i] = Key;
      }
      i++;
      j++;
  
      if (j == 2)
      {
        lcd.print(":"); j++;
      }
      delay(500);
    }
    old = Key;

    if (Key == 15) {
      Key = 0;
      break;
    }
  }
}

void setup() {
  servo_test.attach(10);   // attach the signal pin of servo to pin9 of arduino
  rtc.begin();
  lcd.begin(16,2);
  servo_test.write(0); 
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(pulse, OUTPUT);
  pinMode(data, INPUT);
}

void loop() {

  Serial.begin(9600);
  
  lcd.setCursor(0,0);
  int buttonPress;
  buttonPress = digitalRead(A3);

  if (buttonPress == 1)
    setFeedingTime();
 
   lcd.print("Time:  ");
   String t = "";
   t = rtc.getTimeStr(); 
   t1 = t.charAt(0)-48;
   t2 = t.charAt(1)-48;
   t3 = t.charAt(3)-48;
   t4 = t.charAt(4)-48;
   t5 = t.charAt(6)-48;
   t6 = t.charAt(7)-48;
   
   lcd.print(rtc.getTimeStr());
   lcd.setCursor(0,1);
   lcd.print("Date: ");
   lcd.print(rtc.getDateStr());
   
   if (t1 == r[0] && t2 == r[1] && t3 == r[2] && t4 == r[3]&& t5 < 1 && t6 < 3 && feed == true) { 
    servo_test.write(25); //command to rotate the servo to the specified angle 
     delay(700);   
    servo_test.write(0); 
    feed=false;
   } 
 }
