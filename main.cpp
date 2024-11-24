#include "main.h"


long long unsigned int get_intersecting_line_count(
	const vector_3 sphere_location,
	const real_type sphere_radius)
{
	long long unsigned int count = 0;

	for (size_t i = 0; i < unit_vectors.size(); i++)
	{
		const vector_3 veca = unit_vectors[i];

		vector_3 vecb(sphere_location.x, sphere_radius, 0);
		vecb.normalize();

		vector_3 vecc(sphere_location.x, 0, 0);
		vecc.normalize();

		real_type veca_dot = veca.dot(vecc);
		real_type vecb_dot = vecb.dot(vecc);

		if (veca_dot >= vecb_dot)
			count++;
	}

	return count;
}

int main(int argc, char** argv)
{
	const real_type D = 3; // Dimension
	const size_t n = 100000; // Oscillator count

	cout << "Allocating memory for oscillators" << endl;
	unit_vectors.resize(n);

	for (size_t i = 0; i < n; i++)
	{
		unit_vectors[i] = RandomUnitVector();

		static const size_t output_mod = 10000;

		if (i % output_mod == 0)
			cout << "Getting pseudorandom locations: " << i << " of " << n << endl;
	}

	string filename = to_string(D) + ".txt";
	ofstream out_file(filename.c_str());
	out_file << setprecision(30);

	const real_type start_distance = 10;
	const real_type end_distance = 100;
	const size_t distance_res = 1000;
	const real_type distance_step_size = (end_distance - start_distance) / (distance_res - 1);

	for (size_t z = 0; z < distance_res; z++)
	{
		const real_type r = start_distance + z * distance_step_size;

		const vector_3 receiver_pos(r, 0, 0);
		const real_type receiver_radius = 1;

		const real_type epsilon = 1.0;

		vector_3 receiver_pos_plus = receiver_pos;
		receiver_pos_plus.x += epsilon;

		// https://en.wikipedia.org/wiki/Directional_derivative
		const long long signed int collision_count_plus = get_intersecting_line_count(receiver_pos_plus, receiver_radius);
		const long long signed int collision_count = get_intersecting_line_count(receiver_pos, receiver_radius);
		vector_3 gradient(static_cast<real_type>(collision_count_plus - collision_count) / epsilon, 0, 0);

		const real_type gradient_strength = gradient.length() * pow(receiver_pos.x, D);

		cout << "D: " << D << " r: " << r << " " << gradient_strength << endl;

		out_file << r << " " << gradient_strength << endl;
	}

	out_file.close();

	return 0;
}




