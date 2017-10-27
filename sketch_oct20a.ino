int i = 0;
boolean down = false;
int btnPush = 0;
int cycle = 200;
int crntLED[] = {2, 3, 4, 6, 7};
volatile int state = HIGH;
volatile int flag = LOW;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(2,OUTPUT);
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);
pinMode(5,INPUT_PULLUP);
pinMode(6,OUTPUT);
pinMode(7,OUTPUT);
attachInterrupt(5, btnPressed, RISING);
//attachInterrupt(5, btnPressed, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly: 

//Serial.println(i);


LPM0;
//Serial.println(i, DEC);
if ( i == 0){
//  case 0:
    digitalWrite(2,HIGH);
    delay(cycle);
    digitalWrite(2,LOW);
//    break;
}
else if (i == 1){
//  case 1:
    digitalWrite(3,HIGH);
    delay(cycle*4);
    digitalWrite(3,LOW);
    delay(cycle*4);
//    break;
}
else if (i == 2){
//  case 2:
    digitalWrite(4,HIGH);
    delay(cycle*2);
    digitalWrite(4,LOW);
    delay(cycle*2);
//    break;
}
else if (i == 3){
//  case 3:
    digitalWrite(6,HIGH);
    delay(cycle);
    digitalWrite(6,LOW);
    delay(cycle);
//    break;
}
else if (i == 4){
//  case 4:
    digitalWrite(7,HIGH);
    delay(cycle/2);
    digitalWrite(7,LOW);
    delay(cycle/2);
//    break;
//  default:
//    digitalWrite(2,HIGH);
//    delay(100);
//    digitalWrite(2,LOW);
//    break;
};
  
}

void btnPressed(){
  
  flag = HIGH;
  if (down == false){
    i = i + 1;}
  else if (down == true){
    i = i - 1;
  }
  if (i == 0){
    down = false;}
  else if (i == 4){
    down = true;};
}

