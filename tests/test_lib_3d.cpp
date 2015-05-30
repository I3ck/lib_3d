#include <iostream>

#define CATCH_CONFIG_MAIN
#include "../dependencies/Catch.h" //https://github.com/philsquared/Catch

#include "../lib_3d.h"

using namespace std;
using namespace lib_3d;

#ifdef USE_DOUBLE
using T = double;
#elif USE_LDOUBLE
using T = long double;
#elif USE_FLOAT
using T = float;
#else
using T = int;
#endif


T
    x(0.1),
    y(0.2),
    z(0.3),
    MAX_DELTA(0.00001);

TEST_CASE("testing point") {
    Point<T> p{x, y, z};

    SECTION("testing initialisation") {
        REQUIRE(p.x == x);
        REQUIRE(p.y == y);
        REQUIRE(p.z == z);
    }

    SECTION("testing moving") {
        T moveX(0.01), moveY(0.02), moveZ(0.03);

        p.move_by(moveX, moveY, moveZ);

        REQUIRE(p.x == x+moveX);
        REQUIRE(p.y == y+moveY);
        REQUIRE(p.z == z+moveZ);
    }

    SECTION("testing parsing from and to strings") {
        ///@todo methods missing
    }

    SECTION("testing distances and abs") {
        REQUIRE(abs(  p.abs() - (T)sqrt( pow(p.x,2) + pow(p.y,2) + pow(p.z,2) )  ) < MAX_DELTA);
    }

    SECTION("testing rotation") {
        ///@todo add tests once matrixes are done and rotation was added
    }

    SECTION("testing mirroring") {
        ///@todo add tests once mirroring was added
    }

    SECTION("testing equality checks") {
        Point<T> p2 = p;
        REQUIRE(p2 == p);
        REQUIRE(p2.equal_to(p));

        REQUIRE(p2.similar_to(p, MAX_DELTA));

        p.move_by(MAX_DELTA/2, 0.0, 0.0);

        REQUIRE(p2 != p);
        REQUIRE(!p2.equal_to(p));

        REQUIRE(p2.similar_to(p, MAX_DELTA));

        p.move_by(MAX_DELTA*3, 0.0, 0.0);

        REQUIRE(!p2.similar_to(p, MAX_DELTA));
    }

    SECTION("testing stringstream overload") {
        ///@todo implement once from and to string are done
    }

    SECTION("testing center calculation") {
        Point <T> center{}, centerShould{};

        p = Point<T>{1,1,0.0};
        Point<T> p2 = Point<T> {1,2,0.0};
        centerShould = Point <T> {1, 1.5, 0.0};
        center = p.center_between(p2);

        REQUIRE(center.similar_to(centerShould, MAX_DELTA));
    }
}

TEST_CASE("testing Matrix") {

    Matrix<T> m;

    SECTION("testing initialisation") {
        REQUIRE(m.m[0][0] == 1.0);
        REQUIRE(m.m[1][1] == 1.0);
        REQUIRE(m.m[2][2] == 1.0);
        REQUIRE(m.m[3][3] == 1.0);

        REQUIRE(m.m[0][1] == 0.0);
        REQUIRE(m.m[0][2] == 0.0);
        REQUIRE(m.m[0][3] == 0.0);

        REQUIRE(m.m[1][0] == 0.0);
        REQUIRE(m.m[1][2] == 0.0);
        REQUIRE(m.m[1][3] == 0.0);

        REQUIRE(m.m[2][0] == 0.0);
        REQUIRE(m.m[2][1] == 0.0);
        REQUIRE(m.m[2][3] == 0.0);

        REQUIRE(m.m[3][0] == 0.0);
        REQUIRE(m.m[3][1] == 0.0);
        REQUIRE(m.m[3][2] == 0.0);
    }

    SECTION("testing Matrix*Matrix multiplication") {
        Matrix<T> m2 = m;

        Matrix<T> m3 = m2 * m;

        REQUIRE(m2 == m);
        REQUIRE(m3 == m);

        m2.m[0][0] = 1.0;  m2.m[0][1] = 1.0;  m2.m[0][2] = 1.0;  m2.m[0][3] = 1.0;
        m2.m[1][0] = 1.0;  m2.m[1][1] = 1.0;  m2.m[1][2] = 1.0;  m2.m[1][3] = 1.0;
        m2.m[2][0] = 1.0;  m2.m[2][1] = 1.0;  m2.m[2][2] = 1.0;  m2.m[2][3] = 1.0;
        m2.m[3][0] = 1.0;  m2.m[3][1] = 1.0;  m2.m[3][2] = 1.0;  m2.m[3][3] = 1.0;

        m3 = m2 * m;

        REQUIRE(m3 == m2);
        REQUIRE(m3 != m);

        Matrix<T> m4;

        m4.m[0][0] = 4.0;  m4.m[0][1] = 4.0;  m4.m[0][2] = 4.0;  m4.m[0][3] = 4.0;
        m4.m[1][0] = 4.0;  m4.m[1][1] = 4.0;  m4.m[1][2] = 4.0;  m4.m[1][3] = 4.0;
        m4.m[2][0] = 4.0;  m4.m[2][1] = 4.0;  m4.m[2][2] = 4.0;  m4.m[2][3] = 4.0;
        m4.m[3][0] = 4.0;  m4.m[3][1] = 4.0;  m4.m[3][2] = 4.0;  m4.m[3][3] = 4.0;

        m3 = m2 * m2;

        REQUIRE(m3 == m4);

        m3 = m2 * 4.0;

        REQUIRE(m3 == m4);

        m3 = 4.0 * m2;

        REQUIRE(m3 == m4);
    }
}

