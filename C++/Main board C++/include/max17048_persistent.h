#pragma once
#include <Adafruit_MAX1704X.h>
#include <Wire.h>

// Subclass that skips reset() on begin() to preserve the MAX17048's
// ModelGauge learned state across MCU reboots.  The IC still PORs
// naturally on battery removal.
class MAX17048_Persistent : public Adafruit_MAX17048 {
  public:
    inline bool begin(TwoWire *wire = &Wire) {
        if (i2c_dev) {
            delete i2c_dev;
            delete status_reg;
        }
        i2c_dev = new Adafruit_I2CDevice(MAX17048_I2CADDR_DEFAULT, wire);
        if (!i2c_dev->begin()) {
            return false;
        }
        if (!isDeviceReady()) {
            return false;
        }
        status_reg = new Adafruit_BusIO_Register(i2c_dev, MAX1704X_STATUS_REG);
        enableSleep(false);
        sleep(false);
        wake();
        return true;
    }
};
