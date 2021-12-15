#include <iostream>
#include <vector>
using namespace std;

bool cmp(double a, double b) {
    double eps = 1e-8;
    return abs(a - b) <= eps;
}

class Point {
public:
    double x;
    double y;

    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
    bool operator== (const Point&) const;

};

bool Point::operator== (const Point& point) const {
    if (cmp(point.x, x) && cmp(point.y, y)) {
        return true;
    }
    return false;
}

istream& operator>> (istream& in, Point& p) {
    cin >> p.x >> p.y;
    return in;
}

class Line {
public:
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;
    Line() = default;
    Line(const Point& point1, const Point& point2) {
        a = point1.y - point2.y;
        b = point2.x - point1.x;
        c = (point1.x * point2.y) - (point2.x * point1.y);
    }
    Line(double a, double b, double c) : a(a), b(b), c(c) {}
    bool operator== (const Line&) const;
    bool is_belong(Point p) const {
        return cmp(a * p.x + b * p.y + c, 0);
    }
};


std::pair<Point, bool> intersect(const Line& line1, const Line& line2) {
    Point result;
    double a1, a2, b1, b2, c1, c2;
    a1 = line1.a;
    b1 = line1.b;
    c1 = line1.c;
    a2 = line2.a;
    b2 = line2.b;
    c2 = line2.c;
    double det = a1 * b2 - a2 * b1;
    if (!cmp(det, 0.0)) {
        result.x = cmp((b1 * c2 - b2 * c1) / det, 0.0) ? 0.0 : (b1 * c2 - b2 * c1) / det;
        result.y = cmp((a2 * c1 - a1 * c2) / det, 0.0) ? 0.0 : (a2 * c1 - a1 * c2) / det;
        return std::make_pair(result, true);
    }
    return std::make_pair(Point(), false);
}


class Ray {
public:
    Point p;
    Line l;
    Ray() = default;
    Ray(Point p, Line l) : p(p), l(l) {}
};

inline bool is_between_x(Point p, Point a1, Point a2) {
    return (((p.x < a1.x || cmp(p.x, a1.x)) && (p.x > a2.x || cmp(p.x, a2.x))) || ((p.x > a1.x || cmp(p.x, a1.x)) && (p.x < a2.x || cmp(p.x, a2.x))));
}

inline bool is_between_y(Point p, Point a1, Point a2) {
    return (((p.y < a1.y || cmp(p.y, a1.y)) && (p.y > a2.y || cmp(p.y, a2.y))) || ((p.y > a1.y || cmp(p.y, a1.y)) && (p.y < a2.y || cmp(p.y, a2.y))));
}

inline bool is_between(Point p, Point a1, Point a2) {
    return (is_between_x(p, a1, a2) && is_between_y(p, a1, a2));
}

inline bool is_between2(Point p, Point a1, Point a2) { // specific version
    if (p == a1) {
        return true;
    }
    if (p == a2) {
        return false;
    }
    return (is_between_x(p, a1, a2) && is_between_y(p, a1, a2));
}

bool is_parallel_to_ray(Ray ray, Point a1, Point a2) {
    Line seg_line(a1, a2);
    pair<Point, bool> intersected = ::intersect(ray.l, seg_line);
    return !intersected.second;
}

bool intersect_with_ray(Ray ray, Point a1, Point a2, int index1, const vector<Point>& a) {
    // only if a1 != a2
    Line seg_line(a1, a2);
    pair<Point, bool> intersected = ::intersect(ray.l, seg_line);
    Point p1 = intersected.first;
    if (intersected.second && p1.x > ray.p.x) {
        // the segment intersects the ray
        if (p1 == a1) { // strange algorithm but working
            bool is_under_original = (a1.y > a2.y);
            int ptr = index1;
            Point temp1 = a[(ptr + a.size() - 1) % a.size()];
            Point temp2 = a[ptr];
            while (is_parallel_to_ray(ray, temp1, temp2)) {
                if (temp1.x < ray.p.x) {
                    return false;
                }
                --ptr;
                temp1 = a[(ptr + a.size() - 1) % a.size()];
                temp2 = a[(ptr + a.size()) % a.size()];
            }
            bool is_under_temp = (temp2.y > temp1.y);
            return ((is_under_original && !is_under_temp) || (!is_under_original && is_under_temp));
        }
        if (cmp(p1.x, ray.p.x)) throw 1; // if p1 (intersection) is the start of the ray => instant win
        bool is_b = is_between2(p1, a1, a2);
        return is_b;
    } else {
        // the segment is parallel to ray
        return false;
    }
}

int main() {
    Point p;
    int n;
    cin >> n >> p;
    vector<Point> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
//        have not worked:
//        if (cmp(a[i].y, p.y)) a[i].y -= 0.00000000000001;
    }
    for (int i = 0; i < n; ++i) { // p is on the edge of a
        if (a[i] == p) {
            cout << "YES\n";
            return 0;
        }
        if (Line(a[i], a[(i + 1) % n]).is_belong(p) && is_between(p, a[i], a[(i + 1) % n])) {
            cout << "YES\n";
            return 0;
        }
    }
    Ray ray = {p, Line(0, 1, -p.y)};
    int count = 0;
    for (int i = 0; i < n; ++i) {
        try {
            if (intersect_with_ray(ray, a[i], a[(i + 1) % n], i, a)) {
                ++count;
            }
        } catch (...) {
            cout << "YES\n";
            return 0;
        }
    }
    if (count % 2 == 0) {
        cout << "NO\n";
    }
    else {
        cout << "YES\n";
    }
    return 0;
}
