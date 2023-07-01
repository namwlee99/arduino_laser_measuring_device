#include <U8glib.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
#include <DHT.h>
#include <SoftwareSerial.h>

SoftwareSerial hc06 (2, 3); //블루투스 모듈

#define switch1 5
#define switch2 6
#define switch3 7
#define trigPin1 10  //tx 초음파 발신핀
#define echoPin1 11  //rx 초음파 수신핀
#define trigPin2 12 //tx 초음파 발신핀
#define echoPin2 13 //rx 초음파 수신핀
#define buzzer 8

int distance_front, distance_back;
int delay1 = 0, oled_front_state = 0, oled_sum_state = 0;

void setup()
{
  Serial.begin(9600); //Baud rate: 9600
  hc06.begin(9600);
  
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(switch1, INPUT);
  pinMode(switch2, INPUT);
  pinMode(switch3, INPUT);
  pinMode(buzzer, OUTPUT);
}

void oled(){
   //OLED 작동 코드
   u8g.firstPage();

   do
   {
      u8g.setFont(u8g_font_unifont);
      u8g.setPrintPos(0,23);
      u8g.drawStr(0,23, "Front Dis: ");
      u8g.setPrintPos(85,23); //픽셀 단위라 x를 작게 잡아주면 글씨가 겹칩니다.
      u8g.print(distance_front);
      u8g.setPrintPos(110,23);
      u8g.print("CM");

      u8g.setFont(u8g_font_unifont);
      u8g.setPrintPos(0,38);
      u8g.drawStr(0,38, "Back  Dis: ");
      u8g.setPrintPos(85,38); //픽셀 단위라 x를 작게 잡아주면 글씨가 겹칩니다.
      u8g.print(distance_back);
      u8g.setPrintPos(110,38);
      u8g.print("CM");

      u8g.setFont(u8g_font_unifont);
      u8g.setPrintPos(0,53);
      u8g.drawStr(0,53, "Sum   Dis: ");
      u8g.setPrintPos(85,53); //픽셀 단위라 x를 작게 잡아주면 글씨가 겹칩니다.
      u8g.print(distance_front + distance_back);
      u8g.setPrintPos(110,53);
      u8g.print("CM");
   }

   while(u8g.nextPage());
}

void oled_front(){
   //OLED 작동 코드
   u8g.firstPage();

   do
   {
      u8g.setFont(u8g_font_unifont);
      u8g.setPrintPos(0,38);
      u8g.drawStr(0,38, "Front Dis: ");
      u8g.setPrintPos(85,38); //픽셀 단위라 x를 작게 잡아주면 글씨가 겹칩니다.
      u8g.print(distance_front);
      u8g.setPrintPos(110,38);
      u8g.print("CM");
   }

   while(u8g.nextPage());
}

void oled_sum(){
   //OLED 작동 코드
   u8g.firstPage();

   do
   {
      u8g.setFont(u8g_font_unifont);
      u8g.setPrintPos(8,38);
      u8g.drawStr(8,38, "Sum Dis: ");
      u8g.setPrintPos(78,38); //픽셀 단위라 x를 작게 잡아주면 글씨가 겹칩니다.
      u8g.print(distance_front + distance_back);
      u8g.setPrintPos(103,38);
      u8g.print("CM");
   }

   while(u8g.nextPage());
}

void oled_dontmove(){
   //OLED 작동 코드
   u8g.firstPage();

   do
   {
      u8g.setFont(u8g_font_unifont);
      u8g.setPrintPos(50,38);
      u8g.drawStr(25,38, "Don't Move");
   }

   while(u8g.nextPage());
}

void sending(){
  //OLED 작동 코드
  u8g.firstPage();

  do
  {
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(18,30);
    u8g.drawStr(18,30, "Please, Wait");
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(25,44);
    u8g.drawStr(25,44, "Sending...");
  }

  while(u8g.nextPage());
}

void bluetooth_front(){
  hc06.print("Front Dis: ");
  hc06.print(distance_front);
  hc06.print("CM");
}

void bluetooth_sum(){
  hc06.print("Sum Dis: ");
  hc06.print(distance_front + distance_back);
  hc06.print("CM");
}

void loop(){
    //초음파센서 한 번의 초기화 하는 과정
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1, HIGH);
    delay(10);
    digitalWrite(trigPin1, LOW);
    distance_front = pulseIn(echoPin1, HIGH);
    distance_front = distance_front*0.034/2; //시간으로 거리 계산
    Serial.println(distance_front);

    //초음파센서 한 번의 초기화 하는 과정
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH);
    delay(10);
    digitalWrite(trigPin2, LOW);
    distance_back = pulseIn(echoPin2, HIGH);
    distance_back = distance_back*0.034/2; //시간으로 거리 계산
    Serial.println(distance_back);

    oled();

    if(delay1 == 1){
      delay(1000);
      delay1 = 2;
    }

    if(digitalRead(switch1) == 1){
      oled_front_state = 1;
    }

    if(digitalRead(switch2) == 1){
      oled_sum_state = 1;
    }

    while(oled_front_state == 1){
      oled_dontmove();
      delay(3000);
      oled_front();
      delay1 = 0;
      digitalWrite(buzzer, HIGH);
      while(delay1 < (6001)){
        delay(10);
        delay1 += 1;
        if(delay1 < 10){
          digitalWrite(buzzer, LOW);
        }
        if((digitalRead(switch1) == 1) | (digitalRead(switch2) == 1)){
          oled_front_state = 0;
          delay1 = 1;
          break;
        }
        if(digitalRead(switch3) == 1){
          sending();
          bluetooth_front();
          oled_front_state = 0;
          delay1 =1;
          delay(1500);
          break;
        }
        if(delay1 == 600000){
          oled_front_state = 0;
          delay1 = 0;
          break;
        }
      }
    }

    while(oled_sum_state == 1){
      oled_dontmove();
      delay(3000);
      oled_sum();
      delay1 = 0;
      digitalWrite(buzzer, HIGH);
      while(delay1 < (6001)){
        delay(10);
        delay1 += 1;
        if(delay1 < 10){
          digitalWrite(buzzer, LOW);
         }
        if((digitalRead(switch1) == 1) | (digitalRead(switch2) == 1)){
          oled_sum_state = 0;
          delay1 = 1;
          break;
        }
        if(digitalRead(switch3) == 1){
          sending();
          bluetooth_sum();
          delay(1500);
          oled_sum_state = 0;
          delay1 = 1;
          break;
        }
        if(delay1 == 600000){
          oled_sum_state = 0;
          delay1 = 0;
          break;
        }
      }
    }
    
}
