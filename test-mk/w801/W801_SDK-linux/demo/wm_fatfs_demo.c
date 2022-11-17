
#include "wm_include.h"
#include "wm_gpio_afsel.h"
#include "ff.h"
#include "wm_demo.h"

#if DEMO_FATFS

#define TEST_DEBUG_EN           1
#if TEST_DEBUG_EN
#define TEST_DEBUG(fmt, ...)    printf("%s: "fmt, __func__, ##__VA_ARGS__)
#else
#define TEST_DEBUG(fmt, ...)
#endif

#define FATFS_TASK_SIZE        1024
#define FATFS_TASK_PRIO	       1

static OS_STK FatfsTaskStk[FATFS_TASK_SIZE];
static int console_cmd = 0;


//FAT Functional test: formatting test, file writing test, file reading test (basic function)
int fatfs_func(void)
{
	FATFS fs; //FatFs file system object
	FIL fnew; // file object
	FRESULT res_sd;// file operation results
	UINT fnum; // The number of files successfully read and written
	BYTE ReadBuffer[256] = {0};
	BYTE work[FF_MAX_SS];
	BYTE WriteBuffer[] = "Successfully ported the FatFs file system!\r\n"; //write buffer
	

 	wm_sdio_host_config(0);

	//mount SD card
	res_sd = f_mount(&fs, "0:", 1);
	
	//***********************formatting test****************************
	if(res_sd == FR_NO_FILESYSTEM)
	{
		while(1)
		{
			TEST_DEBUG("SD The card has no file system and will be formatted soon...\r\n");
			//format
			
			res_sd = f_mkfs("0:", 0, work, sizeof(work));
			
			if(res_sd == FR_OK)
			{
				TEST_DEBUG("SD Card successfully formatted!\r\n");
				// Unmount first after formatting
				res_sd = f_mount(NULL, "0:", 1);
				// remount
				res_sd = f_mount(&fs, "0:", 1);
				break;
			}
			else
			{
				TEST_DEBUG("File formatting failed! error code: %d; will try again...\r\n",res_sd);
			}
		}
	}
	else if(res_sd != FR_OK)
	{
		TEST_DEBUG("Failed to mount file system! Probably because the file initialization failed! error code:%d\r\n", res_sd);
	}
	else
	{
		TEST_DEBUG("The file system is successfully mounted, and the read and write test can be performed!\r\n");
	}
	
	//***********************write tests****************************
	//Open the file, creating it if it doesn't exist
	TEST_DEBUG("A file write test is about to take place.....\r\n");
	//Open the file, create it if it does not exist
	res_sd = f_open(&fnew, "0:FatFs_test_files.txt", FA_CREATE_ALWAYS | FA_WRITE);
	//file opened successfully?
	if(res_sd == FR_OK)
	{
		TEST_DEBUG("Open file successfully! Start writing data!\r\n");
		res_sd= f_write(&fnew, WriteBuffer, sizeof(WriteBuffer), &fnum);
		
		if(res_sd == FR_OK)
		{
			TEST_DEBUG("The data was written successfully, a total of %d characters were written\r\n", fnum);
			TEST_DEBUG("data: %s", WriteBuffer);
		}
		else
		{
			TEST_DEBUG("Data writing failed!\r\n");
		}
		
		//close file
		f_close(&fnew);
	}
	
	//***********************read test****************************
	//Open the file
	TEST_DEBUG("File reading test coming soon....\r\n");
	//Open the file
	res_sd = f_open(&fnew, "0:FatFs_test_files.txt", FA_OPEN_EXISTING | FA_READ);
	//file opened successfully?
	if(res_sd == FR_OK)
	{
		TEST_DEBUG("Open file successfully! Start reading data!\r\n");
		res_sd= f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
		
		if(res_sd == FR_OK)
		{
			TEST_DEBUG("Data read successfully!\r\n");
			TEST_DEBUG("data: %s\r\n", ReadBuffer);
		}
		else
		{
			TEST_DEBUG("Data reading failed!\r\n");
		}
		
		//close file
		f_close(&fnew);
	}
	
	//unmount file system
	f_mount(NULL, "0:", 1);

	return 0;
}


static void fatfs_task(void)
{

	while(1)
	{
		if( console_cmd==1 )
		{
			fatfs_func();
			console_cmd = 0;
		}
		tls_os_time_delay(HZ);
	}
}

int fatfs_test(void)
{
	static int flag = 0;

	console_cmd = 1;
	if( flag == 0 )
	{
		flag = 1;
	    tls_os_task_create(NULL, NULL,
	                       ( void (*))fatfs_task,
	                       NULL,
	                       (void *)FatfsTaskStk,          /* task's stack start address */
	                       FATFS_TASK_SIZE * sizeof(u32), /* task's stack size, unit:byte */
	                       FATFS_TASK_PRIO,
	                       0);
	}
	return 0;
}

#endif

