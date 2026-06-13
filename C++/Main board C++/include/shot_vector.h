#pragma once

struct ShotVector {
    float azimuth = 0.0f;     // degrees, 0-360
    float inclination = 0.0f; // degrees, -90 to +90
    float distance = 0.0f;    // meters

    ShotVector() = default;
    ShotVector(float az, float inc, float dist) : azimuth(az), inclination(inc), distance(dist) {}
};
