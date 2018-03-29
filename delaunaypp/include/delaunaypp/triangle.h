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
		using circumcircle = std::pair<point<double>, double>;

		triangle(PointType p1, PointType p2, PointType p3);
		triangle(triangle &other);
		triangle(triangle &&other) noexcept;

		void operator=(const triangle &other);
		void operator=(triangle &&other) noexcept;

		bool operator==(const triangle &other);
		bool circumcircleContains(const PointType& point);
	private:
		PointType _p1, _p2, _p3;
		EdgeType _e1, _e2, _e3;
		circumcircle _circumcircle;
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
			// TODO
			// Circumcircle has not been calculated.
		}

		// return if the distance from the center to the point is less than or equal to the radius.
		return delaunaypp::distance(point, _circumcircle.first) <= _circumcircle.second;
	}
}
