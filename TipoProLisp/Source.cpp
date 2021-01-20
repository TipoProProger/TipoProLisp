#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "MainHeader.h"
#include "Tokenizer.h"
#include "TokenMarker.h"
#include "SECD_Machine.h"

using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::ifstream;

Tokenizer* tokenizerFactory(string filePath)
{
	ifstream *input = new ifstream();
	input->open(filePath);

	Tokenizer* tokenizer = new Tokenizer(input);

	return tokenizer;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	
	Tokenizer* tokenizer = tokenizerFactory("input.txt");

	tokenizer->createTokens();
	//tokenizer->printTokens();

	cout << "\n\n";

	TokenMarker* tokenMarker = new TokenMarker();
	tokenMarker->markTokens(tokenizer->getTokens());

	//tokenMarker->printMarkedTokens();

	//Создаем рутовую машину
	SECD_Machine* machine = new SECD_Machine(nullptr, tokenMarker->getMarkedTokens());

	cout << "\n\n\n";
	machine->run();
	return 0;
}