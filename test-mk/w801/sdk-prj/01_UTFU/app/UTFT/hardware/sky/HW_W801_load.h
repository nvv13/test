

#include "ff.h"

//#define SERIAL_DEBUG
//#define SERIAL_DEBUG_ALL

//#define BEF_LEN_3X 510 // 170*3
//#define BEF_LEN_2X 512 // 256*2
//static u8 buf_4x_line[BEF_LEN_2X] = {0};

// pack(push,1) - Byte alignment ?
#pragma pack(push, 1)
typedef struct tagBITMAPFILEHEADER
 {
   u16   bfType;
   u32   bfSize;
   u16   bfReserved1;
   u16   bfReserved2;
   u32   bfOffBits;
 } BITMAPFILEHEADER, *PBITMAPFILEHEADER;
#pragma pack(pop)

word UTFT_loadBitmap(int x, int y, int sx, int sy, char *filename)
{


	FIL fnew; // file object
	FRESULT res_sd;// file operation results
	UINT fnum; // The number of files successfully read and written

	int cx, cy, cp;


        if(buf_4x_line==NULL && sizeof(buf_4x_line)<sizeof(BITMAPFILEHEADER))
         {
#ifdef SERIAL_DEBUG
                        printf ("UTFT_loadBitmap:buf_4x_line no ENOUGH\r\n");
#endif
         return FR_NOT_ENOUGH_CORE;
         }
	//Open the file
	res_sd = f_open(&fnew, filename, FA_OPEN_EXISTING | FA_READ);
	//file opened successfully?
	if(res_sd == FR_OK)
	{
#ifdef SERIAL_DEBUG_ALL
                printf ("UTFT_loadBitmap:Open file successfully! Start reading data!\r\n");
#endif
		fnum=sizeof(buf_4x_line)/2;
                if (strstr (filename, ".bmp") != NULL || strstr (filename, ".BMP") != NULL)
                  { //надо пропустить заголовок
   		  res_sd= f_read(&fnew, buf_4x_line, sizeof(BITMAPFILEHEADER), &fnum);
		  if(res_sd == FR_OK)
			{
		        // Parse BMP header to get the information we need
		        PBITMAPFILEHEADER aHead=(PBITMAPFILEHEADER)buf_4x_line;
		        if(aHead->bfType== 0x4D42) 
				{ // BMP file start signature check
#ifdef SERIAL_DEBUG
	                        printf ("UTFT_loadBitmap:sizeof(BITMAPFILEHEADER)=%d, aHead->bfOffBits=%d\r\n",sizeof(BITMAPFILEHEADER),aHead->bfOffBits);
#endif
			        uint32_t bmpImageoffset;        // Start address of image data in file
		           	bmpImageoffset = aHead->bfOffBits - 1; // Start of image data
				res_sd = f_lseek (
					  &fnew,  /* [IN] File object */
					  bmpImageoffset  /* [IN] Offset of file read/write pointer to be set */
					);                                
#ifdef SERIAL_DEBUG
		                        printf ("UTFT_loadBitmap:f_lseek successfully! %d\r\n",bmpImageoffset);
#endif
				fnum=sizeof(buf_4x_line)/2;
				} 
		  	}
                  }
		cbi(P_CS, B_CS);
		cx=0;
		cy=0;
		if (orient==PORTRAIT)
		{
			UTFT_setXY(x, y, x+sx-1, y+sy-1);
		}
		while(fnum==(sizeof(buf_4x_line)/2))
		{
			res_sd= f_read(&fnew, buf_4x_line, sizeof(buf_4x_line)/2, &fnum);
			if(res_sd == FR_OK)
			{
#ifdef SERIAL_DEBUG_ALL
                        printf ("UTFT_loadBitmap:Data read successfully! fnum=%d\r\n",fnum);
                        //printf ("data: %s\r\n", buf_4x_line);
#endif
				if (orient==PORTRAIT) //LANDSCAPE)//
					{
                                        if (1==10 && display_serial_mode == SERIAL_5PIN && _spi_freq!=0)
                                            {
                                                sbi_RS();
                                                UTFT_LCD_Writ_Bus_SPI (buf_4x_line, sizeof(buf_4x_line)/2);
                                            }
                                            else
                                            {
						for (int i=0; i<fnum; i+=2)
							UTFT_LCD_Write_DATA(buf_4x_line[i],buf_4x_line[i+1]);
                                            }

					}
					else
					{
                                        if (1==10 && display_serial_mode == SERIAL_5PIN && _spi_freq!=0)
                                            {

						cp=0;
                                                u8 *buf=buf_4x_line+(sizeof(buf_4x_line)/2);
                                                u16 buf_ind=0;
                                                sbi_RS();
						while (cp<fnum)
						{
							if (((fnum-cp)/2)<(sx-cx))
							{
								UTFT_setXY(x+cx, y+cy, x+cx+((fnum-cp)/2)-1, y+cy);
								for (int i=(fnum-cp)-2; i>=0; i-=2)
                                                                        {
                                                                        buf[buf_ind++]=buf_4x_line[cp+i];buf[buf_ind++]=buf_4x_line[cp+i+1];
                                                                        }
								cx+=((fnum-cp)/2);
								cp=fnum;
							}
							else
							{
								UTFT_setXY(x+cx, y+cy, x+sx-1, y+cy);
								for (int i=sx-cx-1; i>=0; i--)
                                                                        {
                                                                        buf[buf_ind++]=buf_4x_line[cp+(i*2)];buf[buf_ind++]=buf_4x_line[cp+(i*2)+1];
                                                                        }
								cp+=(sx-cx)*2;
								cx=0;
								cy++;
							}
						}

                                                UTFT_LCD_Writ_Bus_SPI (buf, sizeof(buf_4x_line)/2);
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
									UTFT_LCD_Write_DATA(buf_4x_line[cp+i],buf_4x_line[cp+i+1]);
								cx+=((fnum-cp)/2);
								cp=fnum;
							}
							else
							{
								UTFT_setXY(x+cx, y+cy, x+sx-1, y+cy);
								for (int i=sx-cx-1; i>=0; i--)
									UTFT_LCD_Write_DATA(buf_4x_line[cp+(i*2)],buf_4x_line[cp+(i*2)+1]);
								cp+=(sx-cx)*2;
								cx=0;
								cy++;
							}
						}
                                             }
					}

			}
			else
			{
#ifdef SERIAL_DEBUG
                        printf ("UTFT_loadBitmap:Data reading failed!\r\n");
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
}


/*

word UTFT_loadBitmapBMP(int x, int y, int sx, int sy, char *filename)
{


	FIL fnew; // file object
	FRESULT res_sd;// file operation results
	UINT fnum; // The number of files successfully read and written

	int cx, cy, cp;

	//Open the file
	res_sd = f_open(&fnew, filename, FA_OPEN_EXISTING | FA_READ);
	//file opened successfully?
	if(res_sd == FR_OK)
	{
#ifdef SERIAL_DEBUG_ALL
                printf ("Open file successfully! Start reading data!\r\n");
#endif
   		res_sd= f_read(&fnew, buf_4x_line, sizeof(BITMAPFILEHEADER), &fnum);
		if(res_sd == FR_OK)
			{
		        // Parse BMP header to get the information we need
		        PBITMAPFILEHEADER aHead=(PBITMAPFILEHEADER)buf_4x_line;
		        if(aHead->bfType== 0x4D42) 
				{ // BMP file start signature check
			        uint32_t bmpImageoffset;        // Start address of image data in file
		           	bmpImageoffset = aHead->bfOffBits; // Start of image data
				res_sd = f_lseek (
					  &fnew,  // [IN] File object 
					  bmpImageoffset  // [IN] Offset of file read/write pointer to be set 
					);                                
#ifdef SERIAL_DEBUG_ALL
		                        printf ("f_lseek successfully! %d\r\n",bmpImageoffset);
#endif
				fnum=BEF_LEN_3X;
				} 
		  	}

		cbi(P_CS, B_CS);
		cx=0;
		cy=0;
		if (orient==PORTRAIT)
		{
			UTFT_setXY(x, y, x+sx-1, y+sy-1);
		}
		while(fnum==BEF_LEN_3X)
		{
			res_sd= f_read(&fnew, buf_4x_line, BEF_LEN_3X, &fnum);
			if(res_sd == FR_OK)
			{
#ifdef SERIAL_DEBUG_ALL
                        printf ("Data read successfully!\r\n");
                        printf ("data: %s\r\n", buf_4x_line);
#endif
   			UINT bnum=0;
                        u16 u16_word=0;
			for (int i=0; i<fnum; i+=3)
                         {
                         u16_word  =  (buf_4x_line[i] >> 3);
                         u16_word |= ((buf_4x_line[i+1] & 0xFC) << 3);
                         u16_word |= ((buf_4x_line[i+2] & 0xF8) << 8);
                         buf_4x_line[bnum]=(u16_word & 0x00FF);
			 bnum++;
                         buf_4x_line[bnum]=(u16_word >> 8);
			 bnum++;
                         }


					if (orient==PORTRAIT)
					{
						for (int i=0; i<bnum; i+=2)
							UTFT_LCD_Write_DATA(buf_4x_line[i],buf_4x_line[i+1]);
					}
					else
					{
						cp=0;
						while (cp<bnum)
						{
							if (((bnum-cp)/2)<(sx-cx))
							{
								UTFT_setXY(x+cx, y+cy, x+cx+((bnum-cp)/2)-1, y+cy);
								for (int i=(bnum-cp)-2; i>=0; i-=2)
									UTFT_LCD_Write_DATA(buf_4x_line[cp+i],buf_4x_line[cp+i+1]);
								cx+=((bnum-cp)/2);
								cp=bnum;
							}
							else
							{
								UTFT_setXY(x+cx, y+cy, x+sx-1, y+cy);
								for (int i=sx-cx-1; i>=0; i--)
									UTFT_LCD_Write_DATA(buf_4x_line[cp+(i*2)],buf_4x_line[cp+(i*2)+1]);
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
}

*/