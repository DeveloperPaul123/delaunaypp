#pragma once
#include "point.h"

#include <math.h>
#include <complex>

inline double pi() { return std::atan(1) * 4; }

namespace delaunaypp
{
	template<typename T>
	class edge
	{
		using PointType = point<T>;

	public:
		edge(PointType start, PointType end);
		edge(edge &other);
		edge(edge &&other) noexcept;
		~edge() = default;
		void operator=(edge& other);
		void operator=(edge&& other) noexcept;
		bool operator==(edge& other);

		PointType start() const;
		PointType end() const;
		T length();
		PointType midpoint();
		T slope();
	private:
		PointType _start;
		PointType _end;
	};

	template<typename T>
	edge<T> rotate(const edge<T> &input, const double &degrees)
	{
		auto radians = (degrees / 180.0) * pi();
		auto start = input.start();
		auto end = input.end();
		std::complex<double> vect(end.x() - start.x(), end.y() - start.y());
		std::complex<double> rot(std::cos(radians), std::sin(radians)); // 90 degrees
		vect *= rot;

		return edge<T>(start, { vect.real() + start.x(),vect.imag() + start.y() });
	}

	template <typename T>
	edge<T>::edge(PointType start, PointType end) :
		_start(std::move(start)), _end(std::move(end))
	{

	}

	template <typename T>
	edge<T>::edge(edge& other)
	{
		_start = other.start();
		_end = other.end();
	}

	template <typename T>
	edge<T>::edge(edge&& other) noexcept
	{
		_start = std::move(other.start());
		_end = std::move(other.end());
	}

	template <typename T>
	void edge<T>::operator=(edge& other)
	{
		_start = other.start();
		_end = other.end();
	}

	template <typename T>
	void edge<T>::operator=(edge&& other) noexcept
	{
		_start = std::move(other.start());
		_end = std::move(other.end());
	}

	template <typename T>
	bool edge<T>::operator==(edge& other)
	{
		return _start == other.start() && _end == other.end();
	}

	template <typename T>
	typename edge<T>::PointType edge<T>::start() const
	{
		return _start;
	}

	template <typename T>
	typename edge<T>::PointType edge<T>::end() const
	{
		return _end;
	}

	template <typename T>
	T edge<T>::length()
	{
		return std::sqrt(std::pow(_end.x() - _start.x(), 2.0) + std::pow(_end.y() - _start.y(), 2.0));
	}

	template <typename T>
	typename edge<T>::PointType edge<T>::midpoint()
	{
		return { (_start.x() + _end.x()) / 2.0, (_start.y() + _end.y()) / 2.0 };
	}

	template <typename T>
	T edge<T>::slope()
	{
		// TODO check for divide by 0
		return (_end.y() - _start.y()) / (_end.x() - _start.x());
	}
}
