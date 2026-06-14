#include "leg_checker.h"
#include <unity.h>

void setUp() {}
void tearDown() {}

// ── CartesianCoordinate::fromShot ────────────────────────────────────

void test_fromShot_north() {
    // az=0°, inc=0°, 10m → (0, 10, 0)
    CartesianCoordinate c = CartesianCoordinate::fromShot({0.0f, 0.0f, 10.0f});
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 10.0f, c.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.z);
}

void test_fromShot_east() {
    // az=90°, inc=0°, 10m → (10, 0, 0)
    CartesianCoordinate c = CartesianCoordinate::fromShot({90.0f, 0.0f, 10.0f});
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 10.0f, c.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.z);
}

void test_fromShot_vertical_up() {
    // az=any, inc=90°, 10m → (0, 0, 10)
    CartesianCoordinate c = CartesianCoordinate::fromShot({0.0f, 90.0f, 10.0f});
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 10.0f, c.z);
}

// ── CartesianLegChecker ──────────────────────────────────────────────

void test_cartesian_null_false() {
    CartesianLegChecker checker(10.0f);
    TEST_ASSERT_FALSE(checker.hasValidLeg(nullptr, 1));
}

void test_cartesian_zero_count_false() {
    CartesianLegChecker checker(10.0f);
    Shot shots[] = {{0.0f, 0.0f, 5.0f}};
    TEST_ASSERT_FALSE(checker.hasValidLeg(shots, 0));
}

void test_cartesian_negative_count_false() {
    CartesianLegChecker checker(10.0f);
    Shot shots[] = {{0.0f, 0.0f, 5.0f}};
    TEST_ASSERT_FALSE(checker.hasValidLeg(shots, -1));
}

void test_cartesian_count_too_large_false() {
    CartesianLegChecker checker(10.0f);
    Shot shots[9] = {};
    TEST_ASSERT_FALSE(checker.hasValidLeg(shots, 9));
}

void test_cartesian_single_shot_true() {
    CartesianLegChecker checker(10.0f);
    Shot shots[] = {{45.0f, 30.0f, 5.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 1));
}

void test_cartesian_identical_shots_true() {
    CartesianLegChecker checker(10.0f);
    Shot shots[] = {{45.0f, 30.0f, 5.0f}, {45.0f, 30.0f, 5.0f}, {45.0f, 30.0f, 5.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 3));
}

void test_cartesian_within_tolerance_true() {
    // az=0° and az=1°, flat, 5m → endpoints ~8.7cm apart
    CartesianLegChecker checker(10.0f); // 10cm
    Shot shots[] = {{0.0f, 0.0f, 5.0f}, {1.0f, 0.0f, 5.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 2));
}

void test_cartesian_exceeds_tolerance_false() {
    // az=0° and az=1°, flat, 5m → ~8.7cm apart, fails at 5cm tolerance
    CartesianLegChecker checker(5.0f); // 5cm
    Shot shots[] = {{0.0f, 0.0f, 5.0f}, {1.0f, 0.0f, 5.0f}};
    TEST_ASSERT_FALSE(checker.hasValidLeg(shots, 2));
}

void test_cartesian_vertical_same_direction_true() {
    // Both straight up — azimuth irrelevant, endpoints identical
    CartesianLegChecker checker(10.0f);
    Shot shots[] = {{0.0f, 90.0f, 5.0f}, {180.0f, 90.0f, 5.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 2));
}

void test_cartesian_setTolerance_reduces_passing_to_failing() {
    CartesianLegChecker checker(10.0f);
    Shot shots[] = {{0.0f, 0.0f, 5.0f}, {1.0f, 0.0f, 5.0f}}; // ~8.7cm apart
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 2));
    checker.setTolerance(5.0f);
    TEST_ASSERT_FALSE(checker.hasValidLeg(shots, 2));
}

void test_cartesian_setTolerance_invalid_ignored() {
    // 0.0f is below the 1cm minimum — old tolerance kept
    CartesianLegChecker checker(10.0f);
    checker.setTolerance(0.0f);
    Shot shots[] = {{0.0f, 0.0f, 5.0f}, {1.0f, 0.0f, 5.0f}}; // ~8.7cm apart
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 2));
}

// ── AngularLegChecker ────────────────────────────────────────────────

void test_angular_null_false() {
    AngularLegChecker checker(1.7f);
    TEST_ASSERT_FALSE(checker.hasValidLeg(nullptr, 1));
}

