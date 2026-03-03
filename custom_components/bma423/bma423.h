#pragma once
#include "esphome.h"

#define BMA423_I2C_ADDR_PRIMARY   0x18
#define BMA423_I2C_ADDR_SECONDARY 0x19
#define BMA423_CHIP_ID 0x00
#define BMA423_ACC_X_LSB 0x04
#define BMA423_STEP_CNT_LSB 0x1E
#define BMA423_COMMAND_REG 0x7E
#define BMA423_SOFT_RESET 0xB6
#define BMA423_STEP_RESET 0xB0

class BMA423Component : public PollingComponent {
 public:
  BMA423Component() : PollingComponent(500) {}

  void setup() override;
  void update() override;
  void set_i2c(I2CComponent *i2c) { i2c_ = i2c; }

  Sensor *accel_x_sensor = new Sensor();
  Sensor *accel_y_sensor = new Sensor();
  Sensor *accel_z_sensor = new Sensor();
  Sensor *step_counter_sensor = new Sensor();

  void reset_step_counter();

 private:
  I2CComponent *i2c_;
  bool writeRegister(uint8_t reg, uint8_t value);
  bool readRegister(uint8_t reg, uint8_t *data, uint8_t len);
  void soft_reset();
};