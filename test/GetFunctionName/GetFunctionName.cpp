#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace
{
struct Test : public FunctionPass
{
    static char ID;

    Test() : FunctionPass(ID) { ; }

    virtual bool runOnFunction(Function &function)
    {
        errs() << "Function name:";
        errs().write_escaped(function.getName()) << "\n";
        return false;
    }
};

}

char Test::ID = 0;
static RegisterPass<Test> X("function-name", "FunctionName.", false, false);
