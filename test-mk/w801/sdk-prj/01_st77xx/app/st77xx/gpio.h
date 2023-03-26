
typedef enum
{ Bit_RESET = 0,
  Bit_SET
}BitAction;

#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))



//#define GPIOA      0
//#define GPIOB      1

//#define GPIO_Pin_2 2
//#define GPIO_Pin_3 3


typedef struct
{
/*  __IO uint32_t CRL;
  __IO uint32_t CRH;
  __IO uint32_t IDR;

  __IO uint32_t ODR;
  __IO uint32_t BSRR;
  __IO uint32_t BRR;
  __IO uint32_t LCKR;*/
 int a;
} GPIO_TypeDef;


GPIO_TypeDef* GPIOA;
GPIO_TypeDef* GPIOB;



void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);



typedef enum
{ 
  GPIO_Speed_10MHz = 1,
  GPIO_Speed_2MHz, 
  GPIO_Speed_50MHz
}GPIOSpeed_TypeDef;
#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_Speed_10MHz) || ((SPEED) == GPIO_Speed_2MHz) || \
                              ((SPEED) == GPIO_Speed_50MHz))

/** 
  * @brief  Configuration Mode enumeration  
  */

typedef enum
{ GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,
  GPIO_Mode_IPU = 0x48,
  GPIO_Mode_Out_OD = 0x14,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_AIN) || ((MODE) == GPIO_Mode_IN_FLOATING) || \
                            ((MODE) == GPIO_Mode_IPD) || ((MODE) == GPIO_Mode_IPU) || \
                            ((MODE) == GPIO_Mode_Out_OD) || ((MODE) == GPIO_Mode_Out_PP) || \
                            ((MODE) == GPIO_Mode_AF_OD) || ((MODE) == GPIO_Mode_AF_PP))



typedef struct
{
  uint16_t GPIO_Pin;
  GPIOSpeed_TypeDef GPIO_Speed;
  GPIOMode_TypeDef GPIO_Mode;
}GPIO_InitTypeDef;




//#define gpio_NoPull     0
//#define gpio_PullUp     1
//#define gpio_PullDown   2


//void gpio_SetGPIOmode_In(GPIO_TypeDef* GPIOx, uint16_t Mask, uint8_t PullMode);
//void gpio_SetGPIOmode_Out(GPIO_TypeDef* GPIOx, uint16_t Mask);
void gpio_PortClockStart(GPIO_TypeDef *GPIOx);

void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);


// Процедура настройки канала 1-4 таймера 2 в режиме ШИМ
void tmr2_PWM_init(uint8_t Channel, uint16_t MaxValue, uint16_t Value);

// Процедура установки значения ШИМ для канала 1-4 таймера 2
void tmr2_PWM_set(uint8_t Channel, uint16_t Value);



