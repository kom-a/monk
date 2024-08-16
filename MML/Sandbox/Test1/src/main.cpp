#include <iostream>

#include <MMath/MMath.h>

namespace mmath
{
	std::ostream& operator<< (std::ostream& stream, const mmath::vec3& vec)
	{
		stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return stream;
	}
}

int main()
{
	mmath::vec3 v1(1, 2, 3);
	mmath::vec3 v2(3, 2, 1);
	mmath::vec3 v3 = v1 * v2;

	std::cout << v3 << std::endl;


	return 0;
}