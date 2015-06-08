#include "CGenTreeWalker.h"
#include <iostream>

namespace F4Helen {

    CGenTreeWalker::CGenTreeWalker() {
        //ctor
    }

    CGenTreeWalker::~CGenTreeWalker() {
        //dtor
    }

    int CGenTreeWalker::codegen(AST *ast, std::ostream &f) {
        if (!ast) return 0;
        std::string _end = _gen_with_semi ? ";" : "";
        if (_gen_with_endline) {
            _gen_write_indent(f);
        }
        if (ast->type == "ROOT") {
            f << std::string("#include <vector>\n#include <string>\n#include <iostream>\n");
            _gen_indent = 1;
            _gen_main = "int main()\n{\n";
            _gen_cur_fun = "";
            _gen_to_main = 1;
            _gen_with_semi = _gen_with_endline = 1;
            for (auto v : ast->children) {
                //std::cout << (v ? v->type : "<NULL>") << std::endl;
                codegen(v, f);
            }
            _gen_main += "}\n";
            f << _gen_main;
            return 0;
        }
        if (ast->type == "SEQ") {
            for (auto v : ast->children) {
                codegen(v, f);
            }
            return 0;
        }
        if (ast->type == "NEW") {
            _gen_write(_gen_c_types[ast->children[0]->value] + " ", f);
            _gen_write(ast->children[1]->value + " = ", f);
            _gen_write(_gen_expr(ast->children[2]), f);
            _gen_write(_end, f);
            _gen_varmap[ast->children[0]->value] = ast->children[1]->value;
        }

        if (ast->type == "NEWARR") {
            _gen_write("std::vector<" + _gen_c_types[ast->children[0]->value] + "> " +
                       ast->children[1]->value + "(" + _gen_expr(ast->children[2]) + ")", f);
            _gen_write(_end, f);
            _gen_varmap[ast->children[1]->value] = ast->children[0]->value;
        }
        if (ast->type == "MOV") {
            _gen_write(_gen_expr(ast->children[0]), f);
            if (ast->children[2])
                _gen_write("[" + _gen_expr(ast->children[2]) + " - 1]", f);
            _gen_write(" = " + _gen_expr(ast->children[1]) + _end, f);
        }
        if (ast->type == "OUT") {
            _gen_write("std::cout << " + _gen_expr(ast->children[0]) + " << std::endl" + _end, f);
        }
        if (ast->type == "IN") {
            if (ast->children[1])
                _gen_write("std::cin >> " + _gen_expr(ast->children[0]) +
                           "[" + _gen_expr(ast->children[1]) + " - 1]" + _end, f);
            else
                _gen_write("std::cin >> " + _gen_expr(ast->children[0]) + _end, f);
        }
        if (ast->type == "IF") {
            _gen_write("if (" + _gen_expr(ast->children[0]) + ")\n", f);
            _gen_write("{\n", f);
            codegen(ast->children[1], f);
            _gen_write("}\n", f);
            if (ast->children[2]) {
                _gen_write("else\n{\n", f);
                codegen(ast->children[2], f);
                _gen_write("}\n", f);
            }
        }
        if (ast->type == "LOOP") {
            _gen_write("for (", f);
            auto old_indent = _gen_indent;
            _gen_indent = 0;
            _gen_with_endline = 0;
            codegen(ast->children[0], f);
            _gen_write(" ", f);
            _gen_indent = old_indent;
            _gen_write(_gen_expr(ast->children[1]) + _end + " ", f);
            _gen_with_semi = 0;
            codegen(ast->children[2], f);
            _gen_with_endline = 1;
            _gen_with_semi = 1;
            _gen_write(")\n", f);
            _gen_write("{\n", f);
            codegen(ast->children[3], f);
            _gen_write("}", f);
        }
        if (ast->type == "FUN") {
            //_gen_funmap[ast->children[0]->value] = ast->children[2];
            // TODO Function
        }
        if (ast->type == "DECL") {
            //_gen_funmap[ast->children[0]->value] = ast->children[2];
            // TODO Extern
        }
        if (ast->type == "RSZ") {
            _gen_write(_gen_expr(ast->children[0]) + "->resize(" +
                       _gen_expr(ast->children[1]) + ")" + _end, f);
        }
        if (ast->type == "RET") {
            _gen_write("return " + _gen_expr(ast->children[0]) + _end, f);
        }
        if (ast->type == "EXPR") {
            _gen_write(_gen_expr(ast->children[0]) + _end, f);
        }
        if (_gen_with_endline) _gen_write("\n", f);
        return 0;
    }

