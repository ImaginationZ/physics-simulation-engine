#include "basic_element.h"

//coordinate::coordinate(double px,double py,double pz):x(px),y(py),z(pz){}

double coordinate::length() const{
  return std::sqrt(x*x + y*y + z*z);
}

bool coordinate::ifNULL() const{
  return (0==x && 0==y && 0==z);
}

coordinate coordinate::unit(){
  if(ifNULL()) return *this;
  else {
    coordinate tmp(x/length(),y/length(),z/length());
    return tmp;
  }
}
bool operator==(const coordinate& x,const coordinate& y){
  return (x.x==y.x && x.y==y.y && x.z==y.z);
}

coordinate operator+(const coordinate& x,const coordinate& y){
  coordinate tmp(x.x+y.x,x.y+y.y,x.z+y.z);
  return tmp;
}

coordinate operator-(const coordinate& x){
  coordinate tmp(-x.x,-x.y,-x.z);
  return tmp;
}

coordinate operator-(const coordinate& x,const coordinate& y){
  coordinate tmp(x.x-y.x,x.y-y.y,x.z-y.z);
  return tmp;
}

coordinate operator*(const double& multiple,const coordinate& x){
  coordinate tmp(multiple * x.x,multiple * x.y,multiple * x.z);
  return tmp;
}

coordinate operator/(const coordinate& x,const double &division){
  return 1/division *x;
}

coordinate operator*(const coordinate &x,const double& multiple){
  return multiple * x;
}

double dot(const coordinate& x,const coordinate& y){
    return x.x*y.x + x.y*y.y + x.z*y.z;
}

double operator*(const coordinate &x,const coordinate &y){
  return x.x*y.x + x.y*y.y + x.z*y.z;
}

coordinate cross(const coordinate &x,const coordinate &y){
    coordinate tmp(x.y*y.z - x.z*y.y,x.z*y.x - x.x*y.z,x.x*y.y - x.y*y.x);
    return tmp;
}

coordinate operator%(const coordinate &x,const coordinate &y){
  coordinate tmp(x.y*y.z - x.z*y.y,x.z*y.x - x.x*y.z,x.x*y.y - x.y*y.x);
  return tmp;
}

bool parrallel(const coordinate& x,const coordinate& y){
  return (x.x*y.y == x.y*y.x && x.z*y.x == x.x*y.z && x.y*y.z == x.z*y.y);
}

bool vertical(const coordinate& x,const coordinate& y){
  return (0 == x * y);
}

double angle(const coordinate& x,const coordinate& y){
  if(x.ifNULL() || y.ifNULL()) return -1;
  else return std::acos( (x*y)/(x.length()*y.length()));
}
