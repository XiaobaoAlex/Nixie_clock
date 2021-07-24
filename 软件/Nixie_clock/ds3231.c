//模拟iic
//ds3231驱动
#include "ds3231.h"
 

 
 
 
void DS3231_Delay(void) {    //DS3231通信速率延时,延时5微秒  12T单片机@12M

  unsigned int i;
  unsigned int j;
  
  for(i = 0; i < 100; i++)
     for(j = 0; j < 400; j++);
  
}
 
void DS3231_DelayForWrite(void){  //DS3231写字节延时,延时5毫秒 12T单片机@12M
  unsigned int i;
  unsigned int j;
  
  for(i = 0; i < 100; i++)
     for(j = 0; j < 4000; j++);
}
 
 
//产生开始信号
void DS3231_Start(void) { //模拟DS3231通信开始信号，SCL＝1期间，在SDA上产生一个下降沿
    DS3231_sda = 1;
    DS3231_scl = 1;DS3231_Delay(); 
    DS3231_sda = 0;DS3231_Delay(); 
} 
//产生结束信号
void DS3231_Stop(void) {//模拟DS3231通信结束信号，SCL＝1期间，在SDA上产生一个上升沿
    DS3231_sda = 0;DS3231_Delay();
    DS3231_scl = 1;DS3231_Delay();
    DS3231_sda = 1;DS3231_Delay();
} 
 
unsigned char DS3231_WriteByte(unsigned char SendByte){ //向DS3231设备写一字节数据及8为二进制数据，高位在前 
    unsigned char i=8;
    DS3231_scl = 0;    
    for(i=0; i<8; i++) { 
        if(SendByte&0x80){DS3231_sda = 1;}
        else {DS3231_sda = 0;}
         
        DS3231_scl = 0;DS3231_Delay();                   
        DS3231_scl = 1; DS3231_Delay(); 
        SendByte=SendByte<<1;
        DS3231_scl = 0;DS3231_Delay(); 
    }
    DS3231_sda = 1;DS3231_Delay();
    DS3231_scl = 0;DS3231_Delay();
    DS3231_scl = 1;DS3231_Delay();
    i = DS3231_sda;DS3231_Delay();
    DS3231_scl = 0;DS3231_Delay();//ack位时，scl为1时sda为1，表示nack,结束
    return i;       
} 
 
unsigned char DS3231_ReceiveByte(unsigned char Response) { //接收DS3231发送的数据,Response代表有无应答，当response为1时，代表无应答，传输结束
    unsigned char i=8; 
    unsigned char ReceiveByte=0;
    DS3231_sda = 1;DS3231_Delay();   
    DS3231_scl = 0;DS3231_Delay();     
    for(i=0; i<8; i++){ 
        DS3231_scl = 1;DS3231_Delay();
        ReceiveByte = ReceiveByte << 1;       
        ReceiveByte=ReceiveByte|(unsigned char)DS3231_sda; 
        DS3231_scl = 0;DS3231_Delay(); 
    }
    if(Response) DS3231_sda = 1;    
    else DS3231_sda = 0;    
    DS3231_Delay();
    DS3231_scl = 1;DS3231_Delay();
    DS3231_scl = 0;DS3231_Delay();
    DS3231_sda = 1;DS3231_Delay(); 
    return ReceiveByte; 
}                                                               
 
unsigned char DS3231_ReadOneByteFromE2PROM(unsigned char DataAddress) {//读指定地址E2PROM中的数据，只读输入寄存器地址的数据，不再向后读取
    unsigned char ReadData;
    DS3231_Start();                       //DS3231芯片IIC通信开始信号
    DS3231_WriteByte(DS3231WriteAddress); //写入芯片IIC写地址
    DS3231_WriteByte(DataAddress);        //写入状态寄存器地址?
    DS3231_Start();                       //DS3231芯片IIC通信开始信号
    DS3231_WriteByte(DS3231ReadAddress);  //写入芯片IIC读地址
    ReadData = DS3231_ReceiveByte(0x01);//只接受该种状态寄存器内的值，然后就结束，不再对之后的寄存器内的值进行接收
    DS3231_Stop();                         //DS3231芯片IIC通信停止信号
    return ReadData;   
}
 
