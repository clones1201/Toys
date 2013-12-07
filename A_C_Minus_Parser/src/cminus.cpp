#include "global.h"
#include "cminus.h"



Scaner::Scaner(ifstream &pFile):inputFile(pFile)
{
	status = ST_START;
	linePos=0;
    bufsize=0;
}

Scaner::~Scaner()
{

}

ReservedWordIndex Scaner::isReservedWords(const char* str)
{
    int i;
    for(i = 0 ; i < 6 ; i++)
    {
		if(strcmp(str,ReservedWords[i].c_str())==0)
            return i;
    }
    return -1;
}


Status Scaner::pushInDFA(CharType input)
{
    status = StateSwitchTable[status][input];
    if (! ( status >= ST_RETURN_ID && status <= ST_RETURN_WHITESPACE  ))
    {
        return NOT_RECEIVED;
    }
    else
        return status;
}

Token* Scaner::InsertIdentifier(string TokenContent)
{
    int i;
    for( i = 0 ; i < IDTable.size() ; i++ )
    {
        if ( TokenContent.compare(IDTable[i]->GetContent()) == 0 )
        {
            return IDTable[i];
        }
    }
    Identifier *newIdentifier = new Identifier(TokenContent);
    IDTable.push_back(newIdentifier);
    return IDTable[ IDTable.size()-1 ];
}

bool Scaner::Scan(string outputFileName)
{
    int i;
    int lineIndex = 1;
	char line[256];
	char ch;
    Status ReceivedType;
	string token;
    Token* newIdentifier;
    Token* newNumber;
    Token* newReservedWord;
    Token* newOperator;
    vector<Token*> lineToken;

    ofstream output(outputFileName.c_str());

	while(!inputFile.eof())
    {
		cout<<lineIndex<<":   ";
        output<<lineIndex<<":   ";

		inputFile.getline(line,65536);
		linePos=0;
        lineToken.clear();
        token.clear();
		ch = getNextChar(line);
        while(ch!='\0')
		{
            token.push_back(ch);
			ReceivedType = pushInDFA(classifyChar(ch));
            if (ReceivedType == NOT_RECEIVED )
            {
                1==1;/** do nothing ?**/
            }
            else if (ReceivedType == ST_RETURN_WHITESPACE)
            {
                token.clear();
                status = ST_START;
            }
            else if (ReceivedType == ST_RETURN_ID)
            {
                ungetNextChar();
                token = token.substr(0,token.size()-1);

                if(isReservedWords(token.c_str()) == -1)
                {
                    newIdentifier = InsertIdentifier(token);
                    lineToken.push_back(newIdentifier);
                }
                else
                {
                    newReservedWord = new ReservedWord(isReservedWords(token.c_str()));
                    lineToken.push_back(newReservedWord);
                }
                token.clear();
                status = ST_START;
            }
            else if(ReceivedType == ST_RETURN_NUM)
            {
                ungetNextChar();
                newNumber = new Number(token.substr(0,token.size()-1));
                lineToken.push_back(newNumber);
                status = ST_START;
            }
            else if(ReceivedType == ST_RETURN_COMMENT)
            {
               // token.clear();
                status = ST_START;
            }
            else if(ReceivedType == ST_RETURN_DEVIDE ||
                    ReceivedType == ST_RETURN_BIGGER ||
                    ReceivedType == ST_RETURN_SMALLER ||
                    ReceivedType == ST_RETURN_ERROR ||
                    ReceivedType == ST_RETURN_ASSIGN )
            {
                ungetNextChar();
                token.clear();
                newOperator = new Operator( ReceivedType - 12);
                lineToken.push_back(newOperator);
                status = ST_START;
            }
            else
            {
                token.clear();
                newOperator = new Operator( ReceivedType - 12);
                lineToken.push_back(newOperator);
                status = ST_START;
            }
			ch = getNextChar(line);
		}
        cout<<line<<endl;
        output<<line<<endl;

		TokenTable.push_back(lineToken);

#ifdef _DEBUG
        Token* tk;
        for( i = 0 ; i < lineToken.size() ; i++ )
        {
            cout<<"\t"<<lineIndex<<":   ";
            output<<"\t"<<lineIndex<<":   ";

            tk = lineToken[i];
            switch(tk->GetType())
            {
            case IDENTIFIER:
                cout<<*(Identifier*)tk<<endl;
                output<<*(Identifier*)tk<<endl;
                break;
            case NUMBER:
                cout<<*(Number*)tk<<endl;
                output<<*(Number*)tk<<endl;
                break;
            case OPERATOR:
                cout<<*(Operator*)tk<<endl;
                output<<*(Operator*)tk<<endl;
                break;
            case RESERVEDWORD:
                cout<<*(ReservedWord*)tk<<endl;
                output<<*(ReservedWord*)tk<<endl;
                break;
            default:
                cout<<"Token Type error.."<<endl;
                output<<"Token Type error.."<<endl;
                break;
            }
        }
#endif
        lineIndex++;
    }

    vector<Token*> endoffile;
    endoffile.push_back(new EndOfFile);
    TokenTable.push_back(endoffile);

    return true;
}

