## delaunaypp
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/28225b4a547447248d94fbeabe9b4148)](https://www.codacy.com/app/developerpaul123/delaunaypp?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=DeveloperPaul123/delaunaypp&amp;utm_campaign=Badge_Grade)

### Introduction
A simple, easy to use implementation of Delaunay triangulation in 2D, written in C++. 

This code based on [Paul Bourke's](http://paulbourke.net/papers/) [implmentation](http://paulbourke.net/papers/triangulate/) of the Delaunay triangulation algorithm and is also strongly based on [this](https://github.com/Bl4ckb0ne/delaunay-triangulation) C++ implementation. 

![demo gif](https://github.com/DeveloperPaul123/delaunaypp/blob/master/media/demo.gif)

### Building
To use the `delaunay` class all you need are the files. This is a header only library. To build the tests and the demo application you'll need CMake and Qt5. If you're on windows I recommend using [vcpkg](https://github.com/Microsoft/vcpkg). You can find instructions on how to use `vcpkg` on windows with CMake [here](https://developerpaul123.github.io/c++/using-vcpkg-on-windows/).

To build unit tests, ensure that `BUILD_UNIT_TESTS` is on. To build the demo application, ensure that `BUILD_DEMO_APP` is on. 

### Usage
The interface is quite simple: pass a point cloud (`std::vector` of points) to the `delaunay` object and then call `triangulate()`.

Example:
````cpp
using point = point<double>;
using triangle = triangle<double>;

std::vector<point> data = get_lots_of_points();

delaunaypp::delaunay<point> delaunay(data);
auto triangles = delaunay.triangulate();

// do something with the triangles.
````

I wrote this as a learning exercise, but if you find it useful feel free to use it! Feel free to [contact](https://developerpaul123.github.io//about/) me with any questions. 
