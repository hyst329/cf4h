#include "LLVMTreeWalker.h"

namespace F4Helen {

    using namespace llvm;

    LLVMTreeWalker::LLVMTreeWalker() {
        _builder = new IRBuilder<>(getGlobalContext());
    }

    LLVMTreeWalker::~LLVMTreeWalker() {
        delete _builder;
    }

    int LLVMTreeWalker::codegen(AST *ast, std::ostream &f) {
        return 0;
    }
} // namespace F4Helen
