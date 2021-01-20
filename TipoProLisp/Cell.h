#pragma once

#include <vector>
#include <string>

#include "TokenMarker.h"

class Cell
{
public:
	enum { TYPE_DOUDLE = 0, TYPE_FUNC, TYPE_NIL, TYPE_T };

	Cell(Cell* cdr, Cell* car)
	{
		this->left = car;
		this->right = cdr;
		this->TYPE = TYPE_NIL;
	}
	~Cell()
	{
		if (this->left != nullptr)
		{
			delete this->left;
			this->left = nullptr;
		}

		if (this->right != nullptr)
		{
			delete this->right;
			this->right = nullptr;
		}
	}

	Cell* cdr()
	{
		return this->right;
	}
	Cell* car()
	{
		return this->left;
	}
	Cell* cons(Cell *left, Cell *right)
	{
		Cell* newCell = new Cell(left, right);
		return newCell;
	}
	bool null()
	{
		return (this->left == nullptr && this->right == nullptr);
	}
	void setData(double data)
	{
		this->TYPE = TYPE_DOUDLE;
		this->tokens.push_back(new MarkedToken(std::to_string(data), TOKEN_GROUP_NUMB_CONST));
	}
	void setData(std::vector<MarkedToken*> tokens)
	{
		this->TYPE = TYPE_FUNC;
		this->tokens = tokens;
	}
	void setData()
	{
		this->TYPE = TYPE_NIL;
		this->tokens.push_back(new MarkedToken("nil", TOKEN_GROUP_DEFINED_WORD));
	}
	void setData(bool flag)
	{
		this->TYPE = TYPE_T;
		this->tokens.push_back(new MarkedToken("nil", TOKEN_GROUP_DEFINED_WORD));
	}
	
	std::pair<int, vector<MarkedToken*> > getData()
	{
		std::cout << "Cell getData()\n";
		return std::make_pair(TYPE, tokens);
	}

private:
	Cell* left;
	Cell* right;
	int TYPE;
	vector<MarkedToken*> tokens;
};