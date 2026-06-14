#pragma once

#include "config.h"
#include "shot_vector.h"
#include <Arduino.h>
#include <cstdint>

float degreesToRadians(float);

class ILegChecker {
  public:
    virtual bool hasValidLeg(const ShotVector *shots, uint8_t count) const = 0;
    virtual ~ILegChecker() = default;
};

struct CartesianCoordinate {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    float distanceTo(const CartesianCoordinate &other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return sqrtf((dx * dx) + (dy * dy) + (dz * dz));
    }

    static CartesianCoordinate fromShot(ShotVector shot) {
        float azimuth = degreesToRadians(shot.azimuth);
        float inclination = degreesToRadians(shot.inclination);
        float horiz = shot.distance * cosf(inclination);

        float x = horiz * sinf(azimuth);
        float y = horiz * cosf(azimuth);
        float z = shot.distance * sinf(inclination);

        return CartesianCoordinate{x, y, z};
    }
};

// Converts each shot to a Cartesian endpoint and checks that each endpoint is within tolerance of each other
// endpoint. Handles steep legs correctly (unlike angular comparison, which breaks down near vertical where
// azimuth becomes meaningless).
class CartesianLegChecker : public ILegChecker {
  public:
    CartesianLegChecker(float toleranceCm) : toleranceM_(toleranceCm / 100.0f) {}

    void setTolerance(float toleranceCm) {
        if (!isValidTolerance(toleranceCm)) {
            return;
        }
        toleranceM_ = toleranceCm / 100.0f;
    }

    // Checks that each point is within the tolerated distance of each other point.
    bool hasValidLeg(const ShotVector *shots, uint8_t count) const override {
        if (!shots) {
            return false;
        }

        // Number of shots should always be 3.
        if (count != 3) {
            return false;
        }

        CartesianCoordinate endpoints[3];

        // Convert to co-ordinates
        for (int i = 0; i < 3; i++) {
            endpoints[i] = CartesianCoordinate::fromShot(shots[i]);
        }

        // Check tolerances
        for (int i = 0; i < 3; i++) {
            for (int j = i + 1; j < 3; j++) {
                CartesianCoordinate a = endpoints[i];
                CartesianCoordinate b = endpoints[j];

                if (a.distanceTo(b) > toleranceM_) {
                    return false;
                }
            }
        }
        return true;
    }

  private:
    float toleranceM_;

    static bool isValidTolerance(float toleranceCm) {
        return isfinite(toleranceCm) && toleranceCm >= Defaults::cartesianToleranceMin &&
               toleranceCm <= Defaults::cartesianToleranceMax;
    }

    static float degreesToRadians(float degrees) { return degrees * (3.14159265358979f / 180.0f); }
};
