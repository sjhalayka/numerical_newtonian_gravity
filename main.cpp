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
	const real_type D,
	const long long signed int num_iterations)
{
	const real_type disk_like = 3.0 - D;

	long long signed int count = 0;

	for (long long signed int i = 0; i < num_iterations; i++)
	{
		generator.seed(i);

		for (long long signed int j = 0; j < n; j++)
		{
			if (j % 10000000 == 0)
				cout << float(j) / float(n) << endl;

			const vector_3 p = RandomUnitVector();
			const vector_4 q = RayEllipsoid(vector_3(0, 0, 0), p, vector_3(1.0 - disk_like, 1.0, 1.0 - disk_like));
			const vector_3 normal = EllipsoidNormal(vector_3(q.y, q.z, q.w), vector_3(1.0 - disk_like, 1.0, 1.0 - disk_like));

			if (circle_intersect(normal, sphere_location.x, sphere_radius))
				count++;
		}
	}

	return long long signed int(real_type(count) / real_type(num_iterations));
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
	real_type emitter_radius = sqrt((10e11 * G * hbar * log(2.0)) / (k * c3 * pi));

	const real_type emitter_area =
		4.0 * pi * emitter_radius * emitter_radius;

	// Field line count
	// re: holographic principle:
	const real_type n =
		(k * c3 * emitter_area)
		/ (log(2.0) * 4.0 * G * hbar);

	const real_type emitter_mass = c2 * emitter_radius / (2.0 * G);

	const string filename = "newton.txt";
	ofstream out_file(filename.c_str());
	out_file << setprecision(30);

	const real_type D = 3.0;
	
	const real_type start_distance = 100.0;
	const real_type end_distance = 1000.0;
	const size_t distance_res = 10;
		
	const real_type distance_step_size =
		(end_distance - start_distance)
		/ (distance_res - 1);

	//for (size_t step_index = 0; step_index < distance_res; step_index++)
	{
		const real_type r = 200;
			//start_distance + step_index * distance_step_size;

		const vector_3 receiver_pos(r, 0, 0);

		const real_type epsilon = 1.0;

		const long long signed int num_iterations = 1;

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

		const long long unsigned int collision_count_plus_integer =
			get_intersecting_line_count_integer(
				static_cast<long long signed int>(n),
				receiver_pos_plus,
				receiver_radius,
				D,
				num_iterations);

		const long long unsigned int collision_count_integer =
			get_intersecting_line_count_integer(
				static_cast<long long signed int>(n),
				receiver_pos,
				receiver_radius,
				D,
				num_iterations);

		const real_type gradient_integer =
			(static_cast<real_type>(collision_count_plus_integer) - static_cast<real_type>(collision_count_integer))
			/ epsilon;

		cout << gradient / gradient_integer << endl;


		const real_type gradient_strength =
			-gradient_integer
			/ (receiver_radius * receiver_radius);



		cout << gradient_strength << endl;
		cout << n / (2.0 * pow(receiver_pos.x, D)) << endl;

		cout << gradient_strength / (n / (2.0 * pow(receiver_pos.x, D))) << endl;


		const real_type newton_strength =
			G * emitter_mass / pow(receiver_pos.x, 2.0);

		const real_type newton_strength_ =
			gradient_strength * receiver_pos.x * c * hbar * log(2)
			/ (k * 2 * pi * emitter_mass);

		const real_type newton_strength__ =
			n * c * hbar * log(2)
			/ (4 * k * pi * pow(receiver_pos.x, D - 1.0) * emitter_mass);



		//cout << "r: " << r << " newton strength_: " << newton_strength / newton_strength_ << endl;
		//cout << "r: " << r << " newton strength__: " << newton_strength / newton_strength__ << endl;





		//out_file << r << " " << gradient / gradient_integer << endl;
	}

	out_file.close();

	return 0;
}




