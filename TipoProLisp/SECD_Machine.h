#pragma once

#include <vector>
#include <string>
#include <stack>
#include <map>

#include "Cell.h"
#include "TokenMarker.h"

using std::vector;
using std::cout;
using std::pair;
using std::map;

class SECD_Machine
{
public: SECD_Machine(SECD_Machine* previousMachine, std::vector<MarkedToken*> markedTokens);

private: Cell* eval(std::vector<MarkedToken*> codeRegister, std::map<string, Cell*>& environmentRegister);
private: Cell* functionCall(Cell* function, std::vector<Cell*> args, std::map<string, Cell*> environment);
public: Cell* run();

#pragma region Arifmetic functions
private: void addProc(Cell* a, Cell* b);
private: void subProc(Cell* a, Cell* b);
private: void divProc(Cell* a, Cell* b);
private: void mulProc(Cell* a, Cell* b);
#pragma endregion

#pragma region Logic functions
private: Cell* lessProc(Cell* a, Cell* b);
private: Cell* lessEquProc(Cell* a, Cell* b);
private: Cell* moreProc(Cell* a, Cell* b);
private: Cell* moreEquProc(Cell* a, Cell* b);
private: Cell* equProc(Cell* a, Cell* b);
private: Cell* notProc(Cell* a);
#pragma endregion

#pragma region Utility functions
private: std::vector<MarkedToken*> getFirstElem(std::vector<MarkedToken*>& codeRegister);
private: std::vector<MarkedToken*> getBracers(std::vector<MarkedToken*>& codeRegister, int begin, int end);
private: double getNumberFromToken(MarkedToken* token);
private: bool getBoolFromToken(MarkedToken* token);
private: int findBracers(std::vector<MarkedToken*> tokens);
#pragma endregion

#pragma region Cout functions
private: static string tokensToString(Cell* cell);
public: static string treePartToString(Cell* root);
private: static string treePart(Cell* root);
#pragma endregion

public:
	//регистр окружения для известных переменных
	std::map<std::string, Cell*> environmentRegister;
	//регистр кода
	std::vector<MarkedToken*> codeRegister;	
};