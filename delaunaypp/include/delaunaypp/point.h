#pragma once
#include <type_traits>
#include <array>
#include <functional>
#include <exception>
#include <algorithm>
#include <math.h>
#include <iomanip>

namespace delaunaypp {
#pragma region Point class declaration
	template<typename T, std::size_t N = 2>
	class point
	{
		static_assert(std::is_arithmetic<T>::value, "T must be a numeric type.");
		static_assert(N > 0, "Size must be positive and greater than 0.");

	public:
		using point_type = point<T, N>;
		using reference = point<T, N>&;
		using value_type = T;
		using value_type_reference = T & ;
		using pointer = point<T, N>*;

		point();
		~point() = default;
		explicit point(std::array<T, N> data);

		explicit point(const T &x);
		point(const T& x, const T& y);
		point(const T& x, const T& y, const T& z);
		point(std::initializer_list<T> data);
		point(const point_type& other);
		point(point_type&& other) noexcept;

		value_type x() const;
		template<class S = value_type>
		typename std::enable_if<N >= 2, S>::type y() const;
		template<class S = value_type>
		typename std::enable_if<N >= 3, S>::type z() const;
		static std::size_t dimension();

		value_type_reference& operator[](const std::size_t& index);
		value_type operator[](const std::size_t& index) const;
		reference operator=(const point_type& other);
		reference operator=(point_type&& other) noexcept;
		point_type operator+(const point_type& other);
        point_type operator-(const point_type& other);
		point_type operator+(const point_type& other) const;
		point_type operator-(const point_type &other) const;
	private:
		std::array<T, N> point_data_;
	};
#pragma endregion 

#pragma region Implementation
	template <typename T, std::size_t N>
	point<T, N>::point()
	{
		std::fill(point_data_.begin(), point_data_.end(), T());
	}

	template <typename T, std::size_t N>
	point<T, N>::point(std::array<T, N> data) :
		point_data_(std::move(data))
	{
	}

	template <typename T, std::size_t N>
	point<T, N>::point(const T& x)
	{
		point_data_[0] = x;
	}

	template <typename T, std::size_t N>
	point<T, N>::point(const T& x, const T& y)
	{
		static_assert(N >= 2, "X,Y constructor only available in 2D+");
		point_data_[0] = x;
		point_data_[1] = y;
	}


	template <typename T, std::size_t N>
	point<T, N>::point(const T& x, const T& y, const T& z)
	{
		static_assert(N >= 3, "X,Y,Z constructor only available in 3D+");
		point_data_[0] = x;
		point_data_[1] = y;
		point_data_[2] = z;
	}

	template <typename T, std::size_t N>
	point<T, N>::point(std::initializer_list<T> data)
	{
		if(data.size() != N)
		{
			throw std::runtime_error("Initializer list size must match size of point.");
		}
		std::copy(data.begin(), data.end(), point_data_.begin());
	}

	template <typename T, std::size_t N>
	point<T, N>::point(const point_type& other)
	{
		point_data_ = other.point_data_;
	}

	template <typename T, std::size_t N>
	point<T, N>::point(point_type&& other) noexcept :
		point_data_(std::move(other.point_data_))
	{
	}

	template <typename T, std::size_t N>
	T point<T, N>::x() const
	{
		return point_data_[0];
	}

	template <typename T, std::size_t N>
	template <class S>
	typename std::enable_if<N >= 2, S>::type point<T, N>::y() const
	{
		return point_data_[1];
	}

	template <typename T, std::size_t N>
	template <class S>
	typename std::enable_if<N >= 3, S>::type point<T, N>::z() const
	{
		return point_data_[2];
	}

	template <typename T, std::size_t N>
	std::size_t point<T, N>::dimension()
	{
		return N;
	}

	template <typename T, std::size_t N>
	T& point<T, N>::operator[](const std::size_t& index)
	{
		if (index >= N)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		return point_data_[index];
	}

	template <typename T, std::size_t N>
	T point<T, N>::operator[](const std::size_t& index) const
	{
		if (index >= N)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		return point_data_[index];
	}

	template <typename T, std::size_t N>
	point<T, N>& point<T, N>::operator=(const point_type& other)
	{
		point_data_ = other.point_data_;
		return *this;
	}

	template <typename T, std::size_t N>
	point<T, N>& point<T, N>::operator=(point_type&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}
		point_data_ = std::move(other.point_data_);
		return *this;
	}

	template <typename T, std::size_t N>
	typename point<T, N>::point_type point<T, N>::operator+(const point_type& other)
	{
		point_type return_point(*this);
		for (auto i = 0; i < N; i++)
		{
			return_point[i] += other[i];
		}
		return return_point;
	}

	template <typename T, std::size_t N>
	typename point<T, N>::point_type point<T, N>::operator-(const point_type& other)
	{
		point_type return_point(*this);
		for (auto i = 0; i < N; i++)
		{
			return_point[i] += other[i];
		}

		return return_point;
	}

	template <typename T, std::size_t N>
	typename point<T, N>::point_type point<T, N>::operator+(const point_type& other) const
	{
		point_type return_point;
		for (auto i = 0; i < N; i++)
		{
			return_point[i] = point_data_[i] + other[i];
		}

		return return_point;
	}

	template <typename T, std::size_t N>
	typename point<T, N>::point_type point<T, N>::operator-(const point_type& other) const
	{
		point_type return_point;
		for (auto i = 0; i < N; i++)
		{
			return_point[i] = point_data_[i] - other[i];
		}

		return return_point;
	}
