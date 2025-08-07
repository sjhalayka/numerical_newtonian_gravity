#include "main.h"


vector_3 RandomUnitVector(void)
{
	const real_type z = dis(generator) * 2.0 - 1.0;
	const real_type a = dis(generator) * 2.0 * pi;

	const real_type r = sqrt(1.0f - z * z);
	const real_type x = r * cos(a);
	const real_type y = r * sin(a);

	return vector_3(x, y, z).normalize();
}

vector_3 SLerp(vector_3 s0, vector_3 s1, const real_type t)
{
	vector_3 s0_norm = s0;
	s0_norm.normalize();

	vector_3 s1_norm = s1;
	s1_norm.normalize();

	const real_type cos_angle = s0_norm.dot(s1_norm);
	const real_type angle = acos(cos_angle);

	const real_type p0_factor = sin((1 - t) * angle) / sin(angle);
	const real_type p1_factor = sin(t * angle) / sin(angle);

	return s0 * p0_factor + s1 * p1_factor;
}

bool circle_intersect(
	const vector_3 location,
	const vector_3 normal, 
	const real_type circle_location, 
	const real_type circle_radius)
{
	vector_3 outline_dir(circle_location, circle_radius, 0);
	outline_dir.normalize();

	static const vector_3 v(1, 0, 0);

	const real_type d = outline_dir.dot(v);

	if (d <= 0)
		return false;

	const real_type d_ = normal.dot(v);

	if (d_ <= d)
		return false;

	return true;
}

long long signed int get_intersecting_line_count_integer(
	const long long signed int n,
	const vector_3 sphere_location,
	const real_type sphere_radius,
	const real_type D)
{
	const real_type disk_like = 3.0 - D;
	//const real_type fractionality = 1.0 - 2.0 * (0.5 - fmod(D, 1.0));

	long long signed int count = 0;

	generator.seed(static_cast<unsigned>(0));

	for (long long signed int j = 0; j < n; j++)
	{
		if (j % 10000000 == 0)
			cout << float(j) / float(n) << endl;

		const vector_3 p = RandomUnitVector();

		vector_3 p_disk = p;
		p_disk.y = 0;
		p_disk.normalize();

		if (p_disk.length() == 0)
			cout << "uh oh" << endl;

		const vector_3 normal = SLerp(p, p_disk, disk_like);

		if (circle_intersect(vector_3(0, 0, 0), normal, sphere_location.x, sphere_radius))
			count++;
	}

	return count;
}


int main(int argc, char** argv)
{
	const real_type receiver_radius = 1.0;
	real_type emitter_radius = sqrt((10e8 * G * hbar * log(2.0)) / (k * c3 * pi));

	const real_type emitter_area =
		4.0 * pi * emitter_radius * emitter_radius;

	// Field line count
	// re: holographic principle:
	const real_type n =
		(k * c3 * emitter_area)
		/ (log(2.0) * 4.0 * G * hbar);

	const real_type emitter_mass = c2 * emitter_radius / (2.0 * G);

	ofstream out_file("numerical");
	out_file << setprecision(30);

	ofstream out_file2("analytical");
	out_file2 << setprecision(30);

	const real_type start_dim = 2.001;
	const real_type end_dim = 3.0;
	const size_t dim_res = 1000; // Larger than 1
	const real_type dim_step_size = (end_dim - start_dim) / (dim_res - 1);

	for (real_type D = start_dim; D <= end_dim; D += dim_step_size)
	{
		const vector_3 receiver_pos(100, 0, 0);

		const real_type epsilon = 1.0;

		vector_3 receiver_pos_plus = receiver_pos;
		receiver_pos_plus.x += epsilon;

		const long long unsigned int collision_count_plus_integer =
			get_intersecting_line_count_integer(
				static_cast<long long signed int>(n),
				receiver_pos_plus,
				receiver_radius,
				D);

		const long long unsigned int collision_count_integer =
			get_intersecting_line_count_integer(
				static_cast<long long signed int>(n),
				receiver_pos,
				receiver_radius,
				D);

		const real_type gradient_integer =
			(static_cast<real_type>(collision_count_plus_integer) - static_cast<real_type>(collision_count_integer))
			/ epsilon;


		real_type gradient_strength =
			-gradient_integer
			/ (receiver_radius * receiver_radius);



		real_type x = n / (2.0 * pow(receiver_pos.x, D));

		cout << gradient_strength << endl;
		cout << x << endl;

		cout << gradient_strength / x << endl;




		//const real_type newton_strength =
		//	G * emitter_mass / pow(receiver_pos.x, 2.0);

		//const real_type newton_strength_ =
		//	gradient_strength * receiver_pos.x * c * hbar * log(2)
		//	/ (k * 2 * pi * emitter_mass);

		//const real_type newton_strength__ =
		//	n * c * hbar * log(2)
		//	/ (4 * k * pi * pow(receiver_pos.x, D - 1.0) * emitter_mass);

		//cout << "r: " << r << " newton strength_: " << newton_strength / newton_strength_ << endl;
		//cout << "r: " << r << " newton strength__: " << newton_strength / newton_strength__ << endl;





		out_file << D << " " << gradient_strength << endl;

		out_file2 << D << " " << x << endl;
	}

	return 0;
}




