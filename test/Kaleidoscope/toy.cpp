#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
  tok_eof = -1,

  // commands
  tok_def = -2, tok_extern = -3,

  // primary
  tok_identifier = -4, tok_number = -5,
};

static std::string IdentifierStr;  // Filled in if tok_identifier
static double NumVal;              // Filled in if tok_number

/// gettok - Return the next token from standard input.
static int gettok() {
    static int LastChar = ' ';

    // Skip any whitespaces.
    while(isspace(LastChar)){
        LastChar = getchar();
        
        if (isalpha(LastChar)) { // identifier: [a-zA-Z]
            IdentifierStr += LastChar;
            
            while(isalnum((LastChar = getchar()))){
                IdentifierStr += LastChar;
            }
            
            if (IdentifierStr == "def") {
                return tok_def;
            }
            
            if (IdentifierStr == "extern") {
                return tok_extern;
            }
            
        }
        
        if (isdigit(LastChar) || LastChar == '.') {
            std::string NumStr;
            
            do
            {
                NumStr += LastChar;
                LastChar = getchar();
            } while (isdigit(LastChar) || LastChar == '.');
            NumVal = strtod(NumStr.c_str(), 0);
            return tok_number;   
        }
        
        if (LastChar == '#') {
            // Comments
            
            do
            {
                LastChar = getchar();
            } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
            
            if (LastChar != EOF) {
                return gettok();
            }    
        }

        // The end of file
        if (LastChar == EOF) {
            return tok_eof;
        }
        
        // Return ascii
        int ThisChar = LastChar;
        LastChar = getchar();
        return ThisChar;
        
    }
    
}
//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//
namespace {
/// ExprAST - Base class for all expression nodes.
class ExprAST
{
  public:
    virtual ~ExprAST() {}
    };
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
    public:
        NumberExprAST(double val) {}
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
    std::string Name;
    public:
        VariableExprAST(const std::string &name) : Name(name) {}
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
    public:
        BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs) {}
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<ExprAST*> Args;
    public:
        CallExprAST(const std::string &callee, std::vector<ExprAST*> &args)
        : Callee(callee), Args(args) {}
};

/// PrototypeAST - captures its name, argument names.
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
    public:
        PrototypeAST(const std::string &name, const std::vector<std::string> &args)
        : Name(name), Args(args) {}
};

/// FunctionAST - define a function
class FunctionAST {
    public:
        FunctionAST(PrototypeAST *proto, ExprAST *body) {}
};
///-----end namespace------------

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

/// CurTok/getNextToken - 
static int CurTok;
static int getNextToken() {
    return CurTok = gettok();
}
/// Error* - Print Errors
ExprAST *Error(const char *Str) {fprintf(stderr, "Error: %s\n", Str); return 0; }
PrototypeAST *ErrorP(const char *Str) {Error(Str); return 0; }
FunctionAST *ErrorF(const char *Str) {Error(Str); return 0; }

/// numberExpr - Parseing numbers
static ExprAST *ParseNumberExpr() {
    ExprAST *Result = new NumberExprAST(NumVal);
    getNextToken(); /// <-- Consume the numbers
    return Result;
}

/// parenExpr - expressions
static ExprAST *ParseParenExpr() {
    getNextToken(); /// <-- eat (
    ExprAST *V = ParseExpression();
    
    if (!V) {
        return 0;
    }
    
    if (CurTok != ')') {
        return Error("expected ')'");
    } 
    getNextToken(); /// <-- eat )
    return V;
}

/// identifierExpr - '(' experssions* ')'
static ExprAST *ParseIdentifierExpr() {
    std::string IdName = IdentifierStr;

    getNextToken(); // <==

    
    if (CurTok != '(') {
        return new VariableExprAST(IdName);
    }

    // Call
    getNextToken();
    std::vector<ExprAST*> Args;
    
    if (CurTok != ')') {
        while(1){
            ExprAST *Arg = ParseExpression();
            if (!Arg) {
                return 0;
            }
            Args.push_back(Arg);
            if (CurTok == ')') {
                break;
            }

            if (CurTok != ',') {
                return Error("Exprected ')' or ',', in argument list");
            }
            getNextToken();
        }
    }
    /// eat the ')'
    getNextToken();
    return new CallExprAST(IdName, Args);
}