char Scaner::getNextChar(char* line)
{
	if(linePos < 256)
		return line[linePos++];
	return -1;
}

void Scaner::ungetNextChar()
{
	if(linePos >= 0)
		linePos--;
}

int Scaner::classifyChar(char ch)   //对输入字符进行分类
{
    if( ch == ' '  || ch == '\t' )
        return CH_WHITESPACE;
    else if( ( ch >= 'a' && ch <= 'z' ) || ( ch >= 'A' && ch <= 'Z' ) )
        return CH_LETTER;
    else if( ch >= '0' && ch <= '9')
        return CH_DIGIT;
    else
        return classifyOpt(ch);
}

int Scaner::classifyOpt(char ch)
{
    int i;
    for( i = 0 ; i < 16 ; i++ )
    {
        if( OptTable[i] == ch )
            return i + 3;
    }
}

vector< vector<Token*> > Scaner::GetTokenTable()
{
    return Scaner::TokenTable;
}

vector<Identifier*> Scaner::GetIDTable()
{
    return Scaner::IDTable;
}

/***********************/
/*  Implement of Parse */
/***********************/

Parse::Parse()
{
    ParseTree = NULL;
    iLineNo = 0;
    iTokenPos = -1;
    TokenTable.clear();
}

Parse::Parse(Scaner &scaner)
{
    iLineNo = 0;
    iTokenPos = -1;
    TokenTable = scaner.GetTokenTable();
    IDTable = scaner.GetIDTable();


}

Parse::~Parse()
{

}

Token* Parse::ConsumeNextToken()
{
    while( (iTokenPos+1) >= TokenTable[iLineNo].size()
        &&  iLineNo < TokenTable.size() )
    {
        iLineNo++;
        iTokenPos = -1;
        if( (iTokenPos+1) < TokenTable[iLineNo].size() )
        {
            iTokenPos++;
            return TokenTable[iLineNo][iTokenPos];
        }
    }
    iTokenPos++;
    return TokenTable[iLineNo][iTokenPos];
}


Token* Parse::GetToken(int n)
{
    int i;
    int local_LineNo,local_TokenPos;

    local_LineNo = iLineNo;
    local_TokenPos = iTokenPos;

    if( n <= 0 )
        return NULL;

    for( i = 0 ; i < n ; i++ )
    {
        while( (local_TokenPos+1) >= TokenTable[local_LineNo].size()
        &&  local_LineNo < TokenTable.size() )
        {
            local_LineNo++;
            local_TokenPos = -1;

        }
        if( (local_TokenPos+1) < TokenTable[local_LineNo].size() )
        {
                local_TokenPos++;
        }
    }
    return TokenTable[local_LineNo][local_TokenPos];
}


Node* Parse::program()
{
    ParseTree =(Node*) new NonTerminalNode(NTT_PROGRAM);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() != ENDOFFILE )
    {
        ParseTree->GetChilds()->push_back(declaration_list());
    }
    else
    {
        ParseTree->GetChilds()->push_back(empty());
    }
    return ParseTree;
}

Node* Parse::declaration_list()
{
    Node* result = new NonTerminalNode(NTT_DECLARATION_LIST);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == RESERVEDWORD )
    {
        ReservedWord* RWord = (ReservedWord*)nextToken;
        if( RWord->GetReservedWord() == ReservedWords[RW_INT] ||
            RWord->GetReservedWord() == ReservedWords[RW_VOID] )
        {
            result->GetChilds()->push_back(declaration());
            result->GetChilds()->push_back(declaration_list());
        }
        else
        {
            result->GetChilds()->push_back(new Error(ERR_WRONG_TYPE));
            ConsumeNextToken();
            result = declaration_list();//跳过一个单词再次尝试
        }
    }
    else if( nextToken->GetType() == ENDOFFILE )
    {
        result->GetChilds()->push_back(empty());
    }
    return result;
}

Node* Parse::declaration()
{
    Node* result = new NonTerminalNode(NTT_DECLARATION);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == RESERVEDWORD )
    {
        ReservedWord* RWord = (ReservedWord*)nextToken;
        if( RWord->GetReservedWord() == ReservedWords[RW_INT] ||
            RWord->GetReservedWord() == ReservedWords[RW_VOID] )
        {
            result->GetChilds()->push_back(type_specifier());
            result->GetChilds()->push_back(identifier());
            result->GetChilds()->push_back(declaration_body());
        }
        else
        {
            result->GetChilds()->push_back(new Error(ERR_WRONG_TYPE));
            ConsumeNextToken();
            delete result;
            result = NULL;
            result = declaration();   //跳过一个单词再次尝试
        }
    }
    else if( nextToken->GetType() == ENDOFFILE )
    {
        result->GetChilds()->push_back(empty());
    }

    return result;
}



