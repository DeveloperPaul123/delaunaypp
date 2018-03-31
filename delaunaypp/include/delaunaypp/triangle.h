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
		using EdgeType = edge<T>;
		using Circumcircle = std::pair<point<double>, double>;

		triangle(PointType p1, PointType p2, PointType p3);
		triangle(triangle &other);
		triangle(triangle &&other) noexcept;

		void operator=(const triangle &other);
		void operator=(triangle &&other) noexcept;

		bool operator==(const triangle &other);
		bool circumcircleContains(const PointType& point);
		Circumcircle circumcircle();

	private:
		PointType _p1, _p2, _p3;
		EdgeType _e1, _e2, _e3;
		Circumcircle _circumcircle;

		void calculateCircumcircle();
	};

	template <typename T>
	triangle<T>::triangle(PointType p1, PointType p2, PointType p3) :
		_p1(std::move(p1)), _p2(std::move(p2)), _p3(std::move(p3)), _e1(p1, p2), _e2(p2, p3), _e3(p3, p1), _circumcircle(std::make_pair(PointType(0.0,0.0), -1.0))
	{
	}

	template <typename T>
	triangle<T>::triangle(triangle& other)
	{
		_p1 = other->_p1;
		_p2 = other->_p2;
		_p3 = other->_p3;
		_e1 = other->_e1;
		_e2 = other->_e2;
		_e3 = other->_e3;
	}

	template <typename T>
	triangle<T>::triangle(triangle&& other) noexcept
	{
		_p1(std::move(other->_p1));
		_p2(std::move(other->_p2));
		_p3(std::move(other->_p3));

		_e1(std::move(other->_e1));
		_e2(std::move(other->_e2));
		_e3(std::move(other->_e3));
	}

	template <typename T>
	void triangle<T>::operator=(const triangle& other)
	{
		_p1 = other->_p1;
		_p2 = other->_p2;
		_p3 = other->_p3;
		_e1 = other->_e1;
		_e2 = other->_e2;
		_e3 = other->_e3;
	}

	template <typename T>
	void triangle<T>::operator=(triangle&& other) noexcept
	{
		_p1(std::move(other->_p1));
		_p2(std::move(other->_p2));
		_p3(std::move(other->_p3));

		_e1(std::move(other->_e1));
		_e2(std::move(other->_e2));
		_e3(std::move(other->_e3));
	}

	template <typename T>
	bool triangle<T>::operator==(const triangle& other)
	{
		return (_p1 == other._p1 || _p1 == other._p2 || _p1 == other._p3) &&
			(_p2 == other._p1 || _p2 == other._p2 || _p2 == other._p3) &&
			(_p3 == other._p1 || _p3 == other._p2 || _p3 == other._p3);
	}

	template <typename T>
	bool triangle<T>::circumcircleContains(const PointType& point)
	{
		if(_circumcircle.second < 0.0)
		{
			// Circumcircle has not been calculated so calculate it.
			calculateCircumcircle();
		}

		// return if the distance from the center to the point is less than or equal to the radius.
		return delaunaypp::distance(point, _circumcircle.first) <= _circumcircle.second;
	}

	template <typename T>
	typename triangle<T>::Circumcircle triangle<T>::circumcircle()
	{
		if (_circumcircle.second < 0.0)
		{
			calculateCircumcircle();
		}
		return _circumcircle;
	}

	template <typename T>
	void triangle<T>::calculateCircumcircle()
	{
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
		_circumcircle.second = circumradius;

		// OM refers to edge from circum center to midpoint on line AB, ie edge 1
		auto length_om = std::sqrt(std::abs((d*d) - (circumradius*circumradius)));

		// rotate clockwise, note negative angle
		auto rotated_ab = rotate(_e1, 90.0);
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
			_circumcircle.first = test;
		}
		else
		{
			_circumcircle.first = midpoint_ab - mo_p;
		}
	}
}

