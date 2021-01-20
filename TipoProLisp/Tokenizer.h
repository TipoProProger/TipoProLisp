#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using std::vector;
using std::string;

/*
	Данный класс считывает из переданного потока ввода программу
	и разбивает ее на токены
*/

class Tokenizer
{
public:
	Tokenizer(std::ifstream *input);
	Tokenizer();
	void createTokens();
	void printTokens();

	vector<string> getTokens();
	~Tokenizer();
private:
	string extractToken(string& program, unsigned int& currentPosition, bool addlSymbol = false);
	bool emptyString(string str);
	void addToken(vector<string>& tokens, string& program, unsigned int& currentPosition, bool addSymbol = false);
	vector<string> createTokensFromString(string programLine);
	void mergeTokens(vector<string>& tokens, vector<string> tokensToAdd);

	std::ifstream *input;
	vector<string> tokens;
};

