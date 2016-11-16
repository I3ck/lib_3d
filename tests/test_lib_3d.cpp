#include <iostream>

#define CATCH_CONFIG_MAIN
#include "../dependencies/Catch.h" //https://github.com/philsquared/Catch

#include "../lib_3d.h"

using namespace std;
using namespace lib_3d;

using T = double;


T
    x(0.1),
    y(0.2),
    z(0.3),
    MAX_DELTA(0.00001);

///@todo some tests might be incorrectly named now or duped since change from PointVec and Point to Vec

TEST_CASE("testing point") {
    Vec<T> p{x, y, z};

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
        Vec<T> p2 = p;
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
        Vec <T> center{}, centerShould{};

        p = Vec<T>{1,1,0.0};
        Vec<T> p2 = Vec<T> {1,2,0.0};
        centerShould = Vec <T> {1, 1.5, 0.0};
        center = p.center_between(p2);

        REQUIRE(center.similar_to(centerShould, MAX_DELTA));
    }

    SECTION("testing operator overloads") {
        Vec<T>
            p2{1.0, 2.0, 3.0},
            p3,
            p4;

        Vec<T> vec1{10.0, 11.0, 12.0};

        p3 = p2;
        p3.move_by(10.0, 11.0, 12.0);
        p4 = p2 + vec1;

        REQUIRE(p3 == p4);

        p3.move_by(10.0, 11.0, 12.0);

        p4 += vec1;

        REQUIRE(p3 == p4);
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

        m4 = m3;
        m3 = m2 * m3;
        m4 *= m2;

        REQUIRE(m3 == m4);

        m3 = m3 * 2.0;
        m4 *= 2.0;

        REQUIRE(m3 == m4);
    }
}

