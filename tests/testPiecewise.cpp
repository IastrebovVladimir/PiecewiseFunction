#include <gtest/gtest.h>
#include "../Lab_2/types.h"
#include "../Lab_2/ArraySequence.h"
#include "../PolynomialFunction.h"
#include "../RationalFunction.h"
#include "../Segment.h"
#include "../PiecewiseFunction.h"
#include "../Complex.h"

TEST(ComplexTest, DivisionByZeroThrows) {
    Complex a(1, 2);
    Complex b(0, 0);
    EXPECT_THROW(a / b, std::runtime_error);
}

TEST(PolynomialTest, EmptyCoefficientsThrows) {
    MutableArraySequence<int> coeffs;
    EXPECT_THROW(PolynomialFunction<int> p(coeffs), std::invalid_argument);
}

TEST(PolynomialTest, EvaluateLinearPolynomial) {
    MutableArraySequence<int> coeffs;
    coeffs.Append(1);
    coeffs.Append(2); // f(x) = 1 + 2x

    PolynomialFunction<int> p(coeffs);
    EXPECT_EQ(p.Evaluate(0.0), 1);
    EXPECT_EQ(p.Evaluate(2.0), 5);
    EXPECT_EQ(p.Evaluate(-1.0), -1);
}

TEST(PolynomialTest, DerivativeWorks) {
    MutableArraySequence<int> coeffs;
    coeffs.Append(1);
    coeffs.Append(2);
    coeffs.Append(3); // f(x) = 1 + 2x + 3x^2

    PolynomialFunction<int> p(coeffs);
    Function<int>* d = p.Derivative();

    EXPECT_EQ(d->Evaluate(0.0), 2);
    EXPECT_EQ(d->Evaluate(1.0), 8);

    delete d;
}

TEST(SegmentTest, ContainsBorders) {
    MutableArraySequence<int> coeffs;
    coeffs.Append(1);
    coeffs.Append(2);

    auto* f = new PolynomialFunction<int>(coeffs);
    Segment<int> s(-2.0, 0.0, f);

    EXPECT_TRUE(s.Contains(-2.0));
    EXPECT_TRUE(s.Contains(0.0));
    EXPECT_FALSE(s.Contains(1.0));

    delete f;
}

TEST(SegmentTest, InvalidBordersThrow) {
    MutableArraySequence<int> coeffs;
    coeffs.Append(1);

    auto* f = new PolynomialFunction<int>(coeffs);
    EXPECT_THROW(
            Segment<int> s(2.0, -1.0, f),
            std::invalid_argument
    );

    delete f;
}

TEST(SegmentTest, NullptrFunctionThrows) {
    EXPECT_THROW(
            Segment<int> s(0.0, 1.0, nullptr),
            std::invalid_argument
    );
}

TEST(SegmentTest, EvaluateWorks) {
    MutableArraySequence<int> coeffs;
    coeffs.Append(3);
    coeffs.Append(2); // f(x) = 3 + 2x

    auto* f = new PolynomialFunction<int>(coeffs);
    Segment<int> s(0.0, 2.0, f);

    EXPECT_EQ(s(0.0), 3);
    EXPECT_EQ(s(1.0), 5);
    EXPECT_EQ(s(2.0), 7);

    delete f;
}

TEST(SegmentTest, IncreasingMonotonicity) {
    MutableArraySequence<int> coeffs;
    coeffs.Append(1);
    coeffs.Append(2); // f(x) = 1 + 2x

    auto* f = new PolynomialFunction<int>(coeffs);
    Segment<int> s(0.0, 5.0, f);
    EXPECT_EQ(s.GetMonotonicity(), 2); // возрастает
}

TEST(SegmentTest, DecreasingMonotonicity) {
    MutableArraySequence<int> coeffs;
    coeffs.Append(5);
    coeffs.Append(-3); // f(x) = 5 - 3x

    auto* f = new PolynomialFunction<int>(coeffs);
    Segment<int> s(0.0, 5.0, f);
    EXPECT_EQ(s.GetMonotonicity(), 1); // убывает
    delete f;
}

TEST(PiecewiseTest, EvaluateInsideSegments) {
    MutableArraySequence<int> c1;
    c1.Append(1); // f1(x) = 1

    MutableArraySequence<int> c2;
    c2.Append(2); // f2(x) = 2

    MutableArraySequence<Segment<int>> segments;
    auto* f1 = new PolynomialFunction<int>(c1);
    auto* f2 = new PolynomialFunction<int>(c2);
    segments.Append(Segment<int>(0.0, 1.0, f1)); // [0,1]
    segments.Append(Segment<int>(1.0, 2.0, f2)); // [1,2]

    PiecewiseFunction<int> pw(segments);

    EXPECT_EQ(pw.Evaluate(0.5), 1);
    EXPECT_EQ(pw.Evaluate(1.5), 2);

    delete f1;
    delete f2;
}

