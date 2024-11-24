#ifndef main_H
#define main_H


#include "custom_math.h"
using custom_math::vector_3;
using custom_math::line_segment_3;

#include <iostream>
using std::cout;
using std::endl;

#include <iomanip>
using std::setprecision;

#include <vector>
using std::vector;

#include <string>
using std::string;
using std::to_string;

#include <sstream>
using std::ostringstream;
using std::istringstream;

#include <fstream>
using std::ofstream;
using std::ifstream;

#include <set>
using std::set;

#include <map>
using std::map;

#include <utility>
using std::pair;

#include <mutex>
using std::mutex;

#include <thread>
using std::thread;

#include <random>
std::mt19937 generator(0);
std::uniform_real_distribution<long double> dis(0.0, 1.0);




const real_type G = 6.67430e-11;
const real_type c = 299792458;
const real_type pi = 4.0 * atan(1.0);


vector<vector_3> unit_vectors;


// https://paulbourke.net/geometry/circlesphere/raysphere.c
int RaySphere(
	vector_3 p1, 
	vector_3 p2, 
	vector_3 sc, 
	real_type r, 
	real_type* mu1, 
	real_type* mu2)
{
	real_type a, b, c;
	real_type bb4ac;
	vector_3 dp;

	dp.x = p2.x - p1.x;
	dp.y = p2.y - p1.y;
	dp.z = p2.z - p1.z;
	a = dp.x * dp.x + dp.y * dp.y + dp.z * dp.z;
	b = 2 * (dp.x * (p1.x - sc.x) + dp.y * (p1.y - sc.y) + dp.z * (p1.z - sc.z));
	c = sc.x * sc.x + sc.y * sc.y + sc.z * sc.z;
	c += p1.x * p1.x + p1.y * p1.y + p1.z * p1.z;
	c -= 2 * (sc.x * p1.x + sc.y * p1.y + sc.z * p1.z);
	c -= r * r;
	bb4ac = b * b - 4 * a * c;

	if (fabs(a) < 1e-5 || bb4ac < 0) {
		*mu1 = 0;
		*mu2 = 0;
		return(false);
	}

	*mu1 = (-b + sqrt(bb4ac)) / (2 * a);
	*mu2 = (-b - sqrt(bb4ac)) / (2 * a);

	return(true);
}


vector_3 RandomUnitVector(void)
{
	real_type z = dis(generator) * 2.0 - 1.0;
	real_type a = dis(generator) * 2.0 * pi;

	real_type r = sqrt(1.0f - z * z);
	real_type x = r * cos(a);
	real_type y = r * sin(a);
	return vector_3(x, y, z).normalize();
}


#endif
