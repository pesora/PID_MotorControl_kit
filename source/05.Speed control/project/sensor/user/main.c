#include "sysinc.h"
#include "motor.h"
#include "encoder.h"

void key_deal(void);
void key_init(void);
int PWM_MAX = 9000;
/**
  * @function incremental PI controller
  * @param[in] Encoder encoder measurement value, current speed
  * @param[in] Target target speed
  * @retval pwm calculated PWM value, representing incremental output
  * @node Incremental discrete PID formula pwm+=Kp[e(k)-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k -2)]
  * @node e(k) represents this deviation
  * @node e(k-1) represents the last deviation
  * @node e(k-2) represents the deviation from the previous time
  * @node In the speed control closed-loop system, only PI control is used pwm+=Kp[e(k)-e(k-1)]+Ki*e(k)
  */
float SKp=5,SKi=10;
int Incremental_PI (int Encoder,int Target)
{
static int Bias,Pwm,Last_bias;
Bias=Encoder-Target; //Calculate bias
Pwm+=SKp*(Bias-Last_bias)+SKi*Bias; //Incremental PI controller
Last_bias=Bias; //Save the last deviation
return Pwm; //incremental output
}

int SPWM = 0;
int target_v = 20;
int encoder_v = 0;

void timer3()
{
if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){ //Check whether TIM3 update interrupt occurs

encoder_v = get_encoder_val(); //Get the encoder value

SPWM = Incremental_PI( encoder_v, target_v); //Algorithm to obtain PWM
if(SPWM > PWM_MAX) SPWM = PWM_MAX;
else if(SPWM < -PWM_MAX) SPWM = -PWM_MAX;
set_motor_pwm(SPWM);

printf("qh-0-%d,%d-ed\n", encoder_v, target_v);
key_deal();

TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //Clear the TIMx update interrupt flag
}
}


u8 flag_rev = 0; //Serial port interrupt reception flag bit
u8 flag_rev_num = 0; //Serial port data index
u8 rxbuf[55]={1,9,3,4}; //Serial port interrupt receiving data BUFF

//Serial port interrupt function
void uart1(uint16_t Res);


int main(void){

NVIC_init();
SYSTICK_DelayInit(); //Delay function initialization
GPIO_QuickInit(HW_GPIOC, GPIO_Pin_13, GPIO_Mode_Out_PP);//Initialize the hardware interface connected to the LED
UART_QuickInit(HW_UART1, 115200, 2, 2, ENABLE);
UART_CallbackInstall(HW_UART1, uart1); //Serial port interrupt function registration

motor_init();
encoder_init();
key_init();

IIC_Init();
OLED_Init();


OLED_CLS();
OLED_P8x16Str(0,0," TAR: "); OLED_P8x16Num(48, 0, target_v , 0);
OLED_P8x16Str(0,2," CUR: "); OLED_P8x16Num(48, 2, encoder_v, 0 );
OLED_P8x16Str(0,4," P :"); OLED_P8x16Num(48, 4, SKp, 2);
OLED_P8x16Str(0,6," I :"); OLED_P8x16Num(48, 6, SKi, 2);

TIM_ms_Init(TIM3, 100, TIM_IT_Update,2, ENABLE);
  TIMER_CallbackInstall(HW_TIMER3, timer3);

   while(1){
PCout(13)= !PCout(13);
OLED_P8x16Str(0,0," TAR:"); OLED_P8x16Num(48, 0, target_v , 0);
OLED_P8x16Str(0,2," CUR: "); OLED_P8x16Num(48, 2, encoder_v, 0 );
OLED_P8x16Str(0,4," P :"); OLED_P8x16Num(48, 4, SKp, 2);
OLED_P8x16Str(0,6," I :"); OLED_P8x16Num(48, 6, SKi, 2);
}
}










///////////////The following functions are not important, they are called internally and have nothing to do with the algorithm/////////////////////





//String to number function
float getNum(u8 rb[]){
float data = 0;
int i;
for( i = 0; i < 8; i++){
if( i == 0 && rb[i] == '-' ) continue;
if(rb[i] != '.' && rb[i] != 121 && rb[i] != ',' ){
data = (data)*10 + rb[i]-48;
}else{
break;
}
}
if(rb[i] == '.'){ data = (data*10 + rb[i+1]-48)/10; }
if(rb[0] == '-'){ data = -1*data; }
return data;
}


