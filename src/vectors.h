#ifndef VECTORS_H_INCLUDED
#define VECTORS_H_INCLUDED
#include <vector>

constexpr auto M_PI = 3.14159265358979323846;

using std::vector;

class vecXf{
public:
    vector <double> val;
    vecXf();
    vecXf(double a);
    vecXf(double a, double b);
    vecXf(double a, double b, double c);
    vecXf(double a, double b, double c, double d);
    vecXf(double a, double b, double c, double d, double e);
    vecXf(double a, double b, double c, double d, double e, double f);
    void set(double a);
    void set(double a, double b);
    void set(double a, double b, double c);
    void set(double a, double b, double c, double d);
    void set(double a, double b, double c, double d, double e);
    void set(double a, double b, double c, double d, double e, double f);
};

class vec2f{
public:
    double x, y, length;
    vec2f();
    vec2f(double x1, double y1);
    vec2f(double x1, double y1, double x2, double y2);
    void set(vec2f vec);
    void set(double x1, double y1);
    void set(double x1, double y1, double x2, double y2);
    bool isEqual(double x1, double y1);
    bool isEqual(vec2f vec);
    void setLength(double newLength);
    void translate(vec2f vec);
    void translate(double x, double y);
    void rotate(double degrees);
    double getAngle();
    double getAngle(vec2f from);
    double countLength();
    void normalize();
    void multiply(vec2f vec);
    void multiply(double var);
    void multiply(double var1, double var2);
    void divide(vec2f vec);
    void divide(double var);
    void divide(double var1, double var2);
};

class vec2i{
public:
    int x, y, length;
    vec2i();
    vec2i(int x1, int y1);
    vec2i(int x1, int y1, int x2, int y2);
    void set(int x1, int y1, int x2, int y2);
    void set(int x1, int y1);
    void set(vec2i vec);
    bool isEqual(int x1, int y1);
    void setLength(int newLength);
    void translate(vec2i vec);
    void translate(int x, int y);
    void rotate(double degrees);
    int countLength();
    void normalize();
};



#endif // VECTORS_H_INCLUDED
