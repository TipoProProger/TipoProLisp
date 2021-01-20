#include "TokenMarker.h"

MarkedToken::MarkedToken(string token, int tokenGroup)
{
	this->token = token;
	this->tokenGroup = tokenGroup;
}

TokenMarker::TokenMarker()
{
	this->definedSymbolTokens.insert("(");
	this->definedSymbolTokens.insert(")");
	this->definedSymbolTokens.insert("+");
	this->definedSymbolTokens.insert("-");
	this->definedSymbolTokens.insert("*");
	this->definedSymbolTokens.insert("/");
	this->definedSymbolTokens.insert("<");
	this->definedSymbolTokens.insert(">");
	this->definedSymbolTokens.insert("<=");
	this->definedSymbolTokens.insert(">=");
	this->definedSymbolTokens.insert("=");
	this->definedSymbolTokens.insert("!");

	this->definedWordTokens.insert("car");
	this->definedWordTokens.insert("cdr");
	this->definedWordTokens.insert("cons");
	this->definedWordTokens.insert("null");
	this->definedWordTokens.insert("defun");
	this->definedWordTokens.insert("set");
	this->definedWordTokens.insert("if");
	this->definedWordTokens.insert("then");
	this->definedWordTokens.insert("else");
	this->definedWordTokens.insert("t");
	this->definedWordTokens.insert("nil");
}

void TokenMarker::markTokens(vector<string> tokens)
{
	unsigned int i = 0;
	while (i < tokens.size())
	{		
		if (definedWordTokens.find(tokens[i]) != definedWordTokens.end())
		{
			//token is defined word
			this->markedTokens.push_back(new MarkedToken(tokens[i], TOKEN_GROUP_DEFINED_WORD));
			++i;
		}
		else if (definedSymbolTokens.find(tokens[i]) != definedSymbolTokens.end())
		{
			//token is defined symbol
			this->markedTokens.push_back(new MarkedToken(tokens[i], TOKEN_GROUP_DEFINED_SYMBOL));
			++i;
		}
		else if (tokens[i] == "\"")
		{
			//token is a start of string const
			string multyToken = tokens[i++];
			
			while (i < tokens.size() && tokens[i] != "\"")
				multyToken += tokens[i++];
			
			multyToken += tokens[i++];
			this->markedTokens.push_back(new MarkedToken(multyToken, TOKEN_GROUP_STRING_CONST));
		}
		else if (tokens[i] == ";")
		{
			//token is a start of comment
			string multyToken = tokens[i++];
			multyToken += tokens[i++];
			this->markedTokens.push_back(new MarkedToken(multyToken, TOKEN_GROUP_COMMENT));
		}
		else if (tokens[i][0] >= '0' && tokens[i][0] <= '9')
		{
			//token is a number
			this->markedTokens.push_back(new MarkedToken(tokens[i], TOKEN_GROUP_NUMB_CONST));
			++i;
		}
		else if (tokens[i][0] == '_' || tokens[i][0] >= 'a' && tokens[i][0] <= 'z')
		{
			//token is a variable
			this->markedTokens.push_back(new MarkedToken(tokens[i], TOKEN_GROUP_VARIABLE));
			++i;
		}
		else
		{
			//unrecognized token
			this->markedTokens.push_back(new MarkedToken(tokens[i], TOKEN_GROUP_UNRECOGNISED));
			++i;
		}
	}
}

vector <MarkedToken*> TokenMarker::getMarkedTokens()
{
	return this->markedTokens;
}

void TokenMarker::printMarkedTokens()
{
	for (unsigned int i = 0; i < this->markedTokens.size(); ++i)
	{
		if (markedTokens[i]->tokenGroup == TOKEN_GROUP_COMMENT)
			std::cout << "COMMENT ";
		else if (markedTokens[i]->tokenGroup == TOKEN_GROUP_DEFINED_SYMBOL)
			std::cout << "DEFINED_SYMBOL ";
		else if (markedTokens[i]->tokenGroup == TOKEN_GROUP_DEFINED_WORD)
			std::cout << "DEFINED_WORD ";
		else if (markedTokens[i]->tokenGroup == TOKEN_GROUP_STRING_CONST)
			std::cout << "STRING_CONST ";
		else if (markedTokens[i]->tokenGroup == TOKEN_GROUP_NUMB_CONST)
			std::cout << "NUMB_CONST ";
		else if (markedTokens[i]->tokenGroup == TOKEN_GROUP_VARIABLE)
			std::cout << "VARIABLE ";
		else std::cout << "  UNRECOGNISED ";
		
		std::cout << markedTokens[i]->token << "\n";
	}
}

TokenMarker::~TokenMarker()
{
	for (unsigned int i = 0; i < this->markedTokens.size(); ++i)
		delete markedTokens[i];

	markedTokens.clear();
}