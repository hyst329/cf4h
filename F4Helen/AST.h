#ifndef F4HELEN_AST_H
#define F4HELEN_AST_H

#include <string>
#include <vector>

namespace F4Helen {

    struct AST {
        std::string type;
        std::string value; // only for literals
        std::vector<AST *> children;

        AST(std::string type, int childCount = 0, ...);

        AST(std::string type, std::string value);
    };


} // namespace F4Helen

#endif // F4HELEN_AST_H
