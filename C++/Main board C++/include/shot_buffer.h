#pragma once

#include <Arduino.h>

struct ShotVector {
    float azimuth = 0.0f;     // degrees, 0-360
    float inclination = 0.0f; // degrees, -90 to +90
    float distance = 0.0f;    // meters

    ShotVector() = default;
    ShotVector(float az, float inc, float dist) : azimuth(az), inclination(inc), distance(dist) {}
};

// Ring buffer of up to 3 ShotVectors for leg-consistency checking.
class ShotBuffer {
  public:
    static constexpr uint8_t CAPACITY = 3;

    void push(const ShotVector &s) {
        if (count_ < CAPACITY) {
            buf_[count_++] = s;
        } else {
            buf_[0] = buf_[1];
            buf_[1] = buf_[2];
            buf_[2] = s;
        }
    }

    void clear() { count_ = 0; }

    uint8_t count() const { return count_; }
    bool full() const { return count_ == CAPACITY; }

    const ShotVector &operator[](uint8_t i) const { return buf_[i]; }

    // True if all buffered shots agree within tolerances.
    // angleTol applies to both azimuth (circular) and inclination (linear).
    bool isConsistent(float angleTol, float distTol) const {
        if (count_ < CAPACITY) {
            return false;
        }
        for (uint8_t i = 0; i < count_; i++) {
            for (uint8_t j = i + 1; j < count_; j++) {
                if (circularDiff(buf_[i].azimuth, buf_[j].azimuth) > angleTol) {
                    return false;
                }
                if (fabsf(buf_[i].inclination - buf_[j].inclination) > angleTol) {
                    return false;
                }
                if (fabsf(buf_[i].distance - buf_[j].distance) > distTol) {
                    return false;
                }
            }
        }
        return true;
    }

  private:
    ShotVector buf_[CAPACITY];
    uint8_t count_ = 0;

    static float circularDiff(float a, float b) {
        float d = fmodf(a - b + 180.0f, 360.0f) - 180.0f;
        return fabsf(d);
    }
};
