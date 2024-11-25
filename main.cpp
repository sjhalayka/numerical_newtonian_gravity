#include "main.h"


long long unsigned int get_intersecting_line_count(
	const vector<vector_3> &unit_vectors,
	const vector_3 sphere_location,
	const real_type sphere_radius)
{
	long long unsigned int count = 0;

	vector_3 cross_section_edge_dir(sphere_location.x, sphere_radius, 0);
	cross_section_edge_dir.normalize();

	vector_3 receiver_dir(sphere_location.x, 0, 0);
	receiver_dir.normalize();

	const real_type min_dot = cross_section_edge_dir.dot(receiver_dir);

	for (size_t i = 0; i < unit_vectors.size(); i++)
		if (unit_vectors[i].dot(receiver_dir) >= min_dot)
			count++;

	return count;
}


int main(int argc, char** argv)
{
	const size_t n = 100000; // Oscillator count


	cout << "Allocating memory for oscillators" << endl;
	vector<vector_3> unit_vectors(n);

	for (size_t i = 0; i < n; i++)
	{
		unit_vectors[i] = RandomUnitVector();

		static const size_t output_mod = 10000;

		if (i % output_mod == 0)
			cout << "Getting pseudorandom locations: " << i << endl;
	}


	//const size_t repulsion_rounds = 10;

	//for (size_t i = 0; i < repulsion_rounds; i++)
	//{
	//	cout << "Repulsing round " << i << endl;

	//	vector<vector_3> unit_vectors_backup = unit_vectors;

	//	for (size_t j = 0; j < n - 1; j++)
	//	{
	//		vector_3 accel;

	//		for (size_t k = j + 1; k < n; k++)
	//		{
	//			if (j == k)
	//				continue;

	//			vector_3 grav_dir = unit_vectors_backup[k] - unit_vectors_backup[j];
	//			
	//			real_type distance = grav_dir.length();
	//			grav_dir.normalize();

	//			// Repulsion
	//			accel += -grav_dir / distance;
	//		}

	//		unit_vectors[j] += accel;
	//	}

	//	for (size_t j = 0; j < n; j++)
	//		unit_vectors[j].normalize();
	//}


	string filename = "newton.txt";
	ofstream out_file(filename.c_str());
	out_file << setprecision(30);

	const real_type start_distance = 10;
	const real_type end_distance = 100;
	const size_t distance_res = 1000;

	const real_type distance_step_size = (end_distance - start_distance) / (distance_res - 1);

	for (size_t step_index = 0; step_index < distance_res; step_index++)
	{
		const real_type r = start_distance + step_index * distance_step_size;

		const vector_3 receiver_pos(r, 0, 0);
		const real_type receiver_radius = 1.0;

		const real_type epsilon = 1.0;

		vector_3 receiver_pos_plus = receiver_pos;
		receiver_pos_plus.x += epsilon;

		// https://en.wikipedia.org/wiki/Directional_derivative
		const long long signed int collision_count_plus = get_intersecting_line_count(unit_vectors, receiver_pos_plus, receiver_radius);
		const long long signed int collision_count = get_intersecting_line_count(unit_vectors, receiver_pos, receiver_radius);
		vector_3 gradient(static_cast<real_type>(collision_count_plus - collision_count) / epsilon, 0, 0);

		const real_type gradient_strength = gradient.length() * pow(receiver_pos.x, 3.0);

		cout << "r: " << r << " gradient strength: " << gradient_strength << endl;

		out_file << r << " " << gradient_strength << endl;
	}

	out_file.close();

	return 0;
}




