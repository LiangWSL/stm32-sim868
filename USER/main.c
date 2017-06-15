#include "sim900a.h"
#include "usart.h"
#include "usart2.h"
#include "delay.h"
#include "sys.h"
#include "bmp.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_dma.h"
#include "string.h"
#include "gpio.h"

//GPS_EN��Ҫ�ӵ�PB.13���߽ӵ�EXT��
char ip[]="182.92.123.13";		//������IP������
char port[]="6666";						//�˿�
char id[]="15291453406��";		//�豸ID�����ֻ���,����12


int main(void)
 {
	u8 res=1;
	char message[100];
	char zigbee[100];
	delay_init();	    	 			//��ʱ������ʼ��	  
	NVIC_Configuration(); 	 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 
  GPIO_Config();          	//�������IO
  uart_init(115200);      	//��ʼ������1
	USART2_Init(115200);			//��ʼ������2 
  USART3_Init(115200);    	//��ʼ������3 
	USART4_Init(115200);
	TIM2_NVIC_Configuration(); /* TIM2 ��ʱ���� */
	TIM2_Configuration(); 		
	//EXTIX_Init();//�ⲿ�ж�
  delay_ms(10);							//�ȴ�ϵͳ�ȶ�
  SIM868_EN(ON);						//��SIM868��Դ
  GPS_EN(ON);								//ʹ��SIM868 GPS 
  delay_ms(100);						//�ȴ�ϵͳ�ȶ�
  printf("\r\n****����汾V1.0*****\r\n");
  printf("\r\n******ϵͳ����*******\r\n");
	while(res)
	{
		res=sim900a_word_test();	
		switch(res)
		{
			case 1:	//��ͨ��ʧ��
								 
				break;
			case 2:	//��SIM��

				break;
			case 3:	//�ȴ�ע�ᵽ����

				break;
				
			default:
				break;				
		}	
    
	} //ϵͳ������	
    //����GPRS������
  printf("\r\n******SIM868�豸����*******\r\n");
	res=1;
	while(res)
	{		
		res=SIM900A_CONNECT_SERVER((u8*)ip,(u8*)port);
	}
	delay_ms(1000);
  printf("\r\n******GPRS���ӳɹ�*******\r\n");
	while(1)
	{

        if(USART3_RX_STA&0x8000)
        {
            strcpy(&message[0],(const char*)id);
            printf("\r\n���Ľڵ�GPS����:%s\r\n",USART3_RX_BUF);
            strcat(&message[0],(const char*)USART3_RX_BUF);
            USART3_RX_STA=0;
        }
				if(USART4_RX_STA&0x8000)
        {
            strcpy(&zigbee[0],(const char*)id);
            printf("\r\nZigbeeģ������:%s\r\n",USART4_RX_BUF);
            strcat(&zigbee[0],(const char*)USART4_RX_BUF);
            USART4_RX_STA=0;
        }
				if((Flag_Send_data&0x8080))
				{
					Flag_Send_data=0;
					START_TIME;
					res=SIM900A_GPRS_SEND_DATA((u8 *)message);
					//SIM900A_GPRS_SEND_DATA("\r\n");
					SIM900A_GPRS_SEND_DATA((u8 *)zigbee);
					USART4_RX_STA=0;
					switch(res)
					{
						case 0:
							//START_TIME;
						default:
							while(res)
							{		
								res=SIM900A_CONNECT_SERVER((u8*)ip,(u8*)port);
							}					
					}
				}
		}	
}

