#include "main.h"


int main(int argc, char** argv)
{
	const real_type D = 3; // Dimension
	const size_t n = 10000000; // Oscillator count

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

	for (real_type r = start_distance; r <= end_distance; r += distance_step_size)
	{
		const vector_3 receiver_pos(r, 0, 0);
		const real_type receiver_radius = 1;

		const real_type epsilon = 1.0;// 0.1;

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




