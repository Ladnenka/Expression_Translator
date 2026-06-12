#include "TEST_SimplifyPlus.cpp"

int main(int argc, char *argv[]) {
    int status = 0;
    { TEST_SimplifyPlus       t; status |= QTest::qExec(&t, argc, argv); }
    return status;
}
