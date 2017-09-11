/*시리얼 통신을 위한 전역 변수 선언*/
int state = 0; int _pulse; char c; bool p_changed = false; bool d_changed = false;
char _drive;

/* bit define */
#define bit0 0b00000001
#define bit1 0b00000010
#define bit2 0b00000100
#define bit3 0b00001000
#define bit4 0b00010000
#define bit5 0b00100000
#define bit6 0b01000000
#define bit7 0b10000000

/* constant number define */
#define maxV 24 // maximum voltage 24V
#define sT 0.001 // sampling time

/* Port define ( PWM ) 12번 PWM, 13번 DIR, 11번 BRAKE, 10번 ENABLE*/
#define PWM_PWM bit6
#define PWM_DIR bit7
#define PWM_ENABLE bit4
#define PWM_BRAKE bit5

/* Port define (Pulse) 6번핀 - Dir, 7번 핀 - Pulse신호, 5번 핀에 enable, 4번,3번핀에 ccw-, cw- 아무렇게나 꽂기 */
#define Pulse_Pulse bit4
#define Pulse_DIR bit3
#define Pulse_enable bit3
#define Pulse_ccw bit5
#define Pulse_cw bit5

/*position*/
/*조향 제어의 범위는 -30 ~ 30 degree 이며 기어 비는 약 2:1 이므로 실제로 모터의 제어 범위는 -60 ~ 60 degree*/
volatile int cnt_pos = 1667;

/*BLDC 속도 설정*/
volatile int velocity = 400;// 0<velocity<800사이로 조정할 것

/*조향 설정*/
volatile int dir = 1; //dir=0이면 cnt_pos<input_Dir일 때 ccw, 1이면 cnt_pos<input_Dir일 때 cw

 /* function declaration */
 /* timer interrupt setup */
void TimerInit(); // 1000 Hz

 /* PWM setup */
void PWMInit(); // 10 kHz (0~800)

 // Input Output setup 
void IOInit(); // data 8 pin, enc io 4 pin pwm dir, pwm (2pin)

 /*시리얼통신으로 값을 받아서 저장할 inputString 지정*/
volatile int Temp=0;
char a[10]; // 배열인데...? command로 복사함
int i = 0;
char command; //'g'입력 시 BLDC작동, 's' 입력 시 BLDC정지, 'u'입력 시 속도 증가, 'd' 입력 시 속도 감소 'h' 입력시 최고속도로 이동
volatile int inputDir = 1667;

 /* the setup part */
void setup()
{
  Serial.begin(115200);
  TimerInit();
  PWMInit();
  IOInit();
  
  PORTE |= Pulse_enable;

//  command.reserve(20);
  sei(); // global interrupt enable
}

void loop()
{
  /*구동제어 부분(여기서부터)*/
  if(d_changed){
    command = _drive; d_changed = false;
  }
  if(command == 'g'){
    OCR1B = velocity; //go 입력시 초기 속도로 회전
    Serial.println(command);
    Serial.print("speed is ");
    Serial.println(OCR1B);
  }

  else if(command == 'u'){
    if(OCR1B < 700){
      OCR1B += 100; // up입력시 속도 증가
      Serial.println(command);
      Serial.print("speed is ");
      Serial.println(OCR1B);
    }

    else{
      OCR1B = 799; // OCR1B가 OCR1A 이하가 되도록 함
      Serial.println(command);
      Serial.print("speed is ");
      Serial.println(OCR1B);
    }
  }

  else if(command == 'h'){
    OCR1B = 800; // 최고속도로 회전
    Serial.println(command);
    Serial.print("speed is ");
    Serial.println(OCR1B);
  }

  else if(command == 'd'){
    if(OCR1B > 100){
      OCR1B -= 100; //down 입력시 속도 감소
      Serial.println(command);
      Serial.print("speed is ");
      Serial.println(OCR1B);
    }

    else{
      OCR1B = 0; //OCR1B가 0 미만이 되지 않도록 함
      Serial.println(command);
      Serial.print("speed is ");
      Serial.println(OCR1B);
    }
  }

  else if(command == 's'){
    OCR1B = 0; //stop 입력 시 정지
    Serial.println(command);
    Serial.print("speed is ");
    Serial.println(OCR1B);
  }

//  else{
//    Temp = atoi(a);
//
//    if(Temp != 0){
//      inputDir = Temp;
//      Serial.println(inputDir);
//    }
//  }
  /*구동제어 부분(여기까지)*/
  delay(10);
    
  for(int b=0; b<10; b++){
    a[b]=NULL;
    
  }
  i=0;
  command = ""; //command 초기화

//  if(p_changed){
//     inputDir = _pulse; p_changed = false;
//  }
}


 /* ISR(TIMER3_COMPA_vect) function are executed every 1ms */
