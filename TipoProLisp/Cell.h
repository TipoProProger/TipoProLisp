#pragma once

#include <vector>
#include <string>

#include "TokenMarker.h"

class Cell
{
public:
	enum { TYPE_DOUDLE = 0, TYPE_STRING, TYPE_FUNC, TYPE_NIL, TYPE_T };

	Cell(Cell* car, Cell* cdr);
	~Cell();

	Cell* cdr();
	Cell* car();
	bool null();
	void setData(std::vector<MarkedToken*> tokens, int type);
	void setData(double data);
	void setData();
	void setData(MarkedToken* token);
	void setData(bool flag);
	void setData(Cell* cell);
	void setData(std::vector<MarkedToken*> args, std::vector<MarkedToken*> tokens);

	std::pair<int, vector<MarkedToken*> > getData();

	void clearData();
	vector<MarkedToken*> getArgsNames();

private:
	Cell* left;
	Cell* right;
	int TYPE;
	vector<MarkedToken*> tokens;
	vector<MarkedToken*> args;
};