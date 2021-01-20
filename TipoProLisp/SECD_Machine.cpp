#include "SECD_Machine.h"

SECD_Machine::SECD_Machine(SECD_Machine* previousMachine, vector<MarkedToken*> markedTokens)
{
	this->codeRegister = markedTokens;	
}

Cell* SECD_Machine::eval(vector<MarkedToken*> codeRegister, map<string, Cell*>& environmentRegister)
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
		t->setData(stod(token->token));
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
			vector<MarkedToken*> buffer = getBracers(codeRegister, 0, findBracers(codeRegister));

			return eval(buffer, environmentRegister);
		}
		else if (token->token == "+")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* buffer = new Cell(nullptr, nullptr);
			buffer->setData((double)0);
			while (!codeRegister.empty())
			{
				addProc(buffer, eval(getFirstElem(codeRegister), environmentRegister));
			}

			return buffer;
		}
		else if (token->token == "-")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* buffer = new Cell(nullptr, nullptr);
			buffer->setData(eval(getFirstElem(codeRegister), environmentRegister));
			while (!codeRegister.empty())
			{
				subProc(buffer, eval(getFirstElem(codeRegister), environmentRegister));
			}

			return buffer;
		}
		else if (token->token == "/")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* buffer = new Cell(nullptr, nullptr);
			buffer->setData(eval(getFirstElem(codeRegister), environmentRegister));
			while (!codeRegister.empty())
			{
				divProc(buffer, eval(getFirstElem(codeRegister), environmentRegister));
			}

			return buffer;
		}
		else if (token->token == "*")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* buffer = new Cell(nullptr, nullptr);
			buffer->setData(eval(getFirstElem(codeRegister), environmentRegister));
			while (!codeRegister.empty())
			{
				mulProc(buffer, eval(getFirstElem(codeRegister), environmentRegister));
			}

			return buffer;
		}
		else if (token->token == "<")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* buffer = new Cell(nullptr, nullptr);
			buffer->setData(eval(getFirstElem(codeRegister), environmentRegister));
			while (!codeRegister.empty())
			{
				buffer = lessProc(buffer, eval(getFirstElem(codeRegister), environmentRegister));
			}

			return buffer;
		}
		else if (token->token == "<=")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* buffer = new Cell(nullptr, nullptr);
			buffer->setData(eval(getFirstElem(codeRegister), environmentRegister));
			while (!codeRegister.empty())
			{
				buffer = lessEquProc(buffer, eval(getFirstElem(codeRegister), environmentRegister));
			}

			return buffer;
		}
		else if (token->token == "=")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* buffer = new Cell(nullptr, nullptr);
			buffer->setData(eval(getFirstElem(codeRegister), environmentRegister));
			while (!codeRegister.empty())
			{
				buffer = equProc(buffer, eval(getFirstElem(codeRegister), environmentRegister));
			}

			return buffer;
		}
		else if (token->token == ">")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* buffer = new Cell(nullptr, nullptr);
			buffer->setData(eval(getFirstElem(codeRegister), environmentRegister));
			while (!codeRegister.empty())
			{
				buffer = moreProc(buffer, eval(getFirstElem(codeRegister), environmentRegister));
			}

			return buffer;
		}
		else if (token->token == ">=")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* buffer = new Cell(nullptr, nullptr);
			buffer->setData(eval(getFirstElem(codeRegister), environmentRegister));
			while (!codeRegister.empty())
			{
				buffer = moreEquProc(buffer, eval(getFirstElem(codeRegister), environmentRegister));
			}

			return buffer;
		}
		else if (token->token == "!")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* buffer = new Cell(nullptr, nullptr);
			buffer->setData(eval(getFirstElem(codeRegister), environmentRegister));
			buffer = notProc(buffer);

			return buffer;
		}
		break;
	}
	case TOKEN_GROUP_DEFINED_WORD:
	{
		if (token->token == "car")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* cell = eval(getFirstElem(codeRegister), environmentRegister);
			return cell->car();
		}
		else if (token->token == "cdr")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* cell = eval(getFirstElem(codeRegister), environmentRegister);
			return cell->cdr();
		}
		else if (token->token == "cons")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* carPart = eval(getFirstElem(codeRegister), environmentRegister);
			Cell* cdrPart = eval(getFirstElem(codeRegister), environmentRegister);

			return new Cell(carPart, cdrPart);
		}
		else if (token->token == "null")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* cell = eval(getFirstElem(codeRegister), environmentRegister);

			Cell* result = new Cell(nullptr, nullptr);
			if (cell == nullptr)
			{
				result->setData(true);
			}
			else
			{
				result->setData(cell->null());
			}
			return result;
		}
		else if (token->token == "defun")
		{
			codeRegister.erase(codeRegister.begin());

			//function name
			string functionName = codeRegister[0]->token;
			codeRegister.erase(codeRegister.begin());

			//args or empty bracers
			vector<MarkedToken*> argsTokens = getBracers(codeRegister, 0, findBracers(codeRegister));

			vector<MarkedToken*> functionBody = getBracers(codeRegister, 0, findBracers(codeRegister));
			Cell* functionBodyCell = new Cell(nullptr, nullptr);
			functionBodyCell->setData(argsTokens, functionBody);

			environmentRegister.insert(make_pair(functionName, functionBodyCell));

			return new Cell(nullptr, nullptr);
		}
		else if (token->token == "set")
		{
			codeRegister.erase(codeRegister.begin());

			string name = getFirstElem(codeRegister)[0]->token;
			Cell* cell = eval(getFirstElem(codeRegister), environmentRegister);

			environmentRegister.insert(make_pair(name, cell));

			//no return value
			return new Cell(nullptr, nullptr);
		}
		else if (token->token == "if")
		{
			codeRegister.erase(codeRegister.begin());

			Cell* ifClause = eval(getFirstElem(codeRegister), environmentRegister);
			bool value = getBoolFromToken(ifClause->getData().second[0]);

			vector<MarkedToken*> thenClauseTokens = getBracers(codeRegister, 0, findBracers(codeRegister));
			thenClauseTokens.erase(thenClauseTokens.begin());//then word

			vector<MarkedToken*> elseClauseTokens;

			Cell* result = nullptr;
			if (!codeRegister.empty())
			{
				//else exists
				elseClauseTokens = getBracers(codeRegister, 0, findBracers(codeRegister));
				elseClauseTokens.erase(elseClauseTokens.begin());//else word

				if (value)
					result = eval(thenClauseTokens, environmentRegister);
				else result = eval(elseClauseTokens, environmentRegister);
			}
			else
			{
				if (value)
					result = eval(thenClauseTokens, environmentRegister);
				else result = new Cell(nullptr, nullptr);
			}

			return result;
		}
		else if (token->token == "t")
		{
			Cell* cell = new Cell(nullptr, nullptr);
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
		Cell* cell = environmentRegister.find(token->token)->second;
		codeRegister.erase(codeRegister.begin());

		if (cell->getData().first == Cell::TYPE_FUNC)
		{

			vector<Cell*> args;
			while (!codeRegister.empty())
			{
				args.push_back(eval(getFirstElem(codeRegister), environmentRegister));
			}

			Cell* result = functionCall(cell, args, environmentRegister);

			return result;
		}
		else
		{
			Cell* answer = new Cell(nullptr, nullptr);
			answer->setData(cell);
			return answer;
		}

		break;
	}
	}
}

