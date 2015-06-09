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
        if (!ast) return 0;
        if (ast->type == "ROOT") {

        }
        if (ast->type == "SEQ") {
            for (auto v : ast->children) {
                codegen(v, f);
            }
            return 0;
        }
        if (ast->type == "NEW") {

        }
        if (ast->type == "NEWARR") {

        }
        if (ast->type == "MOV") {

        }
        if (ast->type == "OUT") {

        }
        if (ast->type == "IN") {

        }
        if (ast->type == "IF") {

        }
        if (ast->type == "LOOP") {

        }
        if (ast->type == "FUN") {

        }
        if (ast->type == "DECL") {

        }
        if (ast->type == "RSZ") {

        }
        if (ast->type == "RET") {

        }
        if (ast->type == "EXPR") {
            _expr(ast);
        }
        return 0;
    }

    Value *LLVMTreeWalker::_expr(AST *ast) {
        if (!ast) return nullptr;
        auto op = ast->type;
        if (op == "ID") {
            Value *v = _value_map[ast->value];
            return v ?: _error_v("No such identifier");
        }
        if (op == "ADD" || op == "SUB" || op == "MUL" || op == "DIV") {
            Value *op1 = _expr(ast->children[0]), *op2 = _expr(ast->children[1]);
            if (op == "ADD") return _builder->CreateFAdd(op1, op2, "addtmp");
            if (op == "SUB") return _builder->CreateFSub(op1, op2, "subtmp");
            if (op == "MUL") return _builder->CreateFMul(op1, op2, "multmp");
            if (op == "DIV") return _builder->CreateFDiv(op1, op2, "divtmp");
        }
        if (op == "INT") return ConstantInt::get(getGlobalContext(), APInt(64, strtoull(ast->value.c_str(), 0, 10), 1));
        if (op == "REAL") return ConstantFP::get(getGlobalContext(), APFloat(strtof(ast->value.c_str(), 0)));
        // TODO Char
        // TODO String
    }
} // namespace F4Helen
