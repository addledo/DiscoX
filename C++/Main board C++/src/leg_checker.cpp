#include "leg_checker.h"

#include "config.h"
#include <Arduino.h>

namespace {
constexpr float DEG2RAD = 3.14159265358979f / 180.0f;

float degreesToRadians(float degrees) { return degrees * DEG2RAD; }
} // namespace

CartesianCoordinate::CartesianCoordinate(float x, float y, float z) : x(x), y(y), z(z) {}

float CartesianCoordinate::distanceTo(const CartesianCoordinate &other) const {
    float dx = x - other.x;
    float dy = y - other.y;
    float dz = z - other.z;
    return sqrtf((dx * dx) + (dy * dy) + (dz * dz));
}

CartesianCoordinate CartesianCoordinate::fromShot(const ShotVector &shot) {
    float azimuth = degreesToRadians(shot.azimuth);
    float inclination = degreesToRadians(shot.inclination);
    float horiz = shot.distance * cosf(inclination);

    float x = horiz * sinf(azimuth);
    float y = horiz * cosf(azimuth);
    float z = shot.distance * sinf(inclination);

    return CartesianCoordinate{x, y, z};
}

CartesianLegChecker::CartesianLegChecker(float toleranceCm) : toleranceM_(toleranceCm / 100.0f) {}

void CartesianLegChecker::setTolerance(float toleranceCm) {
    if (!isValidTolerance(toleranceCm)) {
        return;
    }
    toleranceM_ = toleranceCm / 100.0f;
}

bool CartesianLegChecker::hasValidLeg(const ShotVector *shots, uint8_t count) const {
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

bool CartesianLegChecker::isValidTolerance(float toleranceCm) {
    return isfinite(toleranceCm) && toleranceCm >= Defaults::cartesianToleranceMin &&
           toleranceCm <= Defaults::cartesianToleranceMax;
}
