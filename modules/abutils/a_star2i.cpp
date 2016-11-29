/*************************************************************************/
/*  a_star2i.cpp                                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2016 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "a_star2i.h"
//#include "geometry.h"


int AStar2i::_point2i_to_id(const Point2i& p_pt) {
	return p_pt.x << 16 + p_pt.y;
}

Point2i AStar2i::get_available_point() const {

	if (points.empty()) {
		return Point2i(0,1);
	}
	int id= points.back()->key() + 1;
	return Point2i(id>>16, id & 65535);
}

void AStar2i::add_point(const Point2i&  p_pos, float p_weight_scale) {
	int p_id = _point2i_to_id(p_pos);
	ERR_FAIL_COND(p_id<0);
	if (!points.has(p_id)) {
		Point *pt = memnew( Point );
		pt->id=p_id;
		pt->pos=p_pos;
		pt->weight_scale=p_weight_scale;
		pt->prev_point=NULL;
		pt->last_pass=0;
		points[p_id]=pt;
	} else {
		points[p_id]->pos=p_pos;
		points[p_id]->weight_scale=p_weight_scale;
	}
}
/*
Point2i AStar2i::get_point_pos(int p_id) const{

	ERR_FAIL_COND_V(!points.has(p_id),Point2i());

	return points[p_id]->pos;

}*/
float AStar2i::get_point_weight_scale(const Point2i&  p_pt) const{

	int p_id = _point2i_to_id(p_pt);

	ERR_FAIL_COND_V(!points.has(p_id),0);

	return points[p_id]->weight_scale;

}
void AStar2i::remove_point(const Point2i&  p_pt){

	int p_id = _point2i_to_id(p_pt);

	ERR_FAIL_COND(!points.has(p_id));

	Point* p = points[p_id];

	for(int i=0;i<p->neighbours.size();i++) {

		Segment s(p_id,p->neighbours[i]->id);
		segments.erase(s);
		p->neighbours[i]->neighbours.erase(p);
	}

	memdelete(p);
	points.erase(p_id);
}

void AStar2i::connect_points(const Point2i&  p_pt, const Point2i&  p_with_pt){

	int p_id = _point2i_to_id(p_pt);
	int p_with_id = _point2i_to_id(p_with_pt);

	ERR_FAIL_COND(!points.has(p_id));
	ERR_FAIL_COND(!points.has(p_with_id));
	ERR_FAIL_COND(p_id==p_with_id);


	Point* a = points[p_id];
	Point* b = points[p_with_id];
	a->neighbours.push_back(b);
	b->neighbours.push_back(a);

	Segment s(p_id,p_with_id);
	if (s.from==p_id) {
		s.from_point=a;
		s.to_point=b;
	} else {
		s.from_point=b;
		s.to_point=a;
	}

	segments.insert(s);


}
void AStar2i::disconnect_points(const Point2i&  p_pt, const Point2i&  p_with_pt){

	int p_id = _point2i_to_id(p_pt);
	int p_with_id = _point2i_to_id(p_with_pt);

	Segment s(p_id,p_with_id);
	ERR_FAIL_COND(!segments.has(s));


	segments.erase(s);

	Point *a = points[p_id];
	Point *b = points[p_with_id];
	a->neighbours.erase(b);
	b->neighbours.erase(a);

}
bool AStar2i::are_points_connected(const Point2i&  p_pt, const Point2i&  p_with_pt) const{

	int p_id = _point2i_to_id(p_pt);
	int p_with_id = _point2i_to_id(p_with_pt);

	Segment s(p_id,p_with_id);
	return segments.has(s);
}

void AStar2i::clear(){

	for (const Map<int,Point*>::Element *E=points.front();E;E=E->next()) {

		memdelete(E->get());
	}
	segments.clear();
	points.clear();
}


