#ifdef __cplusplus
extern "C" {
#endif

enum sensor_channel_oemph{
    SENSOR_CHAN_OEMPH_PH = SENSOR_CHAN_PRIV_START,
};

enum sensor_attribute_oemph{
    SENSOR_ATTR_OEMPH_CALIBRATION_CLEAR = SENSOR_ATTR_PRIV_START,
    SENSOR_ATTR_OEMPH_CALIBRATION_LOW,
    SENSOR_ATTR_OEMPH_CALIBRATION_MID,
    SENSOR_ATTR_OEMPH_CALIBRATION_HIGH,
};

#ifdef __cplusplus
}
#endif