Node* Parse::declaration_body()
{
    Node* result = new NonTerminalNode(NTT_DECLARATION_BODY);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_LPAREN )
        {
            result->GetChilds()->push_back(func_declaration_body());
        }
        else if( opt->GetOptType() == OPT_LSQUARE ||
            opt->GetOptType() == OPT_SEMICOLON )
        {
            result->GetChilds()->push_back(var_declaration_body());
        }
        else
        {
            result->GetChilds()->push_back(new Error(ERR_MISSING_SEMI));
        }
    }
    else
    {
        result->GetChilds()->push_back(new Error(ERR_MISSING_SEMI));
    }
    return result;
}

Node* Parse::var_declaration_body()
{
    Node* result = new NonTerminalNode(NTT_VAR_DECLARATION_BODY);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;
        if( opt->GetOptType() == OPT_LSQUARE ||
            opt->GetOptType() == OPT_SEMICOLON )
        {
            result->GetChilds()->push_back(SQUARE());
        }
        else
        {
            ConsumeNextToken();
            delete result;
            result = NULL;
            result = new Error(ERR_MISSING_SEMI);
        }
    }
    else
    {
        delete result;
        result = NULL;
        result = new Error(ERR_MISSING_SEMI);
    }

    return result;
}

Node* Parse::func_declaration_body()
{
    Node* result = new NonTerminalNode( NTT_FUNC_DECLARATION_BODY);

    Token* nextToken;

    result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));

    result->GetChilds()->push_back(params());

    nextToken = GetToken(1);
    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_RPAREN )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            result->GetChilds()->push_back(new Error(ERR_UNMATCHED_PAREN));
        }
    }
    else
    {
        result->GetChilds()->push_back(new Error(ERR_UNMATCHED_PAREN));
    }

    result->GetChilds()->push_back(compound_stmt());

    return result;
}


Node* Parse::type_specifier()
{

    Node* result = new NonTerminalNode(NTT_TYPE_SPECIFIER);
    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == RESERVEDWORD )
    {
        ReservedWord* RWord = (ReservedWord*)nextToken;
        if( RWord->GetReservedWord() == ReservedWords[RW_INT] ||
            RWord->GetReservedWord() == ReservedWords[RW_VOID] )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            result->GetChilds()->push_back(new Error(ERR_WRONG_TYPE));
        }
    }
    else
    {
        result->GetChilds()->push_back(new Error(ERR_WRONG_TYPE));
    }
    return result;
}

Node* Parse::param_list()
{
    Node* result = new NonTerminalNode(NTT_PARAM_LIST);

    result->GetChilds()->push_back( param());

    result->GetChilds()->push_back( more_param());


    return result;
}

Node* Parse::params()
{
    Node* result = new NonTerminalNode(NTT_PARAMS);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == RESERVEDWORD )
    {
        ReservedWord* RWord = (ReservedWord*)nextToken;
        if( RWord->GetReservedWord() == ReservedWords[RW_VOID] )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            result->GetChilds()->push_back( param_list() );
        }
    }
    else
    {
        result->GetChilds()->push_back(new Error(ERR_WRONG_TYPE));
    }

    return result;
}

Node* Parse::more_param()    //more_param -> COMMA param_list;
	                         //more_param -> EMPTY;

{
    Node* result = new NonTerminalNode(NTT_MORE_PARAM);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_COMMA )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
            result->GetChilds()->push_back(param_list());
        }
        else
        {
            result->GetChilds()->push_back( empty());
        }
    }
    else
    {
        result->GetChilds()->push_back( empty());
    }
    return result;
}

Node* Parse::param()  //param -> type_specifier ID ESQUARE ;
{
    Node* result = new NonTerminalNode(NTT_PARAM);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == RESERVEDWORD )
    {
        ReservedWord* RWord = (ReservedWord*)nextToken;
        if( RWord->GetReservedWord() == ReservedWords[RW_INT] ||
            RWord->GetReservedWord() == ReservedWords[RW_VOID] )
        {
            result->GetChilds()->push_back(type_specifier());
            result->GetChilds()->push_back(identifier());
            result->GetChilds()->push_back(EMSQUARE());
        }
        else
        {
            result->GetChilds()->push_back(new Error(ERR_WRONG_TYPE));
        }
    }
    else
    {
        result->GetChilds()->push_back(new Error(ERR_WRONG_TYPE));
    }

    return result;
}

Node* Parse::SQUARE()  //SQUARE -> LSQUARE NUM RSQUARE;
                       //SQUARE -> EMPTY;

{
    Node* result = new NonTerminalNode(NTT_SQUARE);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_LSQUARE )
        {

            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));

            result->GetChilds()->push_back(number());
            nextToken = GetToken(1);

            if( nextToken->GetType() == OPERATOR )
            {
                Operator* opt = (Operator*)nextToken;

                if( opt->GetOptType() == OPT_RSQUARE )
                {
                    result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
                }
                else
                {
                    result->GetChilds()->push_back(new Error(ERR_UNMATCHED_SQUARE));
                }
            }
            else
            {
                result->GetChilds()->push_back(new Error(ERR_UNMATCHED_SQUARE));
            }
        }
        else
        {
            result->GetChilds()->push_back( empty());
        }
    }
    else
    {
        result->GetChilds()->push_back( empty());
    }

    return result;
}

