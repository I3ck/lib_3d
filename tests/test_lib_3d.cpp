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


TEST_CASE("testing point") {

    SECTION("testing initialisation") {
        T
            x(0.1),
            y(0.2),
            z(0.3);

        Point<T> p{x, y, z};

        REQUIRE(p.x == x);
        REQUIRE(p.y == y);
        REQUIRE(p.z == z);

    }
}
