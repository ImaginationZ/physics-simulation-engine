#ifndef __basic_ele_h
#define __basic_ele_h
#include <cmath>

//A defination of coordinate, a vector describe position, velosity or acceleration.
class coordinate{
 public:
  //3D system with 3 independent component
  double x;
  double y;
  double z;

  coordinate(){};
  coordinate(double a,double b,double c)
		:x(a),y(b),z(c)
	{}
  ~coordinate(){};


//vector length
  double length() const;
  bool ifNULL() const;//double-type length may not be judged whether NULL, which may lead to zero-divide error
  //vector unit
  coordinate unit();
};


//operator overloading
//vector equality
bool operator==(const coordinate&,const coordinate&);
//vector addition
coordinate operator+(const coordinate&,const coordinate&);
//vector subtraction
  coordinate operator-(const coordinate&);
coordinate operator-(const coordinate&,const coordinate&);
//vector scalar multiplication
coordinate operator*(double const&,coordinate const&);
coordinate operator/(const coordinate&,const double&);
coordinate operator*(const coordinate&,const double&);
//vector dot product
double dot(const coordinate&,const coordinate&);
double operator*(const coordinate&,const coordinate&);
//vector cross product
coordinate cross(const coordinate&,const coordinate&);
coordinate operator%(const coordinate&,const coordinate&);
//vector parrallel
bool parrallel(const coordinate&,const coordinate&);
//vector vertical
bool vertical(const coordinate&,const coordinate&);
//vector angle
double angle(const coordinate&,const coordinate&);
double cos_angle(const coordinate&,const coordinate&);

// A defination of area, a cube in 3D-system
class area{
 private:
  coordinate corner;//the corner point in this area with least x & y & z
  double x;//length by x-axis
  double y;//length by y-axis
  double z;//length by z-axis
 public:
  //determine whether a point is in this area
  bool contain(coordinate);
};

// A defination of point, a basic object with position, vector and accleration.
class point{
 private:
  coordinate position;
  coordinate velosity;
  coordinate acceleration;
 public:
  point();
  point(coordinate a,coordinate b):position(a),velosity(b) {}
  ~point(){}

  coordinate ask_position()
  {
  	return position;
  }
  coordinate ask_velosity()
  {
  	return velosity;
  }
  coordinate ask_acceleration()
  {
  	return acceleration;
  }
  void set_acceleration(coordinate a)
  {
  	acceleration=a;
  }
  //move point by certain distance
  void move(coordinate a)
  {
  	position=position+a;
  }
  void vmove(coordinate a)
  {
  	velosity=velosity+a;
  }
  void set_velosity(coordinate a)
  {
  	velosity=a;
  }
  //move point to certain position
  void moveto(coordinate a)
  {
  	position=a;
  }
};

// A defination of mass point, a basic object in physics model
class mass_point:public point
{
 private:
  double mass;
  coordinate force;
 public:
  mass_point(coordinate a,coordinate b,double c):
		point(a,b),mass(c)
  {}
  ~mass_point(){}
  //return the force this mass point is applied
  coordinate ask_force(){
      return force;
  }
  //add to a force this mass point is applied
  void set_force(const coordinate& f){
      force = force + f;
  }
  void reset_force(){
    force = coordinate(0,0,0);
  }

  double get_mass()
  {
  	return mass;
  }
};

// A defination of point charge, a basic object in electromagnetic model
class charge_point:public mass_point
{
 private:
  double charge;
 public:
  double ask_charge();
};

// A defination of gravity field, a basic area which practise gravity force, as well as non-inertial force
class gravity_field:public area
{
 private:
  coordinate g;//Gravitation acceleration
};

//A defination of electromagnetic field, a basic area which practise electromagnetic force
class electromagnetic_field:public area
{
 private:
  coordinate E;//electric field intensity
  coordinate B;//magnetic field intensity
};
#endif
