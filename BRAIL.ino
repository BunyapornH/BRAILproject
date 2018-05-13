// Date, Time and Alarm functions using a DS3231 RTC connected via I2C and Wire lib

#include "Wire.h"

#include "SPI.h"  // not used here, but needed to prevent a RTClib compile error

#include "RTClib.h"

#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x3F, 16, 2);

RTC_DS3231 RTC;
int m = 0 ;
int h = 0 ;
int a = 1 ;
/*int BRAIL[10][4] = {{HIGH,LOW,LOW,LOW}, {LOW,HIGH,HIGH,HIGH}, {LOW,HIGH,LOW,HIGH}, {LOW,LOW,HIGH,HIGH}, {LOW,LOW,HIGH,LOW}, {LOW,HIGH,HIGH,LOW}, {LOW,LOW,LOW,HIGH}, {LOW,LOW,LOW,LOW}, 
                    {LOW,HIGH,LOW,LOW}, {HIGH,LOW,LOW,HIGH}}; */
int BRAIL[10][4] = {{LOW,HIGH,HIGH,HIGH}, {HIGH,LOW,LOW,LOW}, {HIGH,LOW,HIGH,LOW}, {HIGH,HIGH,LOW,LOW}, {HIGH,HIGH,LOW,HIGH}, {HIGH,LOW,LOW,HIGH}, {HIGH,HIGH,HIGH,LOW}, {HIGH,HIGH,HIGH,HIGH}, 
                    {HIGH,LOW,HIGH,HIGH}, {LOW,HIGH,HIGH,LOW}};
int min0,min1;
int hr0,hr1;

int SW1 = 24;
int SW2 = 25;

int val1;
int val2;

int encoder0PinA1 = 28;
int encoder0PinB1 = 29;
int encoder0Pos1 = 0;
int encoder0PinALast1 = LOW;
int n1 = LOW;

int encoder0PinA2 = 26;
int encoder0PinB2 = 27;
int encoder0Pos2 = 0;
int encoder0PinALast2 = LOW;
int n2 = LOW;
int HR,MIN;
int AHr,AMin;



void setup () {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);

  pinMode(30, OUTPUT);

  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);

  pinMode(encoder0PinA1,INPUT);
  pinMode(encoder0PinB1,INPUT);

  pinMode(encoder0PinA2,INPUT);
  pinMode(encoder0PinB2,INPUT);
  
  Serial.begin(9600);

  Wire.begin();
 
  RTC.begin();

  lcd.begin();
  lcd.backlight();
  lcd.clear();

  // ตั้งเวลาเตือนให้เอา // ข้างหน้า RTC.adjust ออก
  

  RTC.adjust(DateTime(__DATE__, __TIME__));

  if (! RTC.isrunning()) {

    Serial.println("RTC is NOT running!");

    // following line sets the RTC to the date & time this sketch was compiled

    RTC.adjust(DateTime(__DATE__, __TIME__));
    
  }

 //กำหนดให้ นาฬิกาเตือนตอน 13:05

  RTC.setAlarm1Simple(AHr, AMin);

  RTC.turnOnAlarm(1);

  if (RTC.checkAlarmEnabled(1)) {

    Serial.println("Alarm Enabled");
  }
}
void loop () {
  
  val1 = digitalRead(SW1); // อ่านค่าจากตัวแปร val ว่าเป็น HIGH หรือ LOW    LOW = Time / High = Alarm
  val2 = digitalRead(SW2); // LOW = Setting / HIGH = Display
  DateTime now = RTC.now();
   
  if(val2==LOW)
  { 
    if(val1==LOW)
    {
      encoder0Pos1 = now.hour();
      encoder0Pos2 = now.minute();
      EN1();
      RTC.adjust(DateTime(2018,02,22,HR,MIN));
    } 
    if(val1==HIGH) 
    {
      // RTC.adjust(DateTime(2018,02,22,HR,MIN));
      AH();
      RTC.setAlarm1Simple(AHr, AMin);
      digitalWrite(30,LOW);
      //lcd.clear();
    }
  }
  
  if(val2==HIGH)
  {   
     
      DateTime now = RTC.now();
      m = now.minute();
      h = now.hour();

      Min(m);
      Hr(h);
      
      lcd.clear();
      
      digitalWrite(2,BRAIL[min0][0]);
      digitalWrite(3,BRAIL[min0][1]);
      digitalWrite(4,BRAIL[min0][2]);
      digitalWrite(5,BRAIL[min0][3]);
  
      digitalWrite(6,BRAIL[min1][0]);
      digitalWrite(7,BRAIL[min1][1]);
      digitalWrite(8,BRAIL[min1][2]);
      digitalWrite(9,BRAIL[min1][3]);

      digitalWrite(10,BRAIL[hr0][0]);
      digitalWrite(11,BRAIL[hr0][1]);
      digitalWrite(12,BRAIL[hr0][2]);
      digitalWrite(13,BRAIL[hr0][3]);

      digitalWrite(14,BRAIL[hr1][0]);
      digitalWrite(15,BRAIL[hr1][1]);
      digitalWrite(16,BRAIL[hr1][2]);
      digitalWrite(17,BRAIL[hr1][3]);


  Serial.print(' ');

  Serial.print(now.hour(), DEC);

  Serial.print(':');

  Serial.print(now.minute(), DEC);
  

  Serial.print(':');

  Serial.print(now.second(), DEC);
  Serial.print(' ');
  Serial.println();
  delay(1000);
 
  if (RTC.checkIfAlarm(1)) {

    // ถ้านาฬิกาเดินถึง 13:05 จะมาทำงานในนี้
    digitalWrite(30,HIGH);
    Serial.println("Alarm Triggered");
    lcd.setCursor(0,0);
    lcd.print("Alarm Triggered");
  }
  if (RTC.checkIfAlarm(!1)) {
    digitalWrite(30,LOW);
    lcd.clear();
  }
 }
}

