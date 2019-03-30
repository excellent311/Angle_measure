/*
 * lewei.c
 *
 *  Created on: 2015-11-7
 *      Author: Administrator
 */

#include"lewei.h"

#define UserKey 4878ed4970b446baa1f8638a718695c9
#define DATA_BUF_SIZE  160

void lewei_send(float value)
{

		char remote_server[] = "open.lewei50.com";
		char str_tmp[48] = {0};
	    static char http_request[DATA_BUF_SIZE]="0";

		char http_content[48] = {0};
		sprintf( http_content , "[{\"Name\":\"T1\",\"Value\":\"%d.%d\"}]" , (int)(value),(int)(value*1000)%1000);
		//sprintf( http_content , "[{\"Name\":\"T1\",\"Value\":\"%d\"}]" , (int)value);
		sprintf( http_request , "POST /api/V1/gateway/UpdateSensors/01 HTTP/1.1\r\n",str_tmp);
		sprintf( str_tmp , "userkey:4878ed4970b446baa1f8638a718695c9\r\n");
		strcat( http_request , str_tmp);

		sprintf( str_tmp , "Host:%s\r\n" , remote_server);
		strcat( http_request , str_tmp);
		strcat( http_request , "User-Agent:RT-Thread ART\r\n");
		//strcat( http_request , "Accept: */*\r\n");
		sprintf( str_tmp , "Content-Length:%d\r\n" ,strlen(http_content) );
		strcat( http_request , str_tmp);

		strcat( http_request , "Connection: close\r\n");

		strcat( http_request , "\r\n");

		strcat( http_request , http_content);
		delay_ms(10);
		put("AT+CIPSTART=\"TCP\",\"open.lewei50.com\",80\r\n");

		delay_ms(2000);
		put("AT+CIPMODE=1\r\n");
		delay_ms(100);
		put("AT+CIPSEND\r\n");
		delay_ms(5);
		put(http_request);

}


