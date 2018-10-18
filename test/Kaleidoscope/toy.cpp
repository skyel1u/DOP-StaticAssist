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

//===----------------------------
// Main function
//===----------------------------
int main() {
    return 0;
}