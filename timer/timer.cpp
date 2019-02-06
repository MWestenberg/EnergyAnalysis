#include <iostream>
#include <chrono>


unsigned tot  = 0;

bool IsPrime(int n)
{
	bool aPrime = true;
	for (int t = 2; t <=n; t++)
	{
		if (n % t ==0)
		{
			aPrime = false;
			break;
		}
	}
	return aPrime;
}


int main(){
		
	int max = 50000;
	auto start = std::chrono::steady_clock::now( );

	for (int i = 1; i< max; i++)
	{
		if (IsPrime(i))
			tot++;
	}
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now( ) - start );

	std::cout << "It took " << elapsed.count() << " milliseconds. Primes found: " << tot << "!" << std::endl;

	return 1;
}