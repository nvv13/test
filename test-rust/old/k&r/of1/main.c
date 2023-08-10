#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned char u8;
typedef unsigned int u32;
void taskENTER_CRITICAL (void){};
void taskEXIT_CRITICAL (void){};
void *
tls_mem_alloc (size_t size)
{
  return malloc (size);
};


//**********************************************************************************************************************************//

#define BUF_SIZE                                                              \
  8192 // 8,16,32,64,128,256,512,1024 ... размер буфера обязательно должен быть
  // равен степени двойки! 4096 = 262144 byte
#define BUF_MASK (BUF_SIZE - 1)
#define BUF_BYTE_CHUNK_SIZE 64 //

// static bool b_psram = true;
static volatile int idxIN = 0;
static volatile int idxOUT = 0;
static volatile int idxUSE = 0;
static u8 *ring_buf = NULL; // [BUF_SIZE*BUF_BYTE_CHUNK_SIZE]; //  = NULL; //

static u8 *
CreateRingBuf (void)
{
  taskENTER_CRITICAL ();
  //  if (b_psram)
  //    {
  //      wm_psram_config (0);
  //      psram_init (PSRAM_SPI); // PSRAM_QPI - почему то не заработал, что то
  //      не
  //                              // так сделал
  //      ring_buf = (u8 *)PSRAM_ADDR_START;
  //    }
  //  else
  //    {
  ring_buf = tls_mem_alloc (BUF_SIZE * BUF_BYTE_CHUNK_SIZE);
  //    }
  printf (" CreateRingBuf address= %d, size %d \n\n", (unsigned long)ring_buf,
          BUF_SIZE * BUF_BYTE_CHUNK_SIZE);
  idxIN = 0;
  idxOUT = 0;
  idxUSE = 0;
  taskEXIT_CRITICAL ();
  return ring_buf;
}

static u8 *
PushRingBuf (void)
{
  u8 *bb_put = NULL;
  taskENTER_CRITICAL ();
  if (ring_buf != NULL && idxUSE < BUF_SIZE)
    {
      //При помещении значения value в буфер используется индекс idxIN. Это
      //делается так:
      //  if (b_psram)
      //    memcpy_dma (ring_buf + (idxIN * BUF_BYTE_CHUNK_SIZE), Buffer,
      //                BUF_BYTE_CHUNK_SIZE);
      //  else
      //    memcpy (ring_buf + (idxIN * BUF_BYTE_CHUNK_SIZE), Buffer,
      //            BUF_BYTE_CHUNK_SIZE);
      bb_put = ring_buf + (idxIN * BUF_BYTE_CHUNK_SIZE);
      // printf (" PushRingBuf address= %d, idxIN = %d,
      // idxUSE=%d\n\n",(int)bb_put,idxIN,idxUSE);
      idxIN++;
      idxIN &= BUF_MASK;
      idxUSE++;
    }
  taskEXIT_CRITICAL ();
  return bb_put;
}

static u8 *
PopRingBuf (void)
{
  u8 *bb_get = NULL;
  taskENTER_CRITICAL ();
  if (ring_buf != NULL && idxUSE > 1)
    {
      //Операция выборки из буфера происходит похожим образом, только
      //используется индекс idxOUT:
      //  if (b_psram)
      //    memcpy_dma (Buffer, ring_buf + (idxOUT * BUF_BYTE_CHUNK_SIZE),
      //                BUF_BYTE_CHUNK_SIZE);
      //  else
      //    memcpy (Buffer, ring_buf + (idxOUT * BUF_BYTE_CHUNK_SIZE),
      //            BUF_BYTE_CHUNK_SIZE);
      bb_get = ring_buf + (idxOUT * BUF_BYTE_CHUNK_SIZE);
      // printf (" PopRingBuf address= %d, idxOUT = %d, idxUSE=%d
      // \n\n",(int)bb_get,idxOUT,idxUSE);
      idxOUT++;
      idxOUT &= BUF_MASK;
      idxUSE--;
    }
  taskEXIT_CRITICAL ();
  return bb_get;
}

