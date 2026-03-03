#include "bma423.h"

void BMA423Component::setup() {
  soft_reset();
  uint8_t chip_id;
  if (!readRegister(BMA423_CHIP_ID, &chip_id, 1) || chip_id != 0x42) {
    ESP_LOGE("bma423", "Error inicializando BMA423");
  } else {
    ESP_LOGI("bma423", "BMA423 listo");
  }
}

void BMA423Component::update() {
  uint8_t data[6];
  if (readRegister(BMA423_ACC_X_LSB, data, 6)) {
    int16_t x = (int16_t)((data[1] << 8) | data[0]);
    int16_t y = (int16_t)((data[3] << 8) | data[2]);
    int16_t z = (int16_t)((data[5] << 8) | data[4]);
    accel_x_sensor->publish_state(x);
    accel_y_sensor->publish_state(y);
    accel_z_sensor->publish_state(z);
  }

  uint8_t step_data[2];
  if (readRegister(BMA423_STEP_CNT_LSB, step_data, 2)) {
    uint16_t steps = (uint16_t)((step_data[1] << 8) | step_data[0]);
    step_counter_sensor->publish_state(steps);
  }
}

void BMA423Component::soft_reset() {
  writeRegister(BMA423_COMMAND_REG, BMA423_SOFT_RESET);
  delay(50);
}

void BMA423Component::reset_step_counter() {
  writeRegister(BMA423_COMMAND_REG, BMA423_STEP_RESET);
}

bool BMA423Component::writeRegister(uint8_t reg, uint8_t value) {
  return i2c_->write(reg, &value, 1);
}

bool BMA423Component::readRegister(uint8_t reg, uint8_t *data, uint8_t len) {
  if (!i2c_->write(reg)) return false;
  return i2c_->read(data, len);
}