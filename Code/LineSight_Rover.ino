#include <Wire.h>

#define BUFFER_SIZE 64 // Kích thước buffer
char buffer[BUFFER_SIZE]; // Buffer để gom các ký tự nhận được
int bufferIndex = 0; // Chỉ số của buffer hiện tại

const int motorPin1 = 2; // động cơ 1
const int motorPin2 = 4; // động cơ 2
const int enablePinA = 3; // Chân PWM điều khiển tốc độ bên trái

int countNga3 = 0;
int countNga4 = 0;
const int motorPin3 = 8; // động cơ 3
const int motorPin4 = 7; // động cơ 4
const int enablePinB = 6; // Chân PWM điều khiển tốc độ bên phải
int flag = 0;
int setSpeeds = 90;
//int setSpeed1 = 100;

void setup() {
  Wire.begin(0x08); // Địa chỉ I2C của Arduino
  Wire.setClock(4000000);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  
  //Khởi tạo chân kết nối với động cơ
  pinMode(12,INPUT);
  pinMode(13,INPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePinA, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(enablePinB, OUTPUT);
}


void receiveEvent(int byteCount) {
  while (Wire.available()) {
    char receivedChar = Wire.read();
    flag = 1;
    // Kiểm tra xem buffer đã đầy chưa hoặc ký tự nhận được là ký tự kết thúc chuỗi
    if (bufferIndex < BUFFER_SIZE - 1 && receivedChar != '\0') {
      buffer[bufferIndex] = receivedChar;
      bufferIndex++;
    } else {
      buffer[bufferIndex] = '\0'; // Đặt ký tự kết thúc chuỗi
      Serial.print("Received message: ");
      Serial.println(buffer);
      
      int offset = atoi(buffer);
       if(offset == 0)
       {
        motorForward();
       }
       
       else if (offset >= 12 && offset < 61 )
       {
        motorTurnLeftMedium();
        
       }
       else if(offset <= -12 && offset > -61)
       {
        motorTurnRightMedium();
        
       }

       
       else if (offset > 0 && offset < 12)
       {
        motorTurnLeftBit();
        
       }
       else if(offset < 0 && offset > -12 )
       {
        motorTurnRightBit();
        
       }
      bufferIndex = 0; // Reset buffer cho lần nhận tiếp theo
    }
    delay(40);
  }
}




/*
1,2 là bên trái
3,4 là bên phải 

*/

void motorForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePinA, 130);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  analogWrite(enablePinB, 130);
}
void motorBackward() {

  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(enablePinA, 100);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  analogWrite(enablePinB, 100);
}

// Hàm đi lùi

//Hàm quẹo trái



void motorTurnLeftMedium()
{
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(enablePinA, 150);//-90
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  analogWrite(enablePinB, 200 );//+140
}
void motorTurnRightMedium()
{
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePinA, 200);//-90
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  analogWrite(enablePinB, 190);//+140
}
void motorTurnLeftBit() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePinA, setSpeeds-50);//-90
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  analogWrite(enablePinB, setSpeeds+50 );//+140
}
//Hàm quẹo phải nhe
void motorTurnRightBit() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePinA, setSpeeds + 50);//-90
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  analogWrite(enablePinB, setSpeeds -50);//-140
}

// Hàm dừng động cơ
void motorStop() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePinA, 0);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  analogWrite(enablePinB, 0);
  delay(3000);
}
void QueoPhai()
{
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePinA, 250);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  analogWrite(enablePinB, 200);
  delay(1500);
}
void QueoTrai()
{
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(enablePinA, 200);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  analogWrite(enablePinB, 250);
//  delay(1700);
delay(1500);
}


int count = 0;
void loop() { 
}
