#pragma once
#include <tuple>
#include <vector>
#include <algorithm>

#include "pointaccessor.h"
#include "triangle.h"

namespace delaunaypp
{
	template<typename PointType, typename T = typename PointType::value_type>
	class delaunay
	{
	public:
		explicit delaunay(std::vector<PointType> points, 
			delaunaypp::pointaccess::base<PointType, T>& point_accessor_x = pointaccess::indexer<PointType, T>(),
			delaunaypp::pointaccess::base<PointType, T>& point_accessor_y = pointaccess::indexer<PointType, T>());
		std::vector<triangle> triangulate();

	private:
		std::vector<PointType> _points;
		pointaccess::base<PointType, T> _x_access;
		pointaccess::base<PointType, T> _y_access;
	};

	template <typename PointType, typename T>
	delaunay<PointType, T>::delaunay(std::vector<PointType> points, 
		pointaccess::base<PointType, T> & point_accessor_x, pointaccess::base<PointType, T> & point_accessor_y)
		:_points(std::move(points)), _x_access(point_accessor_x), _y_access(point_accessor_y)
	{

	}

	template <typename PointType, typename T>
	std::vector<triangle> delaunay<PointType, T>::triangulate()
	{
		std::vector<triangle> triangles{};
		// sort triangles by x and then y
		std::sort(_points.begin(), _points.end());

		auto length = _points.size();
		auto min_x = _points[0].x();
		auto min_y = _points[0].y();
		auto max_x = _points[length - 1].x();
		auto max_y = _points[length - 1].y();

		auto dx = max_x - min_x;
		auto dy = max_y - min_y;

		// TODO

		// add super triangle
		return triangles;
	}
}
