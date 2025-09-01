#include <linal/matrix.h>
#include <tests/randf.h>

#include <gtest/gtest.h>
#include <cstdlib>
#include <cmath>

const float eps = 1e-5;

TEST(MatrixTest, IdentityDefault) {
    Matrix<float, 3, 3> I;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            EXPECT_FLOAT_EQ(I[i][j], i == j ? 1.0f : 0.0f);
}

TEST(MatrixTest, AdditionSubtraction) {
    Matrix<float, 3, 3> A, B;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            A[i][j] = randf();
            B[i][j] = randf();
        }
    }

    auto C = A + B;
    auto D = A - B;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_NEAR(C[i][j], A[i][j] + B[i][j], eps);
            EXPECT_NEAR(D[i][j], A[i][j] - B[i][j], eps);
        }
    }
}

TEST(MatrixTest, Multiplication) {
    Matrix<float, 3, 2> M1;
    Matrix<float, 2, 3> M2;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 2; ++j) 
            M1[i][j] = randf();
    
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j) 
            M2[i][j] = randf();

    auto M3 = M1 * M2;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            float sum = 0;
            for (int k = 0; k < 2; ++k) 
                sum += M1[i][k] * M2[k][j];
            EXPECT_NEAR(M3[i][j], sum, eps);
        }
    }
}