Point2i AStar2i::get_closest_point(const Point2i& p_point) const{

	int closest_id=-1;
	float closest_dist=1e20;

	for (const Map<int, Point*>::Element *E = points.front();E;E = E->next()) {

		float d = p_point.distance_to(E->get()->pos, diagonal_cost);

		if (closest_id < 0 || d < closest_dist) {
			closest_dist = d;
			closest_id = E->key();
		}
	}

	return Point2i(closest_id >> 16, closest_id & 65535);
}

Point2i AStar2i::get_closest_pos_in_segment(const Point2i& p_point) const {

	float closest_dist = 1e20;
	bool found=false;
	Point2i closest_point;


	for (const Set<Segment>::Element *E=segments.front();E;E=E->next()) {

		Point2i p = E->get().from_point->pos;

		float d = p_point.distance_to(p, diagonal_cost);

		float d1 = p_point.distance_to(E->get().to_point->pos, diagonal_cost);

		if (d1 < d) {
			p = E->get().to_point->pos;
			d = d1;
		}
		if (!found || d<closest_dist) {

			closest_point=p;
			closest_dist=d;
			found=true;
		}
	}

	return closest_point;
}

bool AStar2i::_solve(Point* begin_point, Point* end_point) {

	pass++;

	SelfList<Point>::List open_list;

	bool found_route=false;


	for(int i=0;i<begin_point->neighbours.size();i++) {

		Point *n = begin_point->neighbours[i];
		n->prev_point=begin_point;
		n->distance=n->pos.distance_to(begin_point->pos, diagonal_cost);
		n->distance*=n->weight_scale;
		n->last_pass=pass;
		open_list.add(&n->list);

		if (end_point==n) {
			found_route=true;
			break;
		}
	}

	while(!found_route) {

		if (open_list.first()==NULL) {
			//could not find path sadly
			break;
		}
		//check open list

		SelfList<Point> *least_cost_point=NULL;
		float least_cost=1e30;

		//this could be faster (cache previous results)
		for (SelfList<Point> *E=open_list.first();E;E=E->next()) {

			Point *p=E->self();

			float cost=p->distance;
			cost+=p->pos.distance_to(end_point->pos, diagonal_cost);
			cost*=p->weight_scale;

			if (cost<least_cost) {

				least_cost_point=E;
				least_cost=cost;
			}
		}


		Point *p=least_cost_point->self();
		//open the neighbours for search
		int es = p->neighbours.size();

		for(int i=0;i<es;i++) {


			Point* e=p->neighbours[i];


			float distance = p->pos.distance_to(e->pos, diagonal_cost) + p->distance;
			distance*=e->weight_scale;



			if (e->last_pass==pass) {
				//oh this was visited already, can we win the cost?

				if (e->distance>distance) {

					e->prev_point=p;
					e->distance=distance;
				}
			} else {
				//add to open neighbours

				e->prev_point=p;
				e->distance=distance;
				e->last_pass=pass; //mark as used
				open_list.add(&e->list);

				if (e==end_point) {
					//oh my reached end! stop algorithm
					found_route=true;
					break;

				}

			}
		}

		if (found_route)
			break;

		open_list.remove(least_cost_point);
	}

	//clear the openf list
	while(open_list.first()) {
		open_list.remove( open_list.first() );
	}

	return found_route;

}

DVector<Point2i> AStar2i::get_point_path(const Point2i&  p_from_pt, const Point2i&  p_to_pt) {

	int p_from_id = _point2i_to_id(p_from_pt);
	int p_to_id = _point2i_to_id(p_to_pt);

	ERR_FAIL_COND_V(!points.has(p_from_id),DVector<Point2i>());
	ERR_FAIL_COND_V(!points.has(p_to_id),DVector<Point2i>());


	pass++;

	Point* a = points[p_from_id];
	Point* b = points[p_to_id];

	if (a==b) {
		DVector<Point2i> ret;
		ret.push_back(a->pos);
		return ret;
	}


	Point *begin_point=a;
	Point *end_point=b;

	bool found_route=_solve(begin_point,end_point);

	if (!found_route)
		return DVector<Point2i>();

	//midpoints
	Point *p=end_point;
	int pc=1; //begin point
	while(p!=begin_point) {
		pc++;
		p=p->prev_point;
	}

	DVector<Point2i> path;
	path.resize(pc);

	{
		DVector<Point2i>::Write w = path.write();

		Point *p=end_point;
		int idx=pc-1;
		while(p!=begin_point) {
			w[idx--]=p->pos;
			p=p->prev_point;
		}

		w[0]=p->pos; //assign first

	}

	return path;

}

