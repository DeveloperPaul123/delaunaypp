#pragma once
#include "point.h"
#include "edge.h"

namespace delaunaypp
{
	template<typename T>
	class triangle
	{
	public:
		
		using PointType = point<T>;
		using EdgeType = internal::edge<T>;
		using Circumcircle = std::pair<point<double>, double>;

		triangle(PointType p1, PointType p2, PointType p3);
		~triangle() = default;
		triangle(const triangle &other) = default;
		triangle(triangle&& other) = default;

		void operator=(const triangle &other);

		bool operator==(const triangle &other);
		bool circumcircleContains(const PointType& point);
		Circumcircle circumcircle();

		std::array<PointType, 3> points();
		std::array<EdgeType, 3> edges();

		EdgeType edge_at(const int & index);
		PointType point_at(const int & index);
	private:
		std::array<PointType, 3> points_;
		std::array<EdgeType, 3> edges_;
		Circumcircle circumcircle_;
		bool is_bad_;

		void calculateCircumcircle();
	};

	template <typename T>
	triangle<T>::triangle(PointType p1, PointType p2, PointType p3) :
		points_{ p1, p2, p3 }, edges_{ EdgeType{p1, p2}, EdgeType{p2, p3}, EdgeType{p3, p1} }, circumcircle_(std::make_pair(PointType(0.0, 0.0), -1.0)), is_bad_{false}
	{
	}

	template <typename T>
	void triangle<T>::operator=(const triangle& other)
	{
		points_ = other.points_;
		edges_ = other.edges_;
		circumcircle_ = other.circumcircle_;
	}

	template <typename T>
	bool triangle<T>::operator==(const triangle& other)
	{
		auto p1 = points_[0];
		auto p2 = points_[1];
		auto p3 = points_[2];
		return (p1 == other.points_[0] || p1 == other.points_[1] || p1 == other.points_[2]) &&
			(p2 == other.points_[0] || p2 == other.points_[1] || p2 == other.points_[2]) &&
			(p3 == other.points_[0] || p3 == other.points_[1] || p3 == other.points_[2]);
	}

	template <typename T>
	bool triangle<T>::circumcircleContains(const PointType& point)
	{
		if(circumcircle_.second < 0.0)
		{
			// Circumcircle has not been calculated so calculate it.
			calculateCircumcircle();
		}

		// return if the distance from the center to the point is less than or equal to the radius.
		return delaunaypp::distance(point, circumcircle_.first) <= circumcircle_.second;
	}

	template <typename T>
	typename triangle<T>::Circumcircle triangle<T>::circumcircle()
	{
		if (circumcircle_.second < 0.0)
		{
			calculateCircumcircle();
		}
		return circumcircle_;
	}

	template <typename T>
	std::array<typename triangle<T>::PointType, 3> triangle<T>::points()
	{
		return points_;
	}

	template <typename T>
	std::array<typename triangle<T>::EdgeType, 3> triangle<T>::edges()
	{
		return edges_;
	}

	template <typename T>
	typename triangle<T>::EdgeType triangle<T>::edge_at(const int& index)
	{
		assert(index >= 0);
		assert(index < 3);
		return edges_[index];
	}

	template <typename T>
	typename triangle<T>::PointType triangle<T>::point_at(const int& index)
	{
		assert(index >= 0);
		assert(index < 3);
		return points_[index];
	}

	template <typename T>
	void triangle<T>::calculateCircumcircle()
	{
		auto _e1 = edges_[0];
		auto _e2 = edges_[1];
		auto _e3 = edges_[2];
		auto _p1 = points_[0];
		auto _p2 = points_[1];
		auto _p3 = points_[2];

		// based on this: https://www.quora.com/What-are-the-ways-find-circumcenter-of-a-triangle-given-3-points
		auto midpoint_ab = _e1.midpoint();
		auto length_a= _e1.length();
		auto length_b = _e2.length();
		auto length_c = _e3.length();

		auto d = distance(_p1, midpoint_ab);
		auto s = (length_a + length_b + length_c) / 2.0;

		// area using Herron's formula
		auto k = std::sqrt(s * (s - length_a) * (s - length_b) * (s - length_c));

		// calculate the radius
		auto circumradius = (length_a* length_b * length_c) / (4.0 * k);
		circumcircle_.second = circumradius;

		// OM refers to edge from circumcenter to midpoint on line AB, ie edge 1
		auto length_om = std::sqrt(std::abs((d*d) - (circumradius*circumradius)));

		// rotate clockwise, note the negative angle
		auto rotated_ab = internal::rotate(_e1, -90.0);
		auto rotated_length = rotated_ab.length();
		auto scaling = length_om / rotated_length;

		// scale using complex
		// MO is vector from midpoint of AB (edge 1) to O the circumcenter
		std::complex<double> mo(rotated_ab.end().x() - rotated_ab.start().x(), 
			rotated_ab.end().y() - rotated_ab.start().y());
		mo *= scaling;

		// test by adding to various points
		point<T> mo_p(mo.real(), mo.imag());

		auto test = midpoint_ab + mo_p;
		auto dist_a = distance(_p1, test);
		auto dist_b = distance(_p2, test);
		auto dist_c = distance(_p3, test);

		if(dist_a == dist_b && dist_b == dist_c)
		{
			// found the center
			circumcircle_.first = test;
		}
		else
		{
			circumcircle_.first = midpoint_ab - mo_p;
		}
	}
}

