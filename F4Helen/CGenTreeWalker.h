#ifndef F4HELEN_CGENTREEWALKER_H
#define F4HELEN_CGENTREEWALKER_H

#include "TreeWalker.h"
#include <map>

namespace F4Helen
{

class CGenTreeWalker : public TreeWalker
{
public:
    CGenTreeWalker();
    virtual ~CGenTreeWalker();
    int codegen(AST *ast, std::ostream &f);
protected:
private:
    std::string _gen_main, _gen_cur_fun;
    bool _gen_with_semi, _gen_with_endline, _gen_to_main;
    int _gen_indent;
    std::map<std::string, std::string> _gen_varmap, _gen_funmap;
    std::map<std::string, std::string> _gen_c_types =
    { {"INT", "int"}, {"REAL", "double"}, {"CHAR", "char"}, {"STRING", "std::string"}, {"STR", "std::string"} };
    void _gen_write(std::string data, std::ostream &f);
    std::string _gen_expr(AST *ast);
    static std::string _gen_get_element_type(std::string array_type)
    {
        return array_type.substr(0, array_type.find("."));
    }
    static std::string _gen_get_widest_type(std::string type1, std::string type2)
    {
        if(type1 == type2) return type1;
        if(type1 == "INT" && type2 == "REAL") return "REAL";
        if(type1 == "REAL" && type2 == "INT") return "REAL";
    }
    std::string _gen_infer_type(AST *ast);
    static std::string _gen_literal(std::string type, std::string value);
    void _gen_write_indent(std::ostream& f)
    {
        for(int i = 0; i < _gen_indent * 4; i++)
            _gen_write(" ", f);
    }
};

} // namespace F4Helen

#endif // F4HELEN_CGENTREEWALKER_H
