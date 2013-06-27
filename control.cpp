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
	if (get_distance(a,b) > a->get_radius() + b->get_radius()) return false;
	return true;
}

void control::set_elastic_force( elastic_ball* a, elastic_ball* b, double delta_time){
    coordinate direction = get_direction(a,b);
    double proportional = a->get_radius() + b->get_radius() - get_distance(a,b);
    if( proportional < 0 ) return;
    double v = dot(a->ask_velosity() - b->ask_velosity(),direction.unit( ));
    if(v>0 &&( 2*v*delta_time > direction.length() ||a->get_radius()+b->get_radius()> 10*direction.length()) ){
        coordinate hforce =  2/(a->get_mass()+b->get_mass())*v * direction / delta_time;
        a->set_force( -hforce *b->get_mass() );
        b->set_force( hforce*a->get_mass() );
    }
    coordinate force =( ( a->get_stiffness() + b->get_stiffness() ) / 2 * proportional ) * direction;
    a->set_force(-force);
    a->set_force(-delta_time* a->ask_velosity());
    b->set_force(force);
    b->set_force(-delta_time* b->ask_velosity());
}

void control::arrange(elastic_ball* a,elastic_ball* b,double delta_time){
    coordinate direction = get_direction(a,b);
    double proportional = a->get_radius() + b->get_radius() - get_distance(a,b);
    if( proportional < 0 ) return;
    double v = dot(a->ask_velosity() - b->ask_velosity(),direction.unit( ));
    if(v>0) {
        coordinate hforce =  v * direction / delta_time;
        a->set_force( -hforce );
        b->set_force( hforce );
    }
    }

void control::arrange(elastic_ball* a,cylinder* c,double delta_time){
            coordinate p = c->get_nearest(a);
    coordinate direction = a->ask_position() - p;
        double proportional = a->get_radius() - direction.length();
    if(proportional<0) return;
    double v = dot(a->ask_velosity() - c->get_velosity( p ),-direction.unit());
        coordinate hforce = v*direction.unit() / delta_time;
        a->set_force(hforce);
        coordinate force =   proportional * a->get_stiffness() * direction.unit();
        a->set_force(force);
    }

void control::set_elastic_force(elastic_ball* a,cylinder* c,double delta_time){
    coordinate p = c->get_nearest(a);
    coordinate direction = a->ask_position() - p;
    double proportional = a->get_radius() - direction.length();
    if(proportional<0) return;
    double v = dot(a->ask_velosity() - c->get_velosity( p ),-direction.unit());
    if(v>0 && (4 * v *delta_time> direction.length() || direction.length()<0.9*a->get_radius())){
        coordinate hforce = 2/(a->get_mass()+c->get_mass())*v*direction.unit() / delta_time;
        a->set_force(c->get_mass()*hforce);
        c->set_force(-a->get_mass()*hforce * cos_angle(c->get_axis(),c->ask_position() -p));
        c->set_moment( cross( p - c->ask_position( ), -hforce ) );
    }
    coordinate force =   proportional * a->get_stiffness() * direction.unit();
    coordinate moment = cross(p - c->ask_position(),-force);
    a->set_force(force);
    c->set_force(-force * cos_angle(c->get_axis(),c->ask_position() -p));
    a->set_force(-a->ask_velosity()*delta_time);
    c->set_force(-c->get_velosity(p)*delta_time);
    c->set_moment(-c->get_speed() *delta_time);
    c->set_moment(moment);
}
