#include <vector>
#include <utility>

using namespace std;
#define PI 3.141592653589

vector<pair<double, double>> RegularPolygon(int n, double a0 = 0) {
    vector<pair<double, double>> p;

    double a = 2 * PI / n;

    for (int i = 0; i < n; i++) 
        p.push_back({cos(a0 + a * i), sin(a0 + a * i)});

    return p;
}

