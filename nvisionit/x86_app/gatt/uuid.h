// Missing UUIDs for BLE / GATT

#ifndef NV_UUID
#define NV_UUID


/** @def BT_UUID_AIOS
*  @brief Automation IO Service (https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.automation_io.xml)
*/
#define BT_UUID_AIOS                       BT_UUID_DECLARE_16(0x1815)
#define BT_UUID_AIOS_VAL                   0x1815

/** @def BT_UUID_AIOS_DIGITAL
 *  @brief AIOS Characteristic Digital (https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.digital.xml)
 */
#define BT_UUID_AIOS_DIGITAL               BT_UUID_DECLARE_16(0x2a56)
#define BT_UUID_AIOS_DIGITAL_VAL           0x2a56

/** @def BT_UUID_AIOS_ANALOG
*  @brief AIOS Characteristic Analog (https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.analog.xml)
*/
#define BT_UUID_AIOS_ANALOG                BT_UUID_DECLARE_16(0x2a58)
#define BT_UUID_AIOS_ANALOG_VAL            0x2a58

/** @def BT_UUID_AIOS_AGGREGATE
*  @brief AIOS Characteristic Aggregate (https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.aggregate.xml)
*/
#define BT_UUID_AIOS_AGGREGATE             BT_UUID_DECLARE_16(0x2a5a)
#define BT_UUID_AIOS_AGGREGATE_VAL         0x2a5a

/** @def BT_UUID_ESS
*  @brief Environmental Sensing Service (https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.environmental_sensing.xml)
*/
#define BT_UUID_ESS                       BT_UUID_DECLARE_16(0x181a)
#define BT_UUID_ESS_VAL                   0x181a

/** @def BT_UUID_ESS_HUMIDITY
*  @brief ESS Characteristic Humidity (https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.humidity.xml)
*/
#define BT_UUID_ESS_HUMIDITY             BT_UUID_DECLARE_16(0x2a6f)
#define BT_UUID_ESS_HUMIDITY_VAL         0x2a6f

/** @def BT_UUID_ESS_PRESSURE
*  @brief ESS Characteristic Pressure (https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.pressure.xml)
*/
#define BT_UUID_ESS_PRESSURE             BT_UUID_DECLARE_16(0x2a6d)
#define BT_UUID_ESS_PRESSURE_VAL         0x2a6d

/** @def BT_UUID_ESS_TEMPERATURE
*  @brief ESS Characteristic Temperature (https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.temperature.xml)
*/
#define BT_UUID_ESS_TEMPERATURE             BT_UUID_DECLARE_16(0x2a6e)
#define BT_UUID_ESS_TEMPERATURE_VAL         0x2a6e

/** @def BT_UUID_ESS_ELEVATION
*  @brief ESS Characteristic Temperature (https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.elevation.xml)
*/
#define BT_UUID_ESS_ELEVATION_VAL         0x2A6C
#define BT_UUID_ESS_ELEVATION             BT_UUID_DECLARE_16(BT_UUID_ESS_ELEVATION_VAL)

/** @def BT_UUID_ESS_GYRO
*  @brief ESS Characteristic Gyrometer - custom characteristic
*/
#define BT_UUID_ESS_GYRO                    BT_UUID_DECLARE_16(0x3000)
#define BT_UUID_ESS_GYRO_VAL                0x3000

/** @def BT_UUID_ESS_ACCEL
*  @brief ESS Characteristic Accelerometer - custom characteristic
*/
#define BT_UUID_ESS_ACCELEROMETER           BT_UUID_DECLARE_16(0x3001)
#define BT_UUID_ESS_ACCELEROMETER_VAL       0x3001

/** @def BT_UUID_ESS_MOIST
*  @brief ESS Characteristic Accelerometer - custom characteristic
*/
#define BT_UUID_ESS_MOISTURE           BT_UUID_DECLARE_16(0x3002)
#define BT_UUID_ESS_MOISTURE_VAL       0x3002

#endif
