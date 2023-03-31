#include <stdio.h>
#include <stdlib.h>

typedef Elem_t int;

struct Node_t {
    Elem_t key;
    int height;
    
    struct Node_t* left;
    struct Node_t* right;
};

//==========AVL_TREE_METHODS============
void _recountHeight(struct Node_t *node);
void _balanceFactor(struct Node_t *node);

struct Node_t *balance(struct Node_t *node);
struct Node_t *rotateLeft(struct Node_t *node);
struct Node_t *rotateRight(struct Node_t *node);

void           addKey (struct Node_t *node, Elem_t key);
struct Node_t *getNext(struct Node_t *node, Elem_t cmpEl);

void graphNode(struct Node_t *node, FILE *tempFile);
void graphDump(struct Node_t *node);

//=========CONTEST STAFF=============
void controller();


#define INPUT_CHECK(readFunc, res) {        \
    int factRes = readFunc;                  \
    if (factRes != res) abort();              \
}                                              \


#define ON_ERROR(expr, errStr, retVal) {                 \
    if (expr) {                                           \
        fprintf(stderr, "FATAL ERROR: %s\n", errStr);      \
        return retVal;                                      \
    }                                                        \
}                                                             \
//===========================================


void _recountHeight(struct Node_t *node) {
    ON_ERROR(!node, "Nullptr",);

    node->height = (node->left->height <= node->right->height ? 
                    node->right->height : node->left->height);
}

void _balanceFactor(struct Node_t *node) {
    ON_ERROR(!node, "Nullptr",);

    return _recountHeight(node->left) - _recountHeight(node->right);
}

struct Node_t *balance(struct Node_t *node) {
    ON_ERROR(!node, "Nullptr", NULL);

    if (_balanceFactor(node) == -2) {
        if (_balanceFactor(node->right) == 1) {
            node->right = rotateRight(node->left);
        }
        return rotateLeft(node);
    }

    if (_balanceFactor(node) == 2) {
        if (_balanceFactor(node->left) == -1) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }

    return node;
}

struct Node_t *rotateLeft(struct Node_t *node) {
    ON_ERROR(!node, "Nullptr", NULL);

    struct Node_t *newRoot = node   ->right;
    node   ->right         = newRoot->left;
    newRoot->left          = node;

    _recountHeight(node);
    _recountHeight(newRoot);

    return newRoot;
}

struct Node_t *rotateRight(struct Node_t *node) {
    ON_ERROR(!node, "Nullptr", NULL);

    struct Node_t *newRoot = node   ->left;
    node   ->left          = newRoot->right;
    newRoot->right         = node;

    _recountHeight(node);
    _recountHeight(newRoot);

    return newRoot;
}

void addKey(struct Node_t *node, Elem_t key) {
    ON_ERROR(!node, "Nullptr",);
}

struct Node_t *getNext(struct Node_t *node, Elem_t cmpEl) {
    ON_ERROR(!node, "Nullptr", NULL);
}

void graphNode(struct Node_t *node, FILE *tempFile) {
    fprintf(
                tempFile, 
                "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{ {val: %d | height: %d ",
                node,
                node->key,
                node->height
            );  

    fprintf(
                tempFile, 
                " | {left: %p | right: %p} }}\"];\n", 
                node->left,
                node->right
            );

    if (node->left)  graphNode(node->left, tempFile);
    if (node->right) graphNode(node->right, tempFile);
}

void graphDump(struct Node_t *node) {
    if (!node) return;

    FILE *tempFile = fopen("temp.dot", "w");
    fprintf(tempFile, "digraph tree {\n");
    fprintf(tempFile, "\trankdir=HR;\n");
    if (!tempFile) return;

    graphNode(node, tempFile);

    fprintf(tempFile, "}");
    fclose(tempFile);

    system("dot -Tsvg temp.dot > graph.png");
}

void controller() {
    int n = 0;
    INPUT_CHECK(scanf("%d", &n), 1);

    for (int i = 0; i < n; i++) {
        unsigned char oper = '\0';
        INPUT_CHECK(scanf("%c", &oper), 1);

        switch (oper)
        {
            case '+':
                {
                int value = 0;
                INPUT_CHECK(scanf("%d", &value), 1);
                }
                break;
            case '?':
                {
                int value = 0;
                INPUT_CHECK(scanf("%d", &value), 1);
                }
                break;
            default:
                break;
        }
    }
}

int main() {
    controller();

    return 0;
}