#include "main.h"


vector_3 random_unit_vector(void)
{
	const real_type z = dis(generator) * 2.0 - 1.0;
	const real_type a = dis(generator) * 2.0 * pi;

	const real_type r = sqrt(1.0f - z * z);
	const real_type x = r * cos(a);
	const real_type y = r * sin(a);

	return vector_3(x, y, z).normalize();
}

vector_3 slerp(vector_3 s0, vector_3 s1, const real_type t)
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

real_type Lerp(real_type a, real_type b, real_type t)
{
	return a + t * (b - a);
}

bool circle_intersect(
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
		if (j % 100000000 == 0)
			cout << float(j) / float(n) << endl;

		const vector_3 p = random_unit_vector();

		vector_3 p_disk = p;
		p_disk.y = 0;
		p_disk.normalize();

		if (p_disk.length() == 0)
			cout << "uh oh" << endl;

		const vector_3 normal = slerp(p, p_disk, disk_like);

		if (circle_intersect(normal, sphere_location.x, sphere_radius))
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

	real_type v_flat_target = 0;


	//ofstream out_file("numerical");
	//out_file << setprecision(30);

	//ofstream out_file2("analytical");
	//out_file2 << setprecision(30);

	const real_type start_dim = 3.0;
	const real_type end_dim = 2.0;
	const size_t dim_res = 10; // Larger than 1
	const real_type dim_step_size = (end_dim - start_dim) / (dim_res - 1);

	// Skip D = 3 for testing purposes
	for (real_type D = start_dim; D >= end_dim; D += dim_step_size)
	{
		const vector_3 receiver_pos(100, 0, 0);

		const real_type epsilon = 0.01;

		vector_3 receiver_pos_plus = receiver_pos;
		receiver_pos_plus.x += epsilon;

		// beta function
		const long long unsigned int collision_count_plus_integer =
			get_intersecting_line_count_integer(
				static_cast<long long signed int>(n),
				receiver_pos_plus,
				receiver_radius,
				D);

		// beta function
		const long long unsigned int collision_count_integer =
			get_intersecting_line_count_integer(
				static_cast<long long signed int>(n),
				receiver_pos,
				receiver_radius,
				D);

		// alpha variable
		const real_type gradient_integer =
			(static_cast<real_type>(collision_count_plus_integer) - static_cast<real_type>(collision_count_integer))
			/ epsilon;

		// g variable
		real_type gradient_strength =
			-gradient_integer
			/ (receiver_radius * receiver_radius);

		//real_type y = n / (2.0 * pow(receiver_pos.x, D));

		// Newtonian acceleration
		real_type a_Newton =
			sqrt(
				(n * G * c * hbar * log(2.0)) /
				(4 * k * pi * pow(receiver_pos.x, 4.0)));



		// Newtonian speed
		real_type v_Newton = sqrt(a_Newton * receiver_pos.x);



		if (v_flat_target == 0)
			v_flat_target = v_Newton * 2.0; // please set this to some constant that suits your liking





		//real_type a_flat_target = sqrt(v_flat_target * receiver_pos.x);

		const real_type a_flat =
			gradient_strength * receiver_pos.x * c * hbar * log(2)
			/ (k * 2 * pi * emitter_mass);

		real_type v_flat = sqrt(a_flat * receiver_pos.x);



		// Newtonian acceleration is proportional to gradient_strength
		//const real_type newton_strength_ =
		//	gradient_strength * receiver_pos.x * c * hbar * log(2)
		//	/ (k * 2 * pi * emitter_mass);
		// ... therefore, the ratio of a_flat/a_Newton is equal to the
		// the ratio of sampled gradient strength / gradient_strength for D = 3

		//real_type D3_gradient_strength = a_Newton * k * 2 * pi * emitter_mass / (receiver_pos.x * c * hbar * log(2));

		//real_type a_ratio = a_flat / a_Newton;
		//real_type grad_ratio = gradient_strength / D3_gradient_strength;

		if (v_flat >= v_flat_target)
		{
			cout << "Final D: " << D << endl;
			cout << v_Newton << " " << v_flat << " " << v_flat_target << endl;
			return 0;
		}
		else
		{
			cout << "Current D: " << D << endl;
			cout << v_Newton << " " << v_flat << " " << v_flat_target << endl;
		}

		//break;

		//out_file << D << " " << gradient_strength << endl;
		//out_file2 << D << " " << y << endl;
	}

	cout << "Found no sufficiently strong D" << endl;
	return 1;
}




