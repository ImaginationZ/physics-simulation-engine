#ifndef __control_h
#define __control_h
#include "ball.h"
#include "wall.h"
#include "cylinder.h"
#include <fstream>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>

const coordinate Gra(0,0,-9.8);

class control{
 private:

  /*construct a container for objects*/

	int n;
	double delta_time,maxx,maxy,maxz;
	elastic_ball **a;
    wall **w;

	double eps;

 public:

    cylinder *c;

	control()
	{
		std::ifstream sin("fuck.txt");
		sin>>n>>delta_time;
		maxx=maxy=maxz=100;
		eps=1e-8;
		a=new elastic_ball*[n+5];
        w=new wall*[6];
        c=new cylinder(5,5,5,0,0,1,10000,10,1);
		double R[9];
		for (int i=0;i<n;++i)
        {
            for (int j=0;j<9;++j) sin>>R[j];
			a[i]=new elastic_ball(coordinate(R[0],R[1],R[2]),
														coordinate(R[3],R[4],R[5]),
														R[6],R[7],R[8]);
        }
        w[0] = new wall(coordinate(10,10,10),coordinate(-1,0,0));
        w[1] = new wall(coordinate(0,0,0),coordinate(1,0,0));
        w[2] = new wall(coordinate(10,10,10),coordinate(0,-1,0));
        w[3] = new wall(coordinate(0,0,0),coordinate(0,1,0));
        w[4] = new wall(coordinate(10,10,10),coordinate(0,0,-1));
        w[5] = new wall(coordinate(0,0,0),coordinate(0,0,1));
		//x y z vx vy vz m r sti
	}

	~control()
	{
		for (int i=0;i<n;++i)
			delete a[i];
		delete [] a;
	}

	void work()
	{
	    //set force
		for (int i=0;i<n;++i)
			a[i]->reset_force();
        c->reset_moment();
        c->reset_force();
		for (int i=0;i<n;++i)
			for (int j=i+1;j<n;++j)
				if (if_touched(a[i],a[j]))
					renew(a[i],a[j]);
		for (int i=0;i<n;++i)
		{
            for(int j=0;j<6;++j){
                renew(a[i],w[j]);
            }
            set_elastic_force(a[i],c,delta_time);
            set_gravity(a[i],Gra);
		}

		//move point
		for (int i=0;i<n;++i)
			motivate(a[i]);
        motivate(c);
        c->motivate(delta_time);
	}

	void renew(elastic_ball *&a,wall *&w)
    {
        set_elastic_force(a,w,delta_time);
	}

	void renew(elastic_ball *&a,elastic_ball *&b)
	{
		set_elastic_force(a,b);
	}

	coordinate ask_position(int x)
	{
		return get_position(a[x]);
	}

	double ask_radius(int x)
	{
		return a[x]->get_radius();
	}

	int ask_total_number()
	{
		return n;
	}
    void motivate(mass_point* p){
    p->set_acceleration(p->ask_force()/p->get_mass());
    p->vmove(delta_time*get_accelerate(p));
    p->move(delta_time*get_velosity(p));
    }
  //for points
  //get a point's position
  coordinate get_position(point*);
  //get a point's velosity
  coordinate get_velosity(point*);
  //get a point's accelerate
  coordinate get_accelerate(point*);
  //move a point to a certain coordinate
  void moveto(point*,const coordinate&);
  //move a point by a certain distence
  void moveby(point*,const coordinate&);
  //set a point's accelerate
  void set_accelarate(point*,const coordinate&);
  //set a point's velosity
  void set_velosity(point*,const coordinate&);
  //return two points' relative position(point from a to b)
  coordinate get_direction(point*, point*);
  //return two points' distance
  double get_distance( point*, point* );


  //for mass points
  double get_mass(mass_point*);
  void set_gravity(mass_point*,const coordinate&);

  //for balls
  //get a ball's radius
  double get_radius(ball*);
  //check if two balls touch
  bool if_touched(ball*, ball*);

  //for elastic balls
  //set the ball's force
  void set_elastic_force(elastic_ball*,elastic_ball*) ;
  void set_elastic_force(elastic_ball*,wall*,double);
    void set_elastic_force(elastic_ball*,cylinder*,double);
};

#endif
