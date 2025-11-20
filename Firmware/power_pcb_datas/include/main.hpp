#include <Wire.h>
#include <Adafruit_INA219.h>
// Using STS30 temperature sensor with Adafruit SHT31 library
// STS30 is compatible with SHT3x commands, but provides temperature only
// Humidity values returned will be invalid or unused
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <Arduino.h>
#include "rosidl_runtime_c/string.h"
#include "rosidl_runtime_c/string_functions.h"
#include "esp_adc_cal.h"

#include "wifi_secrets.h"

#include <std_msgs/msg/string.h>


#define PUBLISH_DATA_FREQUENCY 1 // Hz

/*ROS2 Constants*/
#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){errorLoop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

void errorLoop();
bool safePublish(rcl_publisher_t* publisher, void* msg, const char* publisher_name);
void publishData(rcl_timer_t * timer, int64_t last_call_time);
void microrosInit();
void microrosCleanup();
float readTemperature();