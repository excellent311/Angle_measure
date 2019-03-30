/*
 * yeelink.c
 *
 *  Created on: 2015-11-5
 *      Author: Administrator
 */

#include "yeelink.h"



#define APIKEY   "b0a3a040522ef188eda51ccb30084434"    //请注意将这里换成自己的APIKEY
//#define DATA_BUF_SIZE  200


void yeelink_send(const char *device_id,const char *sensors_id,float value)
{
		char http_content[48] = {0};
		char str_temp[48]={0};
		put("AT+CIPSTART=\"TCP\",\"api.yeelink.net\",80\r\n");
		delay_ms(1000);
		put("AT+CIPMODE=1\r\n");
		delay_ms(5);
		put("AT+CIPSEND\r\n");
		delay_ms(5);
		sprintf( http_content , "{\"value\":%d.%d}" , (int)(value),(int)(value*1000)%1000);
		sprintf(str_temp,"POST /v1.0/device/%s/sensor/%s/datapoints HTTP/1.1\r\n",device_id,sensors_id);
		put(str_temp);
		put("Host:api.yeelink.net\r\n");
		put("U-ApiKey:b0a3a040522ef188eda51ccb30084434\r\n");
		put("Accept: */*\r\n");
		sprintf(str_temp,"Content-Length:%d\r\n" ,strlen(http_content) );
		put(str_temp);
		put("Content-Type: application/x-www-form-urlencoded\r\n");
		put("Connection: close\r\n");
		put("\r\n");
		delay_ms(5);
		put(http_content);

}


/*
void yeelink_send(const char *device_id,const char *sensors_id,float value)
{

	char remote_server[] = "api.yeelink.net";
	char str_tmp[48] = {0};
    static char http_request[DATA_BUF_SIZE]="0";

	char http_content[32] = {0};
	sprintf(str_tmp,"/v1.0/device/%s/sensor/%s/datapoints",device_id,sensors_id);
	sprintf( http_content , "{\"value\":%d.%d}" , (int)(value),(int)(value*1000)%1000);

	sprintf( http_request , "POST %s HTTP/1.1\r\n",str_tmp);
	sprintf( str_tmp , "U-ApiKey:%s\r\n" , APIKEY);
	strcat( http_request , str_tmp);

	sprintf( str_tmp , "Host:%s\r\n" , remote_server);
	strcat( http_request , str_tmp);*/
//strcat( http_request , "Accept: */*\r\n");
/*	sprintf( str_tmp , "Content-Length:%d\r\n" ,strlen(http_content) );
	strcat( http_request , str_tmp);
	strcat( http_request , "Content-Type: application/x-www-form-urlencoded\r\n");
	strcat( http_request , "Connection: close\r\n");

	strcat( http_request , "\r\n");

	strcat( http_request , http_content);
	delay_ms(10);
	put("AT+CIPSTART=\"TCP\",\"api.yeelink.net\",80\r\n");

	delay_ms(2000);
	put("AT+CIPMODE=1\r\n");
	delay_ms(100);
	put("AT+CIPSEND\r\n");
	delay_ms(5);
	put(http_request);

}*/



