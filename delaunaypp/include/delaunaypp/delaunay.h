#pragma once
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iostream>

#include "triangle.h"

namespace delaunaypp
{
	namespace internal
	{
		template<typename Container, typename T = typename Container::value_type>
		void print_data(const Container &data)
		{
			std::copy(data.begin(), data.end(), std::ostream_iterator<T>(std::cout, " "));
		}
	} // namespace internal
	
	template<typename PointType, typename T = typename PointType::value_type>
	class delaunay
	{
	public:
		using TriangleType = triangle<T>;
		using EdgeType = internal::edge<PointType, T>;

		explicit delaunay(std::vector<PointType> points);
		std::vector<TriangleType> triangulate();

	private:
		std::vector<PointType> points_;
	};

	template <typename PointType, typename T>
	delaunay<PointType, T>::delaunay(std::vector<PointType> points)
		:points_(std::move(points))
	{

	}

	template <typename PointType, typename T>
	std::vector<typename delaunay<PointType, T>::TriangleType> delaunay<PointType, T>::triangulate()
	{
		std::vector<TriangleType> triangles{};
		if(points_.size() == 0)
		{
			return triangles;
		}

		// sort triangles by x and then y
		std::stable_sort(points_.begin(), points_.end());

		auto length = points_.size();
		auto min_point = points_[0];
		auto max_point = points_[length -1];
		auto min_x = min_point.x();
		auto min_y = min_point.y();
		auto max_x = max_point.x();
		auto max_y = max_point.y();

		auto dx = max_x - min_x;
		auto dy = max_y - min_y;
		auto dmax = std::max(dx, dy);
		T denom(2.0);
		auto mid_x = dx / denom;
		auto mid_y = dy / denom;
		T delta(20.0);

		PointType st_p1 = { mid_x - delta * dmax, mid_y - dmax };
		PointType st_p2 = { mid_x, mid_y + delta * dmax };
		PointType st_p3 = { mid_x + delta * dmax, mid_y - dmax };

		// add super trianlge points to the point list.
		points_.emplace_back(st_p1);
		points_.emplace_back(st_p2);
		points_.emplace_back(st_p3);

		// add super triangle to the triangle list
		triangles.emplace_back(TriangleType{ st_p1, st_p2, st_p3 });

		// loop through all the points
		for(const auto &point: points_)
		{
			// initialize EdgeType list
			std::vector<EdgeType> edges;
			// loop through all triangles currently in the list
			for(auto &triangle: triangles)
			{
				// check if the triangle circumcircle contains the point
				auto contains = triangle.circumcircleContains(point);
				if(contains)
				{
					// point is in the triangle circumcenter
					// mark the triangle as bad and add it's edges to the edge buffer.
					edges.push_back(triangle.edge_at(0));
					edges.push_back(triangle.edge_at(1));
					edges.push_back(triangle.edge_at(2));
					std::cout << "Triangle: " << triangle << " is bad" << std::endl;
					triangle.set_is_bad(true);
				}
			}

			// predicate test for remove_if
			auto bad_triangle_test = [&](TriangleType &triangle) -> bool
			{
				return triangle.is_bad();
			};

			// use remove_if to get an iterator
			auto bad_triangle_iterator = std::remove_if(std::begin(triangles),
				std::end(triangles),
				bad_triangle_test);

			std::cout << "Triangles before: " << std::endl;
			internal::print_data(triangles);
			std::cout << std::endl;

			// remove bad triangles. 
			triangles.erase(bad_triangle_iterator, std::end(triangles));

			std::cout << "Triangles after: " << std::endl;
			internal::print_data(triangles);
			std::cout << std::endl;

			// delete doubly specified edges in the polygon list
			for(auto edge1 = std::begin(edges); edge1 != std::end(edges); ++edge1)
			{
				for(auto edge2 = std::begin(edges); edge2 != std::end(edges); ++edge2)
				{
					if (edge1 == edge2) continue;

					if(*edge1 == *edge2)
					{
						std::cout << "Edge: " << *edge1 << " is bad" << std::endl;
						std::cout << "Edge: " << *edge2 << " is bad" << std::endl;
						// mark edges as bad
						edge1->set_is_bad(true);
						edge2->set_is_bad(true);
					}
				}
			}

			// bad EdgeType test lambda
			auto bad_edge_test = [](const auto &edge) -> bool
			{
				return edge.is_bad();
			};

			std::cout << "Edges before: " << std::endl;
			internal::print_data(edges);
			std::cout << std::endl;

			auto bad_edge_iterator = std::remove_if(std::begin(edges), 
				std::end(edges), 
				bad_edge_test);

			// we can safely erase
			edges.erase(bad_edge_iterator, std::end(edges));

			std::cout << "Edges after: " << std::endl;
			internal::print_data(edges);
			std::cout << std::endl;

			// add to the triangle list all triangles formed between the point and the polygon edges. 
			for(const auto & edge: edges)
			{
				triangles.emplace_back(TriangleType{ edge.start(), edge.end(), point });
			}
		} // end for loop through all points

		// test for if a trianle uses the super triangle vertices. 
		auto uses_super_triangle_test = [st_p1, st_p2, st_p3](TriangleType &triangle) -> bool
		{
			auto points = triangle.points();
			for(const auto& point: points)
			{
				auto uses = (point == st_p1 || point == st_p2 || point == st_p3);
				if(uses)
				{
					return true;
				}
			}
			return false;
		};

		// remove any triangles from the trianlge list that use the super triangle verticies
		auto uses_supertrianlge_verts_iterator = std::remove_if(std::begin(triangles), 
			std::end(triangles),
			uses_super_triangle_test);

		std::cout << "Triangles done: " << std::endl;
		internal::print_data(triangles);
		std::cout << std::endl;

		// remove the triangles. 
		triangles.erase(uses_supertrianlge_verts_iterator, std::end(triangles));

		// return results.
		return triangles;
	}
}
