#include <gtest/gtest.h>

#include <delaunaypp/point.h>
#include <delaunaypp/pointaccessor.h>

using namespace delaunaypp;
TEST(PointAccessorTests, testIndexAccessor)
{
	using point = point<double, 2>;
	point test(2.0, 5.0);

	pointaccess::indexer<point> x_access(0);
	pointaccess::indexer<point> y_access(1);

	auto x = x_access(test);
	auto y = y_access(test);

	EXPECT_DOUBLE_EQ(x, test.x());
	EXPECT_DOUBLE_EQ(y, test.y());
}

TEST(PointAccessorTests, testGetterAccessor)
{
	using point = point<double, 2>;
	point test(5.0, 40.0);

	pointaccess::getter<point> x_access(pointaccess::axis::x);
	pointaccess::getter<point> y_access(pointaccess::axis::y);

	auto x = x_access(test);
	auto y = y_access(test);

	EXPECT_DOUBLE_EQ(x, test.x());
	EXPECT_DOUBLE_EQ(y, test.y());
}