#include "lab2_2.h"
#include "lab2_3.h"
#include "lab2_4.h"

int main()
{
	std::cout.setf(std::ios::scientific);
	std::cout << "| TASK 2 | " << std::endl << "==================================" <<std::endl;
	lab2_2::run();
	std::cout << "==================================" << std::endl << std::endl << "| TASK 3 | " << std::endl << std::endl  << "==================================" << std::endl;
	lab2_3::run();
	std::cout << "==================================" << std::endl << std::endl << "| TASK 4 | " << std::endl << std::endl  << "==================================" << std::endl;
	lab2_4::run();
	std::cout << "==================================" << std::endl;
	system("pause");
}