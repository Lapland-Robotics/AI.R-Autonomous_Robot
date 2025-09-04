#include <Wire.h> // unused here but necessary for I2C communication (Adafruit_INA219)
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_INA219.h>
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rclc/timer.h>
#include <Arduino.h>
#include "rosidl_runtime_c/string.h"
#include "rosidl_runtime_c/string_functions.h"
#include "esp_adc_cal.h"

#include "wifi_secrets.h"

#include <std_msgs/msg/string.h>
#include <power_board_msgs/msg/power_datas.h>

#include "main.hpp"

#define STS30_ADDR 0x44 // default I2C address of STS30

unsigned long now = 0;
unsigned long publish_data_LET = 0;

// INA219 instances
Adafruit_INA219 ina0(0x40), // +24V
                ina1(0x41), // +19V
                ina2(0x46), // +16V
                ina3(0x4F), // +12V (1)
                ina4(0x44), // +12V (2)
                ina5(0x45); // +5V
Adafruit_INA219* sensors[] = {&ina0,&ina1,&ina2,&ina3,&ina4,&ina5};
const uint8_t NUM_SENS = sizeof(sensors)/sizeof(sensors[0]);
const char* labels[]  = {"0x40","0x41","0x46","0x4F","0x44","0x45"}; // just for serial printing

// ACS711 Viout
const int ACS_PIN = 32; //(ADC1_CH4)
const int resolution = 12; // bits
const float adcResolution = (float) pow(2, resolution) - 1; // 12 bit resolution (0 - 4095)
const float refVoltage = 3.3; // Volts
const float zeroCurrentVoltage = 2.5; // Output voltage at 0A from ACS711
const float sensitivity = 0.045; // 45mV per Amp
const int nb_samples = 32; // multisampling over 32 values
esp_adc_cal_characteristics_t adc_chars;

/* ROS related variables*/
power_board_msgs__msg__PowerDatas dataMsg;
rcl_publisher_t dataPublisher;
rcl_timer_t publishTimer;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rclc_executor_t ctrlCmdExecutor;
int errorRetryCount = 0;
int safeRetryCount = 0;

void errorLoop() {
  errorRetryCount++;
  delay(1000);
  if (errorRetryCount > 5) {
    ESP.restart();
  }
  else {
    microrosCleanup();
    microrosInit();
  }
}  

bool safePublish(rcl_publisher_t* publisher, void* msg, const char* publisher_name) {
  rcl_ret_t rc = rcl_publish(publisher, msg, NULL);
  if (rc != RCL_RET_OK) {
    safeRetryCount++;
    delay(100);
    if (safeRetryCount > 3) {
      ESP.restart();
    }
    return false;
  }
  safeRetryCount = 0;  // Reset retry count on success
  return true;
}

void publishData(rcl_timer_t * timer, int64_t last_call_time){
  builtin_interfaces__msg__Time stamp;
  RCSOFTCHECK(rmw_uros_sync_session(1000));
  uint64_t millis = rmw_uros_epoch_millis();
  stamp.sec = millis / 1000;
  stamp.nanosec = (millis % 1000) * 1000000;
  dataMsg.timestamp = stamp;

  // read ACS711 sensor data (total current)
  uint32_t adcValue = 0;
  for (int i = 0; i < nb_samples; i ++){
    adcValue += analogRead(ACS_PIN); // multisampling
  }
  adcValue = adcValue / nb_samples;
  uint32_t voltage = esp_adc_cal_raw_to_voltage(adcValue, &adc_chars);
  float currentACS  = (voltage/1000.0f - zeroCurrentVoltage) / sensitivity; // 45mV per Amp
  dataMsg.tot_current_a = currentACS;

  // read STS30 temperature data
  float temperature = readTemperature();
  dataMsg.temp = temperature;

  // read INA219 sensors datas
  // Assign labels (hardcoded or from a lookup table)
  const char *labels[NUM_SENS] = {"", "Jetson", "Lidar", "Router", "Lights", ""};
  for (uint8_t i = 0; i < NUM_SENS; i++) {
    float busV    = sensors[i]->getBusVoltage_V();
    float current = sensors[i]->getCurrent_mA();
    float power   = sensors[i]->getPower_mW();
    float shuntmV = sensors[i]->getShuntVoltage_mV();
    float loadV = busV + (shuntmV / 1000);
    dataMsg.ina_sensors[i].bus_v = (float) busV;
    dataMsg.ina_sensors[i].current_ma = (float) current;
    dataMsg.ina_sensors[i].power_mw = (float) power;
    dataMsg.ina_sensors[i].load_v = (float) loadV;

    // Set label
    rosidl_runtime_c__String__init(&dataMsg.ina_sensors[i].label);
    rosidl_runtime_c__String__assign(&dataMsg.ina_sensors[i].label, labels[i]);
  }
  safePublish(&dataPublisher, &dataMsg, "dataPublisher");
}


