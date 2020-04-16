#ifndef LAB2_3
#define LAB2_3
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_max.h>
#include <cilk/reducer_min.h>
#include <cilk/reducer_vector.h>
#include <chrono>
#include <iostream>
const size_t start_size = 10000;
int *mass = new int[start_size];
namespace lab2_3 {

	void ReducerMaxTest(int *mass_pointer, const long size)
	{
		cilk::reducer<cilk::op_max_index<long, int>> maximum;
		cilk_for(long i = 0; i < size; ++i)
		{
			maximum->calc_max(i, mass_pointer[i]);
		}
		std::cout << "Maximal element = " << maximum->get_reference()
			<< " has index = " << maximum->get_index_reference() << std::endl;
	}

	void ReducerMinTest(int *mass_pointer, const long size)
	{
		cilk::reducer<cilk::op_min_index<long, int>> minimum;
		cilk_for(long i = 0; i < size; ++i)
		{
			minimum->calc_min(i, mass_pointer[i]);
		}
		std::cout << "Minimal element = " << minimum->get_reference()
			<< " has index = " << minimum->get_index_reference() << std::endl;
	}

	void ParallelSort(int *begin, int *end)
	{
		if (begin != end)
		{
			--end;
			int *middle = std::partition(begin, end, std::bind2nd(std::less<int>(), *end));
			std::swap(*end, *middle);
			cilk_spawn ParallelSort(begin, middle);
			ParallelSort(++middle, ++end);
			cilk_sync;
		}
	}

	size_t Resize(int sz) {
		size_t newSize = start_size * sz;
		int *newArray = new int[newSize];

		for (size_t i = 0; i < newSize; i++)
			newArray[i] = (rand() % 25000) + 1;

		delete[] mass;
		mass = newArray;
		return newSize;
	}

	void run()
	{

		srand((unsigned)time(0));
		__cilkrts_set_param("nworkers", "4");
		int size_mult[3] = { 10,50,100 };		
		int *mass_begin, *mass_end;
		for (size_t i = 0; i < 3; i++)
		{
			std::cout << "Array size = " << start_size*size_mult[i] << std::endl;
			Resize(size_mult[i]);
			mass_begin = mass;
			mass_end = mass_begin + start_size * size_mult[i];
			std::cout << "Before sort " << std::endl;
			ReducerMaxTest(mass, start_size * size_mult[i]);
			ReducerMinTest(mass, start_size * size_mult[i]);
			auto t1 = std::chrono::high_resolution_clock::now();
			ParallelSort(mass_begin, mass_end);
			auto t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration = (t2 - t1);
			std::cout << "Sort duration is: " << duration.count() << " seconds" << std::endl;
			std::cout << "After sort " << std::endl;
			ReducerMaxTest(mass, start_size * size_mult[i]);
			ReducerMinTest(mass, start_size * size_mult[i]);
			std::cout << std::endl;
		}	
		
		delete[]mass;
	}
}
#endif 
