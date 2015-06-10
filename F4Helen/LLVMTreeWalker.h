#ifndef F4HELEN_LLVMTREEWALKER_H
#define F4HELEN_LLVMTREEWALKER_H

#include "TreeWalker.h"
#include <LLVM/IR/Verifier.h>
#include <LLVM/IR/DerivedTypes.h>
#include <LLVM/IR/IRBuilder.h>
#include <LLVM/IR/LLVMContext.h>
#include <LLVM/IR/Module.h>


namespace F4Helen {

    using namespace llvm;

    class LLVMTreeWalker : public TreeWalker {
    public:
        virtual int codegen(AST *ast, std::ostream &f);

        LLVMTreeWalker(std::string module);

        virtual ~LLVMTreeWalker();

    protected:
    private:
        AST *_error(const char *str) {
            fprintf(stderr, "Error: %s\n", str);
            return 0;
        }

        Value *_error_v(const char *str) {
            _error(str);
            return 0;
        }

        Value *_expr(AST* ast);

        Module *_module;
        IRBuilder<> *_builder;
        std::map<std::string, Value *> _value_map;
    };

} // namespace F4Helen

#endif // F4HELEN_LLVMTREEWALKER_H
