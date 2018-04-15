#pragma once
#include "point.h"

#include <math.h>
#include <complex>

inline double pi() { return std::atan(1) * 4; }

namespace delaunaypp
{
	namespace internal
	{
		template<typename T>
		class edge
		{
			using PointType = point<T>;

		public:
			edge() = default;
			edge(PointType start, PointType end);
			edge(const edge &other);
			edge(edge &&other) noexcept;
			~edge() = default;
			void operator=(const edge& other);
			void operator=(edge&& other) noexcept;
			bool operator<(edge& other);
			bool operator<(const edge& other) const;
			PointType start() const;
			PointType end() const;
			T length();
			PointType midpoint();
			T slope();
			void set_is_bad(const bool &is_bad);
			bool is_bad() const;
		private:
			PointType start_;
			PointType end_;
			bool is_bad_;
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
			start_(std::move(start)), end_(std::move(end)), is_bad_(false)
		{

		}

		template <typename T>
		edge<T>::edge(const edge& other)
		{
			start_ = other.start();
			end_ = other.end();
			is_bad_ = false;
		}

		template <typename T>
		edge<T>::edge(edge&& other) noexcept
		{
			start_ = std::move(other.start());
			end_ = std::move(other.end());
			is_bad_ = std::move(other.is_bad());
		}

		template <typename T>
		void edge<T>::operator=(const edge& other)
		{
			start_ = other.start();
			end_ = other.end();
		}

		template <typename T>
		void edge<T>::operator=(edge&& other) noexcept
		{
			start_ = std::move(other.start());
			end_ = std::move(other.end());
		}

		template <typename T>
		bool edge<T>::operator<(edge& other)
		{
			return std::tie(start_, end_) < std::tie(other.start(), other.end());
		}

		template <typename T>
		bool edge<T>::operator<(const edge& other) const
		{
			return std::tie(start_, end_) < std::tie(other.start(), other.end());
		}

		template <typename T>
		typename edge<T>::PointType edge<T>::start() const
		{
			return start_;
		}

		template <typename T>
		typename edge<T>::PointType edge<T>::end() const
		{
			return end_;
		}

		template <typename T>
		T edge<T>::length()
		{
			return std::sqrt(std::pow(end_.x() - start_.x(), 2.0) + std::pow(end_.y() - start_.y(), 2.0));
		}

		template <typename T>
		typename edge<T>::PointType edge<T>::midpoint()
		{
			return { (start_.x() + end_.x()) / 2.0, (start_.y() + end_.y()) / 2.0 };
		}

		template <typename T>
		T edge<T>::slope()
		{
			// TODO check for divide by 0
			return (end_.y() - start_.y()) / (end_.x() - start_.x());
		}

		template <typename T>
		void edge<T>::set_is_bad(const bool& is_bad)
		{
			is_bad_ = is_bad;
		}

		template <typename T>
		bool edge<T>::is_bad() const
		{
			return is_bad_;
		}

		template<typename T>
		inline bool operator==(const edge<T> &first, const edge<T>& second)
		{
			return (first.start() == second.start() && first.end() == second.end()) ||
				(first.start() == second.end() && first.end() == second.start());
		}


		template<typename T>
		inline std::ostream& operator<<(std::ostream& stream, const edge<T> &edge)
		{
			stream << "[" << edge.start() << " " << edge.end() << "]";
			return stream;
		}
	}
}
