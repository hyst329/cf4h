#include "LLVMTreeWalker.h"

namespace F4Helen {

    using namespace llvm;

    LLVMTreeWalker::LLVMTreeWalker() {
        _builder = new IRBuilder<>(getGlobalContext());
    }

    LLVMTreeWalker::~LLVMTreeWalker() {
        delete _builder;
    }

} // namespace F4Helen
