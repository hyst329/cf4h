#include "LLVMTreeWalker.h"

namespace F4Helen {

    using namespace llvm;

    LLVMTreeWalker::LLVMTreeWalker(std::string module) {
        _builder = new IRBuilder<>(getGlobalContext());
        _module = new Module(module, getGlobalContext());
    }

    LLVMTreeWalker::~LLVMTreeWalker() {
        delete _builder;
    }

    int LLVMTreeWalker::codegen(AST *ast, std::ostream &f) {
        if (!ast) return 0;
        f << ast->type << std::endl;
        if (ast->type == "ROOT") {
            for (auto v : ast->children) {
                codegen(v, f);
            }
            _module->dump();
            return 0;
        }
        if (ast->type == "SEQ") {
            for (auto v : ast->children) {
                codegen(v, f);
            }
            return 0;
        }
        if (ast->type == "NEW") {
            f << "Created variable " << ast->children[1]->value << " of type " << ast->children[0]->value << "\n";
            _value_map[ast->children[1]->value] = _expr(ast->children[2]);
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
        // Arithmetic operators
        if (op == "ADD" || op == "SUB" || op == "MUL" || op == "DIV") {
            Value *op1 = _expr(ast->children[0]), *op2 = _expr(ast->children[1]);
            Type *op1t = op1->getType(), *op2t = op2->getType();
            if (op1t->isDoubleTy() && op2t->isDoubleTy()) {
                if (op == "ADD") return _builder->CreateFAdd(op1, op2, "addtmp");
                if (op == "SUB") return _builder->CreateFSub(op1, op2, "subtmp");
                if (op == "MUL") return _builder->CreateFMul(op1, op2, "multmp");
                if (op == "DIV") return _builder->CreateFDiv(op1, op2, "divtmp");
            }
            else if (op1t->isIntegerTy() && op2t->isIntegerTy()) {
                if (op == "ADD") return _builder->CreateAdd(op1, op2, "addtmp");
                if (op == "SUB") return _builder->CreateSub(op1, op2, "subtmp");
                if (op == "MUL") return _builder->CreateMul(op1, op2, "multmp");
                if (op == "DIV") return _builder->CreateFDiv(op1, op2, "divtmp");
            }
            else return _error_v("Type mismatch");
        }
        // Comparison operators
        if (op == "EQL" || op == "LEQ" || op == "GEQ" || op == "LES" || op == "GTR") {

        }
        if (op == "INT") return ConstantInt::get(getGlobalContext(), APInt(64, strtoull(ast->value.c_str(), 0, 10), 1));
        if (op == "REAL") return ConstantFP::get(getGlobalContext(), APFloat(strtof(ast->value.c_str(), 0)));
        // TODO Char
        // TODO String
    }
} // namespace F4Helen
