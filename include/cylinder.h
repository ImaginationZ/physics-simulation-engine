#include "basic_element.h"
class cylinder:public mass_point{
private:
    coordinate axis;
    double height;
    double radius;
    coordinate moment;
    coordinate speed;
public:
    cylinder(double x,double y,double z,double ax,double ay,double az,double m,double h,double r)
    :mass_point(coordinate(x,y,z),coordinate(0,0,0),m),radius(r),height(h),axis(ax,ay,az),moment(0,0,0),speed(0,0,0){}
    coordinate get_nearest(point* p){
        coordinate pos = p->ask_position() - ask_position();
        double h = dot(pos,axis.unit());
        coordinate ph = h*axis.unit();
        coordinate pr = pos - ph;
        if( 2*h < height && 2*h > -height){
            if( pr.length() < radius){
                if(radius-pr.length()>height/2-ph.length())
                    return -height/2 * axis.unit() + pr + ask_position();
                else
                    return radius * pr.unit() + ask_position() + ph;
            }
            else
                return (radius+0.5) * pr.unit() + ask_position() + ph;
        }
        return (height/2+0.5) * ph.unit() + (radius+0.5) * pr.unit() + ask_position();
    }

    coordinate get_velosity( const coordinate &p ){
        coordinate pos = p - ask_position( );
        return ask_velosity( ) + cross( speed, pos );
    }

    void set_moment(coordinate m){
        moment = moment + m;
    }
    void reset_moment(){
        moment = coordinate(0,0,0);
    }
    double get_height()
    {
        return height;
    }
    double get_radius()
    {
        return radius;
    }
    coordinate get_axis()
    {
        return axis;
    }
    coordinate get_speed()
    {
        return speed;
    }
    double get_inertial(){
        return get_mass()*(get_radius()*get_radius()+get_height()*get_height()/6);
    }
    void motivate(double delta_time){
        speed = delta_time * moment / get_inertial() + speed;
        axis = axis + delta_time * cross(speed,axis);
        axis = axis.unit();
    }
};

