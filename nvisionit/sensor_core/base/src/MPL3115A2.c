// MPL3115A2 Barometric Pressure & Altimeter & Temperature sensor
// Adafruit converted code from the sample library
// Original code lives here: https://github.com/adafruit/Adafruit_MPL3115A2_Library

#include <MPL3115A2.h>

int mpl3115a2_init(struct device *i2c_dev)
{
  uint8_t whoami;
  int ret = i2c_reg_read_byte(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_WHOAMI, &whoami);
  if (ret)
  {
    printk("Could not read from I2C device %x: %d\n", MPL3115A2_ADDRESS, ret);
  }

  printk("whoami: %x\n", whoami);

  if (whoami != 0xC4)
  {
    return false;
  }

  i2c_reg_write_byte(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1,
                     MPL3115A2_CTRL_REG1_SBYB |
                         MPL3115A2_CTRL_REG1_OS128);

  i2c_reg_write_byte(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_PT_DATA_CFG,
                     MPL3115A2_PT_DATA_CFG_TDEFE |
                         MPL3115A2_PT_DATA_CFG_PDEFE |
                         MPL3115A2_PT_DATA_CFG_DREM);

  return true;
}

float mpl3115a2_get_temperature(struct device *i2c_dev)
{
  int ret;
  ret = i2c_reg_write_byte(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_PT_DATA_CFG,
                           MPL3115A2_PT_DATA_CFG_TDEFE |
                               MPL3115A2_PT_DATA_CFG_PDEFE |
                               MPL3115A2_PT_DATA_CFG_DREM);

  if (ret)
  {
    printk("Could not set config on I2C device %x: %d\n", MPL3115A2_ADDRESS, ret);
  }

  uint8_t sta = 0;
  while (!(sta & MPL3115A2_REGISTER_STATUS_TDR))
  {
    ret = i2c_reg_read_byte(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_STATUS, &sta);
    if (ret)
    {
      sta = 0;
    }
    k_sleep(50);
  }

  uint8_t data[2];
  ret = i2c_burst_read(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_TEMP_MSB, data, sizeof(data));
  if (ret)
  {
    printk("Could not read from I2C device %x: %d\n", MPL3115A2_ADDRESS, ret);
  }

  int16_t t;
  t = data[0];
  t <<= 8;
  t |= data[1];
  t >>= 4;

  float temp = t;
  temp /= 16.0;

  i2c_reg_write_byte(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_TEMP_MSB, 0);

  return temp;
}

float mpl3115a2_get_pressure(struct device *i2c_dev)
{
  int ret;

  ret = i2c_reg_write_byte(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1,
                           MPL3115A2_CTRL_REG1_OS128 |
                               MPL3115A2_CTRL_REG1_OST |
                               MPL3115A2_CTRL_REG1_BAR);

  if (ret)
  {
    printk("Could not set control register on I2C device %x: %d\n", MPL3115A2_ADDRESS, ret);
  }

  uint8_t sta = 0;
  while (!(sta & MPL3115A2_REGISTER_STATUS_PDR))
  {
    ret = i2c_reg_read_byte(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_STATUS, &sta);
    if (ret)
    {
      sta = 0;
    }
    k_sleep(50);
  }

  uint8_t data[3];
  ret = i2c_burst_read(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_PRESSURE_MSB, data, sizeof(data));
  if (ret)
  {
    printk("Could not read from I2C device %x: %d\n", MPL3115A2_ADDRESS, ret);
  }

  int32_t p;
  p = data[0];
  p <<= 8;
  p |= data[1];
  p <<= 8;
  p |= data[2];
  p >>= 4;

  float baro = p;
  baro /= 4.0;

  i2c_reg_write_byte(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_PRESSURE_MSB, 0);

  return baro;
}

float mpl3115a2_get_altitude(struct device *i2c_dev)
{
  int ret;

  ret = i2c_reg_write_byte(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1,
                           MPL3115A2_CTRL_REG1_OS128 |
                               MPL3115A2_CTRL_REG1_OST |
                               MPL3115A2_CTRL_REG1_ALT);

  if (ret)
  {
    printk("Could not set control register on I2C device %x: %d\n", MPL3115A2_ADDRESS, ret);
  }

  uint8_t sta = 0;
  while (!(sta & MPL3115A2_REGISTER_STATUS_PDR))
  {
    ret = i2c_reg_read_byte(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_STATUS, &sta);
    if (ret)
    {
      sta = 0;
    }
    k_sleep(50);
  }

  uint8_t data[3];
  ret = i2c_burst_read(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_PRESSURE_MSB, &data, sizeof(data));
  if (ret)
  {
    printk("Could not read from I2C device %x: %d\n", MPL3115A2_ADDRESS, ret);
  }

  int32_t a;
  a = data[0];
  a <<= 8;
  a |= data[1];
  a <<= 8;
  a |= data[2];
  a >>= 4;

  if (a & 0x80000)
  {
    a |= 0xFFF00000;
  }

  float alti = a;
  alti /= 16.0;

  i2c_reg_write_byte(i2c_dev, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_PRESSURE_MSB, 0);

  return alti;
}
