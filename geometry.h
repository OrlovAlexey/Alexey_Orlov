#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <set>

//using namespace std;
using std::initializer_list;
using std::vector;
using std::pair;
using std::cin;
using std::cout;
using std::abs;
using std::set;
using std::pair;

bool cmp(double a, double b) {
    double eps = 0.000001;//1e-6
    if (abs(a - b) <= eps) {// return abs(a - b) <= eps
        return true;
    }
    else {
        return false;
    }
}

class Point {
public:
    double x;
    double y;
    Point() : x(0), y(0) {}
    Point(double, double);
    bool operator== (const Point&) const;
    bool operator!= (const Point&) const;
    ~Point() = default;
};

Point::Point(double X, double Y) : x(X), y(Y) {}

bool Point::operator== (const Point& point) const {
    if (cmp(point.x, x) && cmp(point.y, y)) {
        return true;
    }
    return false;
}

bool Point::operator!= (const Point& point) const {
    return !(*this == point);
}

double point_distance(Point point1, Point point2) {
    double x1, x2, y1, y2;
    x1 = point1.x;
    x2 = point2.x;
    y1 = point1.y;
    y2 = point2.y;
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// ax  + by + c = 0
class Line {
public:
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;
    Line() = default;
    Line(const Point&, const Point&);
    Line(double, double);
    Line(const Point&, double);
    bool operator== (const Line&) const;
    bool operator!= (const Line&) const;
    ~Line() = default;
};
                  //(0.0,0.0)       (0.0,1.0)
Line::Line(const Point& point1, const Point& point2) {
    a = point1.y - point2.y;
    b = point2.x - point1.x;
    c = (point1.x * point2.y) - (point2.x * point1.y);
}
                //0.0                1.0
Line::Line(double coeff_k, double coeff_b) {
    a = (cmp(coeff_k, 0.0) ? 0.0 : -coeff_k);
    b = 1.0;
    c = (cmp(coeff_b, 0.0) ? 0.0 : -coeff_b);// 0.0*x + 1.0*y - 1.0 = 0
}

Line::Line(const Point& point, double coeff_k) {
    a = (cmp(coeff_k, 0.0) ? 0 : -coeff_k);
    b = 1.0;
    c = -(a * point.x) - (b * point.y);
}

bool Line::operator== (const Line& line) const {
    double ratio_a, ratio_b, ratio_c;
    bool special_case_a = false, special_case_b = false, special_case_c = false;
    if (!cmp(line.a, 0.0)) {
        ratio_a = this->a / line.a;
    }
    else {
        if (!cmp(this->a, 0.0)) {
            return false;
        }
        special_case_a = true;
    }
    if (!cmp(line.b, 0.0)) {
        ratio_b = this->b / line.b;
    }
    else {
        if (!cmp(this->b, 0.0)) {
            return false;
        }
        special_case_b = true;
    }
    if (!cmp(line.c, 0.0)) {
        ratio_c = this->c / line.c;
    }
    else {
        if (!cmp(this->c, 0.0)) {
            return false;
        }
        special_case_c = true;
    }
    if (special_case_a) {
        ratio_a = ratio_b;
    }
    if (special_case_b) {
        ratio_b = ratio_a;
    }
    if (special_case_c) {
        ratio_c = ratio_a;
    }
    if (special_case_c && special_case_a) {
        ratio_c = ratio_b;
    }

    if (cmp(ratio_a, ratio_b) && cmp(ratio_b, ratio_c)) {
        return true;
    }
    return false;
}

bool Line::operator!= (const Line& line) const {
    return !(*this == line);
}

Point intersect(Line line1, Line line2) {
    Point result;
    double a1, a2, b1, b2, c1, c2;
    a1 = line1.a; // 0
    b1 = line1.b; // 1
    c1 = line1.c; // 0
    a2 = line2.a; // 1
    b2 = line2.b; // 0
    c2 = line2.c; // -1
    double det = a1 * b2 - a2 * b1;
    if (!cmp(det, 0.0)) {
        result.x = cmp((b1 * c2 - b2 * c1) / det, 0.0) ? 0.0 : (b1 * c2 - b2 * c1) / det;
        result.y = cmp((a2 * c1 - a1 * c2) / det, 0.0) ? 0.0 : (a2 * c1 - a1 * c2) / det;
        return result;
    }
    return Point(1000000001, 1000000001);
}


class Shape {
public:
    virtual double perimeter() = 0;
    virtual double area() = 0;
    virtual bool operator== (const Shape&) const = 0;
    virtual bool operator!= (const Shape&) const = 0;
    virtual bool isCongruentTo(const Shape&) = 0;
    virtual bool isSimilarTo(const Shape&) = 0;
    virtual bool containsPoint(Point point) = 0 ;

    virtual void rotate(Point center, double) = 0;
    virtual void reflex(Point) = 0;
    virtual void reflex(Line) = 0;
    virtual void scale(Point, double) = 0;
    virtual ~Shape() = 0;
};

Shape::~Shape() {}

class Polygon : public Shape {
protected:
    vector<Point> vertices;
public:
    Polygon() = default;
    Polygon(const vector<Point>&);
    Polygon(initializer_list<Point>);
    int verticesCount() const;
    const vector<Point> getVertices();
    bool isConvex() const;
    double perimeter() override;
    double area() override;
    bool operator== (const Shape&) const override;
    bool operator!= (const Shape&) const override;
    bool isCongruentTo(const Shape&) override;
    bool isSimilarTo(const Shape&) override;
    bool containsPoint(Point point) override;

    void rotate(Point, double) override;
    void reflex(Point) override;
    void reflex(Line) override;
    void scale(Point, double) override;
    ~Polygon() = default;
};

Polygon::Polygon(const vector<Point>& points) {
    int size = points.size();
    vertices.resize(size);
    for (int i = 0; i < size; ++i) {
        vertices[i] = points[i];
    }
}

Polygon::Polygon(initializer_list<Point> lst) {
    int size = lst.size();
    vertices.resize(size);
    std::copy(lst.begin(), lst.end(), vertices.begin());
}

int Polygon::verticesCount() const {
    return vertices.size();
}

const vector<Point> Polygon::getVertices() {
    return vertices;
}

bool Polygon::isConvex() const {
    int i, j, k;
    int size = vertices.size();
    double cross_product;
    int flag = 0;

    if (size < 3) {
        return false;
    }
    for (i = 0; i < size; ++i) {
        j = (i + 1) % size;
        k = (i + 2) % size;
        cross_product = (vertices[j].x - vertices[i].x) * (vertices[k].y - vertices[j].y);
        cross_product -= (vertices[j].y - vertices[i].y) * (vertices[k].x - vertices[j].x);
        if (cross_product < 0) {
            flag |= 1;
        }
        else if (cross_product > 0) {
            flag |= 2;
        }
        if (flag == 3) {
            return false;
        }
    }
    if (flag != 0)
        return true;
    else
        return false;
}

double Polygon::perimeter() {
    double result = 0.0;
    int size = vertices.size();
    for (int i = 1; i < size + 1; ++i) {
        result += point_distance(vertices[i-1], vertices[i % size]);
    }
    return result;
}

double Polygon::area() {
    double result = 0.0;
    int size = vertices.size();
    for (int i = 0; i < size; ++i) {
        result += vertices[i].x * vertices[(i+1) % size].y;
        result -= vertices[(i+1) % size].x * vertices[i].y;
    }
    return 0.5 * abs(result);
}

bool Polygon::operator== (const Shape& shape) const {
    Polygon poly;
    try {
        poly = dynamic_cast<const Polygon&>(shape);
    } catch (...) {
        return false;
    }
    int size1 = vertices.size();
    int size2 = poly.vertices.size();
    if (size1 != size2) {
        return false;
    }
    bool flag;
    for (int i = 0; i < size1; ++i) {
        flag = true;
        for (int j = 0; j < size2; ++j) {
            if (vertices[(i+j) % size1] != poly.vertices[j]) {
                flag = false;// Здесь по хорошему стоит делать сразу break, тогда в 99% случаев алгоритм станет линеен, а не квадратичен как сейчас
            }
        }
        if (flag) {
            return true;
        }
    }
    for (int i = 0; i < size1; ++i) {
        flag = true;
        for (int j = size2-1; j >= 0; --j) {
            if (vertices[(i - j + size1) % size1] != poly.vertices[j]) {
                flag = false;
            }
        }
        if (flag) {
            return true;
        }
    }
    return false;
//    Point temp;
//    for (int i = 0; i < size1; ++i) {
//        temp = vertices[i];
//        if (std::find(poly.vertices.begin(), poly.vertices.end(), temp) == poly.vertices.end()) {
//            return false;
//        }
//    }
//    return true;
}

bool Polygon::operator!= (const Shape& shape) const {
    return !(*this == shape);
}

// Эквивалентные расстояния не гарантируют подобие, углы при этом можно сделать какие угодно, см классические цепочки
bool Polygon::isCongruentTo(const Shape& shape) {
    Polygon poly;
    try {
        poly = dynamic_cast<const Polygon&>(shape);
    } catch (...) {// - Кажется у вас outOfRange - хмм, кидай false... ладно, это следующий семестр 
        return false;
    }
    vector<double> sides1, sides2;
    int size1 = this->vertices.size();
    int size2 = poly.vertices.size();
    if (size1 != size2) {
        return false;
    }
    sides1.clear();
    for (int i = 0; i < size1; ++i) {
        sides1.push_back(point_distance(vertices[i],vertices[(i+1) % size1]));// Для начала векторам можно было resize сделать.
    }
    sides2.clear();
    for (int i = 0; i < size2; ++i) {
        sides2.push_back(point_distance(poly.vertices[i],poly.vertices[(i+1) % size1]));
    }
    bool flag;
    for (int i = 0; i < size1; ++i) {
        flag = true;
        for (int j = 0; j < size2; ++j) {
            if (!cmp(sides1[(i + j) % size1], sides2[j])) {
                flag = false;// А если ещё сюда добавить break, то можно и не думать о создании векторов и кешировании вычислений
            }
        }
        if (flag) {
            return true;
        }
    }
    for (int i = 0; i < size1; ++i) {
        flag = true;
        for (int j = size2-1; j >= 0; --j) {
            if (!cmp(sides1[(i - j + size1) % size1], sides2[j])) {
                flag = false;
            }
        }
        if (flag) {
            return true;
        }
    }
    return false;
    

bool Polygon::isSimilarTo(const Shape& shape) {
    Polygon poly;
    try {
        poly = dynamic_cast<const Polygon&>(shape);
    } catch (...) {
        return false;
    }
    vector<double> sides1, sides2;
//    std::multiset<double> cosinuses1, cosinuses2;
//    std::multiset<double> sinuses1, sinuses2;

    int size1 = this->vertices.size();
    int size2 = poly.vertices.size();
    if (size1 != size2) {
        return false;
    }
    sides1.clear();
    for (int i = 0; i < size1; ++i) {
        sides1.push_back(point_distance(vertices[i],vertices[(i+1) % size1]));
    }
    sides2.clear();
    for (int i = 0; i < size2; ++i) {
        sides2.push_back(point_distance(poly.vertices[i],poly.vertices[(i+1) % size1]));
    }
    bool flag;
    for (int i = 0; i < size1; ++i) {
        flag = true;
        for (int j = 0; j < size2; ++j) {
            if (!cmp(sides1[(i + j) % size1] / sides1[(i + j + 1) % size1], sides2[j] / sides2[(j + 1) % size2])) { //vertices[(i+j) % size1] != poly.vertices[j]
                flag = false;// Аналогично
            }
        }
        if (flag) {
            return true;
        }
    }
    for (int i = 0; i < size1; ++i) {
        flag = true;
        for (int j = 0; j < size2; ++j) {
            if (!cmp(sides1[(i + j) % size1] / sides1[(i + j + 1) % size1], sides2[(j + 1) % size2] / sides2[j])) { //vertices[(i+j) % size1] != poly.vertices[j]
                flag = false;
            }
        }
        if (flag) {
            return true;
        }
    }
    for (int i = 0; i < size1; ++i) {
        flag = true;
        for (int j = size2-1; j >= 0; --j) {
            if (!cmp(sides1[(i - j + size1) % size1] / sides1[(i - j + size1 + 1) % size1], sides2[j] / sides2[(j+1) % size2])) { //vertices[(i - j + size1) % size1] != poly.vertices[j]
                flag = false;
            }
        }
        if (flag) {
            return true;
        }
    }
    for (int i = 0; i < size1; ++i) {
        flag = true;
        for (int j = size2-1; j >= 0; --j) {
            if (!cmp(sides1[(i - j + size1) % size1] / sides1[(i - j + size1 + 1) % size1], sides2[(j+1) % size2] / sides2[j])) { //vertices[(i - j + size1) % size1] != poly.vertices[j]
                flag = false;
            }
        }
        if (flag) {
            return true;
        }
    }
    return false;
}


bool Polygon::containsPoint(Point point) {
    int size = vertices.size();
    bool flag = 0;
    double S, S1, S2, S3;
    int i1, i2;
    for (int i = 0; i < size; ++i) {
        i1 = (i+1) % size;
        while (!flag) {
            i2 = (i1 + 1) % size;
            if (i2 == i) {
                break;
            }
            S = abs (vertices[i1].x * (vertices[i2].y - vertices[i].y) +
                     vertices[i2].x * (vertices[i].y - vertices[i1].y) +
                     vertices[i].x  * (vertices[i1].y - vertices[i2].y));
            S1 = abs(vertices[i1].x * (vertices[i2].y - point.y) +
                    vertices[i2].x * (point.y - vertices[i1].y) +
                    point.x * (vertices[i1].y - vertices[i2].y));
            S2 = abs (vertices[i].x * (vertices[i2].y - point.y) +
                      vertices[i2].x * (point.y - vertices[i].y) +
                      point.x * (vertices[i].y - vertices[i2].y));
            S3 = abs (vertices[i1].x * (vertices[i].y - point.y) +
                      vertices[i].x * (point.y - vertices[i1].y) +
                      point.x * (vertices[i1].y - vertices[i].y));
            if (cmp(S, S1 + S2 + S3)) {
                flag = true;
                break;
            }
            i1 = i1 + 1;
            if (i1 >= size) {
                break;
            }
        }
        if (!flag) {
            break;
        }
    }
    return flag;
}

void Polygon::rotate(Point center, double angle) {
    double angleRadian = angle * M_PI / 180;
    double cosinus = cos(angleRadian);
    double sinus = sin(angleRadian);
    for (size_t j = 0; j < vertices.size(); j++)
    {
        double x = ((vertices[j].x - center.x) * cosinus - (vertices[j].y - center.y) * sinus + center.x);
        double y = ((vertices[j].x - center.x) * sinus + (vertices[j].y - center.y) * cosinus + center.y);
        if (cmp(x,0.0)) {
            x = 0.0;
        }
        if (cmp(y,0.0)) {
            y = 0.0;
        }
        vertices[j].x = x;
        vertices[j].y = y;
    }
}

void Polygon::reflex(Point point) {
    this->scale(point, -1.0);
}
/*
struct v {
    double x, y;
    v () = default;
    v (double X, double Y) {
        x = X;
        y = Y;
    }
}; //вектор
v operator+ (v a, v b) {
    return v(a.x + b.x, a.y + b.y);
}
v operator- (v a, v b) {
    return v(a.x - b.x, a.y - b.y);
}
v operator* (double k, v a) {
    a.x *= k;
    a.y *= k;
    return a;
} // умножение на скаляр
double operator* (v a, v b) {
    return a.x * b.x + a.y * b.y;
} // скалярное
v pr (Line line, v& c) {
    double A = line.a, B = line.b, C = line.c;
    v a,b;
    if (cmp(A, 0.0)) {
        a = v(0, -C / B);
        b = v(1, -(C + A) / B);
    }
    else {
        a = v(-C / A, 0);
        b = v(-(C + B) / A, 1);
    }
//    c = c - a;
    return b + (a * b / (a.x * a.x + a.y * a.y)) * a;
} // проекция b на а
v reflect (Line line, v c) {
    return c + 2 * (pr(line, c) - c);
}
*/
Point reflect_over_Line(Line line, Point point) {  // 0.0*x + 1.0*y - 1.0 = 0
    Point result;
    double m, d;
    if (!cmp(line.b, 0.0)) {
        m = -line.a / line.b;
        d = (point.x + (point.y + line.c / line.b) * m) / (1 + m * m);
        result.x = 2 * d - point.x;
        result.y = 2 * d * m - point.y - 2 * line.c / line.b;
    }
    else {
        result.x = 2 * (-line.c / line.a - point.x) + point.x;
        result.y = point.y;
    }

//    v vec(point.x, point.y);
//    vec = reflect(line, vec);
//    result.x = vec.x;
//    result.y = vec.y;
    return result;
}
void Polygon::reflex(Line axis) {
    int size = vertices.size();
    for (int i = 0; i < size; ++i) {
        vertices[i] = reflect_over_Line(axis, vertices[i]);
    }
}

void Polygon::scale(Point center, double coeff) {
    double x0 = center.x, y0 = center.y;
    int size = vertices.size();
    for (int i = 0; i < size; ++i) {
        vertices[i].x = x0 + coeff * (vertices[i].x - x0);
        vertices[i].y = y0 + coeff * (vertices[i].y - y0);
    }
}


class Ellipse : public Shape {
public:
    double a, b, c, e;// Что это (имена), и почему в public?
    Point f1, f2;
    
    Ellipse() = default;
    Ellipse(Point, Point, double);
    std::pair<Point,Point> focuses() const;
    std::pair<Line, Line> directrices() const;
    
    double eccentricity() const;
    Point center() const;
    double perimeter() override;
    double area() override;
    bool operator== (const Shape&) const override;
    bool operator!= (const Shape&) const override;
    bool isCongruentTo(const Shape&) override;
    bool isSimilarTo(const Shape&) override;
    bool containsPoint(Point point) override;

    void rotate(Point, double) override;
    void reflex(Point) override;
    void reflex(Line) override;
    void scale(Point, double) override;
    ~Ellipse() = default;
};

Ellipse::Ellipse(Point focus1, Point focus2, double distance) {
    c = 0.5 * point_distance(focus1, focus2);
    a = 0.5 * distance;
    b = sqrt(a * a - c * c);
    e = c / a;
    f1 = focus1;
    f2 = focus2;
}

pair<Point,Point> Ellipse::focuses() const {
    return pair(f1, f2);
}

pair<Line, Line> Ellipse::directrices() const {
    Point center((f1.x + f2.x) / 2, (f1.y + f2.y) / 2);
    Line main_line(f1, f2);
    Point p1(center.x + a/e * cos(main_line.a / main_line.b), center.y + a/e * sin(main_line.a / main_line.b));
    Point p2(center.x - a/e * cos(main_line.a / main_line.b), center.y - a/e * sin(main_line.a / main_line.b));
    Line d1,d2;
    if (!cmp(main_line.a, 0.0)) {
        d1 = Line(p1, -main_line.b / main_line.a);
        d2 = Line(p2, -main_line.b / main_line.a);
    }
    else {
        Point p3(a/e, 0);
        Point p4(a/e, 1);
        d1 = Line(p3, p4);
        Point p5(-a/e, 0);
        Point p6(-a/e, 1);
        d2 = Line(p5, p6);
    }
    return pair(d1, d2);
}

double Ellipse::eccentricity() const {
    return e;
}

Point Ellipse::center() const {
    return Point((f1.x + f2.x) / 2, (f1.y + f2.y) / 2);
}

double Ellipse::perimeter() {
    return 4 * a * std::comp_ellint_2(e);
}

double Ellipse::area() {
    return M_PI * a * b;
}

bool Ellipse::operator== (const Shape& shape) const {
    Ellipse elp;
    try {
        elp = dynamic_cast<const Ellipse&>(shape);
    } catch (...) {
        return false;
    }
    if (cmp(e, elp.e) && cmp(f1.x, elp.f1.x) &&  cmp(f1.y, elp.f1.y) &&  cmp(f2.x, elp.f2.x) &&  cmp(f2.y, elp.f2.y)) {
        return true;
    }
    return false;
}

bool Ellipse::operator!= (const Shape& shape) const {
    return !(*this == shape);
}

bool Ellipse::isCongruentTo(const Shape& shape) {
    Ellipse elp;
    try {
        elp = dynamic_cast<const Ellipse&>(shape);
    } catch (...) {
        return false;
    }
    if (cmp(e, elp.e) && cmp(point_distance(f1, f2), point_distance(elp.f1, elp.f2))) {
        return true;
    }
    return false;
}

bool Ellipse::isSimilarTo(const Shape& shape) {
    Ellipse elp;
    try {
        elp = dynamic_cast<const Ellipse&>(shape);
    } catch (...) {
        return false;
    }
    if (cmp(e, elp.e)) {
        return true;
    }
    return false;
}

bool Ellipse::containsPoint(Point point) {
    if (point_distance(f1, point) + point_distance(f2, point) <= 2 * a) {
        return true;
    }
    return false;
}

void Ellipse::rotate(Point center, double angle) {
    double angleRadian = angle * M_PI / 180;
    double x0 = center.x;
    double y0 = center.y;
    double x1 = (f1.x - x0) * cos(angleRadian) - (f1.y - y0) * sin(angleRadian) + x0;
    double y1 = (f1.x - x0) * sin(angleRadian) + (f1.y - y0) * cos(angleRadian) + y0;
    double x2 = (f2.x - x0) * cos(angleRadian) - (f2.y - y0) * sin(angleRadian) + x0;
    double y2 = (f2.x - x0) * sin(angleRadian) + (f2.y - y0) * cos(angleRadian) + y0;
    f1.x = x1;
    f1.y = y1;
    f2.x = x2;
    f2.y = y2;
}

void Ellipse::reflex(Point center) {
    this->scale(center, -1);
}

void Ellipse::reflex(Line axis) {
    f1 = reflect_over_Line(axis, f1);
    f2 = reflect_over_Line(axis, f2);
}

void Ellipse::scale(Point center, double coeff) {
    double x0 = center.x, y0 = center.y;
    f1.x = x0 + coeff * (f1.x - x0);
    f1.y = y0 + coeff * (f1.y - y0);
    f2.x = x0 + coeff * (f2.x - x0);
    f2.y = y0 + coeff * (f2.y - y0);
    c = point_distance(f1, f2);
    a = abs(coeff) * a;
    b = sqrt(a * a - c * c);
}



class Circle : public Ellipse {
public:
    using Ellipse::Ellipse;
    Point c;// Зачем, они ведь уже существуют внутри Элипса
    double r;
    
    Circle() = default;
    Circle(Point, double);
    double radius() const;
    double perimeter() override;
    double area() override;
    bool operator== (const Shape&) const override;
    bool operator!= (const Shape&) const override;
    bool isCongruentTo(const Shape&) override;
    bool isSimilarTo(const Shape&) override;
    bool containsPoint(Point point) override;

    void rotate(Point, double) override;
    void reflex(Point) override;
    void reflex(Line) override;
    void scale(Point, double) override;
    ~Circle() = default;
};

Circle::Circle(Point center, double radius) {
    c = center;
    r = radius;
    f1 = c;
    f2 = f1;
    a = r;
}

double Circle::radius() const {
    return r;
}

double Circle::perimeter() {
    return 2.0 * M_PI * r;
}

double Circle::area() {
    return M_PI * r * r;
}

bool Circle::operator== (const Shape& shape) const {
    Circle cir;
    try {
        cir = dynamic_cast<const Circle&>(shape);
    } catch (...) {
        return false;
    }
    if (c == cir.c && cir.r == r) {
        return true;
    }
    return false;
}

bool Circle::operator!= (const Shape& shape) const {
    return !(*this == shape);
}

bool Circle::isCongruentTo(const Shape& shape) {
    Circle cir;
    try {
        cir = dynamic_cast<const Circle&>(shape);
    } catch (...) {
        return false;
    }
    if (cir.r == r) {
        return true;
    }
    return false;
}

bool Circle::isSimilarTo(const Shape& shape) {
    Circle cir;
    try {
        cir = dynamic_cast<const Circle&>(shape);
    } catch (...) {
        return false;
    }
    return true;
}

bool Circle::containsPoint(Point point) {
    if (point_distance(point, c) <= r) {
        return true;
    }
    return false;
}

void Circle::rotate(Point center, double angle) {
    double angleRadian = angle * M_PI / 180;
    double x0 = center.x;
    double y0 = center.y;
    double x = (c.x - x0) * cos(angleRadian) - (c.y - y0) * sin(angleRadian) + x0;
    double y = (c.x - x0) * sin(angleRadian) + (c.y - y0) * cos(angleRadian) + y0;
    c.x = x;
    c.y = y;
    f1 = c, f2 = c;
}

void Circle::reflex(Point center) {
    this->scale(center, -1.0);
}

void Circle::reflex(Line axis) {
    c = reflect_over_Line(axis, c);
    f1 = c, f2 = c;
}

void Circle::scale(Point center, double coeff) {
    double x0 = center.x, y0 = center.y;
    c.x = x0 + coeff * (c.x - x0);
    c.y = y0 + coeff * (c.y - y0);
    r *= abs(coeff);
    f1 = c, f2 = c;
}


class Rectangle : public Polygon {
public:
    using Polygon::Polygon;// Сомнительная фраза, потому что так можно создать прямоугольник из 10 точек без проверки, тут тогда явно нужно делать другие конструкторы
    Point a, b, c , d;
    Rectangle() = default;
    Rectangle(Point, Point, double k);
    Point center() const;
    std::pair<Line, Line> diagonals() const;
    double perimeter() override;
    double area() override;
    using Polygon::operator==;
    using Polygon::operator!=;
//    bool operator== (const Shape&) const override;
//    bool operator!= (const Shape&) const override;
//    bool isCongruentTo(const Shape&) override;
//    bool isSimilarTo(const Shape&) override;
    using Polygon::isCongruentTo;
    using Polygon::isSimilarTo;
    using Polygon::containsPoint;
//    bool containsPoint(Point point) override;

    void rotate(Point, double) override;
    void reflex(Point) override;
    void reflex(Line) override;
    void scale(Point, double) override;
    ~Rectangle() = default;
};


Rectangle::Rectangle(Point point1, Point point2, double k) {
    a = point1;
    c = point2;
//    double beta = (point_distance(a, c) / sqrt(1 + k*k));
//    double alpha = M_PI - atan(k) - atan((c.y - a.y) / (c.x - a.x));
//    double D = (c.x + c.y * tan(alpha)) * (c.x + c.y * tan(alpha)) - (c.y * c.y + c.x * c.x - beta * beta);
//    double x0 = ((c.x + c.y * tan(alpha)) +- sqrt(D)) / (1 + tan(alpha) * tan(alpha));
//    double y0 = x0 * tan(alpha);

    Line h(a, c);
    double another_k1 = tan(atan(k) + atan(-h.a / h.b));
    Line l1(a, another_k1);
    Line l2;
    if (!cmp(another_k1, 0.0)) {
        l2 = Line(c, -1.0 / another_k1);
    }
    else {
        l2 = Line(Point(c.x, 0), Point(c.x, 1));
    }
    b = intersect(l1, l2);

    double another_k2 = tan(atan(-h.a / h.b) - atan(1.0/k));
    Line l3(a, another_k2);
    Line l4;
    if (!cmp(another_k2, 0.0)) {
        l4 = Line(c, -1.0 / another_k2);
    }
    else {
        l4 = Line(Point(c.x, 0), Point(c.x, 1));
    }
    d = intersect(l3, l4);

    vertices.clear();
    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
    vertices.push_back(d);
}

Point Rectangle::center() const {
    return Point((a.x + c.x) / 2, (a.y + c.y) / 2);
}

std::pair<Line, Line> Rectangle::diagonals() const {
    return pair(Line(a, c),Line(b, d));
}

double Rectangle::perimeter() {
    return 2.0 * (point_distance(a,b) + point_distance(b, c));
}

double Rectangle::area() {
    return point_distance(a,b) * point_distance(b, c);
}
/*
bool Rectangle::operator== (const Shape& shape) const {
    Rectangle rect;
    try {
        rect = dynamic_cast<const Rectangle&>(shape);
    } catch (...) {
        return false;
    }

//    rect.vertices.clear();
//    rect.vertices.push_back(rect.a);
//    rect.vertices.push_back(rect.b);
//    rect.vertices.push_back(rect.c);
//    rect.vertices.push_back(rect.d);
    bool flag;
    for (int i = 0; i < 4; ++i) {
        flag = true;
        for (int j = 0; j < 4; ++j) {
            if (vertices[(i+j) % 4] != rect.vertices[j]) {
                flag = false;
            }
        }
        if (flag) {
            return true;
        }
//        if (std::find(rect.vertices.begin(), rect.vertices.end(), temp) == rect.vertices.end()) {
//            return false;
//        }
    }
    rect.vertices.clear();
    rect.vertices.push_back(rect.d);
    rect.vertices.push_back(rect.c);
    rect.vertices.push_back(rect.b);
    rect.vertices.push_back(rect.a);
    for (int i = 0; i < 4; ++i) {
        flag = true;
        for (int j = 0; j < 4; ++j) {
            if (vertices[(i+j) % 4] != rect.vertices[j]) {
                flag = false;
            }
        }
        if (flag) {
            return true;
        }
    }
    return false;
}

bool Rectangle::operator!= (const Shape& shape) const {
    return !(*this == shape);
}
*/
//bool Rectangle::isCongruentTo(const Shape& shape) {}

//bool Rectangle::isSimilarTo(const Shape& shape) {}

void Rectangle::rotate(Point center, double angle) {
    (*this).Polygon::rotate(center, angle);
    a = vertices[0];
    b = vertices[1];
    c = vertices[2];
    d = vertices[3];
}

void Rectangle::reflex(Point center) {
    (*this).Polygon::reflex(center);
    a = vertices[0];
    b = vertices[1];
    c = vertices[2];
    d = vertices[3];
}

void Rectangle::reflex(Line axis) {
    (*this).Polygon::reflex(axis);
    a = vertices[0];
    b = vertices[1];
    c = vertices[2];
    d = vertices[3];
}

void Rectangle::scale(Point center, double coeff) {
    (*this).Polygon::scale(center, coeff);
    a = vertices[0];
    b = vertices[1];
    c = vertices[2];
    d = vertices[3];
}
//void Rectangle::scale(Point center, double coeff) {
//    double x0 = center.x;
//    double y0 = center.y;
//    a.x = x0 + coeff * (a.x - x0);
//    a.y = y0 + coeff * (a.y - y0);
//    b.x = x0 + coeff * (b.x - x0);
//    b.y = y0 + coeff * (b.y - y0);
//    c.x = x0 + coeff * (c.x - x0);
//    c.y = y0 + coeff * (c.y - y0);
//    d.x = x0 + coeff * (d.x - x0);
//    d.y = y0 + coeff * (d.y - y0);
//}


class Square : public Rectangle {
public:
    Square() = default;
    Square(Point, Point);
    Circle inscribedCircle() const;
    Circle circumscribedCircle() const;
    double perimeter() override;
    double area() override;
    using Polygon::operator==;
//    bool operator== (const Shape&) const override;
    using Polygon::operator!=;
//    bool operator!= (const Shape&) const override;
//    bool isCongruentTo(const Shape&) override;
    using Polygon::isCongruentTo;
    using Polygon::isSimilarTo;
//    bool isSimilarTo(const Shape&) override;
//    bool containsPoint(Point point) override;
    using Polygon::containsPoint;

    void rotate(Point, double) override;
    void reflex(Point) override;
    void reflex(Line) override;
    void scale(Point, double) override;
    ~Square() = default;
};

Square::Square(Point point1, Point point2) {
    a = point1;
    c = point2;
    double angleRadian = M_PI / 2;
    double cosinus = cos(angleRadian);
    double sinus = sin(angleRadian);
    double x0 = (a.x + c.x) / 2;
    double y0 = (a.y + c.y) / 2;
    double x1 = ((a.x - x0) * cosinus - (a.y - y0) * sinus + x0);
    if (cmp(x1, 0.0)) x1 = 0.0;
    double y1 = ((a.x - x0) * sinus + (a.y - y0) * cosinus + y0);
    if (cmp(y1, 0.0)) y1 = 0.0;
    double x2 = ((c.x - x0) * cosinus - (c.y - y0) * sinus + x0);
    if (cmp(x2, 0.0)) x2 = 0.0;
    double y2 = ((c.x - x0) * sinus + (c.y - y0) * cosinus + y0);
    if (cmp(y2, 0.0)) y2 = 0.0;
    d = Point(x1, y1);
    b = Point(x2, y2);
    /*
        double k = 1;

//    double beta = (point_distance(a, c) / sqrt(1 + k*k));
//    double alpha = M_PI - atan(k) - atan((c.y - a.y) / (c.x - a.x));
//    double D = (c.x + c.y * tan(alpha)) * (c.x + c.y * tan(alpha)) - (c.y * c.y + c.x * c.x - beta * beta);
//    double x0 = ((c.x + c.y * tan(alpha)) +- sqrt(D)) / (1 + tan(alpha) * tan(alpha));
//    double y0 = x0 * tan(alpha);

    Line h(a, c);
    double another_k1 = tan(atan(k) + atan(-h.a / h.b));
    Line l1(a, another_k1);
    Line l2;
    if (!cmp(another_k1, 0.0)) {
        l2 = Line(c, -1.0 / another_k1);
    }
    else {
        l2 = Line(Point(c.x, 0), Point(c.x, 1));
    }
    b = intersect(l1, l2);

    double another_k2 = tan(atan(-h.a / h.b) - atan(1.0/k));
    Line l3(a, another_k2);
    Line l4;
    if (!cmp(another_k2, 0.0)) {
        l4 = Line(c, -1.0 / another_k2);
    }
    else {
        l4 = Line(Point(c.x, 0), Point(c.x, 1));
    }
    d = intersect(l3, l4);
    */
    vertices.clear();
    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
    vertices.push_back(d);
}

Circle Square::circumscribedCircle() const {
    return Circle(Point((a.x + c.x) / 2, (a.y + c.y) / 2), point_distance(a,c));
}

Circle Square::inscribedCircle() const {
    return Circle(Point((a.x + c.x) / 2, (a.y + c.y) / 2), point_distance(a, b) / 2);
}

double Square::perimeter() {
    return 4 * point_distance(a, b);
}

double Square::area() {
    return point_distance(a, b) * point_distance(a, b);
}

//bool Square::operator== (const Shape& shape) const {
//    Rectangle square;
//    try {
//        square = dynamic_cast<const Rectangle&>(shape);
//    } catch (...) {
//        return false;
//    }
////    square.vertices.clear();
////    square.vertices.push_back(square.a);
////    square.vertices.push_back(square.b);
////    square.vertices.push_back(square.c);
////    square.vertices.push_back(square.d);
//    bool flag;
//    for (int i = 0; i < 4; ++i) {
//        flag = true;
//        for (int j = 0; j < 4; ++j) {
//            if (vertices[(i+j) % 4] != square.vertices[j]) {
//                flag = false;
//            }
//        }
//        if (flag) {
//            return true;
//        }
//
//    }
//    square.vertices.clear();
//    square.vertices.push_back(square.d);
//    square.vertices.push_back(square.c);
//    square.vertices.push_back(square.b);
//    square.vertices.push_back(square.a);
//    for (int i = 0; i < 4; ++i) {
//        flag = true;
//        for (int j = 0; j < 4; ++j) {
//            if (vertices[(i+j) % 4] != square.vertices[j]) {
//                flag = false;
//            }
//        }
//        if (flag) {
//            return true;
//        }
//    }
//    return false;
//}

//bool Square::operator!= (const Shape& shape) const {
//    return !(*this == shape);
//}

//bool Square::isSimilarTo(const Shape& shape) {
//    Square square;
//    try {
//        square = dynamic_cast<const Square&>(shape);
//    } catch (...) {
//        return false;
//    }
//    return true;
//}

void Square::rotate(Point center, double angle) {
    (*this).Polygon::rotate(center, angle);
    a = vertices[0];
    b = vertices[1];
    c = vertices[2];
    d = vertices[3];
}

void Square::reflex(Point center) {
    (*this).Polygon::reflex(center);
    a = vertices[0];
    b = vertices[1];
    c = vertices[2];
    d = vertices[3];
}

void Square::reflex(Line axis) {
    (*this).Polygon::reflex(axis);
    a = vertices[0];
    b = vertices[1];
    c = vertices[2];
    d = vertices[3];
}

void Square::scale(Point center, double coeff) {
    (*this).Polygon::scale(center, coeff);
    a = vertices[0];
    b = vertices[1];
    c = vertices[2];
    d = vertices[3];
}


class Triangle : public Polygon {
public:
    Point a, b, c;
    Triangle() = default;
    Triangle(Point, Point, Point);
    Circle circumscribedCircle();
    Circle inscribedCircle();
    Point centroid();
    Point orthocenter();
    Line EulerLine();
    Circle ninePointsCircle();
    double perimeter() override;
    double area() override;
    using Polygon::operator==;
    using Polygon::operator!=;
//    bool operator== (Shape&) override;
//    bool operator!= (Shape&) override;
    using Polygon::isCongruentTo;
//    bool isCongruentTo(const Shape&) override;
    using Polygon::isSimilarTo;
//    bool isSimilarTo(const Shape&) override;
    using Polygon::containsPoint;
//    bool containsPoint(Point point) override;

    void rotate(Point, double) override;
    void reflex(Point) override;
    void reflex(Line) override;
    void scale(Point, double) override;
    ~Triangle() = default;
};

Triangle::Triangle(Point point1, Point point2, Point point3) {
    a = point1;
    b = point2;
    c = point3;
    vertices.clear();
    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
}

Circle Triangle::circumscribedCircle() {
    double side1 = point_distance(a, b);
    double side2 = point_distance(b, c);
    double side3 = point_distance(c, a);
    double radius = 0.25 * side1 * side2 * side3 / area();
    double x1 = a.x, x2 = b.x, x3 = c.x, y1 = a.y, y2 = b.y, y3 = c.y;
    double det = x1 * (y2 - y3) - y1 * (x2 - x3) + (x2 * y3 - x3 * y2);
    double det1 = (x1 * x1 + y1 * y1) * (y2 - y3) - y1 * (x2 * x2 + y2 * y2 - x3 * x3 - y3 * y3) + ((x2 * x2 + y2 * y2) * y3 - (x3 * x3 + y3 * y3) * y2);
    double det2 = x1 * (x2 * x2 + y2 * y2 - x3 * x3 - y3 * y3) - (x1 * x1 + y1 * y1) * (x2 - x3) + (x2 * (x3 * x3 + y3 * y3) - x3 * (x2 * x2 + y2 * y2));
    Point center(0.5 * det1 / det, 0.5 * det2 / det);
    return Circle(center, radius);
}

Circle Triangle::inscribedCircle() {
    double radius = 2 * area() / perimeter();
    double C = point_distance(a, b);
    double A = point_distance(b, c);
    double B = point_distance(c, a);
    Point center ((A * a.x + B * b.x + C * c.x) / (A + B + C), (A * a.y + B * b.y + C * c.y) / (A + B + C));
    return Circle(center, radius);
}

Point Triangle::centroid() {
    return Point((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3);
}

Point Triangle::orthocenter() {
    Line l1(a, b);//x=0
    Line l2(b, c);//y - 1=0
    Line p1;
    if (!cmp(l2.a, 0.0)) {
        p1 = Line(a, l2.b / l2.a);
    }
    else {
        p1 = Line(Point(a.x, 0.0), Point(a.x, 1.0));
    }
    Line p2;
    if (!cmp(l1.a, 0.0)) {
        p2 = Line(c, l1.b / l1.a);
    }
    else {
        p2 = Line(Point(c.x, 0.0), Point(c.x, 1.0));
    }
    return intersect(p1, p2);
}

Line Triangle::EulerLine() {
    return Line(orthocenter(), circumscribedCircle().center());
}

Circle Triangle::ninePointsCircle() {
    Point o1 = orthocenter();
    Point o2 = circumscribedCircle().center();
    Point center((o1.x + o2.x) / 2, (o1.y + o2.y) / 2);
    Point mid((a.x + b.x) / 2, (a.y + b.y) / 2);
    return Circle(center, point_distance(center, mid));
}

double Triangle::perimeter() {
    double side1 = point_distance(a, b);
    double side2 = point_distance(b, c);
    double side3 = point_distance(c, a);
    return side1 + side2 + side3;
}

double Triangle::area() {
    double side1 = point_distance(a, b);
    double side2 = point_distance(b, c);
    double side3 = point_distance(c, a);
    double p = (side1 + side2 + side3) / 2;
    return sqrt(p * (p - side1) * (p - side2) * (p - side3));
}

void Triangle::rotate(Point center, double angle) {
    (*this).Polygon::rotate(center, angle);
    a = vertices[0];
    b = vertices[1];
    c = vertices[2];
}

void Triangle::reflex(Point center) {
    (*this).Polygon::reflex(center);
    a = vertices[0];
    b = vertices[1];
    c = vertices[2];
}

void Triangle::reflex(Line axis) {
    (*this).Polygon::reflex(axis);
    a = vertices[0];
    b = vertices[1];
    c = vertices[2];
}

void Triangle::scale(Point center, double coeff) {
    (*this).Polygon::scale(center, coeff);
    a = vertices[0];
    b = vertices[1];
    c = vertices[2];
}