TEST(PiecewiseTest, EvaluateExtrapolationWorks) {
    MutableArraySequence<int> c1;
    c1.Append(1); // f1(x) = 1

    MutableArraySequence<int> c2;
    c2.Append(2); // f2(x) = 2

    MutableArraySequence<Segment<int>> segments;
    auto* f1 = new PolynomialFunction<int>(c1);
    auto* f2 = new PolynomialFunction<int>(c2);
    segments.Append(Segment<int>(0.0, 1.0, f1));
    segments.Append(Segment<int>(1.0, 2.0, f2));

    PiecewiseFunction<int> pw(segments);

    EXPECT_EQ(pw.EvaluateExtrapolation(-1.0), 1); // вычисление f1 за пределами области
    EXPECT_EQ(pw.EvaluateExtrapolation(3.0), 2);  // вычисление f2 за пределами области

    delete f1;
    delete f2;
}

TEST(PiecewiseTest, IncreasingMonotonicity) {
    MutableArraySequence<int> c1;
    c1.Append(0);
    c1.Append(1); // f(x) = x

    MutableArraySequence<int> c2;
    c2.Append(1);
    c2.Append(1); // g(x) = 1 + x

    MutableArraySequence<Segment<int>> segments;
    auto* f1 = new PolynomialFunction<int>(c1);
    auto* f2 = new PolynomialFunction<int>(c2);
    segments.Append(Segment<int>(0.0, 1.0, f1));
    segments.Append(Segment<int>(1.0, 2.0, f2));

    PiecewiseFunction<int> pw(segments);
    EXPECT_EQ(pw.CheckMonotonicity(), 2);

    delete f1;
    delete f2;
}

TEST(PiecewiseTest, RedefineMiddleSegment) {
    MutableArraySequence<int> c1;
    c1.Append(1); // f(x) = 1 на [0,3]

    MutableArraySequence<int> c2;
    c2.Append(2); // g(x) = 2 на [1,2]

    MutableArraySequence<Segment<int>> segments;
    auto* f1 = new PolynomialFunction<int>(c1);
    segments.Append(Segment<int>(0.0, 3.0, f1));

    PiecewiseFunction<int> pw(segments);
    PolynomialFunction<int> replacement(c2);

    pw.Redefine(1.0, 2.0, &replacement);

    EXPECT_EQ(pw.GetLength(), 3);
    EXPECT_EQ(pw.Evaluate(0.5), 1);
    EXPECT_EQ(pw.Evaluate(1.5), 2);
    EXPECT_EQ(pw.Evaluate(2.5), 1);

    delete f1;
}

TEST(RationalFunctionTest, EvaluateWorks) {
    MutableArraySequence<int> num;
    num.Append(1);
    num.Append(1); // числитель = 1 + x

    MutableArraySequence<int> den;
    den.Append(1); // знаменатель = 1

    PolynomialFunction<int> numerator(num);
    PolynomialFunction<int> denominator(den);

    RationalFunction<int> f(numerator, denominator); // f(x) = (1 + x) / 1

    EXPECT_EQ(f.Evaluate(0.0), 1);
    EXPECT_EQ(f.Evaluate(2.0), 3);
}

TEST(RationalFunctionTest, DivisionByZeroThrows) {
    MutableArraySequence<int> num;
    num.Append(1); // числитель = 1

    MutableArraySequence<int> den;
    den.Append(0); // знаменатель = 0

    PolynomialFunction<int> numerator(num);
    PolynomialFunction<int> denominator(den);

    RationalFunction<int> f(numerator, denominator);

    EXPECT_THROW(f.Evaluate(1.0), std::domain_error);
}

TEST(PolynomialComplexTest, RealArgumentComplexCoefficients) {
    MutableArraySequence<Complex> coeffs;
    coeffs.Append(Complex(1, 1)); // c0 = 1 + i
    coeffs.Append(Complex(2, 0)); // c1 = 2x

    PolynomialFunction<Complex> p(coeffs); // f(x) = (1 + i) + 2x
    Complex value = p.Evaluate(2.0); // f(2) = (1 + i) + 2*2 = 5 + i

    EXPECT_EQ(value, Complex(5, 1));
}
