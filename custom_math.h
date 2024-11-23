#ifndef custom_math_h
#define custom_math_h


//#include <boost/multiprecision/cpp_bin_float.hpp>
//using namespace boost::multiprecision;
//using cpp_bin_float_100_ = number<backends::cpp_bin_float<1024, backends::digit_base_10, void, std::int16_t, -1022, 1023>, et_off >;
//typedef cpp_bin_float_100_ real_type;
//typedef long double real_type;
//typedef number<backends::cpp_bin_float<237, backends::digit_base_2, void, std::int32_t, -262142, 262143>, et_off>  cpp_bin_float_oct;
//typedef cpp_bin_float_oct real_type;



typedef long double real_type;



#include <algorithm>
using std::sort;

#include <limits>
using std::numeric_limits;

#include <vector>
using std::vector;

#include <set>
using std::set;

#include <map>
using std::map;


#include <iostream>
using std::cout;
using std::endl;

#include <cmath>

namespace custom_math
{
	class vector_3;
	class line_segment_3;

	real_type d(const real_type &a, const real_type &b);
	real_type d_3(const vector_3 &a, const vector_3 &b);
	real_type d_3_sq(const vector_3 &a, const vector_3 &b);
};

class custom_math::vector_3
{
public:
	real_type x, y, z;

	inline bool operator<(const vector_3& right) const
	{
		if (right.x > x)
			return true;
		else if (right.x < x)
			return false;

		if (right.y > y)
			return true;
		else if (right.y < y)
			return false;

		if (right.z > z)
			return true;
		else if (right.z < z)
			return false;

		return false;
	}

	vector_3(const real_type &src_x = 0, const real_type &src_y = 0, const real_type &src_z = 0);
	bool operator==(const vector_3 &rhs);
	bool operator!=(const vector_3 &rhs);
	void zero(void);
	void rotate_x(const real_type &radians);
	void rotate_y(const real_type &radians);
    void rotate_z(const real_type &radians);
    vector_3 operator+(const vector_3 &rhs);
	vector_3 operator-(const vector_3 &rhs) const;
	vector_3 operator*(const vector_3 &rhs);
	vector_3 operator/(const vector_3& rhs);
	vector_3 operator*(const real_type &rhs);
	vector_3 operator/(const real_type &rhs);
	vector_3 &operator=(const vector_3 &rhs);
	vector_3 &operator+=(const vector_3 &rhs);
	vector_3 &operator*=(const vector_3 &rhs);
	vector_3 &operator*=(const real_type &rhs);
	vector_3 operator-(void);
	real_type length(void) const;
	vector_3 &normalize(void);
	real_type dot(const vector_3 &rhs) const;
	real_type self_dot(void) const;
	vector_3 cross(const vector_3 &rhs) const;
};



class custom_math::line_segment_3
{
public:
	vector_3 start, end;

	real_type length(void)
	{
		return d_3(start, end);
	}

	bool operator<(line_segment_3 &rhs)
	{
		return length() < rhs.length();
	}
};








#endif