Cell* SECD_Machine::run()
{
	Cell* answer = nullptr;
	while (!this->codeRegister.empty())
	{
		if (this->codeRegister[0]->token == "(")
			answer = eval(getBracers(this->codeRegister, 0, findBracers(this->codeRegister)), environmentRegister);
		else answer = eval(this->codeRegister, environmentRegister);

		cout << treePartToString(answer);
	}

	return answer;
}

Cell* SECD_Machine::functionCall(Cell* function, vector<Cell*> args, map<string, Cell*> environment)
{
	for (int i(0); i < function->getArgsNames().size(); ++i)
	{
		if (environment.find(function->getArgsNames()[i]->token) != environment.end())
			environment[function->getArgsNames()[i]->token] = args[i];
		else environment.insert(make_pair(function->getArgsNames()[i]->token, args[i]));
	}

	return eval(function->getData().second, environment);
}

#pragma region Arifmetic functions
void SECD_Machine::addProc(Cell* a, Cell* b)
{
	pair<int, vector<MarkedToken*> > aPart = a->getData();
	pair<int, vector<MarkedToken*> > bPart = b->getData();
	if (aPart.first == Cell::TYPE_DOUDLE &&
		bPart.first == Cell::TYPE_DOUDLE)
	{
		double sum = getNumberFromToken(aPart.second[0]);
		sum += getNumberFromToken(bPart.second[0]);
		a->setData(sum);
	}
}
void SECD_Machine::subProc(Cell* a, Cell* b)
{
	pair<int, vector<MarkedToken*> > aPart = a->getData();
	pair<int, vector<MarkedToken*> > bPart = b->getData();
	if (aPart.first == Cell::TYPE_DOUDLE &&
		bPart.first == Cell::TYPE_DOUDLE)
	{
		double sub = getNumberFromToken(aPart.second[0]);
		sub -= getNumberFromToken(bPart.second[0]);
		a->setData(sub);
	}
}
void SECD_Machine::divProc(Cell* a, Cell* b)
{
	pair<int, vector<MarkedToken*> > aPart = a->getData();
	pair<int, vector<MarkedToken*> > bPart = b->getData();
	if (aPart.first == Cell::TYPE_DOUDLE &&
		bPart.first == Cell::TYPE_DOUDLE)
	{
		double div = getNumberFromToken(aPart.second[0]);
		div /= getNumberFromToken(bPart.second[0]);
		a->setData(div);
	}
}
void SECD_Machine::mulProc(Cell* a, Cell* b)
{
	pair<int, vector<MarkedToken*> > aPart = a->getData();
	pair<int, vector<MarkedToken*> > bPart = b->getData();
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
Cell* SECD_Machine::lessProc(Cell* a, Cell* b)
{
	pair<int, vector<MarkedToken*> > aPart = a->getData();
	pair<int, vector<MarkedToken*> > bPart = b->getData();
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
Cell* SECD_Machine::lessEquProc(Cell* a, Cell* b)
{
	pair<int, vector<MarkedToken*> > aPart = a->getData();
	pair<int, vector<MarkedToken*> > bPart = b->getData();
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
Cell* SECD_Machine::moreProc(Cell* a, Cell* b)
{
	pair<int, vector<MarkedToken*> > aPart = a->getData();
	pair<int, vector<MarkedToken*> > bPart = b->getData();
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
Cell* SECD_Machine::moreEquProc(Cell* a, Cell* b)
{
	pair<int, vector<MarkedToken*> > aPart = a->getData();
	pair<int, vector<MarkedToken*> > bPart = b->getData();
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
Cell* SECD_Machine::equProc(Cell* a, Cell* b)
{
	pair<int, vector<MarkedToken*> > aPart = a->getData();
	pair<int, vector<MarkedToken*> > bPart = b->getData();
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
Cell* SECD_Machine::notProc(Cell* a)
{
	pair<int, vector<MarkedToken*> > aPart = a->getData();
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
vector<MarkedToken*> SECD_Machine::getFirstElem(vector<MarkedToken*>& codeRegister)
{
	vector<MarkedToken*> buffer;
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
vector<MarkedToken*> SECD_Machine::getBracers(vector<MarkedToken*>& codeRegister, int begin, int end)
{
	vector<MarkedToken*> tokens;
	for (int i = begin + 1; i < end; ++i)
	{
		tokens.push_back(codeRegister[i]);
	}
	codeRegister.erase(codeRegister.begin() + begin, codeRegister.begin() + end + 1);

	return tokens;
}
double SECD_Machine::getNumberFromToken(MarkedToken* token)
{
	return stod(token->token);
}
bool SECD_Machine::getBoolFromToken(MarkedToken* token)
{
	if (token->token == "t")
		return true;

	if (token->token == "nil")
		return false;
}
int SECD_Machine::findBracers(vector<MarkedToken*> tokens)
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

#pragma region Cout functions
string SECD_Machine::tokensToString(Cell* cell)
{
	vector<MarkedToken*> tokens = cell->getData().second;

	string answer = "";
	for (int i(0); i < tokens.size(); i++)
		answer += tokens[i]->token;

	return answer;
}
string SECD_Machine::treePartToString(Cell* root)
{
	string answer = treePart(root);
	if (answer == "")
		answer = "nil";
	return answer + "\n";
}
string SECD_Machine::treePart(Cell* root)
{
	string cur = tokensToString(root);

	if (root->car() != nullptr)
		cur += treePart(root->car()) + " ";
	if (root->cdr() != nullptr)
		cur += treePart(root->cdr()) + " ";

	return cur;
}
#pragma endregion