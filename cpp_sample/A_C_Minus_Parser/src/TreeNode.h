#ifndef __TREE_NODE_H__
#define __TREE_NODE_H__

#include "Token.h"
#include "Global.h"

/* NTT stand for Non Terminal Type */
#define NTT_PROGRAM                                 0
#define NTT_DECLARATION                             1
#define NTT_DECLARATION_LIST                        2

#define NTT_DECLARATION_BODY                        3
#define NTT_VAR_DECLARATION_BODY                    4
#define NTT_FUNC_DECLARATION_BODY                   5
#define NTT_TYPE_SPECIFIER                          6

#define NTT_MORE_PARAM                              7
#define NTT_PARAMS                                  8   
#define NTT_PARAM_LIST                              9   
#define NTT_PARAM                                   10        

#define NTT_LOCAL_DECLARATION                       11
#define NTT_LOCAL_DECLARATIONS                      12
#define NTT_MORE_LOCAL_DECLARATION                  13

#define NTT_STATEMENT_LIST                          14    
#define NTT_STATEMENT                               15

#define NTT_SQUARE                                  16
#define NTT_EMSQUARE                                17
#define NTT_EXSQUARE                                18

#define NTT_ASSIGN_STMT                             19
#define NTT_COMPOUND_STMT                           20
#define NTT_EXPRESSION_STMT                         21
#define NTT_SELECTION_STMT                          22
#define NTT_ITERATION_STMT                          23  
#define NTT_RETURN_STMT                             24

#define NTT_ELSE_STMT                               25

#define NTT_EXPRESSION                              26
#define NTT_ADDITIVE_EXPRESSION                     27
#define NTT_MORE_ADDITIVE_EXPRESSION                28
#define NTT_TERM                                    29
#define NTT_MORE_TERM                               30
#define NTT_FACTOR                                  31
#define NTT_MORE_FACTOR                             32


#define NTT_VAR                                     33
#define NTT_CALL                                    34
#define NTT_MULOP                                   35
#define NTT_ADDOP                                   36
#define NTT_RELOP                                   37
#define NTT_ARGS                                    38
#define NTT_MORE_ARGS                               39

#define NTT_EMPTY                                   40

typedef int NonTerminalType;

static string NonTerminalName[] = 
{
    "program","declaration","declaration_list","declaration_body","var_declaration_body","func_daclaration_body",
    "type_specifier","more_param","params","param_list","param","local_declaration","local_declarations",
    "more_local_declaration","statement_list","statement","SQUARE","EMSQUARE","EXSQUARE","assign_stmt","compound_stmt","expression_stmt",
    "selection_stmt","iterator_stmt","return_stmt","else_stmt","expression","additive_expression","more_additive_expression",
    "term","more_term","factor","more_factor","var","call","mulop","addop","relop","args","more_args","empty"
};


typedef enum
{ 
    NON_TERMINAL_NODE,TERMINAL_NODE,ERROR
} NodeType;

class Node
{
public:
    Node();
    ~Node();

    virtual NodeType GetType() = 0;
    virtual vector<Node*>* GetChilds() = 0;
};

class NonTerminalNode : public Node
{
private:
    NonTerminalType Type;
    vector<Node*>* Childs;
public:
    NonTerminalNode();
    NonTerminalNode(NonTerminalType);
    ~NonTerminalNode();

    NodeType GetType()
    {
        return NON_TERMINAL_NODE;
    }

    NonTerminalType GetNTType();

    vector<Node*>* GetChilds();
};

class TerminalNode : public Node
{
private:
    Token* tk;
public:
    TerminalNode();
    TerminalNode(Token *);

    ~TerminalNode();

    NodeType GetType()
    {
        return TERMINAL_NODE;
    }

    Token * GetToken();

    vector<Node*>* GetChilds();
};

 // define error message 
#define ERR_WRONG_TYPE              0
#define ERR_MISSING_SEMI            1
#define ERR_MISSING_LPAREN          2
#define ERR_UNMATCHED_PAREN         3
#define ERR_UNMATCHED_SQUARE        4
#define ERR_UNMATCHED_BRACE         5
#define ERR_MISSING_IF              6
#define ERR_NOTALLOWED_DECLAR       7
#define ERR_MISSING_LBRACE          8
#define ERR_WRONG_TOKEN             9

typedef int ErrorType;

static string ErrorMessage[] =
{
    "错误的类型","丢失分号';'","应输入'('","应输入')'","应输入']'","应输入'}'",
    "无匹配的'if'","不允许声明","没有{"," 错误的符号"
};

class Error : public Node
{
private:
    ErrorType Type;
public:
    Error(ErrorType type)
    {
        Type = type;
    }

    ~Error();

    string GetErrorMessage();

    NodeType GetType()
    {
        return ERROR;
    }
    vector<Node*>* GetChilds()
    {
         return NULL;
    }

    friend ostream & operator << ( ostream & out , Error & err);
};

#endif
