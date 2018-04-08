#pragma once
#include <tuple>
#include <vector>
#include <algorithm>

#include "pointaccessor.h"
#include "triangle.h"
#include <queue>

namespace delaunaypp
{
	template<typename PointType, typename T = typename PointType::value_type>
	class delaunay
	{
	public:
		using TriangleType = triangle<T>;
		explicit delaunay(std::vector<PointType> points, 
			delaunaypp::pointaccess::base<PointType, T>& point_accessor_x = pointaccess::indexer<PointType, T>(0),
			delaunaypp::pointaccess::base<PointType, T>& point_accessor_y = pointaccess::indexer<PointType, T>(1));
		std::vector<TriangleType> triangulate();

	private:
		std::vector<PointType> _points;
		pointaccess::base<PointType, T>& _x_access;
		pointaccess::base<PointType, T>& _y_access;
	};

	template <typename PointType, typename T>
	delaunay<PointType, T>::delaunay(std::vector<PointType> points, 
		pointaccess::base<PointType, T> & point_accessor_x, pointaccess::base<PointType, T> & point_accessor_y)
		:_points(std::move(points)), _x_access(point_accessor_x), _y_access(point_accessor_y)
	{

	}

	template <typename PointType, typename T>
	std::vector<typename delaunay<PointType, T>::TriangleType> delaunay<PointType, T>::triangulate()
	{
		std::vector<TriangleType> triangles{};
	
		// sort triangles by x and then y
		std::sort(_points.begin(), _points.end());

		auto length = _points.size();
		auto min_x = _x_access(_points[0]);
		auto min_y = _y_access(_points[0]);
		auto max_x = _x_access(_points[length - 1]);
		auto max_y = _y_access(_points[length - 1]);

		auto dx = max_x - min_x;
		auto dy = max_y - min_y;
		auto dmax = (dx > dy) ? dx : dy;
		auto mid_x = dx / 2.0;
		auto mid_y = dy / 2.0;

		PointType st_p1 = { mid_x - 20.0* dmax, mid_y - dmax };
		PointType st_p2 = { mid_x, mid_y + 20.0 * dmax };
		PointType st_p3 = { mid_x + 20.0 * dmax, mid_y - dmax };

		// add super trianlge points to the point list.
		_points.push_back(st_p1);
		_points.push_back(st_p2);
		_points.push_back(st_p3);

		// add super triangle to the triangle list
		triangles.push_back({ st_p1, st_p2, st_p3 });

		// loop through all the points
		for(const auto &point: _points)
		{
			// initialize edge list
			std::vector<edge<T>> _edges;
			std::map<TriangleType, bool> bad_triangles_map;
			// loop through all triangles currently in the list
			for(auto triangle: triangles)
			{
				auto contains = triangle.circumcircleContains(point);
				if(contains)
				{
					_edges.push_back(triangle.edge_at(0));
					_edges.push_back(triangle.edge_at(1));
					_edges.push_back(triangle.edge_at(2));

					std::remove(std::begin(triangles), std::end(triangles), triangle);
					bad_triangles_map[triangle] = true;
				}
				else
				{
					bad_triangles_map[triangle] = false;
				}

			}

			// remove bad triangles. 
			triangles.erase(std::remove_if(std::begin(triangles), std::end(triangles), [&bad_triangles_map](const TriangleType &tri) -> bool
			{
				return bad_triangles_map[tri];
			}));

			// clear the bad triangles list
			bad_triangles_map.clear();

			// delete double specified edges in the polygon list
			std::map<edge<T>, int> bad_edge_map;
			for(auto edge1 = std::begin(_edges); edge1 != std::end(_edges); ++edge1)
			{
				for(auto edge2 = std::begin(_edges); edge2 != std::end(_edges); ++edge2)
				{
					if (edge1 == edge2) continue;
					edge<T> e1 = *edge1;
					edge<T> e2 = *edge2;
					if(e1 == e2)
					{
						bad_edge_map[e1] = true;;
						bad_edge_map[e2] = true;;
					}
					else
					{
						bad_edge_map[e1] = false;
						bad_edge_map[e2] = false;
					}
				}
			}

			_edges.erase(std::remove_if(std::begin(_edges), std::end(_edges), [&bad_edge_map](auto &edge)-> bool {
				return bad_edge_map[edge];
			}));

			bad_edge_map.clear();

			// add to the triangle list all triangles formed between the point and the polygon edges. 
			for(const auto & edge: _edges)
			{
				triangles.push_back({ edge.start(), edge.end(), point });
			}
		} // end for loop through all points

		// remove any triangles from the trianlge list that use the super triangle verticies
		triangles.erase(std::remove_if(std::begin(triangles), std::end(triangles), [&st_p1, &st_p2, &st_p3](auto triangle) -> bool 
		{
			auto points = triangle.points();

			if (points[0] == st_p1 || points[1] == st_p2 || points[2] == st_p3) return true;
			
			return false;
		}));
		// remove the super triangle verticies from the point list. 
		return triangles;
	}
}
