#pragma once

#include <math.h>

static constexpr float PI      = 3.14159265358979f;
static constexpr float DEG2RAD = PI / 180.0f;
static constexpr float RAD2DEG = 180.0f / PI;

inline float degreesToRadians(float deg) { return deg * DEG2RAD; }
inline float radiansToDegrees(float rad) { return rad * RAD2DEG; }

// Shortest angular distance between two bearings (unsigned), in [0, 180].
inline float circularDiff(float a, float b) { return fabsf(fmodf(a - b + 180.0f, 360.0f) - 180.0f); }