void microrosInit(){
  // in platformio.ini, set the board_microros_transport variable to wifi or serial depending on transport mode you want to use
  set_microros_wifi_transports(WIFI_SSID, WIFI_PASSWORD, agent_ip, agent_port); // microros over wifi
  //  set_microros_serial_transports(Serial); // microros over serial
  allocator = rcl_get_default_allocator();
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator)); //create init_options
  RCCHECK(rclc_node_init_default(&node, "power_board_esp32_node", "", &support));// create node

  // // init publishers
  RCCHECK(rclc_publisher_init_best_effort(&dataPublisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(power_board_msgs, msg, PowerDatas),"/power_board/state")); // create test publisher

  // init timer
  RCCHECK(rclc_timer_init_default(&publishTimer, &support, RCL_MS_TO_NS(1000/PUBLISH_DATA_FREQUENCY), publishData));
 
  // Initialize executor
  RCCHECK(rclc_executor_init(&ctrlCmdExecutor, &support.context, 2, &allocator));
  RCCHECK(rclc_executor_add_timer(&ctrlCmdExecutor, &publishTimer));
}

void microrosCleanup(){
  rcl_ret_t rc;
  rc = rclc_executor_fini(&ctrlCmdExecutor);
  rc = rcl_timer_fini(&publishTimer);
  rc = rcl_publisher_fini(&dataPublisher, &node);
  rc = rcl_node_fini(&node);
  rc = rclc_support_fini(&support);
}

void setup() {
  Serial.begin(115200);
  delay(100);
  // initialize ACS711 sensor
  pinMode(ACS_PIN, INPUT);
  analogReadResolution(12); // 12 bit resolution (0 - 4095)
  analogSetPinAttenuation(ACS_PIN, ADC_11db); // range 0-3.3V
  esp_adc_cal_characterize(ADC_UNIT_1, // We are using ADC unit 1 channel 4
                           ADC_ATTEN_DB_12, // ADC_ATTEN_DB_11 Behave the same as ADC_ATTEN_DB_12 but is deprecated, see adc_atten_t in adc_types.h
                           ADC_WIDTH_BIT_12, // 12 bit resolution (0 - 4095)
                           1100, // default Vref
                           &adc_chars); // eFuse Vref calibration
  
  microrosInit();
  delay(1000);

  // initialize all INA219 sensors
  for (uint8_t i = 0; i < NUM_SENS; i++) {
    if (!sensors[i]->begin()) {
      Serial.printf("No INA219 @ %s\n", labels[i]);
    } else {
      sensors[i]->setCalibration_32V_2A();
    }
  }
  Serial.println("INA219s ready.");

}

void loop() {
  rclc_executor_spin_some(&ctrlCmdExecutor, RCL_MS_TO_NS(100));
}

float readTemperature() {
  uint8_t cmd[2] = {0x24, 0x0B}; // Single-shot, no clock stretching, medium  repeatability
  Wire.beginTransmission(STS30_ADDR);
  Wire.write(cmd, 2);
  Wire.endTransmission();
  
  delay(7); // Wait for measurement to complete  

  Wire.requestFrom(STS30_ADDR, 0x03); // Request 3 bytes of data
  if (Wire.available() < 3) return NAN;

  uint8_t msb = Wire.read();
  uint8_t lsb = Wire.read();
  uint8_t crc = Wire.read(); // Ignore CRC byte

  uint16_t temp_raw = ((uint16_t)msb << 8) | lsb;

  // Convert raw value to temperature in Celsius
  float temperature = -45 + 175 * ((float)temp_raw / 65535.0);
  return temperature;
}