#pragma once

#include "MainHeader.h"

#include <iostream>

#include <vector>
#include <string>
#include <set>

using std::vector;
using std::string;
using std::set;

/*
	������ ����� ����������� ������� ������ ��� ������
	��������� ������:
		- �������� �����
		- �������� ������
		- ��������� ���������
		- �������� ���������
		- �����������
		- ����� ���������������
		- ����������� �����
*/

//��������� ��� �������� ������������ ������
struct MarkedToken
{
	string token;
	int tokenGroup;

	MarkedToken(string token, int tokenGroup);
};

class TokenMarker
{
public:
	TokenMarker();

	void markTokens(vector<string> tokens);
	vector <MarkedToken*> getMarkedTokens();

	void printMarkedTokens();
	
	~TokenMarker();
private:
	vector<MarkedToken*> markedTokens;
	set<string> definedWordTokens;
	set<string> definedSymbolTokens;	
};