Node* Parse::EXSQUARE()     //EXSQUARE -> LSQUARE expression RSQUARE;
                            //EXSQUARE -> EMPTY;

{
    Node* result = new NonTerminalNode(NTT_EXSQUARE);


    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_LSQUARE )
        {

            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));

            result->GetChilds()->push_back(expression());
            nextToken = GetToken(1);

            if( nextToken->GetType() == OPERATOR )
            {
                Operator* opt = (Operator*)nextToken;

                if( opt->GetOptType() == OPT_RSQUARE )
                {
                    result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
                }
                else
                {
                    result->GetChilds()->push_back(new Error(ERR_UNMATCHED_SQUARE));
                }
            }
            else
            {
                result->GetChilds()->push_back(new Error(ERR_UNMATCHED_SQUARE));
            }
        }
        else
        {
            result->GetChilds()->push_back( empty());
        }
    }
    else
    {
        result->GetChilds()->push_back( empty());
    }



    return result;
}

Node* Parse::EMSQUARE()
                            //EMSQUARE -> EMPTY;
                            //EMSQUARE -> LSQUARE RSQUARE;

{
    Node* result = new NonTerminalNode(NTT_EMSQUARE);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_LSQUARE )
        {

            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));

            nextToken = GetToken(1);

            if( nextToken->GetType() == OPERATOR )
            {
                Operator* opt = (Operator*)nextToken;

                if( opt->GetOptType() == OPT_RSQUARE )
                {
                    result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
                }
                else
                {
                    result->GetChilds()->push_back(new Error(ERR_UNMATCHED_SQUARE));
                }
            }
            else
            {
                result->GetChilds()->push_back(new Error(ERR_UNMATCHED_SQUARE));
            }
        }
        else
        {
            result->GetChilds()->push_back( empty());
        }
    }
    else
    {
        result->GetChilds()->push_back( empty());
    }


    return result;
}


Node* Parse::local_declarations()   //local_declarations -> local_declaration local_declarations;
                                    //local_declarations -> EMPTY ;
{
    Node* result = new NonTerminalNode(NTT_LOCAL_DECLARATIONS);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == RESERVEDWORD )
    {
        ReservedWord* RWord = (ReservedWord*)nextToken;
        if( RWord->GetReservedWord() == ReservedWords[RW_INT] ||
            RWord->GetReservedWord() == ReservedWords[RW_VOID] )
        {
            result->GetChilds()->push_back(local_declaration());
            result->GetChilds()->push_back(local_declarations());
        }
        else
        {
            result->GetChilds()->push_back(empty());
        }
    }
    else
    {
        result->GetChilds()->push_back(empty());
    }
    return result;
}

Node* Parse::local_declaration()
{
    Node* result = new NonTerminalNode(NTT_DECLARATION);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == RESERVEDWORD )
    {
        ReservedWord* RWord = (ReservedWord*)nextToken;
        if( RWord->GetReservedWord() == ReservedWords[RW_INT] ||
            RWord->GetReservedWord() == ReservedWords[RW_VOID] )
        {
            result->GetChilds()->push_back( type_specifier());
            result->GetChilds()->push_back( identifier() );
            result->GetChilds()->push_back( SQUARE() );

            nextToken = GetToken(1);

            if( nextToken->GetType() == OPERATOR )
            {
                Operator* opt = (Operator*)nextToken;

                if( opt->GetOptType() == OPT_SEMICOLON )
                {
                    result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
                }
                else
                {
                    result->GetChilds()->push_back(new Error(ERR_MISSING_SEMI));
                }
            }
            else
            {
                result->GetChilds()->push_back(new Error(ERR_MISSING_SEMI));
            }
        }
        else
        {
            result->GetChilds()->push_back(new Error(ERR_WRONG_TYPE));
        }
    }
    else
    {
        result->GetChilds()->push_back(new Error(ERR_WRONG_TYPE));
    }

    return result;
}

Node* Parse::statement_list()
{
    Node* result = new NonTerminalNode(NTT_STATEMENT_LIST);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == IDENTIFIER )
    {
        result->GetChilds()->push_back( statement() );
        result->GetChilds()->push_back( statement_list() );
    }
    else if( nextToken->GetType() == RESERVEDWORD )
    {
        ReservedWord* RWord = (ReservedWord*)nextToken;
        if( RWord->GetReservedWord() == ReservedWords[RW_IF] ||
            RWord->GetReservedWord() == ReservedWords[RW_WHILE] ||
            RWord->GetReservedWord() == ReservedWords[RW_RETURN] )
        {
            result->GetChilds()->push_back( statement() );
            result->GetChilds()->push_back( statement_list() );
        }
        else if( RWord->GetReservedWord() == ReservedWords[RW_ELSE] )
        {
            result->GetChilds()->push_back( new Error(ERR_MISSING_IF) );
        }
        else
        {
            result->GetChilds()->push_back( new Error(ERR_NOTALLOWED_DECLAR));
        }
    }
    else if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_LBRACE )
        {
            result->GetChilds()->push_back( statement() );
            result->GetChilds()->push_back( statement_list() );
        }
        else
        {
            result->GetChilds()->push_back(empty());
        }
    }
    else
    {
        result->GetChilds()->push_back(empty());
    }
    return result;
}

