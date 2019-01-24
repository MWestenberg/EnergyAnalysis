#pragma once
#include <iostream>
#include <string>

class ProgressBar
{
public:
	int neededProgress = 100;

	void update(double newProgress);

	void print();

	void SetTotalNumber(unsigned total);

	void PrintNumber() const;


	std::string firstPartOfpBar = "[", //Change these at will (that is why I made them public)
		lastPartOfpBar = "]",
		pBarFiller = "=",
		pBarUpdater = "/=\\|";


private:
	int amountOfFiller,
		pBarLength = 100,
		currUpdateVal = 0;
	int m_totalNumber = 0;
	double currentProgress = 0;


};

