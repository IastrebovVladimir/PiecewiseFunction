#include <gtest/gtest.h>
#include "../Lab_2/types.h"
#include "../Lab_2/ArraySequence.h"
#include "../Functions/PolynomialFunction.h"
#include "../Segment.h"
#include "../functions/PiecewiseFunction.h"

TEST(SeqmentTest, DerivativeWorks) {
    MutableArraySequence<int> coeffs;
    coeffs.Append(1);
    coeffs.Append(2);
    coeffs.Append(3); // f(x) = 1 + 2x + 3x^2

    PolynomialFunction<int> p(coeffs);
    auto d = p.Derivative();

    EXPECT_EQ(d->Evaluate(0.0), 2);
    EXPECT_EQ(d->Evaluate(1.0), 8);
}

TEST(SegmentTest, ContainsBorders) {
    MutableArraySequence<int> coeffs;
    coeffs.Append(1);
    coeffs.Append(2);

    PolynomialFunction<int> f(coeffs);
    Segment<int> s(-2.0, 0.0, f);

    EXPECT_TRUE(s.Contains(-2.0));
    EXPECT_TRUE(s.Contains(0.0));
    EXPECT_FALSE(s.Contains(1.0));
}

TEST(SegmentTest, InvalidBordersThrow) {
    MutableArraySequence<int> coeffs;
    coeffs.Append(1);

    PolynomialFunction<int> f(coeffs);
    EXPECT_THROW(
            Segment<int> s(2.0, -1.0, f),
            std::invalid_argument
    );
}

TEST(SegmentTest, EvaluateWorks) {
    MutableArraySequence<int> coeffs;
    coeffs.Append(3);
    coeffs.Append(2); // f(x) = 3 + 2x

    PolynomialFunction<int> f(coeffs);
    Segment<int> s(0.0, 2.0, f);

    EXPECT_EQ(s(0.0), 3);
    EXPECT_EQ(s(1.0), 5);
    EXPECT_EQ(s(2.0), 7);
}

TEST(SegmentTest, IncreasingMonotonicity) {
    MutableArraySequence<int> coeffs;
    coeffs.Append(1);
    coeffs.Append(2); // f(x) = 1 + 2x

    PolynomialFunction<int> f(coeffs);
    Segment<int> s(0.0, 5.0, f);
    EXPECT_EQ(s.GetMonotonicity(), 2);
}

TEST(SegmentTest, DecreasingMonotonicity) {
    MutableArraySequence<int> coeffs;
    coeffs.Append(5);
    coeffs.Append(-3); // f(x) = 5 - 3x

    PolynomialFunction<int> f(coeffs);
    Segment<int> s(0.0, 5.0, f);
    EXPECT_EQ(s.GetMonotonicity(), 1);
}

TEST(PiecewiseTest, EvaluateInsideSegments) {
    MutableArraySequence<int> c1;
    c1.Append(1); // f1(x) = 1

    MutableArraySequence<int> c2;
    c2.Append(2); // f2(x) = 2

    MutableArraySequence<Segment<int>> segments;
    PolynomialFunction<int> f1(c1);
    PolynomialFunction<int> f2(c2);

    segments.Append(Segment<int>(0.0, 1.0, f1)); // [0,1]
    segments.Append(Segment<int>(1.0, 2.0, f2)); // [1,2]

    PiecewiseFunction<int> pw(segments);

    EXPECT_EQ(pw.Evaluate(0.5), 1);
    EXPECT_EQ(pw.Evaluate(1.5), 2);
}

TEST(PiecewiseTest, EvaluateExtrapolationWorks) {
    MutableArraySequence<int> c1;
    c1.Append(1);

    MutableArraySequence<int> c2;
    c2.Append(2);

    MutableArraySequence<Segment<int>> segments;
    PolynomialFunction<int> f1(c1);
    PolynomialFunction<int> f2(c2);

    segments.Append(Segment<int>(0.0, 1.0, f1));
    segments.Append(Segment<int>(1.0, 2.0, f2));

    PiecewiseFunction<int> pw(segments);

    EXPECT_EQ(pw.EvaluateExtrapolation(-1.0), 1);
    EXPECT_EQ(pw.EvaluateExtrapolation(3.0), 2);
}

TEST(PiecewiseTest, IncreasingMonotonicity) {
    MutableArraySequence<int> c1;
    c1.Append(0);
    c1.Append(1); // f(x) = x

    MutableArraySequence<int> c2;
    c2.Append(1);
    c2.Append(1); // g(x) = 1 + x

    MutableArraySequence<Segment<int>> segments;
    PolynomialFunction<int> f1(c1);
    PolynomialFunction<int> f2(c2);

    segments.Append(Segment<int>(0.0, 1.0, f1));
    segments.Append(Segment<int>(1.0, 2.0, f2));

    PiecewiseFunction<int> pw(segments);
    EXPECT_EQ(pw.CheckMonotonicity(), 2);
}

TEST(PiecewiseTest, RedefineMiddleSegment) {
    MutableArraySequence<int> c1;
    c1.Append(1); // f(x) = 1 на [0,3]

    MutableArraySequence<int> c2;
    c2.Append(2); // g(x) = 2 на [1,2]

    MutableArraySequence<Segment<int>> segments;
    PolynomialFunction<int> f1(c1);
    segments.Append(Segment<int>(0.0, 3.0, f1));

    PiecewiseFunction<int> pw(segments);
    PolynomialFunction<int> replacement(c2);

    pw.Redefine(1.0, 2.0, replacement);
    
    EXPECT_EQ(pw.Evaluate(0.5), 1);
    EXPECT_EQ(pw.Evaluate(1.5), 2);
    EXPECT_EQ(pw.Evaluate(2.5), 1);
}

TEST(PiecewiseTest, DiscontinuousFunctionIsNotContinuous) {
    MutableArraySequence<int> c1;
    c1.Append(0);
    c1.Append(1); // x

    MutableArraySequence<int> c2;
    c2.Append(1); // константа 1

    MutableArraySequence<Segment<int>> segments;
    PolynomialFunction<int> f1(c1);
    PolynomialFunction<int> f2(c2);

    segments.Append(Segment<int>(0.0, 1.0, f1));
    segments.Append(Segment<int>(1.0, 2.0, f2));

    PiecewiseFunction<int> pw(segments);
    EXPECT_TRUE(pw.IsContinuous());
}

TEST(PiecewiseTest, EvaluateAtRightBorder) {
    MutableArraySequence<int> c1;
    c1.Append(1);

    MutableArraySequence<Segment<int>> segments;
    PolynomialFunction<int> f(c1);
    segments.Append(Segment<int>(0.0, 2.0, f));

    PiecewiseFunction<int> pw(segments);

    EXPECT_EQ(pw.Evaluate(2.0), 1);
}

TEST(PiecewiseTest, FunctionIsContinuous) {
    MutableArraySequence<int> c1;
    c1.Append(0);
    c1.Append(1); // x

    MutableArraySequence<int> c2;
    c2.Append(1);
    c2.Append(1); // 1 + x

    MutableArraySequence<Segment<int>> segments;
    PolynomialFunction<int> f1(c1);
    PolynomialFunction<int> f2(c2);
    
    segments.Append(Segment<int>(0.0, 1.0, f1));
    segments.Append(Segment<int>(1.0, 2.0, f1)); 

    PiecewiseFunction<int> pw(segments);
    EXPECT_TRUE(pw.IsContinuous());
}
