#include <vector>
#include <cassert>
using namespace std;

struct Matrix{
    int r, c;
    vector<int> m;

    Matrix();
    Matrix(int r, int c);
    Matrix& operator+=(Matrix& mat);
    Matrix& operator-=(Matrix& mat);
};

vector<int> operator*(Matrix& mat, vector<int>& vec);