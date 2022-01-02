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

    const string h3 = 
    " Uspeshnia - A \n"
    " A - Y \n"
    " Y - X \n";

    const string h4 = 
    " Uspeshnia - A \n"
    " A - X \n"
    " X - Y \n";

    Hierarchy hr3(h3);
    Hierarchy hr4(h4);
    REQUIRE_THROWS(hr3.join(hr4));


    const string h5 = 
    " Uspeshnia - A \n"
    " A - X \n";
   

    const string h6 = 
    "Uspeshnia - X \n"
    "X - T \n"
    "T - A \n";

    Hierarchy hr5(h5);
    Hierarchy hr6(h6);

    REQUIRE_THROWS(hr5.join(hr6));


}