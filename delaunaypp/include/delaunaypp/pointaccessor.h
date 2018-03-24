#pragma once
#include <exception>

namespace delaunaypp
{
	namespace pointaccess
	{
		enum class axis
		{
			x, 
			y
		};

		template<typename PointType, typename T = typename PointType::value_type>
		struct base {
			virtual ~base() = default;
			virtual T operator()(const PointType & point) = 0;
		};

		template<typename PointType, typename T = typename PointType::value_type>
		struct indexer : public base<PointType>
		{
			explicit indexer(const int &index)
				: _index(index)
			{
				
			}
			T operator()(const PointType& point) override 
			{
				return point[_index];
			}
		private:
			int _index;
		};

		template<typename PointType, typename T = typename PointType::value_type>
		struct getter : public base<PointType>
		{
			explicit getter(const axis &axis)
				: _axis(axis)
			{

			}
			T operator()(const PointType& point) override
			{
				if(_axis == axis::x)
				{
					return point.x();
				}
				if(_axis == axis::y)
				{
					return point.y();
				}
				throw std::exception("Unsupported axis. Only x and y are supported.");
			}
		private:
			axis _axis;
		};
	}
    
}
