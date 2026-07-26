#include <vector>
#include <iostream>

class Matrix {
  public:
    friend Matrix operator+(const Matrix &x, const Matrix &y) {
        Matrix result;
        if (x.a.size() != y.a.size()) {
            throw std::invalid_argument("Matrix row size does not match");
        }
        // initialize row size
        result.a.resize(x.a.size());
        for (int i = 0; i < x.a.size(); i++) {
            if (x.a[i].size() != y.a[i].size()) {
                throw std::invalid_argument("Matrix column size does not match");
            }
            result.a[i].resize(x.a[i].size());
            for (int j = 0; j < x.a[i].size(); j++) {
                result.a[i][j] = x.a[i][j] + y.a[i][j];
            }
        }
        return result;
    }
    std::vector< std::vector< int > > a;
};

int main() {
    int cases, k;
    std::cin >> cases;
    for (k = 0; k < cases; k++) {
        Matrix x;
        Matrix y;
        Matrix result;
        int n, m, i, j;
        std::cin >> n >> m;
        for (i = 0; i < n; i++) {
            std::vector< int > b;
            int num;
            for (j = 0; j < m; j++) {
                std::cin >> num;
                b.push_back(num);
            }
            x.a.push_back(b);
        }
        for (i = 0; i < n; i++) {
            std::vector< int > b;
            int num;
            for (j = 0; j < m; j++) {
                std::cin >> num;
                b.push_back(num);
            }
            y.a.push_back(b);
        }
        result = x + y;
        for (i = 0; i < n; i++) {
            for (j = 0; j < m; j++) {
                std::cout << result.a[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
