#pragma once

static constexpr float PI      = 3.14159265358979f;
static constexpr float DEG2RAD = PI / 180.0f;
static constexpr float RAD2DEG = 180.0f / PI;

inline float degreesToRadians(float deg) { return deg * DEG2RAD; }
inline float radiansToDegrees(float rad) { return rad * RAD2DEG; }
