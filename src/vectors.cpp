#include "vectors.h"
#include <math.h>


vecXf::vecXf(){
    val.clear();
}
vecXf::vecXf(double a){
    val.push_back(a);
}
vecXf::vecXf(double a, double b){
    val.push_back(a);
    val.push_back(b);
}
vecXf::vecXf(double a, double b, double c){
    val.push_back(a);
    val.push_back(b);
    val.push_back(c);
}
vecXf::vecXf(double a, double b, double c, double d){
    val.push_back(a);
    val.push_back(b);
    val.push_back(c);
    val.push_back(d);
}
vecXf::vecXf(double a, double b, double c, double d, double e){
    val.push_back(a);
    val.push_back(b);
    val.push_back(c);
    val.push_back(d);
    val.push_back(e);
}
vecXf::vecXf(double a, double b, double c, double d, double e, double f){
    val.push_back(a);
    val.push_back(b);
    val.push_back(c);
    val.push_back(d);
    val.push_back(e);
    val.push_back(f);
}
void vecXf::set(double a){
    val.clear();
    val.push_back(a);
}
void vecXf::set(double a, double b){
    val.clear();
    val.push_back(a);
    val.push_back(b);
}
void vecXf::set(double a, double b, double c){
    val.clear();
    val.push_back(a);
    val.push_back(b);
    val.push_back(c);
}
void vecXf::set(double a, double b, double c, double d){
    val.clear();
    val.push_back(a);
    val.push_back(b);
    val.push_back(c);
    val.push_back(d);
}
void vecXf::set(double a, double b, double c, double d, double e){
    val.clear();
    val.push_back(a);
    val.push_back(b);
    val.push_back(c);
    val.push_back(d);
    val.push_back(e);
}
void vecXf::set(double a, double b, double c, double d, double e, double f){
    val.clear();
    val.push_back(a);
    val.push_back(b);
    val.push_back(c);
    val.push_back(d);
    val.push_back(e);
    val.push_back(f);
}


vec2f::vec2f(){
    x = 0.0;
    y = 0.0;
    length = 0.0;
}
vec2f::vec2f(double x1, double y1){
    x = x1;
    y = y1;
    length = 0.0;
}
vec2f::vec2f(double x1, double y1, double x2, double y2){
    x = x2 - x1;
    y = y2 - y1;
    length = countLength();
}
void vec2f::set(vec2f vec){
    x = vec.x;
    y = vec.y;
    length = 0.0;
}
void vec2f::set(double x1, double y1){
    x = x1;
    y = y1;
    length = 0.0;
}
void vec2f::set(double x1, double y1, double x2, double y2){
    x = x2 - x1;
    y = y2 - y1;
    length = countLength();
}
bool vec2f::isEqual(double x1, double y1){
    if(x == x1 && y == y1)
        return true;
    return false;
}
bool vec2f::isEqual(vec2f vec){
    if(x == vec.x && y == vec.y)
        return true;
    return false;
}
double vec2f::countLength(){
    return sqrt(pow(x, 2) + pow(y, 2));
}
void vec2f::translate(vec2f vec){
    x += vec.x;
    y += vec.y;
}
void vec2f::translate(double tx, double ty){
    x += tx;
    y += ty;
}
void vec2f::setLength(double newLength){
    length = newLength;
}
void vec2f::rotate(double degrees) {
	double theta = degrees / 180.0 * M_PI;
	double c = cos(theta);
	double s = sin(theta);
	double tx = x * c - y * s;
	double ty = x * s + y * c;
	x = tx;
	y = ty;
	if(x < 0.0001 && x > 0.0)
        x = 0.0;
    if(y < 0.0001 && y > 0.0)
        y = 0.0;
}
double vec2f::getAngle(){
    vec2f from(0.0, -1.0);
    double dot = x*from.x + y*from.y; //dot product between [x1, y1] and [x2, y2]
    double det = x*from.y - y*from.x; //determinant
    double angle = atan2(det, dot);
    return angle;
}
double vec2f::getAngle(vec2f from){
    double dot = x*from.x + y*from.y; //dot product between [x1, y1] and [x2, y2]
    double det = x*from.y - y*from.x; //determinant
    double angle = atan2(det, dot);
    return angle;
}
void vec2f::normalize() {
    double len = countLength();
	if (len == 0) return;
	x /= len;
	y /= len;
}
void vec2f::multiply(vec2f vec){
    x *= vec.x;
    y *= vec.y;
}
void vec2f::multiply(double var){
    x *= var;
    y *= var;
}
void vec2f::multiply(double var1, double var2){
    x *= var1;
    y *= var2;
}
void vec2f::divide(vec2f vec){
    x /= vec.x;
    y /= vec.y;
}
void vec2f::divide(double var){
    x /= var;
    y /= var;
}
void vec2f::divide(double var1, double var2){
    x /= var1;
    y /= var2;
}

vec2i::vec2i(){
    x = 0.0;
    y = 0.0;
    length = 0.0;
}
vec2i::vec2i(int x1, int y1){
    x = x1;
    y = y1;
    length = 0.0;
}
vec2i::vec2i(int x1, int y1, int x2, int y2){
    x = x2 - x1;
    y = y2 - y1;
    length = countLength();
}
void vec2i::set(vec2i vec){
    x = vec.x;
    y = vec.y;
    length = 0.0;
}
void vec2i::set(int x1, int y1){
    x = x1;
    y = y1;
    length = 0.0;
}
void vec2i::set(int x1, int y1, int x2, int y2){
    x = x2 - x1;
    y = y2 - y1;
    length = countLength();
}
bool vec2i::isEqual(int x1, int y1){
    if(x == x1 && y == y1)
        return true;
    return false;
}
int vec2i::countLength(){
    return sqrt(pow(x, 2) + pow(y, 2));
}
void vec2i::translate(vec2i vec){
    x += vec.x;
    y += vec.y;
}
void vec2i::translate(int tx, int ty){
    x += tx;
    y += ty;
}
void vec2i::setLength(int newLength){
    length = newLength;
}
void vec2i::rotate(double degrees) {
	if(degrees == 0.0)
        return;
	double theta = degrees / 180.0 * M_PI;
	double c = cos(theta);
	double s = sin(theta);
	double tx = x * c - y * s;
	double ty = x * s + y * c;
	x = tx;
	y = ty;

	if(x < 0.0001 && x > 0.0)
        x = 0.0;
    if(y < 0.0001 && y > 0.0)
        y = 0.0;
}
void vec2i::normalize() {
	if (countLength() == 0) return;
	x *= (1.0 / countLength());
	y *= (1.0 / countLength());
}
