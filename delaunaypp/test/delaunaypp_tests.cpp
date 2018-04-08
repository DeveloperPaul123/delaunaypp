#include <gtest/gtest.h>

#include <delaunaypp/point.h>
#include <delaunaypp/pointaccessor.h>
#include "delaunaypp/edge.h"
#include "delaunaypp/triangle.h"
#include "delaunaypp/delaunay.h"

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

TEST(EdgeTests, midpointTest1)
{
	using point = point<double>;
	point p1(0.0, -1.0);
	point p2(0.0, 1.0);
	
	edge<double> edge(p1, p2);

	point answer(0.0, 0.0);

	EXPECT_EQ(edge.midpoint(), answer);
}

TEST(EdgeTests, midpointTest2)
{
	using point = point<double>;
	point p1(-20, 0.0);
	point p2(20.0, 0.0);

	edge<double> edge(p1, p2);

	point answer(0.0, 0.0);

	EXPECT_EQ(edge.midpoint(), answer);
}

TEST(EdgeTests, slopeTest)
{
	using point = point<double>;
	point p1(-20, 0.0);
	point p2(0.0, 20.0);

	edge<double> edge(p1, p2);

	EXPECT_EQ(edge.slope(), 1.0);
}

TEST(EdgeTests, rotationTest)
{
	using point = point<double>;

	point p1(1.0, 1.0);
	point p2(3.0, 1.0);

	edge<double> edge(p1, p2);
	// rotate clockwise by using a negative angle.
	auto rotated = rotate(edge, -90.0);

	EXPECT_EQ(rotated.start(), p1);
	EXPECT_DOUBLE_EQ(rotated.end().x(), 1.0);
	EXPECT_DOUBLE_EQ(rotated.end().y(), -1.0);
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

TEST(TriangleTests, circumcircleTest1)
{
	using point = point<double>;
	using triangle = triangle<double>;

	point p1(0.0, 2.0);
	point p2(2.0, -2.0);
	point p3(-2.0, -2.0);

	point answer(0, -0.5);

	triangle tri(p1, p2, p3);

	auto cc = tri.circumcircle();
	
	std::cout << "Calculated circumcircle center: " << cc.first << std::endl;
	std::cout << "Expected: " << answer << std::endl;

	EXPECT_NEAR(cc.first[0], answer[0], 1e-6);
	EXPECT_NEAR(cc.first[1], answer[1], 1e-6);
}

TEST(TriangleTests, circumcircleTest2)
{
	using point = point<double>;
	using triangle = triangle<double>;

	point p1(1.0, 3.0);
	point p2(5.0, 4.0);
	point p3(-1.0, 1.0);

	point answer(4.5, -2.5);

	triangle tri(p1, p2, p3);

	auto cc = tri.circumcircle();

	std::cout << "Calculated circumcircle center: " << cc.first << std::endl;
	std::cout << "Expected: " << answer << std::endl;

	EXPECT_NEAR(cc.first[0], answer[0], 1e-6);
	EXPECT_NEAR(cc.first[1], answer[1], 1e-6);
}

//TEST(DelaunayTests, simpleSquareTests)
//{
//	using point = point<double>;
//	using triangle = triangle<double>;
//
//	const point p1(-2.0, 2.0);
//	const point p2(2.0, 2.0);
//	const point p3(2.0, -2.0);
//	const point p4(-2.0, -2.0);
//
//	delaunay<point> del({ p1, p2, p3, p4 });
//	auto triangles = del.triangulate();
//
//	ASSERT_EQ(triangles.size(), 2);
//}