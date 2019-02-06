// TimerTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <chrono>


unsigned tot = 0;

bool IsPrime(int n)
{
	bool aPrime = true;
	for (int t = 2; t <= n; t++)
	{
		if (n % t == 0)
		{
			aPrime = false;
			break;
		}
	}
	return aPrime;
}


int main() {

	int max = 100000;
	auto start = std::chrono::steady_clock::now();

	for (int i = 1; i < max; i++)
	{
		if (IsPrime(i))
			tot++;
	}
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);

	std::cout << "It took " << elapsed.count() << " milliseconds. Primes found: " << tot << "!" << std::endl;

	return 1;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
