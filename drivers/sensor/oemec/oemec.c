/*
 * Copyright (c) 2023 Arribada Initiative CIC
 *
 * SPDX-License-Identifier: MIT
 */
#define DT_DRV_COMPAT atlas_oemec

#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include "oemec.h"

LOG_MODULE_REGISTER(OEMEC, CONFIG_SENSOR_LOG_LEVEL);

#define OEMEC_DEVICE_TYPE         0x00
#define OEMEC_FW_VERSION          0x01
#define OEMEC_ADDR_UNLOCK         0x02
#define OEMEC_ADDR                0x03
#define OEMEC_INT_CONTROL         0x04
#define OEMEC_LED_CONTROL         0x05
#define OEMEC_ACT_HIBERNATE       0x06
#define OEMEC_NEW_READING         0x07
#define OEMEC_PROBE_MSB           0x08
#define OEMEC_PROBE_LSB           0x09
#define OEMEC_CALIB_MSB           0x0A
#define OEMEC_CALIB_HIGH_BYTE     0x0B
#define OEMEC_CALIB_LOW_BYTE      0x0C
#define OEMEC_CALIB_LSB           0x0D
#define OEMEC_CALIB_REQUEST       0x0E
#define OEMEC_CALIB_CONFIRM       0x0F
#define OEMEC_TEMP_COMP_MSB       0x10
#define OEMEC_TEMP_COMP_HIGH_BYTE 0x11
#define OEMEC_TEMP_COMP_LOW_BYTE  0x12U
#define OEMEC_TEMP_COMP_LSB       0x13
#define OEMEC_TEMP_CONF_MSB       0x14
#define OEMEC_TEMP_CONF_HIGH_BYTE 0x15
#define OEMEC_TEMP_CONF_LOW_BYTE  0x16
#define OEMEC_TEMP_CONF_LSB       0x17
#define OEMEC_SENSOR_MSB          0x18
#define OEMEC_SENSOR_HIGH_BYTE    0x19
#define OEMEC_SENSOR_LOW_BYTE     0x1A
#define OEMEC_SENSOR_LSB          0x1B
#define OEMEC_TDS_MSB             0x1C
#define OEMEC_TDS_HIGH_BYTE       0x1D
#define OEMEC_TDS_LOW_BYTE        0x1E
#define OEMEC_TDS_LSB             0x1F
#define OEMEC_SALINITY_MSB        0x20
#define OEMEC_SALINITY_HIGH_BYTE  0x21
#define OEMEC_SALINITY_LOW_BYTE   0x22
#define OEMEC_SALINITY_LSB        0x23

struct oemec_config {
	struct i2c_dt_spec bus;
	/* uint8_t resolution; */
	/* uint8_t mtreg; */
};

struct oemec_data {
	float sample;
};

static int oemec_read_regs(const struct device *dev, uint8_t addr, void *buf, size_t len)
{
	const struct oemec_config *config = dev->config;
	int err;

	err = i2c_write_read_dt(&config->bus, &addr, sizeof(addr), buf, len);
	if (err != 0) {
		LOG_ERR("failed to read reg addr 0x%02x, len %d (err %d)", addr, len, err);
		return err;
	}

	return 0;
}

static int oemec_write_regs(const struct device *dev, uint8_t addr, void *buf, size_t len)
{
	const struct oemec_config *config = dev->config;
	uint8_t block[sizeof(addr) + len];
	int err;

	block[0] = addr;
	memcpy(&block[1], buf, len);

	err = i2c_write_dt(&config->bus, block, sizeof(block));
	if (err != 0) {
		LOG_ERR("failed to write reg addr 0x%02x, len %d (err %d)", addr, len, err);
		return err;
	}

	return 0;
}

static int oemec_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
	LOG_INF("Fetching samples");	
	/* Initiate sensor read */

	/* uint8_t devicetype_firmware[2]={0,0}; */
	/* oemec_read_regs(dev,OEMEC_DEVICE_TYPE,&devicetype_firmware,sizeof(devicetype_firmware)); */
	/* LOG_INF("Device type : %d",devicetype_firmware[0]); */
	/* LOG_INF("FW version  : %d", devicetype_firmware[1]); */

	/* Get the values */

	uint8_t sensor_values[4]={1,2,3,4};
	unsigned long sensor_values_long=0;
	float sensor_values_float;
	oemec_read_regs(dev,OEMEC_SENSOR_MSB,&sensor_values,sizeof(sensor_values));

	sensor_values_long += sensor_values[0] << 24;
	sensor_values_long += sensor_values[1] << 16;
	sensor_values_long += sensor_values[2] << 8;
	sensor_values_long += sensor_values[3];

	sensor_values_float = (float)sensor_values_long;
	sensor_values_float = sensor_values_float/100;
	
	LOG_INF("Sensor values as 4 regs :%x %x %x %x",sensor_values[0],sensor_values[1],sensor_values[2],sensor_values[3]);
	LOG_INF("Sensor value as long %lu",sensor_values_long);
	LOG_INF("Sensor value as float %f",sensor_values_float);
	
	return 0;
}

static int oemec_set_probe()
{
	return 0;
}

static int oemec_set_calibration()
{
	return 0;
}

static int oemec_channel_get(const struct device *dev, enum sensor_channel chan,
			     struct sensor_value *val)
{
	LOG_INF("Fetching value for channel %d", chan);
	return 0;
}

static int oemec_attr_get(const struct device *dev, enum sensor_channel chan,
			   enum sensor_attribute attr, struct sensor_value *val)
{
	return 0;
}

static int oemec_attr_set(const struct device *dev, enum sensor_channel chan,
			  enum sensor_attribute attr, const struct sensor_value *val)
{
	switch(attr){
	case SENSOR_ATTR_CALIBRATION:
		return oemec_set_calibration();
	case SENSOR_ATTR_OEMEC_PROBE:
		return oemec_set_probe();
	default:
		LOG_ERR("Attribute not supported");
		return -ENOTSUP;
		
	}
	return 0;
}

static const struct sensor_driver_api oemec_driver_api = {
	.sample_fetch = oemec_sample_fetch,
	.channel_get = oemec_channel_get,
	.attr_set = oemec_attr_set,
	.attr_get = oemec_attr_get,
};

static int oemec_init(const struct device *dev)
{
	LOG_DBG("Inside oemec_init");
	return 0;
}

#define DEFINE_OEMEC(inst)                                                                         \
	static struct oemec_data oemec_data_##inst;                                                \
                                                                                                   \
	static const struct oemec_config oemec_config_##inst = {                                   \
		.bus = I2C_DT_SPEC_INST_GET(inst),                                                 \
	};                                                                                         \
	SENSOR_DEVICE_DT_INST_DEFINE(inst, oemec_init, NULL, &oemec_data_##inst,                   \
				     &oemec_config_##inst, POST_KERNEL,                            \
				     CONFIG_SENSOR_INIT_PRIORITY, &oemec_driver_api);

DT_INST_FOREACH_STATUS_OKAY(DEFINE_OEMEC)
