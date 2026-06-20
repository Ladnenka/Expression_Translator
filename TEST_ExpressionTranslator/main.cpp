#include "TEST_SimplifyPlus.cpp"
#include "TEST_SimplifyMinus.cpp"
#include "TEST_SimplifyDivide.cpp"
#include "TEST_GetFunctionDescription.cpp"
#include "TEST_Translate.cpp"

int main(int argc, char *argv[]) {
    int status = 0;
    { TEST_SimplifyPlus       t; status |= QTest::qExec(&t, argc, argv); }
    { TEST_SimplifyMinus      t; status |= QTest::qExec(&t, argc, argv); }
    { TEST_SimplifyDivide     t; status |= QTest::qExec(&t, argc, argv); }
    { TEST_GetFuncDescription t; status |= QTest::qExec(&t, argc, argv); }
    { TEST_Translate          t; status |= QTest::qExec(&t, argc, argv); }
    return status;
}
