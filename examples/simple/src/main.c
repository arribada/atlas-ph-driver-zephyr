#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <oemph.h>

LOG_MODULE_REGISTER(simple_app);

int main(void)
{
	int rc;
	LOG_INF("Running OEMPH simple app on %s\n", CONFIG_BOARD);
	const struct device *dev = DEVICE_DT_GET_ANY(atlas_oemph);
	if (dev == NULL) {
		LOG_INF("No device found");
		return 0;
	}
	LOG_INF("Setting attributes");	

	struct sensor_value conductivity;
	sensor_attr_set(dev,SENSOR_CHAN_OEMPH_CONDUCTIVITY,SENSOR_ATTR_OEMPH_PROBE,conductivity);
	while (true) {
		rc = sensor_sample_fetch(dev);
		if (rc != 0 ){
		        LOG_ERR("OEMPH failed: %d",rc);
			break;
		}
		else { // read channels
			rc = sensor_channel_get(dev, SENSOR_CHAN_OEMPH_CONDUCTIVITY, &conductivity);
		        LOG_INF("Conductivity value %f", sensor_value_to_double(&conductivity));
		}
		k_msleep(2000);
	}

	return 0;
}
