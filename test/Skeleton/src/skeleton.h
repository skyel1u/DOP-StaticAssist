#include "llvm/Pass.h"
#include <algorithm>
#include <unordered_set>
#include <iostream>

namespace skeleton {
    class examplePass : public llvm::ModulePass {
        private:
            void tPrint(std::string message);
        public:
            static char ID;
            examplePass() : llvm::ModulePass(ID){

            }
            bool runOnModule(llvm::Module& M) override;
            void debugPrint(std::string message);
    };
}