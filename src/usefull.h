#ifndef USEFULL_H_INCLUDED
#define USEFULL_H_INCLUDED

#define ALLEGRO_UNSTABLE

#include <math.h>
#include <string>

using std::string;


string intToStr4(int integer);
string intToStr8(int integer);
double countDistance(double x1, double y1, double x2, double y2);
double countDistance2(double & x1, double & y1, double & x2, double & y2);
double randomDouble(double minValue, double maxValue);


#endif // USEFULL_H_INCLUDED
