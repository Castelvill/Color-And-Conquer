#include "usefull.h"

string intToStr4(int integer){
    char buff[4];
    sprintf(buff, "%d", integer);
    return buff;
}
string intToStr8(int integer){
    char buff[8];
    sprintf(buff, "%d", integer);
    return buff;
}
double countDistance(double x1, double y1, double x2, double y2){
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}
double countDistance2(double & x1, double & y1, double & x2, double & y2){
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}
double randomDouble(double minValue, double maxValue){
    return minValue + ((rand() % 10000) / 10000.0) * (maxValue - minValue);
    //return minValue + ((double)rand() / (double)RAND_MAX) * (maxValue - minValue);
}
