// lab5_2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/polygon/voronoi.hpp>
#include <ctime>

namespace bg = boost::geometry;

using boost::polygon::voronoi_diagram;
typedef voronoi_diagram<double>::cell_type cell_type;
typedef voronoi_diagram<double>::edge_type edge_type;
typedef voronoi_diagram<double>::vertex_type vertex_type;
typedef boost::polygon::point_data<double> point_type;

using namespace std;

vector<point_type> vpoints;

double get_len(size_t i, size_t j)
{
	return sqrt((vpoints[i].x() - vpoints[j].x())*(vpoints[i].x() - vpoints[j].x()) + (vpoints[i].y() - vpoints[j].y())*(vpoints[i].y() - vpoints[j].y()));
}

int main() {
	for (int i = 0; i < 10000; i++)
		vpoints.push_back(point_type((0.001*(rand() % 10000)), (0.001*(rand() % 10000))));
	
	unsigned int start_time1 = clock();

	voronoi_diagram<double> vd;
	construct_voronoi(vpoints.begin(), vpoints.end(), &vd);

	for (voronoi_diagram<double>::const_cell_iterator it = vd.cells().begin(); it != vd.cells().end(); ++it) {
		if (it->contains_point())
		{
			size_t index = it->source_index();
			size_t res = (index + 1) % vpoints.size();
			double len = get_len(index, (index + 1) % vpoints.size());
			const voronoi_diagram<double>::edge_type* edge = (*it).incident_edge();
			do {
				size_t index2 = (*edge).twin()->cell()->source_index();
				double temp = get_len(index, index2);
				if ((temp < len) && (index2 != index))
				{
					res = index2;
					len = temp;
				}
				edge = edge->next();
			} while (edge != (*it).incident_edge());
			//cout << "for dot " << index << " nearest dot:" << res << " " << len << "\n";
		}
	}
	unsigned int end_time1 = clock();
	cout << "\n";
	unsigned int start_time2 = clock();
	for (int i = 0; i < vpoints.size(); i++)
	{
		size_t index = i;
		size_t res = (index + 1) % vpoints.size();
		double len = get_len(index, (index + 1) % vpoints.size());
		for (int j = 0; j < vpoints.size(); j++)
		{
			size_t index2 = j;
			double temp = get_len(index, index2);
			if ((temp < len) && (index2 != index))
			{
				res = index2;
				len = temp;
			}
		}
		//cout << "for dot " << index << " nearest dot:" << res << " " << len << "\n";
	}
	unsigned int end_time2 = clock();
	cout << end_time1 - start_time1 << " " << end_time2 - start_time2;
	system("pause");
	return 0;
}