#include "LLVMTreeWalker.h"

namespace F4Helen {

LLVMTreeWalker::LLVMTreeWalker() : _builder(getGlobalContext())
{
    //ctor
}

LLVMTreeWalker::~LLVMTreeWalker()
{
    //dtor
}

} // namespace F4Helen
