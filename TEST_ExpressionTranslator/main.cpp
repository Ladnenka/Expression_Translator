#include "TEST_SimplifyPlus.cpp"
#include "TEST_SimplifyMinus.cpp"
#include "TEST_SimplifyDivide.cpp"

int main(int argc, char *argv[]) {
    int status = 0;
    { TEST_SimplifyPlus       t; status |= QTest::qExec(&t, argc, argv); }
    { TEST_SimplifyMinus       t; status |= QTest::qExec(&t, argc, argv); }
    { TEST_SimplifyDivide       t; status |= QTest::qExec(&t, argc, argv); }
    return status;
}
