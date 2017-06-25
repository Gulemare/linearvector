#include "gtest/gtest.h"
#include "linearvector.h"
#include <complex>
//--------------------------------------------------------------------
TEST(Constructors, defaultZeros) {
    LinearVector<3> v;
    EXPECT_EQ(v[0], 0.0);
    EXPECT_EQ(v[1], 0.0);
    EXPECT_EQ(v[2], 0.0);
}

TEST(Constructors, initList) {
    LinearVector<3, int> v = { 10, 10, 10 };
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 10);
    EXPECT_EQ(v[2], 10);
}

TEST(Constructors, copy) {
    LinearVector<3,int> dummy = { 999, 999, 999 };
    LinearVector<3,int> v = dummy;
    EXPECT_EQ(v[0], 999);
    EXPECT_EQ(v[1], 999);
    EXPECT_EQ(v[2], 999);
}
//--------------------------------------------------------------------
TEST(Access, get) {
    LinearVector<3,int> v = { 5, 5, 5 };
    EXPECT_EQ(v[0], 5);
}

TEST(Access, set) {
    LinearVector<2,int> v;
    v[0] = 10;
    v[1] = 11;
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 11);
}
//--------------------------------------------------------------------
class VectorWithVectorTest : public ::testing::Test {
protected:
    VectorWithVectorTest() : 
        a({  1,  2,  3 }),
        b({ 10, 10, 10 }) 
    {}
    LinearVector<3,int> a;
    LinearVector<3,int> b;
};

TEST_F(VectorWithVectorTest, unarySum) {
    a += b;
    EXPECT_EQ(a[0], 11);
    EXPECT_EQ(a[1], 12);
    EXPECT_EQ(a[2], 13);
}

TEST_F(VectorWithVectorTest, unaryDifference) {
    a -= b;
    EXPECT_EQ(a[0], -9);
    EXPECT_EQ(a[1], -8);
    EXPECT_EQ(a[2], -7);
}

TEST_F(VectorWithVectorTest, equal) {
    a = b;
    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(a[1], 10);
    EXPECT_EQ(a[2], 10);
}

TEST_F(VectorWithVectorTest, sum) {
    LinearVector<3, int> res = a + b;
    EXPECT_EQ(res[0], 11);
    EXPECT_EQ(res[1], 12);
    EXPECT_EQ(res[2], 13);
}

TEST_F(VectorWithVectorTest, difference) {
    LinearVector<3, int> res = a - b;
    EXPECT_EQ(res[0], -9);
    EXPECT_EQ(res[1], -8);
    EXPECT_EQ(res[2], -7);
}
//--------------------------------------------------------------------
class VectorWithNumberTest : public ::testing::Test {
protected:
    VectorWithNumberTest() :
        v({ 1.0,  2.0,  3.0 }),
        num(2.0)
    {}
    LinearVector<3> v; //double values by default
    double num;
};

TEST_F(VectorWithNumberTest, unarySum) {
    v += static_cast<double>(num);
    EXPECT_EQ(v[0], 3.0);
    EXPECT_EQ(v[1], 4.0);
    EXPECT_EQ(v[2], 5.0);
}

TEST_F(VectorWithNumberTest, unaryDifference) {
    v -= num;
    EXPECT_EQ(v[0], -1.0);
    EXPECT_EQ(v[1], 0.0);
    EXPECT_EQ(v[2], 1.0);
}

TEST_F(VectorWithNumberTest, unaryMultiply) {
    v *= num;
    EXPECT_EQ(v[0], 2.0);
    EXPECT_EQ(v[1], 4.0);
    EXPECT_EQ(v[2], 6.0);
}

TEST_F(VectorWithNumberTest, unaryDivision) {
    v /= num;
    EXPECT_EQ(v[0], 0.5);
    EXPECT_EQ(v[1], 1);
    EXPECT_EQ(v[2], 1.5);
}

TEST_F(VectorWithNumberTest, sum) {
    auto res = v + num;
    EXPECT_EQ(res[0], 3.0);
    EXPECT_EQ(res[1], 4.0);
    EXPECT_EQ(res[2], 5.0);

    res = num + v;
    EXPECT_EQ(res[0], 3.0);
    EXPECT_EQ(res[1], 4.0);
    EXPECT_EQ(res[2], 5.0);
}

TEST_F(VectorWithNumberTest, difference) {
    auto res = v - num;
    EXPECT_EQ(res[0], -1.0);
    EXPECT_EQ(res[1], 0.0);
    EXPECT_EQ(res[2], 1.0);

    res = num - v;
    EXPECT_EQ(res[0], 1.0);
    EXPECT_EQ(res[1], 0.0);
    EXPECT_EQ(res[2], -1.0);
}

