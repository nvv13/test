

#include "ff.h"

//#define SERIAL_DEBUG
//#define SERIAL_DEBUG_ALL

u8 file_buffer[512] = {0};

word UTFT_loadBitmap(int x, int y, int sx, int sy, char *filename)
{


	FIL fnew; // file object
	FRESULT res_sd;// file operation results
	UINT fnum; // The number of files successfully read and written

	int cx, cy, cp;


	//Open the file
	res_sd = f_open(&fnew, /*"0:FatFs_test_files.txt"*/ filename, FA_OPEN_EXISTING | FA_READ);
	//file opened successfully?
	if(res_sd == FR_OK)
	{
#ifdef SERIAL_DEBUG_ALL
                printf ("Open file successfully! Start reading data!\r\n");
#endif
		cbi(P_CS, B_CS);
		cx=0;
		cy=0;
		fnum=sizeof(file_buffer);
		if (orient==PORTRAIT)
		{
			UTFT_setXY(x, y, x+sx-1, y+sy-1);
		}
		while(fnum==sizeof(file_buffer))
		{
			res_sd= f_read(&fnew, file_buffer, sizeof(file_buffer), &fnum);
			if(res_sd == FR_OK)
			{
#ifdef SERIAL_DEBUG_ALL
                        printf ("Data read successfully!\r\n");
                        printf ("data: %s\r\n", file_buffer);
#endif
					if (orient==PORTRAIT)
					{
						for (int i=0; i<fnum; i+=2)
							UTFT_LCD_Write_DATA(file_buffer[i],file_buffer[i+1]);
					}
					else
					{
						cp=0;
						while (cp<fnum)
						{
							if (((fnum-cp)/2)<(sx-cx))
							{
								UTFT_setXY(x+cx, y+cy, x+cx+((fnum-cp)/2)-1, y+cy);
								for (int i=(fnum-cp)-2; i>=0; i-=2)
									UTFT_LCD_Write_DATA(file_buffer[cp+i],file_buffer[cp+i+1]);
								cx+=((fnum-cp)/2);
								cp=fnum;
							}
							else
							{
								UTFT_setXY(x+cx, y+cy, x+sx-1, y+cy);
								for (int i=sx-cx-1; i>=0; i--)
									UTFT_LCD_Write_DATA(file_buffer[cp+(i*2)],file_buffer[cp+(i*2)+1]);
								cp+=(sx-cx)*2;
								cx=0;
								cy++;
							}
						}
					}

			}
			else
			{
#ifdef SERIAL_DEBUG
                        printf ("Data reading failed!\r\n");
#endif
			break;
			}
		}
		//close file
		f_close(&fnew);
		UTFT_setXY(0,0,UTFT_getDisplayXSize()-1,UTFT_getDisplayYSize()-1);
		sbi(P_CS, B_CS);
	}
	
	return res_sd;

/*
	int res;
	int cx, cy, cp;
	word result;

	res=file_openFile(filename, FILEMODE_BINARY);
	if (res==NO_ERROR)
	{
		cbi(P_CS, B_CS);
		cx=0;
		cy=0;
		result=512;
		if (orient==PORTRAIT)
		{
			UTFT_setXY(x, y, x+sx-1, y+sy-1);
		}
		while (result==512)
		{
			result=file_readBinary();
			switch(result)
			{
				case ERROR_WRONG_FILEMODE:
					return ERROR_WRONG_FILEMODE;
					break;
				case ERROR_NO_FILE_OPEN:
					return ERROR_NO_FILE_OPEN;
					break;
				default:
					if (orient==PORTRAIT)
					{
						for (int i=0; i<result; i+=2)
							UTFT_LCD_Write_DATA(file_buffer[i],file_buffer[i+1]);
					}
					else
					{
						cp=0;
						while (cp<result)
						{
							if (((result-cp)/2)<(sx-cx))
							{
								UTFT_setXY(x+cx, y+cy, x+cx+((result-cp)/2)-1, y+cy);
								for (int i=(result-cp)-2; i>=0; i-=2)
									UTFT_LCD_Write_DATA(file_buffer[cp+i],file_buffer[cp+i+1]);
								cx+=((result-cp)/2);
								cp=result;
							}
							else
							{
								UTFT_setXY(x+cx, y+cy, x+sx-1, y+cy);
								for (int i=sx-cx-1; i>=0; i--)
									UTFT_LCD_Write_DATA(file_buffer[cp+(i*2)],file_buffer[cp+(i*2)+1]);
								cp+=(sx-cx)*2;
								cx=0;
								cy++;
							}
						}
					}
					break;
			}              
		}
		file_closeFile();
		UTFT_setXY(0,0,UTFT_getDisplayXSize()-1,UTFT_getDisplayYSize()-1);
		sbi(P_CS, B_CS);
		return 0;
	}
	else
	{
		return res;
	}
*/
}


