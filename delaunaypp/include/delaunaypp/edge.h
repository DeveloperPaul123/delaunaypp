#pragma once
#include "point.h"

#include <math.h>
#include <complex>

inline double pi() { return std::atan(1) * 4; }

namespace delaunaypp
{
	namespace internal
	{
		template<typename PointType, typename T = typename PointType::value_type>
		class edge
		{
		public:
			edge() = default;
			edge(PointType start, PointType end);
			edge(const edge &other);
			~edge() = default;

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

		template<typename PointType, typename T>
		edge<PointType, T> rotate(const edge<PointType, T> &input, const double &degrees)
		{
			auto radians = (degrees / 180.0) * pi();
			auto start = input.start();
			auto end = input.end();
			std::complex<double> vect(end.x() - start.x(), end.y() - start.y());
			std::complex<double> rot(std::cos(radians), std::sin(radians)); // 90 degrees
			vect *= rot;

			return edge<PointType, T>(start, { vect.real() + start.x(),vect.imag() + start.y() });
		}

		template <typename PointType, typename T>
		edge<PointType, T>::edge(PointType start, PointType end) :
			start_(std::move(start)), end_(std::move(end)), is_bad_(false)
		{

		}

		template <typename PointType, typename T>
		edge<PointType, T>::edge(const edge& other)
		{
			start_ = other.start();
			end_ = other.end();
			is_bad_ = false;
		}

		template <typename PointType, typename T>
		bool edge<PointType, T>::operator<(const edge& other) const
		{
			return (start() == other.start() ? end() < other.end() : start() < other.start()) &&
				!(*this == other);
		}

		template <typename PointType, typename T>
		PointType edge<PointType, T>::start() const
		{
			return start_;
		}

		template <typename PointType, typename T>
		PointType edge<PointType, T>::end() const
		{
			return end_;
		}

		template <typename PointType, typename T>
		T edge<PointType, T>::length()
		{
			return std::sqrt(std::pow(end_.x() - start_.x(), 2.0) + std::pow(end_.y() - start_.y(), 2.0));
		}

		template <typename PointType, typename T>
		PointType edge<PointType, T >::midpoint()
		{
			return { (start_.x() + end_.x()) / 2.0, (start_.y() + end_.y()) / 2.0 };
		}

		template <typename PointType, typename T>
		T edge<PointType, T>::slope()
		{
			// TODO check for divide by 0
			return (end_.y() - start_.y()) / (end_.x() - start_.x());
		}

		template <typename PointType, typename T>
		void edge<PointType, T>::set_is_bad(const bool& is_bad)
		{
			is_bad_ = is_bad;
		}

		template <typename PointType, typename T>
		bool edge<PointType, T>::is_bad() const
		{
			return is_bad_;
		}

		template <typename PointType, typename T>
		inline bool operator==(const edge<PointType, T> &first, const edge<PointType, T>& second)
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
