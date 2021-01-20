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

	Cell* eval(std::vector<MarkedToken*> codeRegister)
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
			Cell* t = new Cell(nullptr, nullptr);
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
				std::vector<MarkedToken*> buffer = getBracers(codeRegister, 0, findBracers(codeRegister));

				return eval(buffer);
			}
			else if (token->token == "+")
			{
				codeRegister.erase(codeRegister.begin());

				Cell* buffer = new Cell(nullptr, nullptr);
				buffer->setData((double)0);
				while (!codeRegister.empty())
				{
					addProc(buffer, eval(getFirstElem(codeRegister)));
				}

				return buffer;
			}
			else if (token->token == "-")
			{
				codeRegister.erase(codeRegister.begin());

				Cell* buffer = new Cell(nullptr, nullptr);
				buffer->setData(eval(getFirstElem(codeRegister)));
				while (!codeRegister.empty())
				{
					subProc(buffer, eval(getFirstElem(codeRegister)));
				}

				return buffer;
			}
			else if (token->token == "/")
			{
				codeRegister.erase(codeRegister.begin());

				Cell* buffer = new Cell(nullptr, nullptr);
				buffer->setData(eval(getFirstElem(codeRegister)));
				while (!codeRegister.empty())
				{
					divProc(buffer, eval(getFirstElem(codeRegister)));
				}

				return buffer;
			}
			else if (token->token == "*")
			{
				codeRegister.erase(codeRegister.begin());

				Cell* buffer = new Cell(nullptr, nullptr);
				buffer->setData(eval(getFirstElem(codeRegister)));
				while (!codeRegister.empty())
				{
					mulProc(buffer, eval(getFirstElem(codeRegister)));
				}

				return buffer;
			}
			else if (token->token == "<")
			{
				codeRegister.erase(codeRegister.begin());

				Cell* buffer = new Cell(nullptr, nullptr);
				buffer->setData(eval(getFirstElem(codeRegister)));
				while (!codeRegister.empty())
				{
					buffer = lessProc(buffer, eval(getFirstElem(codeRegister)));
				}

				return buffer;
			}
			else if (token->token == "<=")
			{
				codeRegister.erase(codeRegister.begin());

				Cell* buffer = new Cell(nullptr, nullptr);
				buffer->setData(eval(getFirstElem(codeRegister)));
				while (!codeRegister.empty())
				{
					buffer = lessEquProc(buffer, eval(getFirstElem(codeRegister)));
				}

				return buffer;
			}
			else if (token->token == "=")
			{
				codeRegister.erase(codeRegister.begin());

				Cell* buffer = new Cell(nullptr, nullptr);
				buffer->setData(eval(getFirstElem(codeRegister)));
				while (!codeRegister.empty())
				{
					buffer = equProc(buffer, eval(getFirstElem(codeRegister)));
				}

				return buffer;
			}
			else if (token->token == ">")
			{
				codeRegister.erase(codeRegister.begin());

				Cell* buffer = new Cell(nullptr, nullptr);
				buffer->setData(eval(getFirstElem(codeRegister)));
				while (!codeRegister.empty())
				{
					buffer = moreProc(buffer, eval(getFirstElem(codeRegister)));
				}

				return buffer;
			}
			else if (token->token == ">=")
			{
				codeRegister.erase(codeRegister.begin());

				Cell* buffer = new Cell(nullptr, nullptr);
				buffer->setData(eval(getFirstElem(codeRegister)));
				while (!codeRegister.empty())
				{
					buffer = moreEquProc(buffer, eval(getFirstElem(codeRegister)));
				}

				return buffer;
			}
			else if (token->token == "!")
			{
				codeRegister.erase(codeRegister.begin());

				Cell* buffer = new Cell(nullptr, nullptr);
				buffer->setData(eval(getFirstElem(codeRegister)));
				buffer = notProc(buffer);

				return buffer;
			}
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
				//function name
				MarkedToken* funcitonName = codeRegister[1];

				//args or empty bracers


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
				Cell *cell = new Cell(nullptr, nullptr);
				cell->setData(true);
				return cell;
			}
			else if (token->token == "nil")
			{
				Cell* cell = new Cell(nullptr, nullptr);
				cell->setData(false);
				return cell;
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


#pragma region Arifmetic functions
private: void addProc(Cell* a, Cell* b)
{
	std::pair<int, std::vector<MarkedToken*> > aPart = a->getData();
	std::pair<int, std::vector<MarkedToken*> > bPart = b->getData();
	if (aPart.first == Cell::TYPE_DOUDLE &&
		bPart.first == Cell::TYPE_DOUDLE)
	{
		double sum = getNumberFromToken(aPart.second[0]);
		sum += getNumberFromToken(bPart.second[0]);
		a->setData(sum);
	}
}
private: void subProc(Cell* a, Cell* b)
{
	std::pair<int, std::vector<MarkedToken*> > aPart = a->getData();
	std::pair<int, std::vector<MarkedToken*> > bPart = b->getData();
	if (aPart.first == Cell::TYPE_DOUDLE &&
		bPart.first == Cell::TYPE_DOUDLE)
	{
		double sub = getNumberFromToken(aPart.second[0]);
		sub -= getNumberFromToken(bPart.second[0]);
		a->setData(sub);
	}
}
private: void divProc(Cell* a, Cell* b)
{
	std::pair<int, std::vector<MarkedToken*> > aPart = a->getData();
	std::pair<int, std::vector<MarkedToken*> > bPart = b->getData();
	if (aPart.first == Cell::TYPE_DOUDLE &&
		bPart.first == Cell::TYPE_DOUDLE)
	{
		double div = getNumberFromToken(aPart.second[0]);
		div /= getNumberFromToken(bPart.second[0]);
		a->setData(div);
	}
}
private: void mulProc(Cell* a, Cell* b)
{
	std::pair<int, std::vector<MarkedToken*> > aPart = a->getData();
	std::pair<int, std::vector<MarkedToken*> > bPart = b->getData();
	if (aPart.first == Cell::TYPE_DOUDLE &&
		bPart.first == Cell::TYPE_DOUDLE)
	{
		double mul = getNumberFromToken(aPart.second[0]);
		mul *= getNumberFromToken(bPart.second[0]);
		a->setData(mul);
	}
}
#pragma endregion

#pragma region Logic functions
private: Cell* lessProc(Cell* a, Cell* b)
{
	std::pair<int, std::vector<MarkedToken*> > aPart = a->getData();
	std::pair<int, std::vector<MarkedToken*> > bPart = b->getData();
	if (aPart.first == Cell::TYPE_DOUDLE &&
		bPart.first == Cell::TYPE_DOUDLE)
	{
		Cell* answer = new Cell(nullptr, nullptr);
		if (getNumberFromToken(aPart.second[0]) < getNumberFromToken(bPart.second[0]))
			answer->setData(true);
		else answer->setData(false);

		return answer;
	}
	else if (aPart.first == Cell::TYPE_STRING &&
		bPart.first == Cell::TYPE_STRING)
	{
		Cell* answer = new Cell(nullptr, nullptr);
		if (aPart.second[0] < bPart.second[0])
			answer->setData(true);
		else answer->setData(false);

		return answer;
	}
}
private: Cell* lessEquProc(Cell* a, Cell* b)
{
	std::pair<int, std::vector<MarkedToken*> > aPart = a->getData();
	std::pair<int, std::vector<MarkedToken*> > bPart = b->getData();
	if (aPart.first == Cell::TYPE_DOUDLE &&
		bPart.first == Cell::TYPE_DOUDLE)
	{
		Cell* answer = new Cell(nullptr, nullptr);
		if (getNumberFromToken(aPart.second[0]) <= getNumberFromToken(bPart.second[0]))
			answer->setData(true);
		else answer->setData(false);

		return answer;
	}
	else if (aPart.first == Cell::TYPE_STRING &&
		bPart.first == Cell::TYPE_STRING)
	{
		Cell* answer = new Cell(nullptr, nullptr);
		if (aPart.second[0] <= bPart.second[0])
			answer->setData(true);
		else answer->setData(false);

		return answer;
	}
}
private: Cell* moreProc(Cell* a, Cell* b)
{
	std::pair<int, std::vector<MarkedToken*> > aPart = a->getData();
	std::pair<int, std::vector<MarkedToken*> > bPart = b->getData();
	if (aPart.first == Cell::TYPE_DOUDLE &&
		bPart.first == Cell::TYPE_DOUDLE)
	{
		Cell* answer = new Cell(nullptr, nullptr);
		if (getNumberFromToken(aPart.second[0]) > getNumberFromToken(bPart.second[0]))
			answer->setData(true);
		else answer->setData(false);

		return answer;
	}
	else if (aPart.first == Cell::TYPE_STRING &&
		bPart.first == Cell::TYPE_STRING)
	{
		Cell* answer = new Cell(nullptr, nullptr);
		if (aPart.second[0] > bPart.second[0])
			answer->setData(true);
		else answer->setData(false);

		return answer;
	}
}
private: Cell* moreEquProc(Cell* a, Cell* b)
{
	std::pair<int, std::vector<MarkedToken*> > aPart = a->getData();
	std::pair<int, std::vector<MarkedToken*> > bPart = b->getData();
	if (aPart.first == Cell::TYPE_DOUDLE &&
		bPart.first == Cell::TYPE_DOUDLE)
	{
		Cell* answer = new Cell(nullptr, nullptr);
		if (getNumberFromToken(aPart.second[0]) >= getNumberFromToken(bPart.second[0]))
			answer->setData(true);
		else answer->setData(false);

		return answer;
	}
	else if (aPart.first == Cell::TYPE_STRING &&
		bPart.first == Cell::TYPE_STRING)
	{
		Cell* answer = new Cell(nullptr, nullptr);
		if (aPart.second[0] >= bPart.second[0])
			answer->setData(true);
		else answer->setData(false);

		return answer;
	}
}
private: Cell* equProc(Cell* a, Cell* b)
{
	std::pair<int, std::vector<MarkedToken*> > aPart = a->getData();
	std::pair<int, std::vector<MarkedToken*> > bPart = b->getData();
	if (aPart.first == Cell::TYPE_DOUDLE &&
		bPart.first == Cell::TYPE_DOUDLE)
	{
		Cell* answer = new Cell(nullptr, nullptr);
		if (getNumberFromToken(aPart.second[0]) == getNumberFromToken(bPart.second[0]))
			answer->setData(true);
		else answer->setData(false);

		return answer;
	}
	else if (aPart.first == Cell::TYPE_STRING &&
		bPart.first == Cell::TYPE_STRING)
	{
		Cell* answer = new Cell(nullptr, nullptr);
		if (aPart.second[0] == bPart.second[0])
			answer->setData(true);
		else answer->setData(false);

		return answer;
	}
}
private: Cell* notProc(Cell* a)
{
	std::pair<int, std::vector<MarkedToken*> > aPart = a->getData();
	if (aPart.first == Cell::TYPE_T ||
		aPart.first == Cell::TYPE_NIL)
	{
		Cell* answer = new Cell(nullptr, nullptr);
		answer->setData(!getBoolFromToken(aPart.second[0]));

		return answer;
	}
}
#pragma endregion
#pragma region Utility functions
private: std::vector<MarkedToken*> getFirstElem(std::vector<MarkedToken*>& codeRegister)
{
	std::vector<MarkedToken*> buffer;
	if (codeRegister[0]->token == "(")
	{
		return getBracers(codeRegister, 0, findBracers(codeRegister));
	}
	else
	{
		buffer.push_back(codeRegister[0]);

		codeRegister.erase(codeRegister.begin());

		return buffer;
	}
}
private: std::vector<MarkedToken*> getBracers(std::vector<MarkedToken*>& codeRegister, int begin, int end)
{
	std::vector<MarkedToken*> tokens;
	for (int i = begin + 1; i < end; ++i)
	{
		tokens.push_back(codeRegister[i]);
	}
	codeRegister.erase(codeRegister.begin() + begin, codeRegister.begin() + end + 1);

	return tokens;
}
private: double getNumberFromToken(MarkedToken* token)
{
	return stod(token->token);
}
private: bool getBoolFromToken(MarkedToken* token)
{
	if (token->token == "t")
		return true;

	if (token->token == "nil")
		return false;
}
private: int findBracers(std::vector<MarkedToken*> tokens)
{
	int bracersCount = 0;
	int k = 0;
	for (k; k < tokens.size(); k++)
	{
		if (tokens[k]->token == "(")
			bracersCount++;
		else if (tokens[k]->token == ")")
		{
			bracersCount--;
			if (bracersCount == 0)
				break;
		}
	}

	return k;
}
#pragma endregion

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