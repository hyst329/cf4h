//
// Created by tram on 11.06.2015.
//

#include "LLVMExecutionEngine.h"

namespace F4Helen {

    LLVMExecutionEngine::LLVMExecutionEngine(Module *module) : module(module) {
        ee = EngineBuilder(std::unique_ptr<Module>(this->module)).create();
    }

    int LLVMExecutionEngine::executeCode() {
        if (!module) printf("DEBUG: no module\n");
        Function* f = module->getFunction("__main");
        if (!f) printf("DEBUG: no main\n");
        void *main_ptr = ee->getPointerToFunction(f);
        int (*main_fun)() = (int(*)())(main_ptr);
        return main_fun();
    }
}