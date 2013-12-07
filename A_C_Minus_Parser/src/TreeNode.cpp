
#include "Global.h"
#include "TreeNode.h"

Node::Node()
{}

Node::~Node()
{}

NonTerminalNode::NonTerminalNode()
{
    Type = NTT_EMPTY;
    Childs = NULL;
}

NonTerminalNode::NonTerminalNode(NonTerminalType type)
{
    Type = type;
    Childs = new vector<Node*>;
}

NonTerminalNode::~NonTerminalNode()
{
    delete Childs;
    Childs = NULL;
}

vector<Node*>* NonTerminalNode::GetChilds()
{
    return Childs;
}

NonTerminalType NonTerminalNode::GetNTType()
{
    return Type;
}


TerminalNode::TerminalNode()
{
    tk = NULL;
}

TerminalNode::TerminalNode(Token *_tk)
{
    switch(_tk->GetType())
    {
    case IDENTIFIER:
        tk = new Identifier(*(Identifier*)_tk);
        break;
    case NUMBER:
        tk = new Number(*(Number*)_tk);
        break;
    case OPERATOR:
        tk = new Operator(*(Operator*)_tk);
        break;
    case RESERVEDWORD:
        tk = new ReservedWord(*(ReservedWord*)_tk);
        break;
    default:
        cerr<<"Create Terminal Node Failed..."<<endl;
        break;
    }
}

TerminalNode::~TerminalNode()
{
    delete tk;
    tk = NULL;
}

vector<Node*>* TerminalNode::GetChilds()
{
    return NULL;
}

Token* TerminalNode::GetToken()
{
    return tk;
}

string Error::GetErrorMessage()
{
    return ErrorMessage[Type];
}

ostream & operator << ( ostream & out, Error & err)
{
    out<<err.GetErrorMessage();
    return out;
}

