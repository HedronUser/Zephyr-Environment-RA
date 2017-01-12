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

QUARK_SE_IPM_DEFINE(temp_sensor_ipm, 0, QUARK_SE_IPM_OUTBOUND);
QUARK_SE_IPM_DEFINE(baro_sensor_ipm, 1, QUARK_SE_IPM_OUTBOUND);
QUARK_SE_IPM_DEFINE(alti_sensor_ipm, 2, QUARK_SE_IPM_OUTBOUND);
QUARK_SE_IPM_DEFINE(humi_sensor_ipm, 3, QUARK_SE_IPM_OUTBOUND);

void main(void)
{
  struct device *i2c_dev, *dht11_dev;

  struct device *temp_ipm, *baro_ipm, *alti_ipm, *humi_ipm;
  int ret;
  float temp, baro, alti, humi;

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

  // Initialize the devices
  mpl3115a2_init(i2c_dev);

  // Get the IPM channels
  temp_ipm = device_get_binding("temp_sensor_ipm");
  if (!temp_ipm)
  {
    printk("IPM: Device not found.\n");
  }

  baro_ipm = device_get_binding("baro_sensor_ipm");
  if (!baro_ipm)
  {
    printk("IPM: Device not found.\n");
  }

  alti_ipm = device_get_binding("alti_sensor_ipm");
  if (!alti_ipm)
  {
    printk("IPM: Device not found.\n");
  }

  humi_ipm = device_get_binding("humi_sensor_ipm");
  if (!humi_ipm)
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

    ret = ipm_send(temp_ipm, 1, IPM_ID_TEMPERATURE, &temp, sizeof(temp));
    if (ret)
    {
      printk("Failed to send IPM_ID_TEMPERATURE message, error (%d)\n", ret);
    }

    ret = ipm_send(baro_ipm, 1, IPM_ID_BAROMETER, &baro, sizeof(baro));
    if (ret)
    {
      printk("Failed to send IPM_ID_ALTITUDE message, error (%d)\n", ret);
    }

    ret = ipm_send(alti_ipm, 1, IPM_ID_ALTITUDE, &alti, sizeof(alti));
    if (ret)
    {
      printk("Failed to send IPM_ID_ALTITUDE message, error (%d)\n", ret);
    }

    ret = ipm_send(humi_ipm, 1, IPM_ID_HUMIDITY, &humi, sizeof(humi));
    if (ret)
    {
      printk("Failed to send IPM_ID_HUMIDITY message, error (%d)\n", ret);
    }

    k_sleep(500);
  }
}
