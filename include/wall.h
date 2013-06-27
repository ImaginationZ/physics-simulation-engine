#ifndef __wall_h
#define __wall_h

#include "basic_element.h"
#include <algorithm>

class wall{
private:
	coordinate pos;
    coordinate axis; // the vertical direction of the wall
public:
	wall() {}
	coordinate get_axis() {return axis;}
	coordinate get_pos() {return pos;}
	wall(const coordinate& p, coordinate a) { pos = p; axis = a.unit(); }
    double get_distance(point* p){ // get the distance from a point to a wall
        return (dot(p->ask_position() - pos, axis));
    }
};

#endif
