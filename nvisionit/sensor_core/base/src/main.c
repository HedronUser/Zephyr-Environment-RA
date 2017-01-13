/* main_arc.c - main source file for ARC app */

/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <zephyr.h>
#include <stdio.h>
#include <device.h>
#include <sensor.h>
#include <adc.h>

#include <ipm.h>
#include <ipm/ipm_quark_se.h>

#include <qm_ss_i2c.h>

#include <init.h>
#include <string.h>

#include <misc/byteorder.h>
#include <misc/printk.h>
#include <misc/util.h>

#include <ipm_ids.h>

#include <MPL3115A2.h>


QUARK_SE_IPM_DEFINE(sensor_ipm, 0, QUARK_SE_IPM_OUTBOUND);

#define ADC_DEVICE_NAME "ADC_0"
#define ADC_CHANNEL 	10
#define ADC_BUFFER_SIZE 4

static uint8_t seq_buffer[ADC_BUFFER_SIZE];

static struct adc_seq_entry sample = {
	.sampling_delay = 12,
	.channel_id = ADC_CHANNEL,
	.buffer = seq_buffer,
	.buffer_length = ADC_BUFFER_SIZE,
};

static struct adc_seq_table table = {
	.entries = &sample,
	.num_entries = 1,
};

void main(void)
{
  struct device *i2c_dev, *dht11_dev, *adc_dev;

  struct device *sensor_ipm;
  int ret;
  float temp, baro, alti, humi, moist;

  // Get the devices
  i2c_dev = device_get_binding("I2C_0");
  if (!i2c_dev)
  {
    printk("Error getting I2C device.\n");
  }

  dht11_dev = device_get_binding("DHT11");
  if (!dht11_dev)
  {
    printf("DHT11: Device not found.\n");
  }

  adc_dev = device_get_binding(ADC_DEVICE_NAME);
  if (!adc_dev)
  {
    printf("Error getting ADC device.\n");
  }

  // Initialize the devices
  mpl3115a2_init(i2c_dev);
  adc_enable(adc_dev);

  // Get the IPM channels
  sensor_ipm = device_get_binding("sensor_ipm");
  if (!sensor_ipm)
  {
    printk("IPM: Device not found.\n");
  }

  while (i2c_dev)
  {
    // Get the values
    temp = mpl3115a2_get_temperature(i2c_dev);
    baro = mpl3115a2_get_pressure(i2c_dev);
    alti = mpl3115a2_get_altitude(i2c_dev);

    if (sensor_sample_fetch(dht11_dev))
    {
      struct sensor_value val;
      if (sensor_channel_get(dht11_dev, SENSOR_CHAN_HUMIDITY, &val) < 0)
      {
	      printf("Humidity channel read error.\n");
      }
      else
      {
	      humi = (float)(val.val1 / 1000.0);
      }
    }

    if (adc_read(adc_dev, &table) == 0) {
      uint32_t m1 = *((uint32_t*)seq_buffer); // This is faster than bit shifting, no math, just dereference.
      
      uint32_t m = m1 & 0xFFF;
      moist = 100.0 - ((m / 4096.0) * 100);
      // printf("SS Moisture: %d - %d  -  %x\n", m, (uint32_t)moist, m1);
    }

    ret = ipm_send(sensor_ipm, 1, IPM_ID_TEMPERATURE, &temp, sizeof(temp));
    if (ret)
    {
      printk("Failed to send IPM_ID_TEMPERATURE message, error (%d)\n", ret);
    }

    k_sleep(100);

    ret = ipm_send(sensor_ipm, 1, IPM_ID_BAROMETER, &baro, sizeof(baro));
    if (ret)
    {
      printk("Failed to send IPM_ID_ALTITUDE message, error (%d)\n", ret);
    }

    k_sleep(100);

    ret = ipm_send(sensor_ipm, 1, IPM_ID_ALTITUDE, &alti, sizeof(alti));
    if (ret)
    {
      printk("Failed to send IPM_ID_ALTITUDE message, error (%d)\n", ret);
    }

    k_sleep(100);

    ret = ipm_send(sensor_ipm, 1, IPM_ID_HUMIDITY, &humi, sizeof(humi));
    if (ret)
    {
      printk("Failed to send IPM_ID_HUMIDITY message, error (%d)\n", ret);
    }

    k_sleep(100);
    ret = ipm_send(sensor_ipm, 1, IPM_ID_MOISTURE, &moist, sizeof(moist));
    if (ret)
    {
      printk("Failed to send IPM_ID_MOISTURE message, error (%d)\n", ret);
    }

    k_sleep(100);
  }
}