void test_angular_zero_count_false() {
    AngularLegChecker checker(1.7f);
    Shot shots[] = {{0.0f, 0.0f, 5.0f}};
    TEST_ASSERT_FALSE(checker.hasValidLeg(shots, 0));
}

void test_angular_negative_count_false() {
    AngularLegChecker checker(1.7f);
    Shot shots[] = {{0.0f, 0.0f, 5.0f}};
    TEST_ASSERT_FALSE(checker.hasValidLeg(shots, -1));
}

void test_angular_count_too_large_false() {
    AngularLegChecker checker(1.7f);
    Shot shots[9] = {};
    TEST_ASSERT_FALSE(checker.hasValidLeg(shots, 9));
}

void test_angular_single_shot_true() {
    AngularLegChecker checker(1.7f);
    Shot shots[] = {{45.0f, 30.0f, 5.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 1));
}

void test_angular_identical_shots_true() {
    AngularLegChecker checker(1.7f);
    Shot shots[] = {{45.0f, 30.0f, 5.0f}, {45.0f, 30.0f, 5.0f}, {45.0f, 30.0f, 5.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 3));
}

void test_angular_within_tolerance_true() {
    // 1° apart, flat → angle ≈ 1° < 1.7°
    AngularLegChecker checker(1.7f);
    Shot shots[] = {{0.0f, 0.0f, 5.0f}, {1.0f, 0.0f, 5.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 2));
}

void test_angular_exceeds_tolerance_false() {
    // 2° apart, flat → angle ≈ 2° > 1.7°
    AngularLegChecker checker(1.7f);
    Shot shots[] = {{0.0f, 0.0f, 5.0f}, {2.0f, 0.0f, 5.0f}};
    TEST_ASSERT_FALSE(checker.hasValidLeg(shots, 2));
}

void test_angular_vertical_same_direction_true() {
    // Both pointing straight up — dot product = 1 regardless of azimuth
    AngularLegChecker checker(1.7f);
    Shot shots[] = {{0.0f, 90.0f, 5.0f}, {270.0f, 90.0f, 5.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 2));
}

void test_angular_wraps_around_zero() {
    // 1° and 359° are 2° apart, not 358° — passes with 3° tolerance
    AngularLegChecker checker(3.0f);
    Shot shots[] = {{1.0f, 0.0f, 5.0f}, {359.0f, 0.0f, 5.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 2));
}

void test_cartesian_near_vertical_wraps_around_zero() {
    // inc=89° (near-vertical, not exactly vertical): az=4° and az=356° are 8° apart
    // as scalars but the endpoints are only ~2.4cm apart. Cartesian handles this correctly.
    CartesianLegChecker checker(5.0f);
    Shot shots[] = {{4.0f, 89.0f, 10.0f}, {356.0f, 89.0f, 10.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 2));
}

void test_angular_near_vertical_wraps_around_zero() {
    // Same near-vertical case: dot-product gives ~0.08° separation, well within 1.7°.
    // A naive azimuth comparison would give 352° and incorrectly reject these.
    AngularLegChecker checker(1.7f);
    Shot shots[] = {{4.0f, 89.0f, 10.0f}, {356.0f, 89.0f, 10.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 2));
}

void test_cartesian_wraps_around_zero() {
    // 1° and 359° at 5m → endpoints ~17.5cm apart, passes with 20cm tolerance
    CartesianLegChecker checker(20.0f);
    Shot shots[] = {{1.0f, 0.0f, 5.0f}, {359.0f, 0.0f, 5.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 2));
}

// ── Diagonal shots (az and inc both non-zero) ────────────────────────

void test_angular_diagonal_within_tolerance_true() {
    // 1° az + 0.5° inc → ~0.96° combined angular separation, within 1.7°
    AngularLegChecker checker(1.7f);
    Shot shots[] = {{45.0f, 20.0f, 5.0f}, {46.0f, 20.5f, 5.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 2));
}

void test_angular_diagonal_exceeds_tolerance_false() {
    // 1.5° in az + 1.5° in inc → ~2° combined angular separation, exceeds 1.7°
    // (Note: at inc=20°, azimuth differences are NOT compressed much by cos(inc),
    //  so the combined separation here is genuinely > 1.7°)
    AngularLegChecker checker(1.7f);
    Shot shots[] = {{45.0f, 20.0f, 5.0f}, {46.5f, 21.5f, 5.0f}};
    TEST_ASSERT_FALSE(checker.hasValidLeg(shots, 2));
}

