#include "basic_element.h"

class ball:public mass_point
{
	private:

		double radius;

	public:

		ball(coordinate a,coordinate b,double c,double d):
			mass_point(a,b,c),radius(d)
		{}

		~ball(){}

		double get_radius()
		{
			return radius;
		}

		void go(double delta_time)
		{
			move(delta_time*ask_velosity());
		}

};


class collision
{
	private:

		double eps;

		double lim_time;
		//suppose the shortest time between two collisions

		ball ball_1,ball_2;

		coordinate boundary;

		double now_time,last_collided_time,delta_time;

	public:

		double abs(double a)
		{
			if (a<0) a=-a;
			return a;
		}

		collision(double a,ball b,ball c,coordinate d):
			delta_time(a),ball_1(b),ball_2(c),boundary(d)
		{
			last_collided_time=-1;
			now_time=0;
			eps=1e-8;
			lim_time=1e-2;
		}

		void move()
		{
			now_time+=delta_time;
			ball_1.go(delta_time);
			ball_2.go(delta_time);
			coordinate p1=get_ball_1_position();
			coordinate p2=get_ball_2_position();
			coordinate vv1=get_ball_1_velosity();
			coordinate vv2=get_ball_2_velosity();
			double r1=get_ball_1_radius();
			double r2=get_ball_2_radius();
			double m1=get_ball_1_mass();
			double m2=get_ball_2_mass();

			coordinate vec_1_2=p1-p2;

			//vector from ball_1 to ball_2

			if (vec_1_2.length()<r1+r2-eps && abs(now_time-last_collided_time)>lim_time)
			{
				last_collided_time=now_time;
				coordinate vec0_1_2=vec_1_2.unit();
				coordinate pro_1=(dot(vv1,vec0_1_2))*vec_1_2.unit();
				coordinate rest_1=vv1-pro_1;
				double v1=dot(vv1,vec0_1_2);
				coordinate pro_2=(dot(vv2,vec0_1_2))*vec_1_2.unit();
				coordinate rest_2=vv2-pro_2;
				double v2=dot(vv2,vec0_1_2);
				double u1=(m1-m2)/(m1+m2)*v1+(m2+m2)/(m1+m2)*v2;
				double u2=(m1+m1)/(m1+m2)*v1+(m2-m1)*(m1+m2)*v2;
				ball_1.set_velosity(rest_1+u1*vec0_1_2);
				ball_2.set_velosity(rest_2+u2*vec0_1_2);
			}

			overflow_check(ball_1);
			overflow_check(ball_2);
		}

		void overflow_check(ball &a)
		{
		    double r=a.get_radius();
			coordinate p=a.ask_position();
			coordinate v=a.ask_velosity();
			if (p.x-r<-eps && v.x<-eps) v.x=-v.x;
			if (p.x+r>boundary.x+eps && v.x>eps) v.x=-v.x;
			if (p.y-r<-eps && v.y<-eps) v.y=-v.y;
			if (p.y+r>boundary.y+eps && v.y>eps) v.y=-v.y;
			if (p.z-r<-eps && v.z<-eps) v.z=-v.z;
			if (p.z+r>boundary.z+eps && v.z>eps) v.z=-v.z;
			a.set_velosity(v);
		}

		coordinate get_ball_1_position()
		{
			return ball_1.ask_position();
		}

		coordinate get_ball_1_velosity()
		{
			return ball_1.ask_velosity();
		}

		coordinate get_ball_2_velosity()
		{
			return ball_2.ask_velosity();
		}

		coordinate get_ball_2_position()
		{
			return ball_2.ask_position();
		}

		double get_ball_1_radius()
		{
			return ball_1.get_radius();
		}

		double get_ball_2_radius()
		{
			return ball_2.get_radius();
		}

		double get_ball_1_mass()
		{
			return ball_1.get_mass();
		}

		double get_ball_2_mass()
		{
			return ball_2.get_mass();
		}

		~collision(){}

};
