/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: 
 *
 * Date : 2022-05-20
 *****************************************************************************/

#include <stdio.h>
#include <string.h>

#include "wm_type_def.h"
#include "wm_gpio_afsel.h"


#include "../src/app/bleapp/wm_bt_app.h"
#include "../src/app/bleapp/wm_ble_server_api_demo.h"

extern int demo_bt_enable (void);
extern int demo_ble_server_on(void);
extern int tls_ble_server_demo_api_send_notify_msg(uint8_t *data, int data_len);


//BT
#define    MYBLE_TASK_SIZE      2048
static OS_STK 			MyBLETaskStk[MYBLE_TASK_SIZE];
#define  MYBLE_TASK_PRIO               32


void my_ble_msg_task(void *sdata)
{
	u8 msg_state[4];
	u8 *msg;
	
	demo_bt_enable();
	while(bt_adapter_state == WM_BT_STATE_OFF)
	{
		tls_os_time_delay (HZ * 5);
	}
	tls_os_time_delay (HZ * 5);
	demo_ble_server_on();
	printf("ble_server_on ok \r\n");
	while(1)
	{
		//Receive the data sent by the mobile phone, note that the data is received in bytes
		//msg[0] indicates that the data length already contains '\0'
		tls_os_queue_receive(ble_q,((void **)&msg), 0, 0);
		printf("rev main:%s len:%d\n",&msg[1] ,msg[0]);
		//Extract the first 4 bits, the first 4 bits are status flags
		strncpy((char*)msg_state,(char *)&msg[1],4);
		//Judge the state of the first four digits

                //отправим обратно 
		tls_ble_server_demo_api_send_notify_msg(msg_state,sizeof(msg_state));
		
	}
	
}



void
UserMain (void)
{
	//Bluetooth receive message queue
	if(tls_os_queue_create(&ble_q, 32)!=TLS_OS_SUCCESS)
	{
		printf("create queue fail\n");
		return;
	}

	tls_os_task_create(NULL, NULL,
                       my_ble_msg_task,		//Bluetooth receiving task
                       NULL,
                       (void *)MyBLETaskStk,          /* task's stack start address */
                       MYBLE_TASK_SIZE * sizeof(u32), /* task's stack size, unit:byte */
                       MYBLE_TASK_PRIO,
                       0);


  return;
}
