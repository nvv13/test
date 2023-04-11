/*

исходники для jpeg библиотеки, взяты отсюда:
https://gitee.com/openLuat/LuatOS.git

*/


#define LUAT_LOG_TAG "ujpg"
#include "luat_log.h"

#include "tjpgd.h"
#include "tjpgdcnf.h"

#define N_BPP (3 - JD_FORMAT)

/* Session identifier for input/output functions (name, members and usage are as user defined) */
typedef struct {
    FILE *fp;               /* Input stream */
    int x;
    int y;
    // int width;
    // int height;
    uint16_t buff[16*16];
} IODEV;

static unsigned int file_in_func (JDEC* jd, uint8_t* buff, unsigned int nbyte){
    IODEV *dev = (IODEV*)jd->device;   /* Device identifier for the session (5th argument of jd_prepare function) */
    if (buff) {
        /* Read bytes from input stream */
        return luat_fs_fread(buff, 1, nbyte, dev->fp);
    } else {
        /* Remove bytes from input stream */
        return luat_fs_fseek(dev->fp, nbyte, SEEK_CUR) ? 0 : nbyte;
    }
}

//int (*outfunc)(JDEC*,void*,JRECT*)
static int lcd_out_func (JDEC* jd, void* bitmap, JRECT* rect){
    IODEV *dev = (IODEV*)jd->device;
    uint16_t* tmp = (uint16_t*)bitmap;

    // rgb高低位swap
    uint16_t count = (rect->right - rect->left + 1) * (rect->bottom - rect->top + 1);
    for (size_t i = 0; i < count; i++)
    {
      dev->buff[i] = ((tmp[i] >> 8) & 0xFF)+ ((tmp[i] << 8) & 0xFF00);
    }

    // LLOGD("jpeg seg %dx%d %dx%d", rect->left, rect->top, rect->right, rect->bottom);
    // LLOGD("jpeg seg size %d %d %d", rect->right - rect->left + 1, rect->bottom - rect->top + 1, (rect->right - rect->left + 1) * (rect->bottom - rect->top + 1));
    luat_lcd_draw(default_conf, dev->x + rect->left, dev->y + rect->top,
                                dev->x + rect->right, dev->y + rect->bottom,
                                dev->buff);
    return 1;    /* Continue to decompress */
}


static int lcd_draw_jpeg(const char* path, int xpos, int ypos) {
  JRESULT res;      /* Result code of TJpgDec API */
  JDEC jdec;        /* Decompression object */
  void *work;       /* Pointer to the decompressor work area */
#if JD_FASTDECODE == 2
  size_t sz_work = 3500 * 3; /* Size of work area */
#else
  size_t sz_work = 3500; /* Size of work area */
#endif
  IODEV devid;      /* User defined device identifier */

  FILE* fd = luat_fs_fopen(path, "r");
  if (fd == NULL) {
    LLOGW("no such file %s", path);
    return -1;
  }

  devid.fp = fd;
  work = luat_heap_malloc(sz_work);
  if (work == NULL) {
    LLOGE("out of memory when malloc jpeg decode workbuff");
    return -3;
  }
  res = jd_prepare(&jdec, file_in_func, work, sz_work, &devid);
  if (res != JDR_OK) {
    luat_heap_free(work);
    luat_fs_fclose(fd);
    LLOGW("jd_prepare file %s error %d", path, res);
    return -2;
  }
  devid.x = xpos;
  devid.y = ypos;
  // devid.width = jdec.width;
  // devid.height = jdec.height;
  res = jd_decomp(&jdec, lcd_out_func, 0);
  luat_heap_free(work);
  luat_fs_fclose(fd);
  if (res != JDR_OK) {
    LLOGW("jd_decomp file %s error %d", path, res);
    return -2;
  }
  else {
    //lcd_auto_flush(default_conf);
    return 0;
  }
}


int UTFT_ADD_lcd_draw_jpeg (const char* path, int xpos, int ypos)
{
return lcd_draw_jpeg(path,  xpos,  ypos);
}


