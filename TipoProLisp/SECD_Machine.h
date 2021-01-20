#pragma once

#include <vector>
#include <string>
#include <stack>
#include <map>

#include "Cell.h"
#include "TokenMarker.h"

class SECD_Machine
{
public:
	SECD_Machine(SECD_Machine* previousMachine, std::vector<MarkedToken*> markedTokens)
	{
		this->codeRegister = markedTokens;
		this->dumpRegister = previousMachine;
	}

	Cell* eval()
	{
		MarkedToken* token = codeRegister[0];
		
		switch (token->tokenGroup)
		{
		case TOKEN_GROUP_COMMENT://просто выкинуть комент
		{
			codeRegister.erase(codeRegister.begin());
			break;
		}
		case TOKEN_GROUP_NUMB_CONST://нужно вернуть 
		{
			Cell *t = new Cell(nullptr, nullptr);
			t->setData(std::stod(token->token));
			codeRegister.erase(codeRegister.begin());
			return t;
			break;
		}
		case TOKEN_GROUP_STRING_CONST:
		{
			Cell* t = new Cell(nullptr, nullptr);
			t->setData(token);
			codeRegister.erase(codeRegister.begin());
			return t;
			break;
		}
		case TOKEN_GROUP_DEFINED_SYMBOL:
		{
			if (token->token == "(")
			{
				//skip
			}
			else if (token->token == "+")
			{

			}
			else if (token->token == "-")
			{

			}
			else if (token->token == "/")
			{

			}
			else if (token->token == "*")
			{

			}
			///TODO
			break;
		}
		case TOKEN_GROUP_DEFINED_WORD:
		{
			if (token->token == "car")
			{
				
			}
			else if (token->token == "cdr")
			{
			}
			else if (token->token == "cons")
			{
			}
			else if (token->token == "null")
			{
			}
			else if (token->token == "defun")
			{
			}
			else if (token->token == "set")
			{
			}
			else if (token->token == "if")
			{
			}
			else if (token->token == "then")
			{
			}
			else if (token->token == "else")
			{
			}
			else if (token->token == "t")
			{
			}
			else if (token->token == "nil")
			{
			}

			break;
		}
		case TOKEN_GROUP_VARIABLE:
		{
			return environmentRegister.find(token->token)->second;
			break;
		}
		}		
	}

	void apply()
	{

		///TODO
	}

public:
	//регистр вычисленных значений
	std::stack<double> stackRegister;
	//регистр окружения для известных переменных
	std::map<std::string, Cell*> environmentRegister;
	//регистр кода
	std::vector<MarkedToken*> codeRegister;
	//ссылка на предыдущую машину
	SECD_Machine* dumpRegister;
};