#include "tst_utilities.h"
#include "../../src/utilities.h"

void tst_Utilities::tst_parseTitle()
{
    static const QString testTitle1("# some title - 5345 \r wsfawertsdf");
    static const QString expectedTitle1("some title - 5345 ");
    static const QString testTitle2("# some title - 5345 \nfasdfas");
    static const QString expectedTitle2("some title - 5345 ");
    QCOMPARE(Utilities::parseTitle(testTitle1), expectedTitle1);
    QCOMPARE(Utilities::parseTitle(testTitle2), expectedTitle2);
}

QTEST_MAIN(tst_Utilities)
