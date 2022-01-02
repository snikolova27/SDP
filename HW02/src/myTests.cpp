#define CATCH_CONFIG_MAIN
#include "../catch2/catch.hpp"

#include "interface.h"

TEST_CASE("Impossible join")
{
    const string h1 =
    "A - B \n"
    "A - E \n"
    "B - C \n"
    "C - D \n";

    const string h2 =
    "A - D \n"
    "A - C \n"
    "D - E \n"
    "E - B \n";
    Hierarchy hr1(h1);
    Hierarchy hr2(h2);

    REQUIRE_THROWS(hr1.join(hr2));
}