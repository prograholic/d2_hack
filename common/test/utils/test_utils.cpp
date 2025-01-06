#include <format>
#include <iostream>
#include <stdexcept>

#include <d2_hack/common/utils.h>

namespace d2_hack
{
namespace common
{

void CheckGetDataFrom2D(const int* data, const int expectedValue, std::size_t borderSize, std::size_t row, std::size_t col)
{
    const int dataValue = GetDataFrom2D(data, borderSize, row, col);
    if (dataValue != expectedValue)
    {
        throw std::runtime_error{std::format("Expected {}, got {} at row: {}, col: {}", expectedValue, dataValue, row, col)};
    }
}

void TestGetDataFrom2D()
{
    const int data1[] = {
            11
    };
    CheckGetDataFrom2D(data1, 11, 1, 0, 0);

    const int data2[] = {
            11, 12,
            21, 22,
    };
    CheckGetDataFrom2D(data2, 11, 2, 0, 0);
    CheckGetDataFrom2D(data2, 12, 2, 0, 1);
    CheckGetDataFrom2D(data2, 21, 2, 1, 0);
    CheckGetDataFrom2D(data2, 22, 2, 1, 1);

    const int data3[] = {
            11, 12, 13,
            21, 22, 23,
            31, 32, 33,
    };
    CheckGetDataFrom2D(data3, 11, 3, 0, 0);
    CheckGetDataFrom2D(data3, 12, 3, 0, 1);
    CheckGetDataFrom2D(data3, 13, 3, 0, 2);
    CheckGetDataFrom2D(data3, 21, 3, 1, 0);
    CheckGetDataFrom2D(data3, 22, 3, 1, 1);
    CheckGetDataFrom2D(data3, 23, 3, 1, 2);
    CheckGetDataFrom2D(data3, 31, 3, 2, 0);
    CheckGetDataFrom2D(data3, 32, 3, 2, 1);
    CheckGetDataFrom2D(data3, 33, 3, 2, 2);

    const int data4[] = {
            11, 12, 13, 14,
            21, 22, 23, 24,
            31, 32, 33, 34,
            41, 42, 43, 44
    };
    CheckGetDataFrom2D(data4, 11, 4, 0, 0);
    CheckGetDataFrom2D(data4, 12, 4, 0, 1);
    CheckGetDataFrom2D(data4, 13, 4, 0, 2);
    CheckGetDataFrom2D(data4, 14, 4, 0, 3);
    CheckGetDataFrom2D(data4, 21, 4, 1, 0);
    CheckGetDataFrom2D(data4, 22, 4, 1, 1);
    CheckGetDataFrom2D(data4, 23, 4, 1, 2);
    CheckGetDataFrom2D(data4, 24, 4, 1, 3);
    CheckGetDataFrom2D(data4, 31, 4, 2, 0);
    CheckGetDataFrom2D(data4, 32, 4, 2, 1);
    CheckGetDataFrom2D(data4, 33, 4, 2, 2);
    CheckGetDataFrom2D(data4, 34, 4, 2, 3);
    CheckGetDataFrom2D(data4, 41, 4, 3, 0);
    CheckGetDataFrom2D(data4, 42, 4, 3, 1);
    CheckGetDataFrom2D(data4, 43, 4, 3, 2);
    CheckGetDataFrom2D(data4, 44, 4, 3, 3);
}

void CheckMatrix(const int* data, const int* expected, std::size_t borderSize)
{
    for (std::size_t row = 0; row != borderSize; ++row)
    {
        for (std::size_t col = 0; col != borderSize; ++col)
        {
            const int expectedValue = GetDataFrom2D(expected, borderSize, row, col);
            CheckGetDataFrom2D(data, expectedValue, borderSize, row, col);
        }
    }
}

void TestTransposeMatrix1()
{
    int data1[] = {
        1
    };

    const int expected1[] = {
        1
    };

    TransposeInplace(data1, 1);

    CheckMatrix(data1, expected1, 1);
}

void TestTransposeMatrix2()
{
    int data2[] = {
        11, 12,
        21, 22,
    };

    const int expected2[] = {
        11, 21,
        12, 22,
    };

    TransposeInplace(data2, 2);

    CheckMatrix(data2, expected2, 2);
}


void TestTransposeMatrix3()
{
    int data3[] = {
        11, 12, 13,
        21, 22, 23,
        31, 32, 33
    };

    const int expected3[] = {
        11, 21, 31,
        12, 22, 32,
        13, 23, 33
    };

    TransposeInplace(data3, 3);

    CheckMatrix(data3, expected3, 3);
}

void TestTransposeMatrix4()
{
    int data4[] = {
        11, 12, 13, 14,
        21, 22, 23, 24,
        31, 32, 33, 34,
        41, 42, 43, 44
    };

    const int expected4[] = {
        11, 21, 31, 41,
        12, 22, 32, 42,
        13, 23, 33, 43,
        14, 24, 34, 44
    };

    TransposeInplace(data4, 4);

    CheckMatrix(data4, expected4, 4);
}


void TestTransposeMatrix()
{
    TestTransposeMatrix1();
    TestTransposeMatrix2();
    TestTransposeMatrix3();
    TestTransposeMatrix4();
}



void TestReverseColumnInplace1()
{
    int data1[] = {
        1
    };

    const int expected1[] = {
        1
    };

    ReverseColumnInplace(data1, 1);

    CheckMatrix(data1, expected1, 1);
}

void TestReverseColumnInplace2()
{
    int data2[] = {
        11, 12,
        21, 22,
    };

    const int expected2[] = {
        12, 11,
        22, 21,
    };

    ReverseColumnInplace(data2, 2);

    CheckMatrix(data2, expected2, 2);
}


void TestReverseColumnInplace3()
{
    int data3[] = {
        11, 12, 13,
        21, 22, 23,
        31, 32, 33
    };

    const int expected3[] = {
        13, 12, 11,
        23, 22, 21,
        33, 32, 31
    };

    ReverseColumnInplace(data3, 3);

    CheckMatrix(data3, expected3, 3);
}

void TestReverseColumnInplace4()
{
    int data4[] = {
        11, 12, 13, 14,
        21, 22, 23, 24,
        31, 32, 33, 34,
        41, 42, 43, 44
    };

    const int expected4[] = {
        14, 13, 12, 11,
        24, 23, 22, 21,
        34, 33, 32, 31,
        44, 43, 42, 41
    };

    ReverseColumnInplace(data4, 4);

    CheckMatrix(data4, expected4, 4);
}



void TestReverseColumnInplace()
{
    TestReverseColumnInplace1();
    TestReverseColumnInplace2();
    TestReverseColumnInplace3();
    TestReverseColumnInplace4();
}


} // namespace common
} // namespace d2_hack  

int main()
{
    try
    {
        using namespace d2_hack::common;

        TestGetDataFrom2D();
        TestTransposeMatrix();
        TestReverseColumnInplace();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