    void CGenTreeWalker::_gen_write(std::string data, std::ostream &f) {
        if (_gen_to_main)
            _gen_main += data;
        else
            f << data;
    }

    std::string CGenTreeWalker::_gen_expr(AST *ast) {
        std::string res = "";
        if (!ast) return res;
        auto op = ast->type;
        if (op == "ADD") return "(" + _gen_expr(ast->children[0]) + " + " + _gen_expr(ast->children[1]) + ")";
        if (op == "SUB") return "(" + _gen_expr(ast->children[0]) + " - " + _gen_expr(ast->children[1]) + ")";
        if (op == "MUL") return "(" + _gen_expr(ast->children[0]) + " * " + _gen_expr(ast->children[1]) + ")";
        if (op == "DIV") return "(" + _gen_expr(ast->children[0]) + " / " + _gen_expr(ast->children[1]) + ")";
        if (op == "EQL") return "(" + _gen_expr(ast->children[0]) + " == " + _gen_expr(ast->children[1]) + ")";
        if (op == "LEQ") return "(" + _gen_expr(ast->children[0]) + " <= " + _gen_expr(ast->children[1]) + ")";
        if (op == "GEQ") return "(" + _gen_expr(ast->children[0]) + " >= " + _gen_expr(ast->children[1]) + ")";
        if (op == "LES") return "(" + _gen_expr(ast->children[0]) + " < " + _gen_expr(ast->children[1]) + ")";
        if (op == "GTR") return "(" + _gen_expr(ast->children[0]) + " > " + _gen_expr(ast->children[1]) + ")";
        if (op == "IND" || op == "ELEM")
            return "((std::vector<" + _gen_c_types[_gen_get_element_type((_gen_varmap[ast->children[0]->value]))]
                   + ">)(" + ast->children[0]->value + ")[" + _gen_expr(ast->children[1]) + "- 1]";
        if (op == "NEG") return "-" + _gen_expr(ast->children[0]) + ")";
        if (op == "INT" || op == "REAL" || op == "CHAR" || op == "STR")
            return "(" + _gen_c_types[op] + ")(" + _gen_literal(op, ast->value) + ")";
        if (op == "ID") return ast->value;
        if (op == "SIZE") return _gen_expr(ast->children[0]) + "->size()";
        if (op == "CALL") {
            res = _gen_expr(ast->children[0]) + "(";
            if (ast->children[1])
                for (int i = 0; i < ast->children[1]->children.size(); i++) {
                    res += _gen_expr(ast->children[1]->children[i]) +
                           (i == ast->children[1]->children.size() - 1 ? "" : ", ");
                }
            res += ")";
            return res;
        }
        return res;
    }

    std::string CGenTreeWalker::_gen_infer_type(AST *ast) {
        auto op = ast->type;
        if (op == "ADD" || op == "SUB" || op == "MUL")
            return _gen_get_widest_type(_gen_infer_type(ast->children[0]),
                                        _gen_infer_type(ast->children[1]));
        if (op == "DIV")
            return "REAL"; // TODO: fix for non-int types
        if (op == "EQL" || op == "LEQ" || op == "GEQ" || op == "LES" || op == "GTR")
            return "INT";
        if (op == "IND" || op == "ELEM")
            return _gen_get_element_type(_gen_varmap[ast->children[0]->value]);
        if (op == "NEG")
            return _gen_infer_type(ast->children[0]);
        if (op == "INT" || op == "REAL" || op == "CHAR" || op == "STRING")
            return op;
        if (op == "ID")
            return _gen_varmap[ast->value];
        if (op == "CALL")
            return _gen_varmap[ast->children[0]->value];
    }

    std::string CGenTreeWalker::_gen_literal(std::string type, std::string value) {
        if (type == "CHAR") return "'" + value + "'";
        if (type == "STR" || type == "STRING") return "\"" + value + "\"";
        return value;
    }


} // namespace F4Helen
