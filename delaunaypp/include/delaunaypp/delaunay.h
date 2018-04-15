#pragma once
#include <tuple>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "pointaccessor.h"
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
		using EdgeType = internal::edge<T>;

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
	
		// sort triangles by x and then y
		std::sort(points_.begin(), points_.end());

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
		auto mid_x = dx / 2.0;
		auto mid_y = dy / 2.0;

		PointType st_p1 = { mid_x - 20.0* dmax, mid_y - dmax };
		PointType st_p2 = { mid_x, mid_y + 20.0 * dmax };
		PointType st_p3 = { mid_x + 20.0 * dmax, mid_y - dmax };

		// add super trianlge points to the point list.
		points_.emplace_back(st_p1);
		points_.emplace_back(st_p2);
		points_.emplace_back(st_p3);

		// add super triangle to the triangle list
		triangles.emplace_back(TriangleType{ st_p1, st_p2, st_p3 });

		// loop through all the points
		for(const auto &point: points_)
		{
			// initialize edge list
			std::vector<EdgeType> edges;
			std::unordered_map<TriangleType*, bool> bad_triangles_map;
			// loop through all triangles currently in the list
			for(auto &triangle: triangles)
			{
				auto contains = triangle.circumcircleContains(point);
				if(contains)
				{
					edges.push_back(triangle.edge_at(0));
					edges.push_back(triangle.edge_at(1));
					edges.push_back(triangle.edge_at(2));

					bad_triangles_map[&triangle] = true;
				}
				else
				{
					bad_triangles_map[&triangle] = false;
				}

			}

			auto bad_triangle_test = [&bad_triangles_map](TriangleType &triangle) -> bool
			{
				return bad_triangles_map[&triangle];
			};

			auto bad_triangle_iterator = std::remove_if(std::begin(triangles),
				std::end(triangles),
				bad_triangle_test);

			// remove bad triangles. 
			triangles.erase(bad_triangle_iterator, std::end(triangles));

			// clear the bad triangles list
			bad_triangles_map.clear();

			// print edges before erase
			std::cout << "Edges before: " << std::endl;
			internal::print_data(edges);
			std::cout << std::endl;

			// delete double specified edges in the polygon list
			for(auto edge1 = std::begin(edges); edge1 != std::end(edges); ++edge1)
			{
				for(auto edge2 = std::begin(edges); edge2 != std::end(edges); ++edge2)
				{
					if (edge1 == edge2) continue;

					EdgeType &e1 = *edge1;
					EdgeType &e2 = *edge2;
					if(e1 == e2)
					{
						std::cout << "Edge: " << e1 << " is bad" << std::endl;
						std::cout << "Edge: " << e2 << " is bad" << std::endl;
						edge1->set_is_bad(true);
						edge2->set_is_bad(true);
					}
				}
			}

			// bad edge test lambda
			auto bad_edge_test = [](auto &edge) -> bool
			{
				return edge.is_bad();
			};

			auto bad_edge_iterator = std::remove_if(std::begin(edges), 
				std::end(edges), 
				bad_edge_test);

			// we can safely erase
			edges.erase(bad_edge_iterator, std::end(edges));

			std::cout << "Edges after: " << std::endl;
			internal::print_data(edges);
			std::cout << std::endl;

			// clear the bad edge map for the next loop
//			bad_edge_map.clear();

			// add to the triangle list all triangles formed between the point and the polygon edges. 
			for(const auto & edge: edges)
			{
				triangles.push_back(TriangleType{ edge.start(), edge.end(), point });
			}
		} // end for loop through all points

		auto uses_super_triangle_test = [st_p1, st_p2, st_p3](TriangleType &triangle) -> bool
		{
			auto points = triangle.points();
			PointType &p1 = points[0];
			PointType &p2 = points[1];
			PointType &p3 = points[2];
			return (p1 == st_p1 || p1 == st_p2 || p1 == st_p3) ||
				(p2 == st_p1 || p2 == st_p2 || p2 == st_p3) ||
				(p3 == st_p1 || p3 == st_p2 || p3 == st_p3);
		};
		// remove any triangles from the trianlge list that use the super triangle verticies
		auto uses_supertrianlge_verts_iterator = std::remove_if(std::begin(triangles), 
			std::end(triangles),
			uses_super_triangle_test);
		triangles.erase(uses_supertrianlge_verts_iterator, std::end(triangles));

		// remove the super triangle verticies from the point list.
		return triangles;
	}
}