TEST_CASE("testing Matrix factories") {

    SECTION("translation Matrix") {
        Matrix<T> m1 = MatrixFactory<T>::translation(1.0, 1.1, 1.2);
        Matrix<T> m2;

        m2.m[0][0] = 1.0;  m2.m[0][1] = 0.0;  m2.m[0][2] = 0.0;  m2.m[0][3] = 1.0;
        m2.m[1][0] = 0.0;  m2.m[1][1] = 1.0;  m2.m[1][2] = 0.0;  m2.m[1][3] = 1.1;
        m2.m[2][0] = 0.0;  m2.m[2][1] = 0.0;  m2.m[2][2] = 1.0;  m2.m[2][3] = 1.2;
        m2.m[3][0] = 0.0;  m2.m[3][1] = 0.0;  m2.m[3][2] = 0.0;  m2.m[3][3] = 1.0;

        REQUIRE(m1 == m2);
    }

    SECTION("scaling Matrix") {
        Matrix<T> m1 = MatrixFactory<T>::scaling(0.5, 0.6, 0.7);

        Matrix<T> m2;
        m2.m[0][0] = 0.5;  m2.m[0][1] = 0.0;  m2.m[0][2] = 0.0;  m2.m[0][3] = 0.0;
        m2.m[1][0] = 0.0;  m2.m[1][1] = 0.6;  m2.m[1][2] = 0.0;  m2.m[1][3] = 0.0;
        m2.m[2][0] = 0.0;  m2.m[2][1] = 0.0;  m2.m[2][2] = 0.7;  m2.m[2][3] = 0.0;
        m2.m[3][0] = 0.0;  m2.m[3][1] = 0.0;  m2.m[3][2] = 0.0;  m2.m[3][3] = 1.0;

        REQUIRE(m1 == m2);

        Matrix<T> m3 = MatrixFactory<T>::scaling(0.5);

        Matrix<T> m4;
        m4.m[0][0] = 0.5;  m4.m[0][1] = 0.0;  m4.m[0][2] = 0.0;  m4.m[0][3] = 0.0;
        m4.m[1][0] = 0.0;  m4.m[1][1] = 0.5;  m4.m[1][2] = 0.0;  m4.m[1][3] = 0.0;
        m4.m[2][0] = 0.0;  m4.m[2][1] = 0.0;  m4.m[2][2] = 0.5;  m4.m[2][3] = 0.0;
        m4.m[3][0] = 0.0;  m4.m[3][1] = 0.0;  m4.m[3][2] = 0.0;  m4.m[3][3] = 1.0;
    }

    ///@todo rotation (rather complicated)
}

TEST_CASE("Matrix*Point") {

    SECTION("Translate point with Matrix") {
        Point<T> p{1.0, 2.0, 3.0};
        Matrix<T> m = MatrixFactory<T>::translation(1.0, 0.1, 0.3);

        Point<T> p2 = p * m;

        Point<T> p3{2.0, 2.1, 3.3};

        REQUIRE(p2.similar_to(p3, MAX_DELTA));
    }

    SECTION("Rotate point with Matrix") {
        Point<T> p{1.0, 0.0, 0.0};

        Matrix<T> mRotX = MatrixFactory<T>::rotation(90.0, 0.0, 0.0);
        Matrix<T> mRotY = MatrixFactory<T>::rotation(0.0, 90.0, 0.0);
        Matrix<T> mRotZ = MatrixFactory<T>::rotation(0.0, 0.0, 90.0);

        p = p * mRotZ;
        REQUIRE(p.similar_to(Point<T>{0.0, 1.0, 0.0}, MAX_DELTA));

        p = p * mRotZ;
        REQUIRE(p.similar_to(Point<T>{-1.0, 0.0, 0.0}, MAX_DELTA));

        p = p * mRotZ;
        REQUIRE(p.similar_to(Point<T>{0.0, -1.0, 0.0}, MAX_DELTA));

        p = p * mRotX;
        REQUIRE(p.similar_to(Point<T>{0.0, 0.0, -1.0}, MAX_DELTA));

        p = p * mRotY;
        REQUIRE(p.similar_to(Point<T>{-1.0, 0.0, 0.0}, MAX_DELTA));
    }

    SECTION("Scaling a point with Matrix") {
        Point<T> p{1.0, 2.0, 3.0};

        Matrix<T> mScale = MatrixFactory<T>::scaling(2.0, 3.0, 4.0);

        p = p * mScale;
        REQUIRE(p.similar_to(Point<T>{2.0, 6.0, 12.0}, MAX_DELTA));
    }

    SECTION("Using perspective on a point") {
        ///@todo need some proper compare results
        ///@todo not yet implemented
        Matrix<T> mPers = MatrixFactory<T>::perspective(300.0, 300.0, 1.0, 100.0, 90.0);
    }
}