TEST_CASE("testing Matrix factories") {

    SECTION("zeroes Matrix") {
        Matrix<T> m1 = MatrixFactory<T>::zeroes();
        Matrix<T> m2;

        m2.m[0][0] = 0.0;  m2.m[0][1] = 0.0;  m2.m[0][2] = 0.0;  m2.m[0][3] = 0.0;
        m2.m[1][0] = 0.0;  m2.m[1][1] = 0.0;  m2.m[1][2] = 0.0;  m2.m[1][3] = 0.0;
        m2.m[2][0] = 0.0;  m2.m[2][1] = 0.0;  m2.m[2][2] = 0.0;  m2.m[2][3] = 0.0;
        m2.m[3][0] = 0.0;  m2.m[3][1] = 0.0;  m2.m[3][2] = 0.0;  m2.m[3][3] = 0.0;

        REQUIRE(m1 == m2);
    }

    SECTION("unity Matrix") {
        Matrix<T> m1 = MatrixFactory<T>::unity();
        Matrix<T> m2;

        m2.m[0][0] = 1.0;  m2.m[0][1] = 0.0;  m2.m[0][2] = 0.0;  m2.m[0][3] = 0.0;
        m2.m[1][0] = 0.0;  m2.m[1][1] = 1.0;  m2.m[1][2] = 0.0;  m2.m[1][3] = 0.0;
        m2.m[2][0] = 0.0;  m2.m[2][1] = 0.0;  m2.m[2][2] = 1.0;  m2.m[2][3] = 0.0;
        m2.m[3][0] = 0.0;  m2.m[3][1] = 0.0;  m2.m[3][2] = 0.0;  m2.m[3][3] = 1.0;

        REQUIRE(m1 == m2);
    }

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

TEST_CASE("Matrix*Vec") {

    SECTION("Translate Vec with Matrix") {
        Vec<T> p{1.0, 2.0, 3.0};
        Matrix<T> m = MatrixFactory<T>::translation(1.0, 0.1, 0.3);

        Vec<T> p2 = p * m;

        Vec<T> p3{2.0, 2.1, 3.3};

        REQUIRE(p2.similar_to(p3, MAX_DELTA));

        p *= m;

        REQUIRE(p.similar_to(p2, MAX_DELTA));
    }

    SECTION("Rotate Vec with Matrix") {
        Vec<T> p{1.0, 0.0, 0.0};

        Matrix<T> mRotX = MatrixFactory<T>::rotation(90.0, 0.0, 0.0);
        Matrix<T> mRotY = MatrixFactory<T>::rotation(0.0, 90.0, 0.0);
        Matrix<T> mRotZ = MatrixFactory<T>::rotation(0.0, 0.0, 90.0);

        p = p * mRotZ;
        REQUIRE(p.similar_to(Vec<T>{0.0, 1.0, 0.0}, MAX_DELTA));

        p = p * mRotZ;
        REQUIRE(p.similar_to(Vec<T>{-1.0, 0.0, 0.0}, MAX_DELTA));

        p = p * mRotZ;
        REQUIRE(p.similar_to(Vec<T>{0.0, -1.0, 0.0}, MAX_DELTA));

        p = p * mRotX;
        REQUIRE(p.similar_to(Vec<T>{0.0, 0.0, -1.0}, MAX_DELTA));

        p = p * mRotY;
        REQUIRE(p.similar_to(Vec<T>{-1.0, 0.0, 0.0}, MAX_DELTA));
    }

    SECTION("Scaling a Vec with Matrix") {
        Vec<T> p{1.0, 2.0, 3.0};

        Matrix<T> mScale = MatrixFactory<T>::scaling(2.0, 3.0, 4.0);

        p = p * mScale;
        REQUIRE(p.similar_to(Vec<T>{2.0, 6.0, 12.0}, MAX_DELTA));
    }

    SECTION("Using perspective on a Vec") {
        ///@todo need some proper compare results
        ///@todo not yet implemented
        Matrix<T> mPers = MatrixFactory<T>::perspective(300.0, 300.0, 1.0, 100.0, 90.0);
    }
}

TEST_CASE("Testing Vec") {
    SECTION("Normalizing") {
        Vec<T> vec{5.0, 0.0, 0.0};

        vec.normalize();

        Vec<T> vec2{1.0, 0.0, 0.0};

        REQUIRE(vec == vec2);
    }

    SECTION("Retrieving length") {
        Vec<T> vec{0.0, 0.0, 0.0};

        REQUIRE(vec.abs() == 0.0);

        Vec<T> vec2{1.0, 0.0, 0.0};

        REQUIRE(vec2.abs() == 1.0);
    }

    SECTION("Cross product") {
        Vec<T> vec1{1.0, 2.0, 3.0};
        Vec<T> vec2{-7.0, 8.0, 9.0};
        Vec<T> vec3{-6.0, -30.0, 22.0};

        REQUIRE(vec1.cross(vec2) == vec3);
    }

    SECTION("Dot product") {
        Vec<T> vec1{1.0, 3.0, -5.0};
        Vec<T> vec2{4.0, -2.0, -1.0};

        REQUIRE(vec1.dot(vec2) == 3.0);
    }

    SECTION("Vec between Vecs") {
        Vec<T> p1{0.0, 0.0, 0.0};
        Vec<T> p2{1.0, 1.1, 1.2};

        Vec<T> vec1{-1.0, -1.1, -1.2};
        Vec<T> vec2 = p1 - p2;

        REQUIRE(vec1 == vec2);
    }

    SECTION("adding vector to Vec") {
        Vec<T> p1{0.0, 0.0, 0.0};
        Vec<T> p2{1.0, 1.1, 1.2};

        Vec<T> vec1{1.0, 1.1, 1.2};

        REQUIRE((p1 + vec1) == p2);
    }
}

TEST_CASE("PointCloud") {
    SECTION("Constructors") {
        vector< Vec<T> > points = { {0.0, 1.0, 1.2}, {2.0, 2.1, 2.2} };

        PointCloud< Vec<T> > pointCloud;

        REQUIRE(pointCloud.size() == 0);

        PointCloud< Vec<T> > pointCloud2(points.begin(), points.end());

        REQUIRE(pointCloud2.size() == 2);
    }

    SECTION("to and from string") {
        ///@todo
        ///@also add << test
    }

    SECTION("to and from file") {
        ///@todo
    }

    SECTION("filling and emptying and size") {
        Vec<T> p1, p2, p3, p4;
        PointCloud< Vec<T> > pointCloud1, pointCloud2;

        pointCloud1.push_back(p1);
        pointCloud1.push_back(0.0, 0.0, 0.0);
        pointCloud1.emplace_back(0.0, 0.0, 0.0);
        pointCloud1.emplace_back(Vec<T>{0.0, 0.0, 0.0});
        pointCloud1 += p2;

        REQUIRE(pointCloud1.size() == 5);

        pointCloud1.pop_back();

        REQUIRE(pointCloud1.size() == 4);

        pointCloud2.push_back(pointCloud1);
        pointCloud2 += pointCloud1;
        pointCloud2 += p1;

        REQUIRE(pointCloud2.size() == 9);
    }

    SECTION("Length") {
        Vec<T>
            p1{0.0, 0.0, 0.0},
            p2{1.0, 0.0, 0.0},
            p3{2.0, 0.0, 0.0},
            p4{3.0, 0.0, 0.0},
            p5{4.0, 0.0, 0.0};

        PointCloud< Vec<T> > pointCloud;
        pointCloud += p1;
        pointCloud += p2;
        pointCloud += p3;
        pointCloud += p4;
        pointCloud += p5;

        REQUIRE(pointCloud.length() == 4.0);
    }

    SECTION("first, last, clear and reverse") {
        Vec<T>
            p1{0.0, 0.0, 0.0},
            p2{1.0, 0.0, 0.0},
            p3{2.0, 0.0, 0.0},
            p4{3.0, 0.0, 0.0},
            p5{4.0, 0.0, 0.0};

        PointCloud< Vec<T> > pointCloud;
        pointCloud += p1;
        pointCloud += p2;
        pointCloud += p3;
        pointCloud += p4;
        pointCloud += p5;

        REQUIRE(pointCloud.first() == p1);
        REQUIRE(pointCloud.last() == p5);

        pointCloud.reverse();

        REQUIRE(pointCloud.first() == p5);
        REQUIRE(pointCloud.last() == p1);

        pointCloud.clear();

        REQUIRE(pointCloud.size() == 0);
    }

    SECTION("center") {
        Vec<T>
            p1{0.0, 0.0, 0.0},
            p2{1.0, 0.0, 0.0},
            p3{2.0, 0.0, 0.0},
            p4{3.0, 0.0, 0.0},
            p5{4.0, 0.0, 0.0},
            centerShould{2.0, 0.0, 0.0};

        PointCloud< Vec<T> > pointCloud;
        pointCloud += p1;
        pointCloud += p2;
        pointCloud += p3;
        pointCloud += p4;
        pointCloud += p5;

        REQUIRE(centerShould == pointCloud.center());
    }

    SECTION("similarity and equality") {
        Vec<T>
            p1{0.0, 0.0, 0.0},
            p2{1.0, 0.0, 0.0},
            p3{2.0, 0.0, 0.0},
            p4{3.0, 0.0, 0.0},
            p5{4.0, 0.0, 0.0};

        PointCloud< Vec<T> > pointCloud1;
        pointCloud1 += p1;
        pointCloud1 += p2;
        pointCloud1 += p3;
        pointCloud1 += p4;
        pointCloud1 += p5;

        PointCloud< Vec<T> > pointCloud2 = pointCloud1;

        REQUIRE(pointCloud1.equal_to(pointCloud2));
        REQUIRE(pointCloud1 == pointCloud2);
        REQUIRE(pointCloud1.similar_to(pointCloud2, MAX_DELTA));

        p5.move_by(0.1*MAX_DELTA, 0.0, 0.0);

        pointCloud2.pop_back();
        pointCloud2 += p5;

        REQUIRE(!pointCloud1.equal_to(pointCloud2));
        REQUIRE(pointCloud1 != pointCloud2);
        REQUIRE(pointCloud1.similar_to(pointCloud2, MAX_DELTA));

        p5.move_by(1.0*MAX_DELTA, 0.0, 0.0);

        pointCloud2.pop_back();
        pointCloud2 += p5;

        REQUIRE(!pointCloud1.equal_to(pointCloud2));
        REQUIRE(pointCloud1 != pointCloud2);
        REQUIRE(!pointCloud1.similar_to(pointCloud2, MAX_DELTA));
    }

    SECTION("range") {
        Vec<T>
            p1{0.0, 0.0, 0.0},
            p2{1.0, 0.0, 0.0},
            p3{2.0, 0.0, 0.0},
            p4{3.0, 0.0, 0.0},
            p5{4.0, 0.0, 0.0};

        PointCloud< Vec<T> > pointCloud1;
        pointCloud1 += p1;
        pointCloud1 += p2;
        pointCloud1 += p3;
        pointCloud1 += p4;
        pointCloud1 += p5;

        PointCloud< Vec<T> > pointCloud2;
        pointCloud2 += p2;
        pointCloud2 += p3;

        REQUIRE(pointCloud1.range(0, 100) == pointCloud1);
        REQUIRE(pointCloud1.range(50, 1) == pointCloud1);

        REQUIRE(pointCloud1.range(1,2) == pointCloud2);
    }

    SECTION("iterating and random access") {
        vector< Vec<T> > points;
        points.push_back(Vec<T>{0.0, 0.0, 0.0});
        points.push_back(Vec<T>{1.0, 0.0, 0.0});
        points.push_back(Vec<T>{2.0, 0.0, 0.0});
        points.push_back(Vec<T>{3.0, 0.0, 0.0});
        points.push_back(Vec<T>{4.0, 0.0, 0.0});

        PointCloud< Vec<T> > pointCloud;

        for(auto i : points)
            pointCloud += i;

        int i = 0;

        for(auto point : pointCloud) {
            REQUIRE(point == points[i]);
            REQUIRE(point == pointCloud[i]);
            ++i;
        }
    }
}

TEST_CASE("Matrix Pipe") {

    ///@todo look at currently not implemented, add to test once this happened
    SECTION("First test") {
        T
            scale(10.0),
            degX(11.0),
            degY(3.0),
            degZ(1.0),
            moveX(50.0),
            moveY(41.3),
            moveZ(7.9),
            camDegX(3.3),
            camDegY(1.9),
            camDegZ(1.3),
            camMoveX(1.3),
            camMoveY(16.4),
            camMoveZ(3.33),
            width(1000.0),
            height(500.0),
            close(1.0),
            away(100.0),
            fovDeg(90.0);

        Matrix<T>
            matrixTranslation = MatrixFactory<T>::translation(moveX, moveY, moveZ),
            matrixRotation = MatrixFactory<T>::rotation(degX, degY, degZ),
            matrixScale = MatrixFactory<T>::scaling(scale),
            matrixPerspective = MatrixFactory<T>::perspective(width, height, close, away, fovDeg),
            matrixCameraTranslation = MatrixFactory<T>::translation(-camMoveX, -camMoveY, -camMoveZ),
            matrixResult;
            //matrixCameraLook = MatrixFactory<T>::MISSING

        matrixResult = matrixPerspective * matrixCameraTranslation *
                       matrixTranslation * matrixRotation * matrixScale;

        MatrixPipe<T> pipe;

        REQUIRE(pipe.result() == MatrixFactory<T>::unity());

        pipe.add_translation(moveX, moveY, moveZ);
        pipe.add_rotation(degX, degY, degZ);
        pipe.add_scale(scale);
        pipe.add_perspective(width, height, close, away, fovDeg);
        pipe.add_camera_translation(camMoveX, camMoveY, camMoveZ);

        REQUIRE(pipe.result() == matrixResult);

        pipe.remove_translation();
        pipe.remove_rotation();
        pipe.remove_scale();
        pipe.remove_perspective();
        pipe.remove_camera_translation();

        REQUIRE(pipe.result() == MatrixFactory<T>::unity());
    }
}

TEST_CASE("testing mesh") {

    SECTION("construction") {
        ///@todo
    }

    SECTION("loading and saving ascii") {
        Mesh< Vec<T> > mesh, mesh2;
        mesh.load_stl("../../../tests/stlAscii.stl");

        REQUIRE(mesh.number_points() == 8);

        REQUIRE(mesh.number_facets() == 12);

        mesh.save_stl("../../../tests/test_stl_io_ascii.stl.tmp");

        mesh2.load_stl("../../../tests/test_stl_io_ascii.stl.tmp");

        REQUIRE(mesh.number_points() == mesh2.number_points());
        REQUIRE(mesh2.number_facets() == mesh2.number_facets());
    }

    SECTION("loading and saving binary") {
        Mesh< Vec<T> > mesh, mesh2;
        mesh.load_stl("../../../tests/stlBinary.stl", true);

        REQUIRE(mesh.number_points() == 8);

        REQUIRE(mesh.number_facets() == 12);

        mesh.save_stl("../../../tests/test_stl_io_binary.stl.tmp", true);

        mesh2.load_stl("../../../tests/test_stl_io_binary.stl.tmp", true);

        REQUIRE(mesh.number_points() == mesh2.number_points());
        REQUIRE(mesh2.number_facets() == mesh2.number_facets());
    }

    SECTION("getting points and facets or ids") {
        Mesh< Vec<T> > mesh;
        mesh.load_stl("../../../tests/stlAscii.stl");

        REQUIRE(mesh.get_points().size() == 8);

        REQUIRE(mesh.get_facets().size() == 12);

        REQUIRE(mesh.get_ids().size() == 36);
    }

    /*
    ///@todo this code segfaults
    SECTION("getting normals") {
        Mesh< Point<T> > mesh;
        mesh.load_stl("tests/stlAscii.stl");

        std::vector< Vec<T> > normals = mesh.get_normals();
        std::vector< Point<T> > points = mesh.get_points();

        REQUIRE(normals.size() == points.size());

        for(auto normale : normals) {
            REQUIRE(normale.length() == 1.0);
        }
        REQUIRE(false); //has to be checked visually
    }
    */
}

///@todo test Quaternion