Node* Parse::statement()
{
    Node* result = new NonTerminalNode(NTT_STATEMENT);

    Token* nextToken = GetToken(1);
    Operator * opt;
    if( nextToken->GetType() == IDENTIFIER )
    {
        nextToken = GetToken(2);

        if( nextToken->GetType() == OPERATOR )
        {
            opt = (Operator*)nextToken;
            if( opt->GetOptType() == OPT_ASSIGN )
            {
                result->GetChilds()->push_back( assign_stmt() );
            }
            else
            {
                result->GetChilds()->push_back( expression_stmt() );
            }
        }
        else
        {
            result->GetChilds()->push_back( expression_stmt() );
        }
    }
    else if( nextToken->GetType() == RESERVEDWORD )
    {
        ReservedWord* RWord = (ReservedWord*)nextToken;
        if( RWord->GetReservedWord() == ReservedWords[RW_IF])
        {
            result->GetChilds()->push_back( selection_stmt() );
        }
        else if(RWord->GetReservedWord() == ReservedWords[RW_WHILE])
        {
            result->GetChilds()->push_back( iteration_stmt() );
        }
        else if(RWord->GetReservedWord() == ReservedWords[RW_RETURN] )
        {
            result->GetChilds()->push_back( return_stmt() );
        }
        else if( RWord->GetReservedWord() == ReservedWords[RW_ELSE] )
        {
            result->GetChilds()->push_back( new Error(ERR_MISSING_IF) );
        }
        else
        {
            ConsumeNextToken();
            result->GetChilds()->push_back( new Error(ERR_NOTALLOWED_DECLAR));
        }
    }
    else if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_LBRACE )
        {
            result->GetChilds()->push_back( compound_stmt() );
        }
        else
        {
            ConsumeNextToken();
            result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
        }
    }
    else
    {
        ConsumeNextToken();
        result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
    }

    return result;
}

Node* Parse::compound_stmt()
{
    Node* result = new NonTerminalNode(NTT_COMPOUND_STMT);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_LBRACE )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            ConsumeNextToken();
            result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
        }
    }
    else
    {
        ConsumeNextToken();
        result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
    }

    result->GetChilds()->push_back( local_declarations() );

    result->GetChilds()->push_back( statement_list() );

    nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_RBRACE )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            result->GetChilds()->push_back(new Error(ERR_UNMATCHED_BRACE));
        }
    }
    else
    {
        result->GetChilds()->push_back(new Error(ERR_UNMATCHED_BRACE));
    }
    return result;
}

Node* Parse::expression_stmt()  // expression_stmt -> expression SEMI ;
                                // expression_stmt -> SEMI ;
{
    Node* result = new NonTerminalNode(NTT_EXPRESSION_STMT);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_SEMICOLON )
        {
            result->GetChilds()->push_back( new TerminalNode( ConsumeNextToken() ));
        }
        else
        {
            result->GetChilds()->push_back( expression() );
            nextToken = GetToken(1);

            if( nextToken->GetType() == OPERATOR )
            {
                Operator* opt = (Operator*)nextToken;

                if( opt->GetOptType() == OPT_SEMICOLON )
                {
                    result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
                }
                else
                {
                    result->GetChilds()->push_back(new Error(ERR_MISSING_SEMI));
                }
            }
            else
            {
                result->GetChilds()->push_back(new Error(ERR_MISSING_SEMI));
            }
        }
    }
    else
    {
        result->GetChilds()->push_back( expression() );
        nextToken = GetToken(1);

        if( nextToken->GetType() == OPERATOR )
        {
            Operator* opt = (Operator*)nextToken;

            if( opt->GetOptType() == OPT_SEMICOLON )
            {
                result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
            }
            else
            {
                result->GetChilds()->push_back(new Error(ERR_MISSING_SEMI));
            }
        }
        else
        {
            result->GetChilds()->push_back(new Error(ERR_MISSING_SEMI));
        }
    }

    return result;
}

Node* Parse::assign_stmt()
{
    Node* result = new NonTerminalNode(NTT_ASSIGN_STMT);

    Token* nextToken = GetToken(1);

    result->GetChilds()->push_back( var() );

    nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_ASSIGN )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            ConsumeNextToken();
            result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
        }
    }
    else
    {
        ConsumeNextToken();
        result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
    }

    result->GetChilds()->push_back( expression() );

    nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_SEMICOLON )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            result->GetChilds()->push_back(new Error(ERR_MISSING_SEMI));
        }
    }
    else
    {
        result->GetChilds()->push_back(new Error(ERR_MISSING_SEMI));
    }

    return result;
}

