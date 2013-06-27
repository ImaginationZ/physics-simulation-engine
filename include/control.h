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
#include <algorithm>

const coordinate Gra(0,0,-9.8);
static bool flag;

class control{
 private:

  /*construct a container for objects*/

	int n;
	double delta_time,maxx,maxy,maxz;
	elastic_ball **a;
    elastic_ball **w;
	coordinate xqz;
	double eps;

 public:
    cylinder *c;

	control()
	{
		std::ifstream sin("input.txt");
		sin>>n>>delta_time;
		maxx=maxy=maxz=100;
		eps=1e-8;
		a=new elastic_ball*[n+5];
        w=new elastic_ball*[6];
        c=new cylinder(5,5,15,0,0,10,600,10,3);
		double R[9];
		for (int i=0;i<n;++i)
        {
            for (int j=0;j<9;++j) sin>>R[j];
			a[i]=new elastic_ball(coordinate(R[0],R[1],R[2]+10),
														coordinate(R[3],R[4],R[5]),
														R[6],R[7],100*R[8]);
        }
        w[0] = new elastic_ball(coordinate(5,5,-10000),coordinate(0,0,0),1000,10000,10000);
        w[1] = new elastic_ball(coordinate(-1000,5,5),coordinate(0,0,0),1000,1000,10000);
        w[2] = new elastic_ball(coordinate(5,1010,5),coordinate(0,0,0),1000,1000,10000);
        w[3] = new elastic_ball(coordinate(5,-1000,5),coordinate(0,0,0),1000,1000,10000);
        w[4] = new elastic_ball(coordinate(5,5,1010),coordinate(0,0,0),1000,1000,10000);
        w[5] = new elastic_ball(coordinate(1010,5,5),coordinate(0,0,0),1000,1000,10000);
		//x y z vx vy vz m r sti
	}

	~control()
	{
		for (int i=0;i<n;++i)
			delete a[i];
		delete [] a;
	}

    class cmp
    {
        public:
            coordinate xqz;
            double eps;
            cmp(const coordinate &_xqz,const double &_eps):xqz(_xqz),eps(_eps){}
            bool operator ()(elastic_ball* a, elastic_ball* b) const
            {
                return (a->ask_position()-b->ask_position())*xqz<-eps;
            }
    };

	void work()
	{
	    //set force
		for (int i=0;i<n;++i)
			a[i]->reset_force();
        c->reset_force();
        c->reset_moment();

        xqz=coordinate(rand(),rand(),rand());
		xqz=xqz.unit();
		std::sort(a,a+n,cmp(xqz,eps));
		double rmax=0;
		for (int i=0;i<n;++i)
			rmax=std::max(rmax,a[i]->get_radius());
		for (int i=0;i<n;++i)
			for (int j=i+1;j<n;++j)
			{
				double dis=(a[j]->ask_position()-a[i]->ask_position())*xqz;
				if (dis>rmax*2+eps) break;
				if (if_touched(a[i],a[j]))
					renew(a[i],a[j]);
			}

		for (int i=0;i<n;++i)
		{
            for(int j=0;j<1;++j){
                renew(a[i],w[j]);;
                set_elastic_force(w[j],c,delta_time);
            }
            if(flag)
                set_elastic_force(a[i],c,delta_time);
            else
                arrange(a[i],c,delta_time);
            set_gravity(a[i],Gra);
		}

		//move point
		for (int i=0;i<n;++i)
            motivate(a[i]);
            if(flag){
        set_gravity(c,Gra);
        motivate(c);
        c->motivate(delta_time);
            }
       // std::cout << c->ask_position().x <<','<<c->ask_position().y<<','<<c->ask_position().z<<std::endl;

	}

    void motivate(mass_point* p){
        p->set_acceleration(p->ask_force()/p->get_mass());
        p->vmove(delta_time*get_accelerate(p));
        p->move(delta_time*get_velosity(p));
    }

	void renew(elastic_ball *&a,elastic_ball *&b)
	{
	    if(flag)
            set_elastic_force(a,b,delta_time);
        else
            arrange(a,b,delta_time);
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
  void set_elastic_force(elastic_ball*,elastic_ball*,double) ;
    void set_elastic_force(elastic_ball*,cylinder*,double);
    void arrange(elastic_ball*,elastic_ball*,double);
    void arrange(elastic_ball*,cylinder*,double);
};

#endif
