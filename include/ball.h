#include "basic_element.h"

class ball:public mass_point{
private:
    double radius;

public:
		ball(coordinate p,coordinate v,double m,double r):
			radius(r),mass_point(p,v,m) {}
    double get_radius(){
        return radius;
    }
};

//请实现：
//当两个球的距离小于他们的半径和时，他们之间有弹力作用，且满足方程：
//F = (r1+r2-d)*(k1+k2)/2
//F的方向和球心相对方向相同

class elastic_ball:public ball{
private:
    double stiffness;
public:
		elastic_ball(coordinate p,coordinate v,double m,double r,double sti):
			stiffness(sti),ball(p,v,m,r) {}
    double get_stiffness(){
        return stiffness;
    }
};
