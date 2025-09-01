namespace mini_renderer {

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols>::Matrix() {
    // generate identity matrix by default
    m = std::vector<std::vector<T> >(rows, std::vector<T>(cols, 0));
    for (size_t i = 0; i < std::min(rows, cols); i++)
        m[i][i] = 1;
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols> Matrix<T, rows, cols>::operator + (const Matrix<T, rows, cols> &v) {
    Matrix<T, rows, cols> result;
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            result.m[i][j] = m[i][j] + v.m[i][j];
    return result;
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols> Matrix<T, rows, cols>::operator - (const Matrix<T, rows, cols> &v) {
    Matrix<T, rows, cols> result;
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            result.m[i][j] = m[i][j] - v.m[i][j];	
    return result;
}

template<typename T, size_t rows, size_t cols>
template<size_t cols2> 
Matrix<T, rows, cols2> Matrix<T, rows, cols>::operator * (const Matrix<T, cols, cols2> &v) const {
    Matrix<T, rows, cols2> result;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols2; j++) {
            result.m[i][j] = 0;
            for (size_t k = 0; k < cols; k++)
                result.m[i][j] += m[i][k] * v.m[k][j];
        }
    }
    return result;
}

}