void test_cartesian_diagonal_within_tolerance_true() {
    // Same shots as angular within test: endpoints ~9.3cm apart, passes at 10cm
    CartesianLegChecker checker(10.0f);
    Shot shots[] = {{45.0f, 20.0f, 5.0f}, {46.0f, 20.5f, 5.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 2));
}

void test_cartesian_diagonal_exceeds_tolerance_false() {
    // Same shots as angular exceeds test: endpoints ~17.9cm apart, fails at 10cm
    CartesianLegChecker checker(10.0f);
    Shot shots[] = {{45.0f, 20.0f, 5.0f}, {46.5f, 21.5f, 5.0f}};
    TEST_ASSERT_FALSE(checker.hasValidLeg(shots, 2));
}

// ── Long distances ───────────────────────────────────────────────────

void test_cartesian_long_distance_within_tolerance_true() {
    // 0.05° apart at 100m → ~8.7cm endpoint separation, passes at 10cm
    CartesianLegChecker checker(10.0f);
    Shot shots[] = {{0.0f, 0.0f, 100.0f}, {0.05f, 0.0f, 100.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots, 2));
}

void test_cartesian_long_distance_exceeds_tolerance_false() {
    // 0.1° at 100m → ~17.5cm, fails at 10cm; same 0.1° at 5m → ~0.87cm, passes
    CartesianLegChecker checker(10.0f);
    Shot shots_long[]  = {{0.0f, 0.0f, 100.0f}, {0.1f, 0.0f, 100.0f}};
    Shot shots_short[] = {{0.0f, 0.0f, 5.0f},   {0.1f, 0.0f, 5.0f}};
    TEST_ASSERT_FALSE(checker.hasValidLeg(shots_long, 2));
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots_short, 2));
}

void test_angular_long_distance_same_result() {
    // Angular ignores distance — same angle passes regardless of leg length
    AngularLegChecker checker(1.7f);
    Shot shots_long[]  = {{0.0f, 0.0f, 100.0f}, {1.0f, 0.0f, 100.0f}};
    Shot shots_short[] = {{0.0f, 0.0f, 1.0f},   {1.0f, 0.0f, 1.0f}};
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots_long, 2));
    TEST_ASSERT_TRUE(checker.hasValidLeg(shots_short, 2));
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_fromShot_north);
    RUN_TEST(test_fromShot_east);
    RUN_TEST(test_fromShot_vertical_up);

    RUN_TEST(test_cartesian_null_false);
    RUN_TEST(test_cartesian_zero_count_false);
    RUN_TEST(test_cartesian_negative_count_false);
    RUN_TEST(test_cartesian_count_too_large_false);
    RUN_TEST(test_cartesian_single_shot_true);
    RUN_TEST(test_cartesian_identical_shots_true);
    RUN_TEST(test_cartesian_within_tolerance_true);
    RUN_TEST(test_cartesian_exceeds_tolerance_false);
    RUN_TEST(test_cartesian_vertical_same_direction_true);
    RUN_TEST(test_cartesian_setTolerance_reduces_passing_to_failing);
    RUN_TEST(test_cartesian_setTolerance_invalid_ignored);

    RUN_TEST(test_angular_null_false);
    RUN_TEST(test_angular_zero_count_false);
    RUN_TEST(test_angular_negative_count_false);
    RUN_TEST(test_angular_count_too_large_false);
    RUN_TEST(test_angular_single_shot_true);
    RUN_TEST(test_angular_identical_shots_true);
    RUN_TEST(test_angular_within_tolerance_true);
    RUN_TEST(test_angular_exceeds_tolerance_false);
    RUN_TEST(test_angular_vertical_same_direction_true);
    RUN_TEST(test_cartesian_near_vertical_wraps_around_zero);
    RUN_TEST(test_angular_near_vertical_wraps_around_zero);
    RUN_TEST(test_angular_wraps_around_zero);
    RUN_TEST(test_cartesian_wraps_around_zero);

    RUN_TEST(test_angular_diagonal_within_tolerance_true);
    RUN_TEST(test_angular_diagonal_exceeds_tolerance_false);
    RUN_TEST(test_cartesian_diagonal_within_tolerance_true);
    RUN_TEST(test_cartesian_diagonal_exceeds_tolerance_false);

    RUN_TEST(test_cartesian_long_distance_within_tolerance_true);
    RUN_TEST(test_cartesian_long_distance_exceeds_tolerance_false);
    RUN_TEST(test_angular_long_distance_same_result);

    return UNITY_END();
}