Node* Parse::selection_stmt()
{
    Node* result = new NonTerminalNode(NTT_SELECTION_STMT);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == RESERVEDWORD )
    {
        ReservedWord* RWord = (ReservedWord*)nextToken;
        if( RWord->GetReservedWord() == ReservedWords[RW_IF])
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            ConsumeNextToken();
            result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
        }
    }
    else
    {
        ConsumeNextToken();
        result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
    }

    nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_LPAREN )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            ConsumeNextToken();
            result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
        }
    }
    else
    {
        ConsumeNextToken();
        result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
    }

    result->GetChilds()->push_back( expression() );

    nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_RPAREN )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            result->GetChilds()->push_back(new Error(ERR_UNMATCHED_PAREN));
        }
    }
    else
    {
        result->GetChilds()->push_back(new Error(ERR_UNMATCHED_PAREN));
    }

    result->GetChilds()->push_back( statement() );

    result->GetChilds()->push_back( else_stmt() );

    return result;
}

Node* Parse::else_stmt()
{
    Node* result = new NonTerminalNode(NTT_ELSE_STMT);
    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == RESERVEDWORD )
    {
        ReservedWord* RWord = (ReservedWord*)nextToken;
        if( RWord->GetReservedWord() == ReservedWords[RW_ELSE])
        {
            result->GetChilds()->push_back( new TerminalNode(ConsumeNextToken()));

            result->GetChilds()->push_back( statement());
        }
        else
        {
            result->GetChilds()->push_back(empty());
        }
    }
    else
    {
        result->GetChilds()->push_back(empty());
    }
    return result;
}

Node* Parse::iteration_stmt()
{
    Node* result = new NonTerminalNode(NTT_ITERATION_STMT);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == RESERVEDWORD )
    {
        ReservedWord* RWord = (ReservedWord*)nextToken;
        if( RWord->GetReservedWord() == ReservedWords[RW_WHILE])
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            ConsumeNextToken();
            result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
        }
    }
    else
    {
        ConsumeNextToken();
        result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
    }

    nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_LPAREN )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            ConsumeNextToken();
            result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
        }
    }
    else
    {
        ConsumeNextToken();
        result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
    }

    result->GetChilds()->push_back( expression() );

    nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_RPAREN )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            result->GetChilds()->push_back(new Error(ERR_UNMATCHED_PAREN));
        }
    }
    else
    {
        result->GetChilds()->push_back(new Error(ERR_UNMATCHED_PAREN));
    }

    result->GetChilds()->push_back( statement() );

    return result;
}

Node* Parse::return_stmt()          //return_stmt -> ‘RETURN’ SEMI ;
                                    //return_stmt -> ‘RETURN’ expression SEMI ;

{
    Node* result = new NonTerminalNode(NTT_RETURN_STMT);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == RESERVEDWORD )
    {
        ReservedWord* RWord = (ReservedWord*)nextToken;
        if( RWord->GetReservedWord() == ReservedWords[RW_RETURN])
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            ConsumeNextToken();
            result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
        }
    }
    else
    {
        ConsumeNextToken();
        result->GetChilds()->push_back(new Error(ERR_WRONG_TOKEN));
    }

    nextToken = GetToken(1);

    if( nextToken->GetType() == IDENTIFIER )
    {
        result->GetChilds()->push_back( expression() );
    }
    else if( nextToken->GetType() == NUMBER )
    {
        result->GetChilds()->push_back( expression() );
    }
    else if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_LBRACE )
        {
            result->GetChilds()->push_back( expression() );
        }
    }

    nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_SEMICOLON )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            result->GetChilds()->push_back(new Error(ERR_MISSING_SEMI));
        }
    }
    else
    {
        result->GetChilds()->push_back(new Error(ERR_MISSING_SEMI));
    }

    return result;
}


Node* Parse::expression()
{
    Node* result = new NonTerminalNode(NTT_EXPRESSION);

    result->GetChilds()->push_back( additive_expression() );

    result->GetChilds()->push_back( more_additive_expression() );

    return result;
}

Node* Parse::additive_expression()
{
    Node* result = new NonTerminalNode(NTT_ADDITIVE_EXPRESSION);

    result->GetChilds()->push_back( term() );

    result->GetChilds()->push_back( more_term() );

    return result;
}

