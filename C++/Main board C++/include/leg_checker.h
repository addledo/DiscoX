#pragma once

#include "shot_vector.h"
#include <Arduino.h>

class ILegChecker {
  public:
    virtual bool hasValidLeg(const ShotVector *shots, uint8_t count) const = 0;
    virtual ~ILegChecker() = default;
};

class CartesianLegChecker : public ILegChecker {
  public:
    bool hasValidLeg(const ShotVector *shots, uint8_t count) const override { return false; }
};