#pragma endregion 

#pragma region Comparison overloads
	template<typename T, std::size_t N>
	inline bool operator>=(const point<T, N>& lhs, const point<T, N>& rhs)
	{
		std::size_t depth = 0;
		while (true)
		{
			if (lhs[depth] == rhs[depth] && depth < N)
			{
				depth++;
			}
			else if (depth == N)
			{
				// points equal.
				return true;
			}
			return lhs[depth] >= rhs[depth];
		}
	}

	template<typename T, std::size_t N>
	inline bool operator>(const point<T, N>& lhs, const point<T, N> &rhs)
	{
		std::size_t depth = 0;
		while (true)
		{
			if (lhs[depth] == rhs[depth] && depth < N)
			{
				depth++;
			}
			else if (depth == N)
			{
				// points are equal so comparison is false.
				return false;
			}
			return lhs[depth] > rhs[depth];
		}
	}

	template<typename T, std::size_t N>
	inline bool operator<=(const point<T, N>& lhs, const point<T, N> &rhs)
	{
		std::size_t depth = 0;
		while (true)
		{
			if (lhs[depth] == rhs[depth] && depth < N)
			{
				depth++;
			}
			else if (depth == N)
			{
				// points are equal.
				return true;
			}
			return lhs[depth] <= rhs[depth];
		}
	}

	template<typename T, std::size_t N>
	inline bool operator<(const point<T, N>& lhs, const point<T, N> &rhs)
	{
		std::size_t depth = 0;
		while (true)
		{
			if (lhs[depth] == rhs[depth] && depth < N)
			{
				depth++;
			}
			else if (depth == N)
			{
				// points are equal so 
				return false;
			}
			return lhs[depth] < rhs[depth];
		}
	}

	template<typename T, std::size_t N>
	inline bool operator==(const point<T, N> &lhs, const point<T, N> &rhs)
	{
		auto result = true;
		for (auto i = 0; i < N; i++)
		{
			if (lhs[i] != rhs[i])
			{
				result = false;
				break;
			}
		}

		return result;
	}
#pragma endregion

#pragma region Utility functions

	template<typename PointType, typename T = typename PointType::value_type,
	typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
	inline T distance(const PointType &first, const PointType &second)
	{
		// holder for the sum of squares.
		T square_sum = T();
		auto dimension = first.dimension();
		for (std::size_t i = 0; i < dimension; i++)
		{
			// add the sum square.
			square_sum += std::pow(second[i] - first[i], 2);
		}

		// return the square root of the sum of squares.
		return std::sqrt(square_sum);
	}

	template<std::size_t N, typename Iterator, class PointType = typename std::iterator_traits<Iterator>::value_type,
	typename DataType = typename PointType::value_type>
	inline point<double, N> centroid(Iterator begin, Iterator end)
	{
		auto length = std::distance(begin, end);
		// holder for the sum in each dimension from 0 to N. 
		std::array<double, N> dimensions{};
		for (auto itr = begin; itr != end; ++itr)
		{
			// get the point.
			auto point = *itr;
			for (auto i = 0; i < N; i++)
			{
				// add the dimension value to the holder array.
				dimensions[i] += static_cast<double>(point[i]);
			}
		}
	
		// calculate the centroid by dividing by the number of points. 
		for (auto i = 0; i < N; i++)
		{
			dimensions[i] = dimensions[i] / static_cast<double>(length);
		}

		// return the centroid point.
		return point<double, N>(dimensions);
	}

	template<std::size_t N, typename InputType, typename OutputType>
	inline point<OutputType, N> convertPoint(const point<InputType, N> &input)
	{
		std::array<OutputType, N> data;
		for (auto i = 0; i < N; i++)
		{
			data[i] = static_cast<OutputType>(input[i]);
		}

		return point<OutputType, N>(data);
	}

	template<std::size_t N, 
		typename OutputType,
		typename InputPointType, 
		typename InputType = typename InputPointType::value_type,
		typename = typename std::enable_if<std::is_floating_point<InputType>::value>::type,
		typename = typename std::enable_if<std::is_integral<OutputType>::value>::type>
	inline point<OutputType, N> convertPointWithRounding(const InputPointType &input)
	{
		std::array<OutputType, N> data{};
		for (auto i = 0; i < N; i++)
		{
			data[i] = static_cast<OutputType>(std::round(input[i]));
		}

		return point<OutputType, N>(data);
	}

	template<std::size_t N, typename T>
	std::ostream& operator<<(std::ostream& stream, const point<T, N> &point)
	{
		stream << std::setprecision(3) << "(";
		for (auto i = 0; i < N; i++)
		{
			if (i < N - 1) stream << point[i] << ", ";
			else stream << point[i];
		}
		stream << ")";
		return stream;
	}
#pragma endregion
}

