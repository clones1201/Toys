
/*implement of Token */

#include "Token.h"

/*  implement of Identifier  */

TokenType Identifier::GetType()
{
	return IDENTIFIER;
}

string Identifier::GetContent() const
{
    return content;
}

void Identifier::SetContent(string _con)
{
    content = _con;
}

ostream & operator<<(ostream& out,const Identifier &ID)
{
    out<<"ID ,name = "<<ID.GetContent();
    return out;
}


/*   implement of Number   */

TokenType Number::GetType()
{
	return NUMBER;
}

string Number::GetContent() const
{
	return content;
}

void Number::SetContent(string _con)
{
	content = _con;
}

ostream & operator<<(ostream &out,const Number &NUM)
{
	out<<"NUM, val = "<<NUM.GetContent();
	return out;
}

/*  Implement of Operator */

TokenType Operator::GetType()
{
	return OPERATOR;
}

OperatorType Operator::GetOptType()
{
	return Type;
}

ostream & operator<<(ostream& out,const Operator &OPT)
{
	switch(OPT.Type)
	{
	case  OPT_PLUS:
		out<<"+";
		break;
	case  OPT_MINUS:
		out<<"-";
		break;
	case  OPT_TIMES:
		out<<"*";
		break;
	case  OPT_DIVIDE:
		out<<"/";
		break;
	case  OPT_LESS_THAN:
		out<<"<";
		break;
	case  OPT_ENLESS_THAN:
		out<<"<=";
		break;
	case  OPT_GREATER_THAN:
		out<<">";
		break;
	case  OPT_ENGREATER_THAN:
		out<<">=";
		break;
	case  OPT_EQUAL:
		out<<"==";
		break;
	case  OPT_UNEQUAL:
		out<<"!=";
		break;
	case  OPT_ASSIGN:
		out<<"=";
		break;
	case  OPT_SEMICOLON:
		out<<";";
		break;
	case  OPT_COMMA:
		out<<",";
		break;
	case  OPT_LPAREN:
		out<<"(";
		break;
	case  OPT_RPAREN:
		out<<")";
		break;
	case  OPT_LSQUARE:
		out<<"[";
		break;
	case  OPT_RSQUARE:
		out<<"]";
		break;
	case  OPT_LBRACE:
		out<<"{";
		break;
	case  OPT_RBRACE:
		out<<"}";
		break;
	case OPT_ERROR:
		out<<"ERROR...";
		break;
	default:
		out<<"ERROR...";
		break;
	}
	return out;
}

/*  implement of Reserved Word */

TokenType ReservedWord::GetType()
{
	return RESERVEDWORD;
}

string ReservedWord::GetReservedWord()
{
	return ReservedWords[Index];
}

ostream & operator<<(ostream& out,const ReservedWord &RW)
{
	out<<"Reserved word:"<<ReservedWords[RW.Index];
	return out;
}

ostream & operator<<(ostream& out,const EndOfFile &eof)
{
    return out;
}

