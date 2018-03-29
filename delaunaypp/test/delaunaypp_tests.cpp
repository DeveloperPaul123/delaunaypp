#include <gtest/gtest.h>

#include <delaunaypp/point.h>
#include <delaunaypp/pointaccessor.h>
#include "delaunaypp/edge.h"
#include "delaunaypp/triangle.h"

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

TEST(EdgeTests, equalityTest)
{
	using point = point<double>;
	using edge = edge<double>;

	point p1(4.0, 5.0);
	point p2(5.6, 5.0);
	point p3(1.0, -8.0);
	
	edge edge1(p1, p2);
	edge edge2(p2, p3);
	edge edge3(p3, p1);

	EXPECT_FALSE(edge1 == edge2);
	EXPECT_FALSE(edge1 == edge3);
	EXPECT_FALSE(edge2 == edge3);

	EXPECT_TRUE(edge1 == edge1);
	EXPECT_TRUE(edge2 == edge2);
	EXPECT_TRUE(edge3 == edge3);
}

TEST(TriangleTests, equalityTest)
{
	using point = point<double>;

	point p1(4.0, 5.0);
	point p2(5.6, 5.0);
	point p3(1.0, -8.0);

	triangle<double> tri1(p1, p2, p3);
	triangle<double> tri2(p2, p1, p3);

	EXPECT_TRUE(tri1 == tri2);
}