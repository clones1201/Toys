/*
	it's declara
	
*/
#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "Global.h"

#define IDENTIFIER		1
#define NUMBER			2
#define OPERATOR		3
#define RESERVEDWORD	4
#define ENDOFFILE       5

typedef int TokenType;

#define OPT_PLUS			1
#define OPT_TIMES			2
#define OPT_MINUS			3
#define OPT_DIVIDE          4
#define OPT_LESS_THAN           5
#define OPT_GREATER_THAN		6
#define OPT_ENLESS_THAN         7
#define OPT_ENGREATER_THAN		8
#define OPT_EQUAL			    9
#define OPT_ASSIGN			10
#define OPT_UNEQUAL			11
#define OPT_SEMICOLON       12
#define OPT_COMMA           13
#define OPT_LPAREN          14
#define OPT_RPAREN          15
#define OPT_LSQUARE         16
#define OPT_RSQUARE         17
#define OPT_LBRACE          18
#define OPT_RBRACE          19
#define OPT_ERROR			20

typedef int OperatorType;

typedef int ReservedWordIndex;

class Token
{
public:
	Token(){};
	virtual TokenType GetType() = 0;
};

class Identifier : public Token
{
private:
	string content;
public:
	Identifier(string _content)
	{
		content = _content;
	}
	TokenType GetType();
	string GetContent() const;
	void SetContent(string _con);
	friend ostream & operator<<(ostream& out,const Identifier &ID);
};

class ReservedWord : public Token
{
private:
	ReservedWordIndex Index;
public:
	ReservedWord(ReservedWordIndex _index)
	{
		Index = _index;
	}
	TokenType GetType();
	string GetReservedWord();
	friend ostream & operator<<(ostream& out,const ReservedWord &RW);
};

class Number : public Token
{
private:
	string content;
public:
	Number(string _content)
	{
		content = _content;
	}
	TokenType GetType();
	string GetContent() const;
	void SetContent(string _con);
	friend ostream & operator<<(ostream& out,const Number &NUM);
};

class Operator : public Token
{
private:
	OperatorType Type;
public:
	Operator(OperatorType _Type)
	{
		Type = _Type;
	}
	TokenType GetType();
	OperatorType GetOptType();
	friend ostream & operator<<(ostream& out,const Operator &OPT);
};

class EndOfFile : public Token
{
public:
    EndOfFile(){};
    ~EndOfFile(){};

    TokenType GetType()
    {
        return ENDOFFILE;
    }
    friend ostream & operator<<(ostream& out,const EndOfFile &eof);
};

#endif