/*
DVector<int> AStar2i::get_id_path(int p_from_id, int p_to_id) {

	ERR_FAIL_COND_V(!points.has(p_from_id),DVector<int>());
	ERR_FAIL_COND_V(!points.has(p_to_id),DVector<int>());


	pass++;

	Point* a = points[p_from_id];
	Point* b = points[p_to_id];

	if (a==b) {
		DVector<int> ret;
		ret.push_back(a->id);
		return ret;
	}


	Point *begin_point=a;
	Point *end_point=b;

	bool found_route=_solve(begin_point,end_point);

	if (!found_route)
		return DVector<int>();

	//midpoints
	Point *p=end_point;
	int pc=1; //begin point
	while(p!=begin_point) {
		pc++;
		p=p->prev_point;
	}

	DVector<int> path;
	path.resize(pc);

	{
		DVector<int>::Write w = path.write();

		p=end_point;
		int idx=pc-1;
		while(p!=begin_point) {
			w[idx--]=p->id;
			p=p->prev_point;
		}

		w[0]=p->id; //assign first

	}

	return path;
}*/

void AStar2i::set_diagonal_cost(float p_cost) {

	diagonal_cost = p_cost;
}
float AStar2i::get_diagonal_cost() const {
	return diagonal_cost;
}

void AStar2i::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("get_available_point"),&AStar2i::get_available_point);
	ObjectTypeDB::bind_method(_MD("add_point", "pos","weight_scale"),&AStar2i::add_point,DEFVAL(1.0));
//	ObjectTypeDB::bind_method(_MD("get_point_pos","id"),&AStar2i::get_point_pos);
	ObjectTypeDB::bind_method(_MD("get_point_weight_scale","pos"),&AStar2i::get_point_weight_scale);
	ObjectTypeDB::bind_method(_MD("remove_point","pos"),&AStar2i::remove_point);

	ObjectTypeDB::bind_method(_MD("connect_points","from_pos","to_pos"),&AStar2i::connect_points);
	ObjectTypeDB::bind_method(_MD("disconnect_points","from_pos","to_pos"),&AStar2i::disconnect_points);
	ObjectTypeDB::bind_method(_MD("are_points_connected","from_pos","to_pos"),&AStar2i::are_points_connected);

	ObjectTypeDB::bind_method(_MD("clear"),&AStar2i::clear);

	ObjectTypeDB::bind_method(_MD("get_closest_point","to_pos"),&AStar2i::get_closest_point);
	ObjectTypeDB::bind_method(_MD("get_closest_pos_in_segment","to_pos"),&AStar2i::get_closest_pos_in_segment);

	ObjectTypeDB::bind_method(_MD("get_point_path","from_pos","to_pos"),&AStar2i::get_point_path);
//	ObjectTypeDB::bind_method(_MD("get_id_path","from_id","to_id"),&AStar2i::get_id_path);

	ObjectTypeDB::bind_method(_MD("set_diagonal_cost", "cost"), &AStar2i::set_diagonal_cost);
	ObjectTypeDB::bind_method(_MD("get_diagonal_cost"), &AStar2i::get_diagonal_cost);
	ADD_PROPERTYNZ(PropertyInfo(Variant::REAL, "astar2i/diagonal_cost"), _SCS("set_diagonal_cost"), _SCS("get_diagonal_cost"));
}


AStar2i::AStar2i() {

	pass=1;
	diagonal_cost = 1.8f; // isometric cost
}


AStar2i::~AStar2i() {

	pass=1;
}
