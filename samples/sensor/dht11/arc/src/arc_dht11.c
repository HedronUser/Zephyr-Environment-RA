/*
 * Copyright (c) 2016 Intel Corporation
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
#include <misc/util.h>

#define SLEEPTIME 300


/**
 * @brief Helper function for printing a sensor value to a buffer
 *
 * @param buf A pointer to the buffer to which the printing is done.
 * @param len Size of buffer in bytes.
 * @param val A pointer to a sensor_value struct holding the value
 *            to be printed.
 *
 * @return The number of characters printed to the buffer.
 */
static inline int sensor_value_snprintf(char *buf, size_t len,const struct sensor_value *val)
{
	int32_t val1, val2;

	switch (val->type) {
	case SENSOR_VALUE_TYPE_INT:
		return snprintf(buf, len, "%d", val->val1);
	case SENSOR_VALUE_TYPE_INT_PLUS_MICRO:
		if (val->val2 == 0) {
			return snprintf(buf, len, "%d", val->val1);
		}

		/* normalize value */
		if (val->val1 < 0 && val->val2 > 0) {
			val1 = val->val1 + 1;
			val2 = val->val2 - 1000000;
		} else {
			val1 = val->val1;
			val2 = val->val2;
		}

		/* print value to buffer */
		if (val1 > 0 || (val1 == 0 && val2 > 0)) {
			return snprintf(buf, len, "%d.%06d", val1, val2);
		} else if (val1 == 0 && val2 < 0) {
			return snprintf(buf, len, "-0.%06d", -val2);
		} else {
			return snprintf(buf, len, "%d.%06d", val1, -val2);
		}
	case SENSOR_VALUE_TYPE_DOUBLE:
		return snprintf(buf, len, "%f", val->dval);
	default:
		return 0;
	}
}

static void print_temp_data(struct device *dht11)
{
	struct sensor_value val;
	char buf[18];

	if (sensor_channel_get(dht11, SENSOR_CHAN_HUMIDITY, &val) < 0) {
		printf("Humidity channel read error.\n");
		return;
	}

	sensor_value_snprintf(buf, sizeof(buf), &val);

	printf("Humidity (%%): %s\n", buf);
}

static void test_polling_mode(struct device *dht11)
{

	while(1) {

		if (sensor_sample_fetch(dht11) < 0) {
			printf("arc Sample update error.\n");
			// return;
		} else {
			printf("arc Got SAMPLE!\n");
		}

		print_temp_data(dht11);

		/* wait a while */
		k_sleep(SLEEPTIME);
	}

}


void main(void)
{
	 struct device *dht11;

	 printf("IMU: Binding...\n");

	 dht11 = device_get_binding("DHT11");
	 if (!dht11) {
	 	printf("DHT11: Device not found.\n");
	 	return;
	 }

	 printf("Testing the polling mode.\n");

	while(1) {
	  test_polling_mode(dht11);
		k_sleep(1000);
	}

}
