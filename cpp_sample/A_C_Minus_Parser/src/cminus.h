#ifndef __CMINUS_H__
#define __CMINUS_H__

#include "global.h"
#include "Token.h"
#include "TreeNode.h"

typedef int CharType;

/* define the char */
#define CH_LETTER			0		//[a-zA-Z]
#define CH_DIGIT			1		//[0-9]
#define CH_WHITESPACE		2		//
#define CH_SLASH			3		//'/'
#define CH_STAR				4		//'*'
#define CH_PLUS				5		//'+'
#define CH_MINUS			6		//'-'
#define CH_SMALLER          7		//'<'
#define CH_BIGGER           8		//'>'
#define CH_EQUAL	        9		//'='
#define CH_SEMICOLON       10		//';'
#define CH_COMMA           11		//','
#define CH_EXCLAMATION	   12		//'!'
#define CH_LBRACKET        13		//'('
#define CH_RBRACKET        14		//')'
#define CH_LSBRACKET       15		//'['
#define CH_RSBRACKET       16		//']'
#define CH_LBRACE          17		//'{'
#define CH_RBRACE          18		//'}'
#define CH_ERROR           19

typedef int Status;
/* define the stats */
#define ST_START			0
#define ST_INID				1
#define ST_INNUM			2
#define ST_SLASH			3
#define ST_INCOMMENT		4
#define ST_END_COMMENT		5
#define ST_SMALLER          6
#define ST_BIGGER           7
#define ST_EQUAL            8
#define ST_EXCLAMATION      9

/* ½ÓÊÜ×´Ì¬ 13~34 */
#define ST_RETURN_ID            10
#define ST_RETURN_NUM           11
#define ST_RETURN_COMMENT       12
#define ST_RETURN_PLUS		    13
#define ST_RETURN_TIMES			14
#define ST_RETURN_MINUS			15
#define ST_RETURN_DEVIDE		16
#define ST_RETURN_SMALLER		17
#define ST_RETURN_BIGGER        18
#define ST_RETURN_ENSMALLER     19
#define ST_RETURN_ENBIGGER      20
#define ST_RETURN_EQUAL         21
#define ST_RETURN_ASSIGN        22
#define ST_RETURN_UNEQUAL       23
#define ST_RETURN_SEMICOLON     24
#define ST_RETURN_COMMA         25
#define ST_RETURN_LBRACKET      26
#define ST_RETURN_RBRACKET      27
#define ST_RETURN_LSBRACKET     28
#define ST_RETURN_RSBRACKET     29
#define ST_RETURN_LBRACE        30
#define ST_RETURN_RBRACE        31
#define ST_RETURN_ERROR		    32
#define ST_RETURN_WHITESPACE    33
#define NOT_RECEIVED            34

static char OptTable[] =
{
    '/','*','+','-','<','>','=',';',',','!','(',')','[',']','{','}',     //0~15
};

static int StateSwitchTable[10][20] =  //[Presant States][Input Character]
{
    {   1,  2,	33,	3,	14,	13,	15,	6,	7,	8,	24,	25,	9,	26,	27,	28,	29,	30,	31,	32  },
    {   1,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10  },
    {   11,	2,	11, 11,	11,	11,	11,	11,	11,	11,	11,	11,	11,	11,	11,	11,	11,	11,	11,	11  },
    {   16,	16,	16,	16,	4,	16,	16,	16,	16,	16,	16,	16,	16,	16,	16,	16,	16,	16,	16,	16  },
    {   4,	4,	4,	4,	5,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4   },
    {   4,	4,	4,	12,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4, 	4,	4,	4,	4,	4,	4   },
    {   17,	17,	17,	17,	17,	17,	17,	17,	17,	19,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17  },
    {   18,	18,	18,	18,	18,	18,	18,	18,	18,	20,	18,	18,	18,	18,	18,	18,	18,	18,	18,	18  },
    {   22,	22,	22,	22,	22,	22,	22,	22,	22,	21,	22,	22,	22,	22,	22,	22,	22,	22,	22,	22  },
    {   32,	32,	32,	32,	32,	32,	32,	32,	32,	23,	32,	32,	32,	32,	32,	32,	32,	32,	32,	32  }
};



#define BUFLEN 256

class Scaner
{
private:
    ifstream& inputFile;
    char lineBuf[BUFLEN];
    int linePos;
    int bufsize;
    char getNextChar(char* line);
    void ungetNextChar();
	Status status;

    vector<Identifier*> IDTable;
//  vector<int> numTable;
    vector< vector<Token*> > TokenTable;
    Status pushInDFA(CharType input);
	int classifyChar(char ch);
    int classifyOpt(char ch);

    ReservedWordIndex isReservedWords(const char *str);
    Token* InsertIdentifier(string TokenContent);
public:
    Scaner(ifstream &pFile);
    ~Scaner();
    bool Scan(string outputFileName);

    vector< vector<Token*> > GetTokenTable();
    vector<Identifier*> GetIDTable();
};


class Parse
{
private:
    vector< vector<Token*> > TokenTable;
    vector<Identifier*> IDTable;
    Node* ParseTree;

    int iLineNo;
    int iTokenPos;

public:
    Token* ConsumeNextToken();
    Token* GetToken(int n);

    vector<int> stack;
    void printTree( Node* program );
    void printTreeHelp( Node* subTree , int n );

    // Non Terminal Node constructor
public:
    Node* program();

private:

    Node* declaration();

    Node* declaration_list();


    Node* declaration_body();
    Node* var_declaration_body();
    Node* func_declaration_body();

    Node* type_specifier();

    Node* param_list();
    Node* params();
    Node* param();
    Node* more_param();

    Node* SQUARE();
    Node* EXSQUARE();
    Node* EMSQUARE();

    Node* local_declarations();
    Node* local_declaration();

    Node* statement_list();
    Node* statement();
    Node* compound_stmt();
    Node* expression_stmt();

    Node* assign_stmt();
    Node* selection_stmt();
    Node* iteration_stmt();
    Node* return_stmt();

    Node* else_stmt();

    Node* expression();
    Node* additive_expression();
    Node* more_additive_expression();
    Node* term();
    Node* more_term();
    Node* factor();
    Node* more_factor();
    Node* var();
    Node* call();
    Node* args();
    Node* more_args();

        // Terminal Node Constructor
    Node* identifier();
    Node* number();
    Node* relop();
    Node* addop();
    Node* mulop();


    Node* empty();


public:
    Parse();
    Parse(Scaner &scaner);
    ~Parse();
};

bool isExist(int t, vector<int> &vec);

#endif

