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


vector_3 RandomUnitVector(void)
{
	const real_type z = dis(generator) * 2.0 - 1.0;
	const real_type a = dis(generator) * 2.0 * pi;

	const real_type r = sqrt(1.0f - z * z);
	const real_type x = r * cos(a);
	const real_type y = r * sin(a);
	
	return vector_3(x, y, z).normalize();
}


#endif
