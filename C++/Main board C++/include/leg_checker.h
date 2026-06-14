#pragma once

#include "shot_vector.h"
#include <Arduino.h>

class ILegChecker {
  public:
    virtual bool hasValidLeg(const Shot *shots, uint8_t count) const = 0;
    virtual ~ILegChecker() = default;
};

struct CartesianCoordinate {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    CartesianCoordinate() = default;
    CartesianCoordinate(float x, float y, float z);

    float distanceTo(const CartesianCoordinate &other) const;

    static CartesianCoordinate fromShot(const Shot &shot);
};

// Converts each shot to a Cartesian endpoint and checks that each endpoint is within tolerance of each other
// endpoint. Handles steep legs correctly (unlike angular comparison, which breaks down near vertical where
// azimuth becomes meaningless).
class CartesianLegChecker : public ILegChecker {
  public:
    CartesianLegChecker(float toleranceCm);

    void setTolerance(float toleranceCm);

    // Checks that each point is within the tolerated distance of each other point.
    bool hasValidLeg(const Shot *shots, uint8_t count) const override;

  private:
    float toleranceM_;

    static bool isValidTolerance(float toleranceCm);
};
