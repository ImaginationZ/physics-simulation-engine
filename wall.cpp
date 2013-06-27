#include "wall.h"

double wall::get_distance(point *p) {
	return (dot(p->ask_position() - pos, axis));
}

double cylinder_cup::get_distance_bottom(point* p) {
	wall w = wall(pos, axis);
	return w.get_distance(p);
}

double cylinder_cup::get_distance_side(point* p) {
	coordinate s = pos - p->ask_position();
	double t = s.length();
	double d = dot(s, axis) / axis.length();
	return radius - sqrt( t*t - d*d );
}

coordinate cylinder_cup::get_distance_bottom_point(point* p) {
	coordinate s = p->ask_position() - pos;
	coordinate d = dot(s, axis) * axis;
	return d - s + pos;
}

coordinate cylinder_cup::get_distance_side_point(point* p) {
	coordinate s = p->ask_position() - pos;
	coordinate d = dot(s, axis) * axis;
	coordinate r = d - s;
	return  r.unit() * radius - r + p->ask_position();
}

bool cylinder_cup::if_in(point* p) {
	if (dot(p->ask_position() - pos, axis) < 0) return false;
	return get_distance_side(p) >= 0 && get_distance_bottom(p) <= height;
}

coordinate cylinder_cup::get_nearest_point(point* p) {
	coordinate b = get_distance_bottom_point(p);
	coordinate s = get_distance_side_point(p);

	coordinate tb = b - p->ask_position(), ts = s - p->ask_position();
	if (tb.length() < ts.length())
		return b;
	else
		return s;
}
