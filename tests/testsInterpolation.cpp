#include <gtest/gtest.h>
#include "../Lab_2/ArraySequence.h"
#include "../SpliteInterpolation.h"

// y = x
TEST(SplineTest, LinearTwoPoints) {
    MutableArraySequence<double> xs;
    MutableArraySequence<double> ys;

    xs.Append(0.0);
    xs.Append(1.0);

    ys.Append(0.0);
    ys.Append(1.0);

    auto spline = BuildNaturalCubicSpline<double>(xs, ys);

    EXPECT_NEAR(spline.Evaluate(0.0), 0.0, 1e-9);
    EXPECT_NEAR(spline.Evaluate(0.5), 0.5, 1e-9);
    EXPECT_NEAR(spline.Evaluate(1.0), 1.0, 1e-9);
}

// y = x^2
TEST(SplineTest, QuadraticThreePoints) {
    MutableArraySequence<double> xs;
    MutableArraySequence<double> ys;

    xs.Append(-1.0);
    xs.Append(0.0);
    xs.Append(1.0);

    ys.Append(1.0); // (-1)^2
    ys.Append(0.0); // 0^2
    ys.Append(1.0); // 1^2

    auto spline = BuildNaturalCubicSpline<double>(xs, ys);

    EXPECT_NEAR(spline.Evaluate(-1.0), 1.0, 1e-9);
    EXPECT_NEAR(spline.Evaluate(-0.5), 0.25, 0.1);
    EXPECT_NEAR(spline.Evaluate(0.0),  0.0, 1e-9);
    EXPECT_NEAR(spline.Evaluate(0.5),  0.25, 0.1);
    EXPECT_NEAR(spline.Evaluate(1.0),  1.0, 1e-9);
}

TEST(SplineTest, InvalidDataDifferentSizes) {
    MutableArraySequence<double> xs;
    MutableArraySequence<double> ys;

    xs.Append(0.0);
    xs.Append(1.0);

    ys.Append(0.0);

    EXPECT_THROW(
        BuildNaturalCubicSpline<double>(xs, ys),
        std::invalid_argument
    );
}

TEST(SplineTest, InvalidDataNonIncreasingXs) {
    MutableArraySequence<double> xs;
    MutableArraySequence<double> ys;

    xs.Append(0.0);
    xs.Append(0.0);

    ys.Append(0.0);
    ys.Append(1.0);

    EXPECT_THROW(
        BuildNaturalCubicSpline<double>(xs, ys),
        std::invalid_argument
    );
}