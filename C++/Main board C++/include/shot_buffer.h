#pragma once

#include <Arduino.h>

struct ShotVector {
    float azimuth = 0.0f;     // degrees, 0-360
    float inclination = 0.0f; // degrees, -90 to +90
    float distance = 0.0f;    // meters

    ShotVector() = default;
    ShotVector(float az, float inc, float dist) : azimuth(az), inclination(inc), distance(dist) {}
};

class ILegChecker {
  public:
    virtual bool hasValidLeg(const ShotVector* shots, uint8_t count) const = 0;
    virtual ~ILegChecker() = default;
};

class CartesianLegChecker : public ILegChecker {
  public:
    bool hasValidLeg(const ShotVector* shots, uint8_t count) const override {
        return false;
    }
};

// Ring buffer of up to 3 ShotVectors for leg-consistency checking.
class ShotBuffer {
  public:
    static constexpr uint8_t CAPACITY = 3;

    ShotBuffer(const ILegChecker& checker) : checker_(checker) {}

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

    bool hasValidLeg() const {
        if (count_ < CAPACITY) return false;
        return checker_.hasValidLeg(buf_, count_);
    }

  private:
    const ILegChecker& checker_;
    ShotVector buf_[CAPACITY];
    uint8_t count_ = 0;
};
