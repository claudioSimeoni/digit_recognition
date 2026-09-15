#include <vector>
#include <cassert>
using namespace std;

#include "../include/matrix.hpp"

Matrix::Matrix() {}

Matrix::Matrix(int r, int c) : r(r), c(c){
    m.resize(r * c);
}

Matrix& Matrix::operator+=(Matrix& mat){
    assert(r == mat.r && c == mat.c);
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            this->m[r * i + j] += mat.m[r * i + j];
        }
    }
}

Matrix& Matrix::operator-=(Matrix& mat){
    assert(r == mat.r && c == mat.c);
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            this->m[r * i + j] -= mat.m[r * i + j];
        }
    }
}

vector<int> operator*(Matrix& mat, vector<int>& vec){
    assert(mat.c == vec.size());
    vector<int> prod(mat.r);
    for(int i=0; i<mat.r; i++){
        for(int j=0; j<mat.c; j++){
            prod[i] += mat.m[mat.r * i + j] * vec[j];
        }
    }
    return prod;
}