// Primary
static ExprAST *ParsePrimary() {
    switch (CurTok)
    {
        default: return Error("unknow token when expecting an expression");
        case tok_identifier: return ParseIdentifierExpr();
        case tok_number:     return ParseNumberExpr();
        case '(':            return ParseParenExpr();
    }
}

/// Binary op
static std::map<char, int> BinopPrecedence;

static int GetTokPrecedence() {
    if(!isascii(CurTok)) {
        return -1;
    }
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0) {
        return -1;
    }
    return TokPrec;
}

/// expressions
static ExprAST *ParseExpression() {
    ExprAST *LHS = ParsePrimary();
    if (!LHS) {
        return 0;
    }
    return ParseBinOpRHS(0, LHS);
}

/// binary op rhs
static ExprAST *ParseBinOpRHS(int ExprPrec, ExprAST *LHS) {
    while(1){
        int TokPrec = GetTokPrecedence();

        // If
        if(TokPrec < ExprPrec)
            return LHS;
    }
    
}

/// prototype
///   ::= id '(' id* ')'
static PrototypeAST *ParsePrototype()
{
    if (CurTok != tok_identifier)
        return ErrorP("Expected function name in prototype");

    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != '(')
        return ErrorP("Expected '(' in prototype");

    // Read the list of argument names.
    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);
    if (CurTok != ')')
        return ErrorP("Expected ')' in prototype");

    // success.
    getNextToken(); // eat ')'.

    return new PrototypeAST(FnName, ArgNames);
}

/// definition ::= 'def' prototype expression
static FunctionAST *ParseDefinition()
{
    getNextToken(); // eat def.
    PrototypeAST *Proto = ParsePrototype();
    if (Proto == 0)
        return 0;

    if (ExprAST *E = ParseExpression())
        return new FunctionAST(Proto, E);
    return 0;
}

/// external - 'extern' prototype
static PrototypeAST * ParseExtern() {
    getNextToken();
    return ParsePrototype();
}

/// toplevelExpr - expressions
static PrototypeAST *ParseTopLevelExpr() {
    
    if (ExprAST *E = ParseExpression() ) {
        PrototypeAST *Proto = new PrototypeAST("", std::vector<std::string>());
        return new FunctionAST(Proto, E);
    }
    return 0;
}

//===----------------------------------------------------------------------===//
// Top-Level parsing
//===----------------------------------------------------------------------===//
static void HandleDefinition() {
    
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } 
    else
    {
        getNextToken();
    }
}

static void HandleExtern() {
    
    if (ParseExtern()) {
        fprintf(stderr, "Parsed an extern;\n");
    } 
    else
    {
        getNextToken();
    }
}

static void HandleTopLevelExpression() {
    
    if (ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr.\n");
    } 
    else
    {
        getNextToken();
    }
}

/// top ::= definition | external | expression | ';'
static void MainLoop()
{
    while (1)
    {
        fprintf(stderr, "ready> ");
        switch (CurTok)
        {
        case tok_eof:
            return;
        case ';':
            getNextToken();
            break; // ignore top-level semicolons.
        case tok_def:
            HandleDefinition();
            break;
        case tok_extern:
            HandleExtern();
            break;
        default:
            HandleTopLevelExpression();
            break;
        }
    }
}

//===----------------------------------------------------------------------===//
// Main Function
//===----------------------------------------------------------------------===//
int main() {
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40; // highest.

    fprintf(stderr, "ready> ");
    getNextToken();
    /// Let's go~~~~~~~~~~~
    MainLoop();
    return 0;
}