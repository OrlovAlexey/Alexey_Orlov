#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;
using ll = long long;

class Point {
public:
    ll x;
    ll y;
    Point() : x(0), y(0) {}
    Point(ll x, ll y) : x(x), y(y) {}
    bool operator== (const Point&) const;

};

bool Point::operator== (const Point& point) const {
    if (point.x == x && point.y == y) {
        return true;
    }
    return false;
}

std::istream& operator>> (std::istream& in, Point& p) {
    in >> p.x >> p.y;
    return in;
}

struct Vector {
    Point start;
    Point end;
    Vector(const Point& end, const Point& start) : end(end), start(start) {}
    ll length_sq() const {
        return ((start.x - end.x) * (start.x - end.x) + (start.y - end.y) * (start.y - end.y));
    }
};

ll cross(const Vector& a, const Vector& b) {
    ll x1 = a.end.x - a.start.x;
    ll x2 = b.end.x - b.start.x;
    ll y1 = a.end.y - a.start.y;
    ll y2 = b.end.y - b.start.y;
    return (x1 * y2 - y1 * x2);
}

bool x_cmp(const Point& r1, const Point& r2) {
    if (r1.x == r2.x) {
        return (r1.y < r2.y);
    }
    return (r1.x < r2.x);
}

int main() {
    int n;
    cin >> n;
    vector<Point> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    std::sort(a.begin(), a.end(), x_cmp);
    Point p = a[0];
    std::sort(a.begin() + 1, a.end(), [p](const Point& r1, const Point& r2) { // angle_cmp
        ll cr = cross(Vector(p, r1), Vector(p, r2));
        if (cr == 0) {
            return (Vector(p, r1).length_sq() < Vector(p, r2).length_sq());
        }
        return (cr > 0);
    });
    vector<Point> convex;
    convex.push_back(p);
    convex.push_back(a[1]);
    int cnvx_sz;
    for (int i = 2; i < n; ++i) {
        Point new_p = a[i];
        while (convex.size() > 1) {
            cnvx_sz = convex.size();
            Point A = convex[cnvx_sz - 1];
            Point B = convex[cnvx_sz - 2];
            if (cross(Vector(B, A), Vector(A, new_p)) <= 0) {
                convex.pop_back();
            } else {
                break;
            }
        }
        convex.push_back(new_p);
    }
    cnvx_sz = convex.size();
    cout << cnvx_sz << '\n';
    for (int i = 0; i < cnvx_sz; ++i) {
        cout << convex[i].x << " " << convex[i].y << '\n';
    }
    return 0;
}
