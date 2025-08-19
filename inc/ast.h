#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAM,
    NODE_DECL,
    NODE_FUNC,
    NODE_STMT,
    NODE_EXPR
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char *value;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *next;
} ASTNode;

ASTNode* createNode(NodeType type, char *value);
void freeAST(ASTNode *node);

#endif