static void
ResetRingBuf (void)
{
  taskENTER_CRITICAL ();
  //Сбросить данные буфера (т. е. удалить их оттуда) тоже очень просто - для
  //этого в idxOUT записывают значение idxIN: idxOUT = idxIN;
  idxIN = 0;
  idxOUT = 0;
  idxUSE = 0;
  printf (" ResetRingBuf address= %d, size %d \n\n", (unsigned long)ring_buf,
          BUF_SIZE * BUF_BYTE_CHUNK_SIZE);
  taskEXIT_CRITICAL ();
  // printf (" ResetRingBuf \n\n");
}

//сколько данных в буфере.
// static u8
// idxDiff ()
//{
//  return (idxIN - idxOUT) & BUF_MASK;
//
//    if (idxIN >= idxOUT)
//        return (idxIN - idxOUT);
//    else
//        return ((BUF_SIZE - idxOUT) + idxIN);
//
//}

static int
RingBufSpacesAvailable (void)
{
  int i_size = 0;
  // printf (" RingBufSpacesAvailable idxIN = %d idxOUT = %d = %d\n\n",idxIN ,
  // idxOUT, ((idxIN - idxOUT) & BUF_MASK));
  //  return BUF_SIZE - ((idxIN - idxOUT) & BUF_MASK) - 1;
  // if((BUF_SIZE - idxUSE - 4)<=0) return 0;
  taskENTER_CRITICAL ();
  i_size = (BUF_SIZE - idxUSE); // * BUF_BYTE_CHUNK_SIZE;
  taskEXIT_CRITICAL ();
  return i_size;
}

//**********************************************************************************************************************************//

static u8 u8_fill = 0;
static u8 u8_check = 0;
static int i_err_cnt = 0;

void
fill_buf (void)
{
  int i_rand = rand () % BUF_SIZE;
  printf ("%d ", i_rand);
  while (1)
    {
      u32 nSize = BUF_BYTE_CHUNK_SIZE;
      size_t freeSize = RingBufSpacesAvailable ();
      // printf ("free=%d", freeSize);
      if (freeSize == 0 || i_rand-- < 0)
        {
          // printf ("\r\n");
          break;
        }
      u8_fill++;
      u8 *buf_fill = PushRingBuf ();
      // printf ("adr=%d ", (unsigned long)buf_fill);
      for (int iPos = 0; iPos < nSize; iPos++)
        {
          *(buf_fill + iPos) = u8_fill;
        }
    }
}

void
print_buf (void)
{
  int i_rand = rand () % BUF_SIZE;
  printf ("%d ", i_rand);
  int i_cnt = 0;
  while (1)
    {
      if (i_rand-- < 0)
        break;
      u8 *buffer = PopRingBuf ();
      if (buffer != NULL)
        {
          u8_check++;
          i_cnt++;
          for (int iPos = 0; iPos < BUF_BYTE_CHUNK_SIZE; iPos++)
            {
              u8 u8_out = *(buffer + iPos);
              // printf ("%0.2x", u8_out);
              if (u8_check == u8_out)
                ; // printf ("%x", u8_out);
              else
                {
                  i_err_cnt++;
                  // printf (" err%x<>%x", u8_out, u8_check);
                }
            }
          // printf (" cnt_buf=%d address= %d\r\n", i_cnt, (unsigned
          // long)buffer);
        }
      else
        break;
    }
}

void
main ()
{
  printf ("test ring buf\r\n");

  if (ring_buf == NULL)
    {
      CreateRingBuf ();
    }

  srand (time (NULL)); // Initialization, should only be called once.
  u8_fill = 0;
  u8_check = 0;
  i_err_cnt = 0;
  for (int iPos = 0; iPos < 10000; iPos++)
    {
      fill_buf ();
      print_buf ();
    }

  if (i_err_cnt > 0)
    printf ("\r\n\r\ntest Error\r\n");
  else
    printf ("\r\n\r\ntest Ok\r\n");
}