TEST_F(VectorWithNumberTest, multiply) {
    auto res = v * num;
    EXPECT_EQ(res[0], 2.0);
    EXPECT_EQ(res[1], 4.0);
    EXPECT_EQ(res[2], 6.0);

    res = num * v;
    EXPECT_EQ(res[0], 2.0);
    EXPECT_EQ(res[1], 4.0);
    EXPECT_EQ(res[2], 6.0);
}

TEST_F(VectorWithNumberTest, devision) {
    auto res = v / num;
    EXPECT_EQ(res[0], 0.5);
    EXPECT_EQ(res[1], 1.0);
    EXPECT_EQ(res[2], 1.5);

    res = num / v;
    EXPECT_EQ(res[0], 2.0);
    EXPECT_EQ(res[1], 1.0);
    EXPECT_LT(res[2], 0.67);
    EXPECT_GT(res[2], 0.65);
}

//--------------------------------------------------
TEST(Exceptions, initListShorter) {
    bool is_catched = false;
    try {LinearVector<3, int> v = { 1, 2 };}
    catch (std::out_of_range & ex) {is_catched = true;}
    EXPECT_TRUE(is_catched);
}

TEST(Exceptions, initListLonger) {
    bool is_catched = false;
    try {LinearVector<3, int> v = { 1, 2, 3, 4 };}
    catch (const std::out_of_range& ex) {is_catched = true;}
    EXPECT_TRUE(is_catched);
}

TEST(Exceptions, set1) {
    bool is_catched = false;
    try {
        LinearVector<3, int> v = { 5, 5, 5 };
        v[3] = 10; // valid indexes: 0, 1, 2
    }
    catch (std::exception & ex) {is_catched = true;}
    EXPECT_TRUE(is_catched);
}

TEST(Exceptions, set2) {
    bool is_catched = false;
    try {
        LinearVector<3, int> v = { 5, 5, 5 };
        v[-1] = 10; // valid indexes: 0, 1, 2
    }
    catch (std::exception & ex) {is_catched = true;}
    EXPECT_TRUE(is_catched);
}

TEST(Exceptions, get1) {
    bool is_catched = false;
    try {
        LinearVector<3, int> v = { 5, 5, 5 };
        int num = v[3]; // valid indexes: 0, 1, 2
    }
    catch (std::exception & ex) {is_catched = true;}
    EXPECT_TRUE(is_catched);
}

TEST(Exceptions, get2) {
    bool is_catched = false;
    try {
        LinearVector<3, int> v = { 5, 5, 5 };
        int num = v[-1]; // valid indexes: 0, 1, 2
    }
    catch (std::exception & ex) {is_catched = true;}
    EXPECT_TRUE(is_catched);
}
//--------------------------------------------------
class ComplexVectorTest : public ::testing::Test {
protected:
    typedef std::complex<double> Complex;
    typedef LinearVector<3, Complex> Vector;
    ComplexVectorTest() :
        a({ Complex(2.0, 2.0), Complex(0.0, 1.0), Complex(1.0, -1.0) }),
        b({ Complex(0.0, 2.0), Complex(-1.0, 1.0), Complex(1.0, 0.0) })
    {}
    Vector a;
    Vector b;
};

TEST_F(ComplexVectorTest, init) {
    EXPECT_EQ(a[0], Complex(2.0, 2.0));
    EXPECT_EQ(a[1], Complex(0.0, 1.0));
    EXPECT_EQ(a[2], Complex(1.0, -1.0));
}

TEST_F(ComplexVectorTest, sum) {
    Vector res = a + b;
    EXPECT_EQ(res[0], Complex(2.0, 4.0));
    EXPECT_EQ(res[1], Complex(-1.0, 2.0));
    EXPECT_EQ(res[2], Complex(2.0, -1.0));
}

TEST_F(ComplexVectorTest, difference) {
    Vector res = a - b;
    EXPECT_EQ(res[0], Complex(2.0, 0.0));
    EXPECT_EQ(res[1], Complex(1.0, 0.0));
    EXPECT_EQ(res[2], Complex(0.0, -1.0));
}

TEST_F(ComplexVectorTest, unarySum) {
    a += Complex(10.0, 0.0);
    EXPECT_EQ(a[0], Complex(12.0, 2.0));
    EXPECT_EQ(a[1], Complex(10.0, 1.0));
    EXPECT_EQ(a[2], Complex(11.0, -1.0));
}

TEST_F(ComplexVectorTest, unaryDivision) {
    a /= Complex(2.0, 0.0);
    EXPECT_EQ(a[0], Complex(1.0, 1.0));
    EXPECT_EQ(a[1], Complex(0.0, 0.5));
    EXPECT_EQ(a[2], Complex(0.5, -0.5));
}
