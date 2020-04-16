#ifndef LAB2_2
#define LAB2_2
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_max.h>
#include <cilk/reducer_min.h>
#include <cilk/reducer_vector.h>
#include <chrono>
#include <iostream>

namespace lab2_2 {
	// Нахождение максимального значения в массиве
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
	// Нахождение минимального значения в массиве
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
	// Сортировка массива по возрастанию
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
	void run()
	{
		srand((unsigned)time(0));
		__cilkrts_set_param("nworkers", "4");	// задаем максимальное количество потоков	
		const long mass_size = 10000;
		int *mass_begin, *mass_end;
		int *mass = new int[mass_size];
		for (auto i = 0; i < mass_size; ++i)
		{
			mass[i] = (rand() % 25000) + 1;
		}
		mass_begin = mass;						// указатель на начало массива
		mass_end = mass_begin + mass_size;		// указатель на конец массива
		ReducerMaxTest(mass, mass_size);		// нахождение максимума до сортировка
		ReducerMinTest(mass, mass_size);		// нахождение минимума до сортировка
		ParallelSort(mass_begin, mass_end);		// сортировка массива
		ReducerMaxTest(mass, mass_size);		// нахождение максимума после сортировка
		ReducerMinTest(mass, mass_size);		// нахождение минимума после сортировка
		delete[]mass;	
	}
}
#endif 