void DS3231_Initialization(void){            //初始化时钟芯片DS3231,先选择要写入的寄存器，在写入需要设置的指令
    DS3231_Start();                                             //IIC通信起始信号
    DS3231_WriteByte(DS3231WriteAddress);  //写入芯片IIC写地址
    DS3231_WriteByte(DS3231_Hour);         //选择时寄存器为写入地址
    DS3231_WriteByte(0x00);                //写入指令，时钟范围为0-23，即24小时制式
    DS3231_Stop(); 
     
    DS3231_Start();                                                      //IIC通信起始信号
    DS3231_WriteByte(DS3231WriteAddress);      //写入芯片IIC写地址
    DS3231_WriteByte(DS3231_Interrupt);        //选择中断寄存器为写入地址     
    DS3231_WriteByte(0x04);                    //中断寄存器初始化,关闭方波信号，关闭闹钟中断
    DS3231_WriteByte(0x00);                    //状态寄存器初始化,失效32KHz信号输出，不匹配闹钟
    DS3231_Stop(); 
}
 
void DS3231_SetTime(unsigned char *Pointer){    //向DS3231写入设置时间信息,依次是秒、分钟、小时、星期、日、月、年
    unsigned char Number = 0x00;
  unsigned char TransitionData = 0x00;
    DS3231_Start();                             //DS3231芯片IIC通信开始信号
    DS3231_WriteByte(DS3231WriteAddress);       //写入芯片IIC写地址
    DS3231_WriteByte(DS3231_TimeFirst);         //写入时间寄存器首地址
  for(Number=0; Number<7; Number++)  {
    TransitionData = *Pointer++;
    DS3231_WriteByte((TransitionData/10)*16+TransitionData%10);   //向DS3231写入设置时间信息，每次写完地址会递增
  }
  DS3231_Stop();                                   //DS3231芯片IIC通信停止信号
    DS3231_DelayForWrite();
}
 
void DS3231_ReadTime(unsigned char *Pointer){       //从DS3231中读出当前时间
    unsigned char Number  = 7;
  unsigned char Time    = 0x00;
    DS3231_Start();                                  //DS3231芯片IIC通信开始信号
    DS3231_WriteByte(DS3231WriteAddress);            //写入芯片IIC写地址
    DS3231_WriteByte(DS3231_Status);                 //写入状态寄存器首地址
    DS3231_WriteByte(0x00);                          //关闭闹钟中断标志位
    DS3231_Stop();                                   //DS3231芯片IIC通信停止信号
 
    DS3231_Start();                                  //DS3231芯片IIC通信开始信号
    DS3231_WriteByte(DS3231WriteAddress);            //写入芯片IIC写地址
    DS3231_WriteByte(DS3231_TimeFirst);              //写入时间寄存器首地址
    DS3231_Start();                                  //DS3231芯片IIC通信开始信号
    DS3231_WriteByte(DS3231ReadAddress);             //写入芯片IIC读地址
  for(Number=0; Number<6; Number++) {
        Time = DS3231_ReceiveByte(0x00);			//带有ack读取，每次读地址会递增
    *Pointer++ = (Time/16*10+Time%16);    
  }
    Time = DS3231_ReceiveByte(0x01);
    *Pointer++ = (Time/16*10+Time%16); 
  DS3231_Stop();                                 //DS3231芯片IIC通信停止信号
}
void DS3231_get_time_display_array(unsigned char *data)
{
    unsigned char *time_msg_time;
    DS3231_ReadTime(time_msg_time);
    *data=(*time_msg_time)/10;
    *(data+1)=(*time_msg_time)%10;
    *(data+2)=(*(time_msg_time+1))/10;
    *(data+3)=(*(time_msg_time+1))%10;
    *(data+4)=(*(time_msg_time+2))/10;
    *(data+5)=(*(time_msg_time+2))%10;
  
}
void DS3231_get_date_display_array(unsigned char *data)
{
    unsigned char * time_msg_date;
    DS3231_ReadTime(time_msg_date);
    *data=(*(time_msg_date+3))/10;
    *(data+1)=(*(time_msg_date+3))%10;
    *(data+2)=(*(time_msg_date+1))/10;
    *(data+3)=(*(time_msg_date+1))%10;
    *(data+4)=(*(time_msg_date+2))/10;
    *(data+5)=(*(time_msg_date+2))%10;
  
}
 
