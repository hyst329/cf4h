#include "AST.h"
#include <cstdarg>

namespace F4Helen {

    AST::AST(std::string type, int childCount, ...) : type(type), children(childCount) {
        va_list va;
        va_start(va, childCount);
        for (int i = 0; i < childCount; i++) {
            children[i] = (va_arg(va, AST *));
        }
        va_end(va);
    }

    AST::AST(std::string type, std::string value) : type(type), value(value) {
        children = std::vector<AST *>();
    }

} // namespace F4Helen