void Min(int minmin) {
  
  if((minmin>=10)&&(minmin<60)){
    min0 = minmin%10;
    min1 = minmin/10;
  }
  if(minmin<10){
    min0 = minmin;
    min1 = 0;
  }

} 

 void Hr(int hrhr) {

  if((hrhr>=10)&&(hrhr<24)){
    hr0 = hrhr%10;
    hr1 = hrhr/10;
  }
  if(hrhr<10){
    hr0 = hrhr;
    hr1 = 0;
  }

} 

 void EN1() {

   n1 = digitalRead(encoder0PinA1);
   if ((encoder0PinALast1 == LOW) && (n1 == HIGH)) {
     if (digitalRead(encoder0PinB1) == HIGH) {
      if(encoder0Pos1>0)
            encoder0Pos1--;
     } else {
      if(encoder0Pos1<23)
            encoder0Pos1++;
     }
     Serial.println (encoder0Pos1);
     HR = encoder0Pos1;
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Hour = ");
     lcd.setCursor(8,0);
     lcd.print(HR);
   } 
   encoder0PinALast1 = n1;

   n2 = digitalRead(encoder0PinA2);
   if ((encoder0PinALast2 == LOW) && (n2 == HIGH)) {
     if (digitalRead(encoder0PinB2) == HIGH) {
      if(encoder0Pos2>0)
            encoder0Pos2--;
     } else {
      if(encoder0Pos2<59)
            encoder0Pos2++;
     }
     MIN = encoder0Pos2;
     Serial.println (MIN);
     lcd.clear();
     lcd.setCursor(0,1);
     lcd.print("Minute = ");
     lcd.setCursor(9,1);
     lcd.print(MIN);
     
   } 

   encoder0PinALast2 = n2;
 }

  void AH() { 
  
   n1 = digitalRead(encoder0PinA1);
   if ((encoder0PinALast1 == LOW) && (n1 == HIGH)) {
     if (digitalRead(encoder0PinB1) == HIGH) {
      if(encoder0Pos1>0)
            encoder0Pos1--;
     } else {
      if(encoder0Pos1<23)
            encoder0Pos1++;
     }
     AHr = encoder0Pos1;
     Serial.println (AHr);
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Hour = ");
     lcd.setCursor(8,0);
     lcd.print(AHr);
   
     Hr(AHr);
     digitalWrite(10,BRAIL[hr0][0]);
     digitalWrite(11,BRAIL[hr0][1]);
     digitalWrite(12,BRAIL[hr0][2]);
     digitalWrite(13,BRAIL[hr0][3]);

     digitalWrite(14,BRAIL[hr1][0]);
     digitalWrite(15,BRAIL[hr1][1]);
     digitalWrite(16,BRAIL[hr1][2]);
     digitalWrite(17,BRAIL[hr1][3]);

   } 
   encoder0PinALast1 = n1;

   n2 = digitalRead(encoder0PinA2);
   if ((encoder0PinALast2 == LOW) && (n2 == HIGH)) {
     if (digitalRead(encoder0PinB2) == HIGH) {
      if(encoder0Pos2>0)
            encoder0Pos2--;
     } else {
      if(encoder0Pos2<59)
            encoder0Pos2++;
     }
     AMin = encoder0Pos2;
     Serial.println (AMin);
     lcd.clear();
     lcd.setCursor(0,1); // ไปที่ตัวอักษรที่ 0 แถวที่ 1
     lcd.print("Minute = ");
     lcd.setCursor(9,1);
     lcd.print(AMin);
   
     Min(AMin);
     digitalWrite(2,BRAIL[min0][0]);
     digitalWrite(3,BRAIL[min0][1]);
     digitalWrite(4,BRAIL[min0][2]);
     digitalWrite(5,BRAIL[min0][3]);
  
     digitalWrite(6,BRAIL[min1][0]);
     digitalWrite(7,BRAIL[min1][1]);
     digitalWrite(8,BRAIL[min1][2]);
     digitalWrite(9,BRAIL[min1][3]);
   } 
   encoder0PinALast2 = n2;
 }


 
 
