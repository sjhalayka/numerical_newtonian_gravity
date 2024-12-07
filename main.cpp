#include "main.h"

bool circle_intersect(
	vector_3 location, 
	const vector_3 normal, 
	const real_type circle_location, 
	const real_type circle_radius)
{
	if (normal.dot(circle_location) <= 0)
		return false;

	vector_3 v = normal;

	const real_type ratio = v.x / circle_location;

	const vector_3 circle_origin(circle_location, 0, 0);

	v.y = v.y / ratio;
	v.z = v.z / ratio;
	v.x = circle_location;

	vector_3 v2;
	v2.x = circle_origin.x - v.x;
	v2.y = circle_origin.y - v.y;
	v2.z = circle_origin.z - v.z;

	if (v2.length() > circle_radius)
		return false;

	return true;
}

long long signed int get_intersecting_line_count_integer(
	const size_t n,
	const vector_3 sphere_location,
	const real_type sphere_radius)
{
	long long signed int count = 0;

	for (size_t i = 0; i < n; i++)
	{
		vector_3 pos = RandomUnitVector();
		vector_3 normal = pos;

		if (circle_intersect(pos, normal, sphere_location.x, sphere_radius))
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
	real_type emitter_radius = 1.0;// sqrt((10000000 * G * hbar * log(2.0)) / (k * c3 * pi));

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

	const real_type start_distance = (emitter_radius + receiver_radius);
	const real_type end_distance = 100.0;
	const size_t distance_res = 1000;
		
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

		const real_type gradient_strength =
			-gradient
			/ (receiver_radius * receiver_radius);

		const real_type newton_strength =
			G * emitter_mass / pow(receiver_pos.x, 2.0);

		const real_type newton_strength_ =
			gradient_strength * receiver_pos.x * c * hbar * log(2)
			/ (k * 2 * pi * emitter_mass);

		cout << "r: " << r << " newton ratio: "
			<< newton_strength / newton_strength_ << endl;

		out_file << r << " " << newton_strength / newton_strength_ << endl;
	}

	out_file.close();

	return 0;
}




