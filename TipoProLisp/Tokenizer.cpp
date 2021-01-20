#include "Tokenizer.h"

Tokenizer::Tokenizer(std::ifstream* input)
{
	this->input = input;
}
Tokenizer::Tokenizer()
{
	input = nullptr;
}
void Tokenizer::createTokens()
{
	vector<string> tokensBuffer;

	while (!this->input->eof())
	{
		string programLine;
		getline(*input, programLine);
		tokensBuffer = createTokensFromString(programLine);
		mergeTokens(this->tokens, tokensBuffer);
	}
}
void Tokenizer::printTokens()
{
	for (unsigned int i = 0; i < this->tokens.size(); ++i)
		std::cout << this->tokens[i] << '\n';
}
vector<string> Tokenizer::getTokens()
{
	return this->tokens;
}
Tokenizer::~Tokenizer()
{
	if (input != nullptr)
	{
		input->close();
		delete input;
	}
}

string Tokenizer::extractToken(string& program, unsigned int& currentPosition, bool addSymbol)
{
	string buffer = program.substr(0, currentPosition + (addSymbol ? 1 : 0));
	program.erase(0, currentPosition + (addSymbol ? 1 : 0));
	currentPosition = 0;

	return buffer;
}
bool Tokenizer::emptyString(string str)
{
	if (str == "")
		return true;

	while (str[0] == ' ' || str[0] == '\t')
	{
		str.erase(str.begin());
		if (str.empty())
			return true;
	}

	return false;
}
void Tokenizer::addToken(vector<string>& tokens, string& program, unsigned int& currentPosition, bool addSymbol)
{
	string buffer = extractToken(program, currentPosition, addSymbol);
	if (emptyString(buffer))
		return;

	std::transform(buffer.begin(), buffer.end(), buffer.begin(), std::tolower);
	tokens.push_back(buffer);
}
vector<string> Tokenizer::createTokensFromString(string programLine)
{
	vector<string> tokens;
	unsigned int currentPosition = 0;

	/*
		I don't use state for comment, because
		; bla-bla-bla
		Comment will be after symbol ; till the end of line

		I use special state for constant, because it can ends
		before the end of line
	*/
	const int STATE_DEFAULT = 0;
	const int STATE_STRING_CONSTANT = 1;
	int currentState = STATE_DEFAULT;

	for (currentPosition; currentPosition < programLine.size();)
	{
		if (currentState == STATE_DEFAULT)
		{
			if (programLine[currentPosition] == ' ')//end of token
			{
				addToken(tokens, programLine, currentPosition);
				addToken(tokens, programLine, currentPosition, true);
			}
			else if (programLine[currentPosition] == ')'
				|| programLine[currentPosition] == '(')//end of token and token
			{
				addToken(tokens, programLine, currentPosition);
				addToken(tokens, programLine, currentPosition, true);
			}
			else if (programLine[currentPosition] == ';')//token and comment
			{
				addToken(tokens, programLine, currentPosition);
				addToken(tokens, programLine, currentPosition, true);
				break;
			}
			else if (programLine[currentPosition] == '"')
			{
				addToken(tokens, programLine, currentPosition);
				addToken(tokens, programLine, currentPosition, true);
				currentState = STATE_STRING_CONSTANT;
			}
			else ++currentPosition;
		}
		else if (currentState == STATE_STRING_CONSTANT)
		{
			if (programLine[currentPosition] == '"')
			{
				addToken(tokens, programLine, currentPosition);
				addToken(tokens, programLine, currentPosition, true);
				currentState = STATE_DEFAULT;
			}
			else ++currentPosition;
		}
	}

	if (programLine != "")
		tokens.push_back(programLine);

	return tokens;
}
void Tokenizer::mergeTokens(vector<string>& tokens, vector<string> tokensToAdd)
{
	for (unsigned int i = 0; i < tokensToAdd.size(); ++i)
		tokens.push_back(tokensToAdd[i]);
}