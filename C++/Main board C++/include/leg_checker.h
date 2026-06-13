#pragma once

#include "shot_vector.h"
#include <Arduino.h>

class ILegChecker {
  public:
    virtual bool hasValidLeg(const ShotVector *shots, uint8_t count) const = 0;
    virtual ~ILegChecker() = default;
};

// Converts each shot to a Cartesian endpoint and checks that every pair of endpoints
// lies within tolerance metres of each other. Handles steep legs correctly (unlike
// angular comparison, which breaks down near vertical where azimuth becomes meaningless).
class CartesianLegChecker : public ILegChecker {
  public:
    CartesianLegChecker(float toleranceCm = 10.0f) : toleranceCm_(toleranceCm) {}

    void setTolerance(float toleranceCm) { toleranceCm_ = toleranceCm; }

    bool hasValidLeg(const ShotVector *shots, uint8_t count) const override {
        float toleranceM = toleranceCm_ / 100.0f;
        float x[3], y[3], z[3];
        for (uint8_t i = 0; i < count; i++) {
            float azRad = shots[i].azimuth * DEG2RAD;
            float incRad = shots[i].inclination * DEG2RAD;
            float horiz = shots[i].distance * cosf(incRad);
            x[i] = horiz * sinf(azRad);
            y[i] = horiz * cosf(azRad);
            z[i] = shots[i].distance * sinf(incRad);
        }
        for (uint8_t i = 0; i < count; i++) {
            for (uint8_t j = i + 1; j < count; j++) {
                float dx = x[i] - x[j];
                float dy = y[i] - y[j];
                float dz = z[i] - z[j];
                if (sqrtf(dx * dx + dy * dy + dz * dz) > toleranceM) {
                    return false;
                }
            }
        }
        return true;
    }

  private:
    float toleranceCm_;
    static constexpr float DEG2RAD = 3.14159265358979f / 180.0f;
};
