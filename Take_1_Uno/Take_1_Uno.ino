#include <AddicoreRFID.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <Wire.h>
#define SLAVE 4

#define uchar unsigned char
#define uint  unsigned int
#define MAX_LEN 16 //배열의 최대 길이

SoftwareSerial mySerial(9,6); // RX, TX
SoftwareSerial BTSerial(2, 3); 
AddicoreRFID myRFID;
uchar serNumA[5];
uchar fifobytes;
uchar fifoValue;

const int chipSelectPin = 10; //SDA핀 설정
const int NRSTPD = 9;

void setup() {               
   Serial.begin(9600); 
   SPI.begin();  
   mySerial.begin (9600);        
   BTSerial.begin(9600);
   Wire.begin();
   
  pinMode(chipSelectPin,OUTPUT);              // 디지털 핀 10을 OUTPUT으로 설정하여 RFID / ENABLE 핀에 연결
  pinMode(NRSTPD,OUTPUT);                     // 디지털 핀 10, 재설정 안 함 및 전원 꺼짐 설정
  digitalWrite(chipSelectPin, LOW);           // RFID 읽기 활성화
  digitalWrite(NRSTPD, HIGH);
 
  myRFID.AddicoreRFID_Init(); 

  mp3_set_serial (mySerial);    //set softwareSerial for DFPlayer-mini mp3 module 
  delay(1);                     // delay 1ms to set volume
  mp3_set_volume (15);          // value 0~30
}
 
void loop()
{
  Wire.requestFrom(SLAVE, 4); // 1 바이트 크기의 데이터 요청
  int c = Wire.read();
  delay(400);
  Serial.print("값 is :");
  Serial.println(c);
 
  uchar i, tmp, checksum1;
  uchar status;
  uchar str[MAX_LEN];
  uchar RC_size;
  uchar blockAddr; 
  String mynum = "";
  str[1] = 0x4400;
 
    status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str);
    status = myRFID.AddicoreRFID_Anticoll(str);
    if (status == MI_OK)
    {
      checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];
      Serial.print("The tag's number is  : ");
      Serial.println(str[0]);

      if(str[0] == 204)  
      {                  
        
        Serial.print("도서실!\n");
        mp3_play(1);
        BTSerial.write("000\n"); 
      } else if(str[0] == 62) {       
            
        Serial.print("세미나실!\n");
        mp3_play(2);
        BTSerial.write("001\n");
      } else if(str[0] == 53) {     
              
        Serial.print("화장실!\n");
        mp3_play(3);
        BTSerial.write("002\n");
      } else if(str[0] == 195) {     
              
        Serial.print("회의실!\n");
        mp3_play(4);
        BTSerial.write("003\n");
      } else if(str[0] == 121) {     
              
        Serial.print("휴게실!\n");
        mp3_play(5);
        BTSerial.write("004\n");
      }


      if(c == 11){
        if(str[0] == 42) { 
          mp3_play(6);
        }
      }
      Serial.println();
      delay(300);
    } 
        myRFID.AddicoreRFID_Halt();        //절전모드 실행 
}

