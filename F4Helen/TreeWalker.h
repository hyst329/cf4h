#ifndef F4HELEN_TREEWALKER_H
#define F4HELEN_TREEWALKER_H

#include "AST.h"

namespace F4Helen {

    class TreeWalker {
    public:
        TreeWalker();

        virtual ~TreeWalker();

        virtual int codegen(AST *ast, std::ostream &f) = 0;

    protected:
    private:
    };

} // namespace F4Helen

#endif // F4HELEN_TREEWALKER_H
