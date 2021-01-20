#pragma once

#include <vector>
#include <string>

#include "TokenMarker.h"

class Cell
{
public:
	enum { TYPE_DOUDLE = 0, TYPE_STRING, TYPE_FUNC, TYPE_NIL, TYPE_T };

	Cell(Cell* car, Cell* cdr)
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
	bool null()
	{
		return (this->left == nullptr && this->right == nullptr
			&& this->tokens.empty());
	}
	void setData(std::vector<MarkedToken*> tokens, int type = TYPE_FUNC)
	{
		clearData();
		this->TYPE = type;
		this->tokens = tokens;
	}
	void setData(double data)
	{
		std::vector<MarkedToken*> tokens;
		tokens.push_back(new MarkedToken(std::to_string(data), TOKEN_GROUP_NUMB_CONST));
		setData(tokens, Cell::TYPE_DOUDLE);
	}
	void setData()
	{
		std::vector<MarkedToken*> tokens;
		tokens.push_back(new MarkedToken("nil", TOKEN_GROUP_DEFINED_WORD));
		setData(tokens, TYPE_NIL);
	}
	void setData(MarkedToken* token)
	{
		tokens.push_back(token);
		setData(tokens, TYPE_STRING);
	}
	void setData(bool flag)
	{
		std::vector<MarkedToken*> tokens;
		tokens.push_back(new MarkedToken((flag ? "t" : "nil"), TOKEN_GROUP_DEFINED_WORD));
		setData(tokens, TYPE_T);
	}
	void setData(Cell* cell)
	{
		this->TYPE = cell->TYPE;
		for(int i(0); i < cell->tokens.size(); ++i)
			this->tokens.push_back(new MarkedToken(cell->tokens[i]));
	}
	void setData(std::vector<MarkedToken*> args, std::vector<MarkedToken*> tokens)
	{
		setData(tokens);
		this->args = args;	
	}

	std::pair<int, vector<MarkedToken*> > getData()
	{
		return std::make_pair(TYPE, tokens);
	}

	void clearData()
	{
		this->args.clear();

		if (!this->tokens.empty())
		{
			for (int i(0); i < this->tokens.size(); ++i)
			{
				delete this->tokens[i];
			}
			this->tokens.clear();
		}
	}
	vector<MarkedToken*> getArgsNames()
	{
		return this->args;
	}
private:
	Cell* left;
	Cell* right;
	int TYPE;
	vector<MarkedToken*> tokens;
	vector<MarkedToken*> args;
};