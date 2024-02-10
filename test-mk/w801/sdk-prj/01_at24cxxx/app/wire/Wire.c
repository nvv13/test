
#include "Wire.h"

//************************************************************************8
#include "HardWire.h"
#include "SoftWire.h"


void i2c_init(i2c_t dev)
{
  /* scl WM_IO_PA_01 or WM_IO_PB_20 */
  /* sda WM_IO_PA_04 or WM_IO_PB_19 */
  dev->hw_wire = true;
  if (dev->i2c_freq == 0 || !(dev->i2c_scl == WM_IO_PA_01 || dev->i2c_scl == WM_IO_PB_20)
      || !(dev->i2c_sda == WM_IO_PA_04 || dev->i2c_sda == WM_IO_PB_19))
    {
      dev->hw_wire = false;
    }
  if (dev->hw_wire)
    HWire_init (dev);
  else
    SWire_init (dev);

}

void i2c_acquire(i2c_t dev){/*в оригинале, это старт и стоп тактового генератора для аппаратного I2C, типа экономия потребления... */};

void i2c_release(i2c_t dev){};

int i2c_read_reg(i2c_t dev, uint16_t addr, uint16_t reg,
                 void *data, uint8_t flags)
{
  if (dev->hw_wire)
    return HWire_read_regs(dev,addr,reg,&data,1,flags);
  else 
    return SWire_read_regs(dev,addr,reg,&data,1,flags);
};
int i2c_read_regs(i2c_t dev, uint16_t addr, uint16_t reg,
                  void *data, size_t len, uint8_t flags)
{
//  if (flags & (I2C_REG16 | I2C_ADDR10 | I2C_NOSTART | I2C_NOSTOP)) 
//    {
//      return -EOPNOTSUPP;
//    }
  if (dev->hw_wire)
    return HWire_read_regs(dev,addr,reg,data,len,flags);
  else 
    return SWire_read_regs(dev,addr,reg,data,len,flags);
};


int i2c_read_byte(i2c_t dev, uint16_t addr, void *data, uint8_t flags)
{
  if (dev->hw_wire)
    return HWire_read_bytes(dev,addr,&data,1,flags);
  else 
    return SWire_read_bytes(dev,addr,&data,1,flags);
};
int i2c_read_bytes(i2c_t dev, uint16_t addr,
                   void *data, size_t len, uint8_t flags)
{
  if (dev->hw_wire)
    return HWire_read_bytes(dev,addr,data,len,flags);
  else 
    return SWire_read_bytes(dev,addr,data,len,flags);
}

int i2c_write_byte(i2c_t dev, uint16_t addr, uint8_t data, uint8_t flags)
{
  if (dev->hw_wire)
    return HWire_write_bytes(dev,addr,&data,1,flags);
  else 
    return SWire_write_bytes(dev,addr,&data,1,flags);
}

int i2c_write_bytes(i2c_t dev, uint16_t addr, const void *data,
                    size_t len, uint8_t flags)
{
  if (dev->hw_wire)
    return HWire_write_bytes(dev,addr,data,len,flags);
  else 
    return SWire_write_bytes(dev,addr,data,len,flags);
}

int i2c_write_reg(i2c_t dev, uint16_t addr, uint16_t reg,
                  uint8_t data, uint8_t flags)
{
  if (dev->hw_wire)
    return HWire_write_regs(dev,addr,reg,&data,1,flags);
  else 
    return SWire_write_regs(dev,addr,reg,&data,1,flags);
}

int i2c_write_regs(i2c_t dev, uint16_t addr, uint16_t reg,
                  const void *data, size_t len, uint8_t flags)
{
  if (dev->hw_wire)
    return HWire_write_regs(dev,addr,reg,data,len,flags);
  else 
    return SWire_write_regs(dev,addr,reg,data,len,flags);
}


//************************************************************************8