//Get the PID function from the string
void getPID(){
u8 i = 0; flag_rev_num = 5;
u8 buf[15];
for(i =0; i < 15 && rxbuf[4] == '-'; i++){
buf[i] = 121; buf[i+1] = 121; buf[i+2] = 121;
buf[i] = rxbuf[flag_rev_num]; flag_rev_num++;
//printf("%c", buf[i]);
if(rxbuf[flag_rev_num] == ','){ break;}
}
SKp = getNum(buf);

if( rxbuf[flag_rev_num++] == ',' ){
for(i =0; i < 15; i++){
buf[i] = 121; buf[i+1] = 121; buf[i+2] = 121;
buf[i] = rxbuf[flag_rev_num]; flag_rev_num++;
printf("%c", buf[i]);
if(rxbuf[flag_rev_num] == ','){ break;}
}
}
SKi = getNum(buf);

if( rxbuf[flag_rev_num++] == ',' ){
for(i =0; i < 15; i++){
buf[i] = 121; buf[i+1] = 121; buf[i+2] = 121;
buf[i] = rxbuf[flag_rev_num]; flag_rev_num++;
printf("%c", buf[i]);
if(rxbuf[flag_rev_num] == ','){ break;}
}
}
float SKd = getNum(buf);


if( rxbuf[flag_rev_num++] == ',' ){
for(i =0; i < 15; i++){
buf[i] = 121; buf[i+1] = 121; buf[i+2] = 121;
buf[i] = rxbuf[flag_rev_num]; flag_rev_num++;
printf("%c", buf[i]);
if(rxbuf[flag_rev_num] == ','){ break;}
}
}
target_v = getNum(buf);

if( rxbuf[flag_rev_num++] == ',' ){
for(i =0; i < 15; i++){
buf[i] = 121; buf[i+1] = 121; buf[i+2] = 121;
buf[i] = rxbuf[flag_rev_num]; flag_rev_num++;
//printf("%c", buf[i]);
if(rxbuf[flag_rev_num] == '-'){ break;}
}
}
PWM_MAX = getNum(buf);

printf("qh-1-%.1f,%.1f,%.1f,%d,%d-ed", SKp, SKi, SKd, target_v, PWM_MAX );

OLED_P8x16Str(0,0," TAR: "); OLED_P8x16Num(48, 0, target_v , 0);
OLED_P8x16Str(0,2," CUR: "); OLED_P8x16Num(48, 2, encoder_v, 0 );
OLED_P8x16Str(0,4," P : "); OLED_P8x16Num(48, 4, SKp, 2);
OLED_P8x16Str(0,6," I : "); OLED_P8x16Num(48, 6, SKi, 2);
}


//Get parameter instruction classification
void getPraVue(){
int pra = rxbuf[3]-48;
//printf("pra is %d \n", pra);
switch(pra){
case 0: break;
case 1: getPID(); break;
case 2: break;
case 3: break;
}
}

void uart1(uint16_t Res){

if(Res == 'q'){ flag_rev = 1; flag_rev_num = 0; } //If q is received, start receiving data

if(flag_rev == 1 && flag_rev_num < 50 ){
rxbuf[flag_rev_num++] = Res; //Receive data
//UART_WriteByte(HW_UART1,rxbuf[flag_rev_num-1]);
if(Res == 'e'){ flag_rev = 0; getPraVue(); } //receive e, end receiving data, one frame of data reception is completed
}


}

void key_init(){
GPIO_QuickInit(HW_GPIOA, GPIO_Pin_6, GPIO_Mode_IPD); //KEY SUB
GPIO_QuickInit(HW_GPIOA, GPIO_Pin_7, GPIO_Mode_IPD); //KEY SUB
GPIO_QuickInit(HW_GPIOB, GPIO_Pin_0, GPIO_Mode_IPD); //KEY SUB
}

void key_deal(){
if( PAin(6) == 1 ){
SYSTICK_DelayMs(10);
if( PAin(6) == 1 ){
if(target_v < 80)
target_v += 5;
}
OLED_P8x16Str(48,0," "); OLED_P8x16Str(48,2," ");
while( PAin(6) == 1 );
}

if( PAin(7) == 1 ){
SYSTICK_DelayMs(10);
if( PAin(7) == 1 ){
if(target_v > 5)
target_v -= 5;
}
OLED_P8x16Str(48,0," "); OLED_P8x16Str(48,2," ");
while( PAin(7) == 1 );
}

if(PBin(0) == 1){
SYSTICK_DelayMs(10);
if(PBin(0) == 1){
target_v = 20;
}
OLED_P8x16Str(48,0," "); OLED_P8x16Str(48,2," ");
while(PBin(0) == 1);
}

}