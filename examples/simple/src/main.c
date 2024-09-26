#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>
#include <oemph.h>

LOG_MODULE_REGISTER(simple_app);
const struct device *dev;

static int cmd_calib_clear(const struct shell *sh,
                           size_t argc, char **argv, void *data)
{
  int rc = sensor_attr_set(dev,SENSOR_CHAN_OEMPH_PH,SENSOR_ATTR_OEMPH_CALIBRATION_CLEAR,NULL);
  shell_print(sh,"Setting attribute for calibration clear - %d",rc);
  return rc;
}

static int cmd_calib_low(const struct shell *sh,
                           size_t argc, char **argv, void *data)
{
  struct sensor_value calib = {};
  calib.val1 = atoi(argv[1]);
  int rc = sensor_attr_set(dev,SENSOR_CHAN_OEMPH_PH,SENSOR_ATTR_OEMPH_CALIBRATION_LOW,&calib);
  shell_print(sh,"Setting attribute for calibration low - %d",rc);
  return 0;
}

static int cmd_calib_mid(const struct shell *sh,
                         size_t argc, char **argv, void *data)
{
  struct sensor_value calib = {};
  calib.val1 = atoi(argv[1]);
  int rc = sensor_attr_set(dev,SENSOR_CHAN_OEMPH_PH,SENSOR_ATTR_OEMPH_CALIBRATION_MID,&calib);
  shell_print(sh,"Setting attribute for calibration mid - %d",rc);
  return 0;
}

static int cmd_calib_high(const struct shell *sh,
                         size_t argc, char **argv, void *data)
{
  struct sensor_value calib = {};
  calib.val1 = atoi(argv[1]);
  int rc = sensor_attr_set(dev,SENSOR_CHAN_OEMPH_PH,SENSOR_ATTR_OEMPH_CALIBRATION_HIGH,&calib);
  shell_print(sh,"Setting attribute for calibration high - %d",rc);
  return 0;
}

/* Creating subcommands (level 1 command) array for command "demo". */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_demo,
                               SHELL_CMD(clear, NULL, " Clear",
                                         cmd_calib_clear),
                               SHELL_CMD(low,   NULL, " Low", cmd_calib_low),
                               SHELL_CMD(mid,   NULL, " Mid", cmd_calib_mid),
                               SHELL_CMD(high,  NULL, " High", cmd_calib_high),
                               SHELL_SUBCMD_SET_END
                               );
/* Creating root (level 0) command "demo" */
SHELL_CMD_REGISTER(demo, &sub_demo, "Calib commands", NULL);


int main(void)
{
  LOG_INF("Running OEMPH simple app on %s\n", CONFIG_BOARD);
  dev = DEVICE_DT_GET_ANY(atlas_oemph);
  if (dev == NULL) {
    LOG_INF("No device found");
    return 0;
  }
  LOG_INF("Setting attributes");

  struct sensor_value ph;

  /* while (true) { */
  /*   rc = sensor_sample_fetch(dev); */
  /*   if (rc != 0 ){ */
  /*           LOG_ERR("OEMPH failed: %d",rc); */
  /*     break; */
  /*   } */
  /*   else { // read channels */
  /*     rc = sensor_channel_get(dev, SENSOR_CHAN_OEMPH_PH, &ph); */
  /*           LOG_INF("PH value %f", sensor_value_to_double(&ph)); */
  /*   } */
  /*   k_msleep(2000); */
  /* } */
  while(true){
    k_msleep(2000);
  }
  return 0;
}
