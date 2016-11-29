/*************************************************************************/
/*  a_star2i.h                                                        */
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
#ifndef ASTAR2I_H
#define ASTAR2I_H

#include "reference.h"
#include "self_list.h"
/**
	@original author Juan Linietsky <reduzio@gmail.com>
	@edited by Ryan Tam <ryan@anvilbear.com>
*/

class AStar2i: public Reference {

	OBJ_TYPE(AStar2i,Reference)


	uint64_t pass;

	float diagonal_cost;

	struct Point {

		SelfList<Point> list;

		int id;
		Point2i pos;
		float weight_scale;
		uint64_t last_pass;

		Vector<Point*> neighbours;

		//used for pathfinding
		Point *prev_point;
		float distance;

		Point() : list(this) {}
	};

	Map<int,Point*> points;

	struct Segment {
		union {
			struct {
				int32_t from;
				int32_t to;
			};
			uint64_t key;
		};

		Point *from_point;
		Point *to_point;

		bool operator<(const Segment& p_s) const { return key<p_s.key; }
		Segment() { key=0; }
		Segment(int p_from,int p_to) {
			if (p_from > p_to) {
				SWAP(p_from,p_to);
			}

			from=p_from;
			to=p_to;
		}
	};


	Set<Segment> segments;

	bool _solve(Point *begin_point, Point *end_point);

protected:

	static void _bind_methods();
	static int _point2i_to_id(const Point2i& p_pt);
public:

	Point2i get_available_point() const;

	void add_point(const Point2i& p_pos, float p_weight_scale=1);
//	Point2i get_point_pos(int p_id) const; not needed as points are point2i when viewed externally
	float get_point_weight_scale(const Point2i& p_id) const;
	void remove_point(const Point2i& p_id);

	void connect_points(const Point2i& p_id, const Point2i& p_with_id);
	void disconnect_points(const Point2i& p_id, const Point2i& p_with_id);
	bool are_points_connected(const Point2i& p_id, const Point2i& p_with_id) const;

	void clear();

	Point2i get_closest_point(const Point2i& p_point) const;
	Point2i get_closest_pos_in_segment(const Point2i& p_point) const;

	DVector<Point2i> get_point_path(const Point2i& p_from_id, const Point2i& p_to_id);
//	DVector<int> get_id_path(int p_from_id, int p_to_id);

	void set_diagonal_cost(float p_cost);
	float get_diagonal_cost() const;

	AStar2i();
	~AStar2i();
};

#endif // ASTAR2I_H