ISR(TIMER3_COMPA_vect)
{
  /*시리얼 통신을 받아서 cnt_pos와 비교한 뒤, 그보다 크면 오른쪽으로 1pulse, 작으면 왼쪽으로 1pulse를 준다.
  cw, ccw 4개의 선을 이용하여 코드를 수정해야 함. */
  if(cnt_pos < inputDir){
    /*pos_DIR을 0로 한다.*/
    if(dir ==0){
      PORTH &= ~Pulse_DIR;
    }

    else if(dir ==1){
      PORTH |= Pulse_DIR;
    }
    
    PORTH |= Pulse_Pulse;  
    delayMicroseconds(2);
    PORTH &= ~Pulse_Pulse;

    cnt_pos++;
  }
  else if(cnt_pos > inputDir){
    /*pos_DIR을 1로 한다.*/
    if(dir == 0){
      PORTH |= Pulse_DIR;
    }
    else if (dir == 1){
      PORTH &= ~Pulse_DIR;
    }
    PORTH |= Pulse_Pulse; 
    delayMicroseconds(2);
    PORTH &= ~Pulse_Pulse; 

    cnt_pos--;
  }
 Serial.print(cnt_pos); Serial.print(" "); Serial.println(inputDir);
}

 /* Use Timer 3, 1 kHz interrupt (Period : 1 ms) */
void TimerInit()
{
 /* WGM3 3:0 = 0100 CTC mode (TOP = OCRnA) */
  TCCR3A &= ~_BV(WGM31);
  TCCR3A &= ~_BV(WGM30);
  TCCR3B |= _BV(WGM32);
  TCCR3B &= ~_BV(WGM33);

 /* Timer prescaler setup : Clk/64 */
  TCCR3B &= ~_BV(CS32);
  TCCR3B |= _BV(CS31);
  TCCR3B |= _BV(CS30);

  /* f_interrupt = f_clk/(n*(1+TOP)) = 16MHz/(64*(1+249)) = 1000Hz = 1kHz*/
  OCR3A = 249;
  TCNT3 = 0;

  // Timer/Counter1 , Output Compare A Match Interrupt Enable
  TIMSK3 |= _BV(OCIE3A);
}

 /* Use Timer 1, 10 kHz carrier frequency PWM ( 0 to 800 resolution 24 V )*/
void PWMInit()
{
 /* upcounting match Clear , downcounting match Set PWM */
  TCCR1A |= _BV(COM1B1);
  TCCR1A &= ~_BV(COM1B0);

 /* Phase and Frequency Correct PWM */
 /* TOP is OCR1A */
  TCCR1A |= _BV(WGM10);
  TCCR1A &= ~_BV(WGM11);
  TCCR1B &= ~_BV(WGM12);
  TCCR1B |= _BV(WGM13);

 /* Prescaler is Clk/1 */
  TCCR1B &= ~_BV(CS12);
  TCCR1B &= ~_BV(CS11);
  TCCR1B |= _BV(CS10);

 /* TOP is 800 */
 /* f_carrier = f_clk/(2*n*TOP) = 16MHz/(2*1*800) = 10000Hz = 10kHz */
  OCR1A = 800;

 /* Output PWM is OC1B pin (PORTB6) */
 /* setup initial campare match value 0 ( 0 % duty PWM ) */
  OCR1B = 0;
}



 /* Input Output setup */
void IOInit()
{
 /* Output setup for PWM */
 /* OC1B PWM : PORTB6 , Dir. pin : PORTB7 */
  DDRB |= (PWM_PWM|PWM_DIR|PWM_ENABLE|PWM_BRAKE);//13번-PWM핀, 12번-DIR핀, 10번-ENABLE핀, 11번-BRAKE핀, 11번은 무조건 LOW, 10번은 무조건 HIGH로 둬야 한다. 
  DDRH |= (Pulse_Pulse|Pulse_DIR);
  DDRE |= (Pulse_enable|Pulse_cw);
  DDRG |= Pulse_ccw;
 /* Initial PWM direction setup */
  PORTB |= (PWM_DIR|PWM_BRAKE); //12번, 11번 pin HIGH
  PORTB &= ~PWM_ENABLE; //10번 pin LOW

}
/*시리얼 통신을 위한 코드
 *1Cyclse당 3byte의 값을 전송받음
 * state == 0 조향 값의 고 비트부분
 * state == 1 조향 값의 저 비트부분 
 * state == 2 구동 제어 부분*/
void serialEvent(){
  if(state == 0){   
        _pulse = 0;
        c = (char) Serial.read();
        _pulse += c; 
        Serial.print(c, HEX); Serial.print(" ");Serial.println(_pulse, HEX);
        state = 1;
      
   }
   else if(state == 1){
     _pulse = _pulse << 8;
     c = (char) Serial.read();
     _pulse += c; 
     Serial.print(c, HEX); Serial.print(" ");Serial.println(_pulse, HEX);
     Serial.println(_pulse);
      state = 2; inputDir = _pulse;
   }
   else if(state == 2){
    c = (char) Serial.read();
    _drive = c;
    state = 0; d_changed = true;
   }
}