Node* Parse::more_additive_expression()
{
    Node* result = new NonTerminalNode(NTT_MORE_ADDITIVE_EXPRESSION);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_EQUAL ||
            opt->GetOptType() == OPT_UNEQUAL ||
            opt->GetOptType() == OPT_GREATER_THAN ||
            opt->GetOptType() == OPT_ENGREATER_THAN ||
            opt->GetOptType() == OPT_LESS_THAN ||
            opt->GetOptType() == OPT_ENLESS_THAN)
        {
            result->GetChilds()->push_back( relop() );
            result->GetChilds()->push_back( additive_expression() );
            result->GetChilds()->push_back( more_additive_expression() );
        }
        else
        {
            result->GetChilds()->push_back(empty());
        }
    }
    else
    {
        result->GetChilds()->push_back(empty());
    }
    return result;
}

Node* Parse::term()
{
    Node* result = new NonTerminalNode(NTT_TERM);

    result->GetChilds()->push_back( factor() );

    result->GetChilds()->push_back( more_factor() );

    return result;
}

Node* Parse::more_term()
{
    Node* result = new NonTerminalNode(NTT_MORE_TERM);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_PLUS ||
            opt->GetOptType() == OPT_MINUS  )
        {
            result->GetChilds()->push_back( addop() );
            result->GetChilds()->push_back( term() );
            result->GetChilds()->push_back( more_term() );
        }
        else
        {
            result->GetChilds()->push_back(empty());
        }
    }
    else
    {
        result->GetChilds()->push_back(empty());
    }

    return result;
}

Node* Parse::factor()
{
    Node* result = new NonTerminalNode( NTT_FACTOR );
    Operator *opt = NULL;
    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == IDENTIFIER )
    {
        nextToken = GetToken(2);

        if( nextToken->GetType() == OPERATOR )
        {
            opt = (Operator*)nextToken;
            if( opt->GetOptType() == OPT_LPAREN )
            {
                result->GetChilds()->push_back( call() );
            }
            else
            {
                result->GetChilds()->push_back( var() );
            }
        }
    }
    else if( nextToken->GetType() == NUMBER )
    {
        result->GetChilds()->push_back( number() );
    }
    else if( nextToken->GetType() == OPERATOR )
    {
        opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_LPAREN )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
            result->GetChilds()->push_back( expression() );
            nextToken = GetToken(2);

            if( nextToken->GetType() == OPERATOR )
            {
                opt = (Operator*)nextToken;
                if( opt->GetOptType() == OPT_RPAREN )
                {
                    result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
                }
                else
                {
                    result->GetChilds()->push_back( new Error(ERR_UNMATCHED_PAREN));
                }
            }
            else
            {
                result->GetChilds()->push_back( new Error(ERR_UNMATCHED_PAREN));
            }
        }
        else
        {
            result->GetChilds()->push_back( new Error(ERR_UNMATCHED_PAREN));
        }
    }
    else
    {
        ConsumeNextToken();
        result->GetChilds()->push_back( new Error(ERR_WRONG_TOKEN));
    }

    return result;
}

Node* Parse::more_factor()
{
    Node* result = new NonTerminalNode( NTT_MORE_FACTOR );

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_TIMES ||
            opt->GetOptType() == OPT_DIVIDE  )
        {
            result->GetChilds()->push_back( mulop() );
            result->GetChilds()->push_back( factor() );
            result->GetChilds()->push_back( more_factor() );
        }
        else
        {
            result->GetChilds()->push_back(empty());
        }
    }
    else
    {
        result->GetChilds()->push_back(empty());
    }

    return result;
}

Node* Parse::var()
{
    Node* result = new NonTerminalNode(NTT_VAR);

    result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));

    result->GetChilds()->push_back( EXSQUARE() );

    return result;
}

Node* Parse::call()
{
    Node* result = new NonTerminalNode(NTT_CALL);
    Operator *opt;

    result->GetChilds()->push_back( identifier() );

    result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));

    result->GetChilds()->push_back( args() );

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        opt = (Operator*)nextToken;
        if( opt->GetOptType() == OPT_RPAREN )
        {
            result->GetChilds()->push_back(new TerminalNode(ConsumeNextToken()));
        }
        else
        {
            ConsumeNextToken();
            result->GetChilds()->push_back(new Error(ERR_UNMATCHED_PAREN));
        }
    }
    else
    {
        result->GetChilds()->push_back(new Error(ERR_UNMATCHED_PAREN));
    }

    return result;
}

Node* Parse::args()
{
    Node* result = new NonTerminalNode(NTT_ARGS);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == IDENTIFIER )
    {
        result->GetChilds()->push_back( expression() );
        result->GetChilds()->push_back( more_args() );
    }
    else if( nextToken->GetType() == NUMBER )
    {
        result->GetChilds()->push_back( expression() );
        result->GetChilds()->push_back( more_args() );
    }
    else if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_LBRACE )
        {
            result->GetChilds()->push_back( expression() );
            result->GetChilds()->push_back( more_args() );
        }
    }
    else
    {
        result->GetChilds()->push_back( empty() );
    }

    return result;
}

