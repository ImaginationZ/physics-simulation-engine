#include "control.h"

coordinate control::get_position(point* p){
	return p->ask_position();
}

coordinate control::get_velosity(point* p){
	return p->ask_velosity();
}

coordinate control::get_accelerate(point* p){
	return p->ask_acceleration();
}

void control::moveto(point* p, const coordinate& c) {
	p->moveto(c);
}

void control::set_accelarate(point* p, const coordinate& c) {
	p->set_acceleration(c);
}

void control::set_velosity(point* p, const coordinate& c) {
	p->set_velosity(c);
}

coordinate control::get_direction(point* a, point* b) {
	coordinate dis = b->ask_position() - a->ask_position();
	return dis.unit();
}

double control::get_distance( point* a, point* b ){
  	coordinate dis = b->ask_position() - a->ask_position();
        return dis.length();
}

double control::get_mass(mass_point* p){
	return p->get_mass();
}
void control::set_gravity(mass_point* p, const coordinate& g){
    p->set_force(g * p->get_mass());
}

double control::get_radius(ball* b) {
	return b->get_radius();
}

bool control::if_touched(ball* a, ball* b) {
	if (get_distance(a, b) > a->get_radius() + b->get_radius() - eps) return false;
	return true;
}

void control::set_elastic_force( elastic_ball* a, elastic_ball* b ){
    coordinate direction = get_direction(a,b);
    double proportional = a->get_radius() + b->get_radius() - get_distance(a,b);
    if( proportional <= 0 ) return;
    coordinate force =( ( a->get_stiffness() + b->get_stiffness() ) / 2 * proportional ) * direction;
    a->set_force(-force);
   // a->set_force(-0.1 * a->ask_velosity());
    b->set_force(force);
   // b->set_force(-0.1 * b->ask_velosity());
}

void control::set_elastic_force(elastic_ball* a, wall* w,double delta_time){
    double proportional = a->get_radius() - w->get_distance(a);
    if(proportional < 0) return;
    double v = dot(a->ask_velosity(),-w->get_axis());
    if(v>0 && 0.1 > w->get_distance(a))
        a->set_velosity(a->ask_velosity() + 2*v*w->get_axis().unit());
    coordinate force = a->get_stiffness() * proportional * w->get_axis().unit();
    a->set_force(force);
   // a->set_force(-0.1 * a->ask_velosity());
}

void control::set_elastic_force(elastic_ball* a,cylinder* c,double delta_time){
    coordinate p = c->get_nearest(a);
    coordinate direction = a->ask_position() - p;
    double proportional = a->get_radius() - direction.length();
    if(proportional<0) return;
    double v = dot(a->ask_velosity(),-direction.unit());
    if(v>0 && 2 * v *delta_time> direction.length()){
        a->set_velosity(a->ask_velosity() + 2*v*direction.unit());
        c->set_velosity(c->ask_velosity() - 2*v*a->get_mass()/c->get_mass()*direction.unit());
    }
    coordinate force = 1 * proportional * a->get_stiffness() * direction.unit();
    coordinate moment = cross(p - c->ask_position(),-force);
    a->set_force(force);
    c->set_force(-force);
    c->set_moment(moment);
}
