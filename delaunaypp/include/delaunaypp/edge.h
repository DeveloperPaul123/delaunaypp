#pragma once
#include "point.h"

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

	private:
		PointType _start;
		PointType _end;
	};

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
		_start(std::move(other->_start));
		_end(std::move(other->_end));
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
		_start(std::move(other->_start));
		_end(std::move(other->_end));
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

}
