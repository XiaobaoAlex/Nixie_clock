//ģ��iic
//ds3231����
#include "ds3231.h"
 

 
 
 
void DS3231_Delay(void) {    //DS3231ͨ��������ʱ,��ʱ5΢��  12T��Ƭ��@12M

  unsigned int i;
  unsigned int j;
  
  for(i = 0; i < 100; i++)
     for(j = 0; j < 400; j++);
  
}
 
void DS3231_DelayForWrite(void){  //DS3231д�ֽ���ʱ,��ʱ5���� 12T��Ƭ��@12M
  unsigned int i;
  unsigned int j;
  
  for(i = 0; i < 100; i++)
     for(j = 0; j < 4000; j++);
}
 
 
//������ʼ�ź�
void DS3231_Start(void) { //ģ��DS3231ͨ�ſ�ʼ�źţ�SCL��1�ڼ䣬��SDA�ϲ���һ���½���
    DS3231_sda = 1;
    DS3231_scl = 1;DS3231_Delay(); 
    DS3231_sda = 0;DS3231_Delay(); 
} 
//���������ź�
void DS3231_Stop(void) {//ģ��DS3231ͨ�Ž����źţ�SCL��1�ڼ䣬��SDA�ϲ���һ��������
    DS3231_sda = 0;DS3231_Delay();
    DS3231_scl = 1;DS3231_Delay();
    DS3231_sda = 1;DS3231_Delay();
} 
 
unsigned char DS3231_WriteByte(unsigned char SendByte){ //��DS3231�豸дһ�ֽ����ݼ�8Ϊ���������ݣ���λ��ǰ 
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
    DS3231_scl = 0;DS3231_Delay();//ackλʱ��sclΪ1ʱsdaΪ1����ʾnack,����
    return i;       
} 
 
unsigned char DS3231_ReceiveByte(unsigned char Response) { //����DS3231���͵�����,Response��������Ӧ�𣬵�responseΪ1ʱ��������Ӧ�𣬴������
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
 
unsigned char DS3231_ReadOneByteFromE2PROM(unsigned char DataAddress) {//��ָ����ַE2PROM�е����ݣ�ֻ������Ĵ�����ַ�����ݣ���������ȡ
    unsigned char ReadData;
    DS3231_Start();                       //DS3231оƬIICͨ�ſ�ʼ�ź�
    DS3231_WriteByte(DS3231WriteAddress); //д��оƬIICд��ַ
    DS3231_WriteByte(DataAddress);        //д��״̬�Ĵ�����ַ?
    DS3231_Start();                       //DS3231оƬIICͨ�ſ�ʼ�ź�
    DS3231_WriteByte(DS3231ReadAddress);  //д��оƬIIC����ַ
    ReadData = DS3231_ReceiveByte(0x01);//ֻ���ܸ���״̬�Ĵ����ڵ�ֵ��Ȼ��ͽ��������ٶ�֮��ļĴ����ڵ�ֵ���н���
    DS3231_Stop();                         //DS3231оƬIICͨ��ֹͣ�ź�
    return ReadData;   
}
 
void DS3231_Initialization(void){            //��ʼ��ʱ��оƬDS3231,��ѡ��Ҫд��ļĴ�������д����Ҫ���õ�ָ��
    DS3231_Start();                                             //IICͨ����ʼ�ź�
    DS3231_WriteByte(DS3231WriteAddress);  //д��оƬIICд��ַ
    DS3231_WriteByte(DS3231_Hour);         //ѡ��ʱ�Ĵ���Ϊд���ַ
    DS3231_WriteByte(0x00);                //д��ָ�ʱ�ӷ�ΧΪ0-23����24Сʱ��ʽ
    DS3231_Stop(); 
     
    DS3231_Start();                                                      //IICͨ����ʼ�ź�
    DS3231_WriteByte(DS3231WriteAddress);      //д��оƬIICд��ַ
    DS3231_WriteByte(DS3231_Interrupt);        //ѡ���жϼĴ���Ϊд���ַ     
    DS3231_WriteByte(0x04);                    //�жϼĴ�����ʼ��,�رշ����źţ��ر������ж�
    DS3231_WriteByte(0x00);                    //״̬�Ĵ�����ʼ��,ʧЧ32KHz�ź��������ƥ������
    DS3231_Stop(); 
}
 
void DS3231_SetTime(unsigned char *Pointer){    //��DS3231д������ʱ����Ϣ,�������롢���ӡ�Сʱ�����ڡ��ա��¡���
    unsigned char Number = 0x00;
  unsigned char TransitionData = 0x00;
    DS3231_Start();                             //DS3231оƬIICͨ�ſ�ʼ�ź�
    DS3231_WriteByte(DS3231WriteAddress);       //д��оƬIICд��ַ
    DS3231_WriteByte(DS3231_TimeFirst);         //д��ʱ��Ĵ����׵�ַ
  for(Number=0; Number<7; Number++)  {
    TransitionData = *Pointer++;
    DS3231_WriteByte((TransitionData/10)*16+TransitionData%10);   //��DS3231д������ʱ����Ϣ��ÿ��д���ַ�����
  }
  DS3231_Stop();                                   //DS3231оƬIICͨ��ֹͣ�ź�
    DS3231_DelayForWrite();
}
 
void DS3231_ReadTime(unsigned char *Pointer){       //��DS3231�ж�����ǰʱ��
    unsigned char Number  = 7;
  unsigned char Time    = 0x00;
    DS3231_Start();                                  //DS3231оƬIICͨ�ſ�ʼ�ź�
    DS3231_WriteByte(DS3231WriteAddress);            //д��оƬIICд��ַ
    DS3231_WriteByte(DS3231_Status);                 //д��״̬�Ĵ����׵�ַ
    DS3231_WriteByte(0x00);                          //�ر������жϱ�־λ
    DS3231_Stop();                                   //DS3231оƬIICͨ��ֹͣ�ź�
 
    DS3231_Start();                                  //DS3231оƬIICͨ�ſ�ʼ�ź�
    DS3231_WriteByte(DS3231WriteAddress);            //д��оƬIICд��ַ
    DS3231_WriteByte(DS3231_TimeFirst);              //д��ʱ��Ĵ����׵�ַ
    DS3231_Start();                                  //DS3231оƬIICͨ�ſ�ʼ�ź�
    DS3231_WriteByte(DS3231ReadAddress);             //д��оƬIIC����ַ
  for(Number=0; Number<6; Number++) {
        Time = DS3231_ReceiveByte(0x00);			//����ack��ȡ��ÿ�ζ���ַ�����
    *Pointer++ = (Time/16*10+Time%16);    
  }
    Time = DS3231_ReceiveByte(0x01);
    *Pointer++ = (Time/16*10+Time%16); 
  DS3231_Stop();                                 //DS3231оƬIICͨ��ֹͣ�ź�
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
 
