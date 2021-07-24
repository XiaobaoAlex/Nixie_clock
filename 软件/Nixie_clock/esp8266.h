#ifndef __esp8266_H
#define __esp8266_H
struct time
{
	unsigned char second[6];
	unsigned char minute[6];
	unsigned char hour[6];
	unsigned char weekday[6];
	unsigned char day[6];
	unsigned char month[6];
	unsigned char year[6];	
};
unsigned char atk_send_cmd(unsigned char *cmd,unsigned char *result);  
void esp8266_start(void);
void esp8266_get_time(struct time *time_msg);
void esp8266_get_time_array(unsigned char *time_array);
#endif