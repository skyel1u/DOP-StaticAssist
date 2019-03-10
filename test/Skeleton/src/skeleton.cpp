#include <skeleton.h>
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Debug.h"

namespace skeleton {
    char examplePass::ID = 0;
    bool examplePass::runOnModule(llvm::Module& M) {
        for(auto& F : M) {
            if (F.isDeclaration()) {
                continue;
            }
            for(auto& B : F) {
                for(auto& I : B) {
                    llvm::errs() << "Instructions: " << I <<" ;";
                }
            }
            
        }
    }
    static llvm::RegisterPass<examplePass> X("example-skeleton", "tese");
}