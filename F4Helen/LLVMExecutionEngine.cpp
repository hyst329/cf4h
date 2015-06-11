//
// Created by tram on 11.06.2015.
//

#include "LLVMExecutionEngine.h"

namespace F4Helen {

    LLVMExecutionEngine::LLVMExecutionEngine(Module *module) : module(module) {
        ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
    }

    int LLVMExecutionEngine::executeCode() {
        void *main_ptr = ee->getPointerToFunction(module->getFunction("__main"));
        int (*main_fun)() = (int(*)())(main_ptr);
        return main_fun();
    }
}