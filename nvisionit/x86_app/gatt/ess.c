/** @file
 *  @brief ESS Service sample
 */

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
#include "ess.h"
#include "uuid.h"


static struct bt_gatt_ccc_cfg  blvl_ccc_cfg[CONFIG_BLUETOOTH_MAX_PAIRED] = {};
static uint8_t simulate_blvl;

static float ess_humidity = 2;
static float ess_pressure = 3;
static float ess_elevation = 4;
static float ess_temp = 5;

static void blvl_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	simulate_blvl = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
}

static ssize_t read_temperature(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
  // BLE ESS Temperature requires a 16 bit signed integer in units of 0.01 degrees Celcius
	int16_t temp = (int16_t) (ess_temp * 100);

	void* value_pt = &temp;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value_pt, 2);
}

static ssize_t read_humidity(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
  uint16_t humi = (uint16_t) (ess_humidity * 100);

	void* value_pt = &humi;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value_pt, 2);
}

static ssize_t read_pressure(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{ 
  // BLE ESS Pressure requires a 32 bit unsigned integer in units of 0.1 Pa
	uint32_t pres = (uint32_t) (ess_pressure * 10);

	void* value_pt = &pres;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value_pt, 4);
}

static ssize_t read_elevation(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
  // BLE ESS Elevation requires a 24 bit signed integer in units of 0.01m
	int32_t elev = (int32_t)(ess_elevation * 100);	

	void* value_pt = &elev;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value_pt, 3);
}

/* Automation IO Service Declaration */
// https://nexus.zephyrproject.org/content/sites/site/org.zephyrproject.zephyr/dev/api/html/de/d63/gatt_8h.html
static struct bt_gatt_attr attrs[] = {
	BT_GATT_PRIMARY_SERVICE(BT_UUID_ESS),

	BT_GATT_CHARACTERISTIC(BT_UUID_ESS_TEMPERATURE, BT_GATT_CHRC_READ),
	BT_GATT_DESCRIPTOR(BT_UUID_ESS_TEMPERATURE, BT_GATT_PERM_READ, read_temperature, NULL, NULL),

	BT_GATT_CHARACTERISTIC(BT_UUID_ESS_HUMIDITY, BT_GATT_CHRC_READ),
	BT_GATT_DESCRIPTOR(BT_UUID_ESS_HUMIDITY, BT_GATT_PERM_READ, read_humidity, NULL, NULL),

	BT_GATT_CHARACTERISTIC(BT_UUID_ESS_PRESSURE, BT_GATT_CHRC_READ),
	BT_GATT_DESCRIPTOR(BT_UUID_ESS_PRESSURE, BT_GATT_PERM_READ, read_pressure, NULL, NULL),

	BT_GATT_CHARACTERISTIC(BT_UUID_ESS_ELEVATION, BT_GATT_CHRC_READ),
	BT_GATT_DESCRIPTOR(BT_UUID_ESS_ELEVATION, BT_GATT_PERM_READ, read_elevation, NULL, NULL),
};

void ess_init(void)
{
	bt_gatt_register(attrs, ARRAY_SIZE(attrs));
}

void ess_temperature_notify(float* value_pf32)
{
	ess_temp = *value_pf32;
};

void ess_pressure_notify(float* value_pf32)
{
	ess_pressure = *value_pf32;
};

void ess_elevation_notify(float* value_pf32)
{
	ess_elevation = *value_pf32;
};

void ess_humidity_notify(float* value_pf32)
{
	ess_humidity = *value_pf32;
};