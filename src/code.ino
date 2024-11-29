#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>//https://m.blog.naver.com/aul-_-/221793940620 여기 나온대로 라이브러리 다운 받기

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int mag0 = 3;
int mag1 = 4;
int mag2 = 5;
int a=0;

void setup() 
{
  Serial.begin(9600);//테스트 용 시리얼 통신
  pwm.begin();  
  pwm.setPWMFreq(50);//작동 잘 안되면 50에서부터 다른 수 넣기-frequency
  pinMode(mag0, OUTPUT);
  pinMode(mag1, OUTPUT);
  pinMode(mag2, OUTPUT);
}

void loop() 
{
  digitalWrite(3,HIGH); //전자석
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  a = Serial.read();
  if(a== '0')//전진
  {
    Serial.print(a);
    act(0,1);//여기있는 숫자는 모듈에 꽂은 핀 번호 기준으로 한 모터 구분  
  }
  else if(a=='1')//왼쪽
  {
    Serial.print(a);
    act(2,0);
  }
  else if(a=='2')//오른쪽
  {
    Serial.print(a);
    act(1,2);
  }
}


void act(int num1, int num2)//움직이기
{
  //if문 1
//  if(num1==0) //num1 = 0, num2 = 1
//  {
//    digitalWrite(mag0,HIGH);
//    digitalWrite(mag1,HIGH);
//    digitalWrite(mag2,LOW);
//  }
//  
//  else if(num1==1) //num1 = 1, num2 = 2
//  {
//    digitalWrite(mag0,LOW);
//    digitalWrite(mag1,HIGH);
//    digitalWrite(mag2,HIGH);
//  }
//  else if(num1==2) //num1 = 2, num2 = 0
//  {
//    digitalWrite(mag0,HIGH);
//    digitalWrite(mag1,LOW);
//    digitalWrite(mag2,HIGH);
//  }
//  delay(3000);
  
  for(double a=30,b=150;a>=9.155,b>=108.31; a-=0.1,b-=0.2)//실수형 변수(다시 계산 : 41.69 / 20.845) / 기존 값 42.24 / 141.12
  {
    double rx = constrain(map(a,0,180,150,600),150,600); //입력받은 x값을 매핑(0~180도를 150~600으로 확장, constrain으로 범위를 한정(0보다 작은 수를 150보다 작은 수로 변환하지 않게)
    double ry = constrain(map(b,0,180,150,600),150,600);
    pwm.setPWM(num1,0,rx); //pca9685의 (num1)번 핀-->여기 괄호 속 첫번째는 핀번호, 두번째는 무조건 0으로, 마지막은 변환된 각도
    pwm.setPWM(num2,0,ry); //pca9685의 (num2)번 핀
    
//    Serial.print(rx);//요거는 각도 제대로 입력하는지 시리얼 모니터로 테스트 할 수 있는 코드
//    Serial.print('\n');
    
    delay(10);
  }
  
  for(double a=9.155,b=108.31;a<=108.31,b>=9.155;a+=0.1,b-=0.1)
  {
    double rx = constrain(map(a,0,180,150,600),150,600); 
    double ry = constrain(map(b,0,180,150,600),150,600);
    pwm.setPWM(num1,0,rx); 
    pwm.setPWM(num2,0,ry); 
    
//    Serial.print(rx);
//    Serial.print('\n');
    
    delay(10);
   
  }
  for(double a=108.31,b=9.155;a<=150,b<=30;a+=0.2,b+=0.1)
  {
    double rx = constrain(map(a,0,180,150,600),150,600); 
    double ry = constrain(map(b,0,180,150,600),150,600);
    pwm.setPWM(num1,0,rx); 
    pwm.setPWM(num2,0,ry); 

//    Serial.print(rx);
//    Serial.print('\n');
    
    delay(10);
   
  }
  //if 2
//  if(num1==0) //num1 = 0, num2 = 1
//  {
//    digitalWrite(mag0,LOW);
//    digitalWrite(mag1,HIGH);
//    digitalWrite(mag2,HIGH);
//  }
//  else if(num1==1)//num1 = 1, num2 = 2
//  {
//    digitalWrite(mag0,HIGH);
//    digitalWrite(mag1,LOW);
//    digitalWrite(mag2,HIGH);
//  }
//  else if(num1==2)//num1 = 2, num2 = 0
//  {
//    digitalWrite(mag0,HIGH);
//    digitalWrite(mag1,HIGH);
//    digitalWrite(mag2,LOW);
//  }
//  delay(3000);
  pwm.setPWM(num1,0,225);//다시 초기 상태로 조절(120도)
  
  //if 3
//  if(num1==0)//num1 = 0, num2 = 1
//  {
//    digitalWrite(mag0,HIGH);
//    digitalWrite(mag1,LOW);
//    digitalWrite(mag2,HIGH);
//  }
//  else if(num1==1)// num1 = 1, num2 = 2
//  {
//    digitalWrite(mag0,HIGH);
//    digitalWrite(mag1,HIGH);
//    digitalWrite(mag2,LOW);
//  }
//  else if(num1==2)//num1 = 2, num2 = 0
//  {
//    digitalWrite(mag0,LOW);
//    digitalWrite(mag1,HIGH);
//    digitalWrite(mag2,HIGH);
//  }
//  delay(3000);
  pwm.setPWM(num2,0,525);//다시 초기 상태로 조절(0도)
  
  delay(5000);//제대로 작동하는지 확인하기 위해 딜레이 길게 설정
}