Node* Parse::more_args()
{
    Node* result = new NonTerminalNode(NTT_MORE_ARGS);

    Token* nextToken = GetToken(1);

    if( nextToken->GetType() == OPERATOR )
    {
        Operator* opt = (Operator*)nextToken;

        if( opt->GetOptType() == OPT_COMMA )
        {
            result->GetChilds()->push_back( new TerminalNode(ConsumeNextToken()));
            result->GetChilds()->push_back( args() );
            result->GetChilds()->push_back( more_args() );
        }
        else
        {
            result->GetChilds()->push_back( empty() );
        }
    }
    else
    {
        result->GetChilds()->push_back( empty() );
    }
    return result;
}



    // Terminal Node Constructor
Node* Parse::identifier()
{
    Node* result = new TerminalNode(ConsumeNextToken());
    return result;
}

Node* Parse::number()
{
    Node* result = new TerminalNode(ConsumeNextToken());
    return result;
}

Node* Parse::relop()
{
    Node* result = new NonTerminalNode(NTT_RELOP);

    result->GetChilds()->push_back( new TerminalNode(ConsumeNextToken()));

    return result;
}

Node* Parse::addop()
{
    Node* result = new NonTerminalNode(NTT_ADDOP);

    result->GetChilds()->push_back( new TerminalNode(ConsumeNextToken()));
    return result;
}

Node* Parse::mulop()
{
    Node* result = new NonTerminalNode(NTT_MULOP);

    result->GetChilds()->push_back( new TerminalNode(ConsumeNextToken()));
    return result;
}

Node* Parse::empty()
{
    Node* result = new NonTerminalNode;
    return result;
}



void Parse::printTree( Node* program )
{
    int i;

    ofstream outfile( "parseFile");

    vector<Node*>* root = program->GetChilds();

    int iNumberOfSubTree = root->size();

    NonTerminalNode* NTN = (NonTerminalNode*)program;

    cout<<NonTerminalName[ NTN->GetNTType() ]<<endl;
    outfile<<NonTerminalName[ NTN->GetNTType() ]<<endl;

    vector<Node*>::iterator iter = root->begin();

    stack.push_back(0);

    for( i = 0 ; i < iNumberOfSubTree ; i++ )
    {
        if( i == iNumberOfSubTree - 1 )
            stack.pop_back();

        printTreeHelp( *iter , 1 );

        iter++;
    }

    outfile.close();

}

void Parse::printTreeHelp( Node* subTree , int n )
{
    ofstream outfile( "parseFile" ,ios_base::app);

    int i,j;
    NonTerminalNode* NTN = (NonTerminalNode*)subTree;
    TerminalNode* TN;
    Token* tk;

    for( i = 0 ; i < n-1 ; i++ )
    {
        if( isExist( i , stack ) )
        {
            cout<<"│";
            outfile<<"│";
        }
        else
        {
            cout<<" ";
            outfile<<" ";
        }
    }
    cout<<"└";
    outfile<<"└";

    cout<<NonTerminalName[ NTN->GetNTType() ]<<endl;
    outfile<<NonTerminalName[ NTN->GetNTType() ]<<endl;

    stack.push_back(n);

    if( NTN->GetNTType() != NTT_EMPTY)
    {
        vector<Node*>* root = subTree->GetChilds();

        int iNumberOfSubTree = root->size();

        vector<Node*>::iterator iter = root->begin();

        for( i = 0 ; i < iNumberOfSubTree ; i++ )
        {
            if( (*iter)->GetType() == NON_TERMINAL_NODE )
            {
                printTreeHelp( *iter , n + 1 );
            }
            else
            {
                for( j = 0 ; j < n ; j++ )
                {
                    if( isExist( i , stack ) )
                    {
                        cout<<"│";
                        outfile<<"│";
                    }
                    else
                    {
                        cout<<"  ";
                        outfile<<"  ";
                    }
                }
                cout<<"└";
                outfile<<"└";

                TN = (TerminalNode*)(*iter);
                tk = TN->GetToken();
                switch(tk->GetType())
                {
                case IDENTIFIER:
                    cout<<*(Identifier*)tk<<endl;
                    outfile<<*(Identifier*)tk<<endl;
                    break;
                case NUMBER:
                    cout<<*(Number*)tk<<endl;
                    outfile<<*(Number*)tk<<endl;
                    break;
                case OPERATOR:
                    cout<<*(Operator*)tk<<endl;
                    outfile<<*(Operator*)tk<<endl;
                    break;
                case RESERVEDWORD:
                    cout<<*(ReservedWord*)tk<<endl;
                    outfile<<*(ReservedWord*)tk<<endl;
                    break;
                default:
                    cout<<"Token Type error.."<<endl;
                    outfile<<"Token Type error.."<<endl;
                    break;
                }
            }
            iter++;
        }
    }
    stack.pop_back();
    outfile.close();
}

bool isExist(int t, vector<int> &vec)
{
    vector<int>::iterator iter;
    iter = vec.begin();

    bool result = false;

    int i;
    for( i = 0 ; i < vec.size() ; i++ )
    {
        if( *iter == t )
        {
            result = true;
        }
        iter++;
    }
    return result;
}
