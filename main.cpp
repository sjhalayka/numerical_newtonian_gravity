#include "main.h"




vector_4 RayEllipsoid(vector_3 ro, vector_3 rd, vector_3 r)
{
	vector_3 r2 = r * r;
	real_type a = rd.dot(rd / r2);
	real_type b = ro.dot(rd / r2);
	real_type c = ro.dot(ro / r2);
	real_type h = b * b - a * (c - 1.0);

	if (h < 0.0)
		return vector_4(-1, 0, 0, 0);

	real_type t = (-b - sqrt(h)) / a;

	vector_3 pos = ro + rd * t;

	return vector_4(t, pos.x, pos.y, pos.z);
}

vector_3 EllipsoidNormal(vector_3 pos, vector_3 ra)
{
	vector_3 normal = (pos / (ra * ra));
	normal.normalize();

	return -normal;
}




bool circle_intersect(
	vector_3 location, 
	const vector_3 normal, 
	const real_type circle_location, 
	const real_type circle_radius)
{
	vector_3 lowest_dir(circle_location, circle_radius, 0);
	lowest_dir.normalize();

	const vector_3 v(1, 0, 0);

	const real_type d = lowest_dir.dot(v);

	if (d <= 0)
		return false;

	const real_type d_ = normal.dot(v);

	if (d_ <= d)
		return false;

	return true;



	//const vector_3 circle_origin(circle_location, 0, 0);

	//if (normal.dot(circle_origin) <= 0)
	//	return false;

	//vector_3 v = location + normal;

	//const real_type ratio = v.x / circle_origin.x;

	//v.y = v.y / ratio;
	//v.z = v.z / ratio;
	//v.x = circle_origin.x;

	//vector_3 v2;
	//v2.x = circle_origin.x - v.x;
	//v2.y = circle_origin.y - v.y;
	//v2.z = circle_origin.z - v.z;

	//if (v2.length() > circle_radius)
	//	return false;

	//return true;
}

//long long signed int get_intersecting_line_count_integer(
//	const size_t n,
//	const vector_3 sphere_location,
//	const real_type sphere_radius)
//{
//	long long signed int count = 0;
//
//	for (size_t i = 0; i < n; i++)
//	{
//		vector_3 p = RandomUnitVector();
//
//		if (circle_intersect(vector_3(0, 0, 0), p, sphere_location.x, sphere_radius))
//			count++;
//	}
//
//	return count;
//}

//long long signed int get_intersecting_line_count_integer(
//	const size_t n,
//	const vector_3 sphere_location,
//	const real_type sphere_radius)
//{
//	long long signed int count = 0;
//
//	for (size_t i = 0; i < n; i++)
//	{
//		vector_3 p = RandomUnitVector();
//
//		if (circle_intersect(p, p, sphere_location.x, sphere_radius))
//			count++;
//	}
//
//	return count;
//}

long long signed int get_intersecting_line_count_integer(
	const size_t n,
	const vector_3 sphere_location,
	const real_type sphere_radius)
{
	const real_type D = 3.0;
	const real_type disk_like = 3.0 - D;

	long long signed int count = 0;

	for (size_t i = 0; i < n; i++)
	{
		const vector_3 p = RandomUnitVector();
		const vector_4 q = RayEllipsoid(vector_3(0, 0, 0), p, vector_3(1.0 - disk_like, 1.0, 1.0 - disk_like));
		const vector_3 normal = EllipsoidNormal(vector_3(q.y, q.z, q.w), vector_3(1.0 - disk_like, 1.0, 1.0 - disk_like));

		if (circle_intersect(vector_3(0, 0, 0), normal, sphere_location.x, sphere_radius))
			count++;
	}

	return count;
}



real_type get_intersecting_line_count_real(
	const real_type n,
	const vector_3 sphere_location,
	const real_type sphere_radius)
{
	const real_type big_area =
		4 * pi
		* sphere_location.x * sphere_location.x;

	const real_type small_area =
		pi
		* sphere_radius * sphere_radius;

	const real_type ratio =
		small_area
		/ big_area;

	return n * ratio;
}


int main(int argc, char** argv)
{
	const real_type receiver_radius = 1.0;
	real_type emitter_radius = sqrt((1e7 * G * hbar * log(2.0)) / (k * c3 * pi));

	const real_type emitter_area =
		4.0 * pi * emitter_radius * emitter_radius;

	// Field line count
	// re: holographic principle:
	const real_type n =
		(k * c3 * emitter_area)
		/ (log(2.0) * 4.0 * G * hbar);

	const real_type emitter_mass = c2 * emitter_radius / (2.0 * G);

	// 2.39545e47 is the 't Hooft-Susskind constant:
	// the number of field lines for a black hole of
	// unit Schwarzschild radius
	//
	//const real_type G_ = 
	//	(k * c3 * pi) 
	//	/ (log(2.0) * hbar * 2.39545e47);

	const string filename = "newton.txt";
	ofstream out_file(filename.c_str());
	out_file << setprecision(30);

	const real_type start_distance = 10.0;
	const real_type end_distance = 1000.0;
	const size_t distance_res = 10;
		
	const real_type distance_step_size =
		(end_distance - start_distance)
		/ (distance_res - 1);

	for (size_t step_index = 0; step_index < distance_res; step_index++)
	{
		const real_type r =
			start_distance + step_index * distance_step_size;

		const vector_3 receiver_pos(r, 0, 0);

		const real_type epsilon = 1.0;

		vector_3 receiver_pos_plus = receiver_pos;
		receiver_pos_plus.x += epsilon;

		const real_type collision_count_plus =
			get_intersecting_line_count_real(
				n,
				receiver_pos_plus,
				receiver_radius);

		const real_type collision_count =
			get_intersecting_line_count_real(
				n,
				receiver_pos,
				receiver_radius);

		const real_type gradient =
			(static_cast<real_type>(collision_count_plus) - static_cast<real_type>(collision_count))
			/ epsilon;

		const real_type collision_count_plus_integer =
			get_intersecting_line_count_integer(
				n,
				receiver_pos_plus,
				receiver_radius);

		const real_type collision_count_integer =
			get_intersecting_line_count_integer(
				n,
				receiver_pos,
				receiver_radius);

		const real_type gradient_integer =
			(static_cast<real_type>(collision_count_plus_integer) - static_cast<real_type>(collision_count_integer))
			/ epsilon;

		cout << gradient / gradient_integer << endl;


		const real_type gradient_strength =
			-gradient
			/ (receiver_radius * receiver_radius);

		const real_type newton_strength =
			G * emitter_mass / pow(receiver_pos.x, 2.0);

		const real_type newton_strength_ =
			gradient_strength * receiver_pos.x * c * hbar * log(2)
			/ (k * 2 * pi * emitter_mass);	

		cout << "r: " << r << " newton strength: " << newton_strength / newton_strength_ << endl;

		out_file << r << " " << gradient / gradient_integer << endl;
	}

	out_file.close();

	return 0;
}




