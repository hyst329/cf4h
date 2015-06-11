//
// Created by tram on 11.06.2015.
//

#ifndef CF4H_LLVMEXECUTIONENGINE_H
#define CF4H_LLVMEXECUTIONENGINE_H

#include <LLVM/IR/Module.h>
#include <LLVM/ExecutionEngine/ExecutionEngine.h>

using namespace llvm;
namespace F4Helen {
    class LLVMExecutionEngine {

    public:
        LLVMExecutionEngine(Module *module);
        // Execute the '__main' function, returning the result
        int executeCode();

    private:
        ExecutionEngine* ee;
        Module *module;
    };
}

#endif //CF4H_LLVMEXECUTIONENGINE_H
