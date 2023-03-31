#include <stdio.h>
#include <stdlib.h>

typedef int Elem_t;

struct Node_t {
    Elem_t key;
    int height;
    
    struct Node_t* left;
    struct Node_t* right;
};

struct AVL_t {
    struct Node_t* root;
};

//==========AVL_TREE_METHODS============
struct AVL_t  *treeCtor();
struct Node_t *nodeCtor(Elem_t key);
void           nodeDtor(struct Node_t*root);
void           treeDtor(struct AVL_t *tree);

void _recountHeight(struct Node_t *node);
int  _balanceFactor(struct Node_t *node);

struct Node_t *balance(struct Node_t *node);
struct Node_t *rotateLeft(struct Node_t *node);
struct Node_t *rotateRight(struct Node_t *node);

struct Node_t *_addKey(struct Node_t *node, Elem_t key);
void            addKey(struct AVL_t *tree, Elem_t key);
struct Node_t *getNext(struct Node_t *node, Elem_t cmpEl);

void graphNode(struct Node_t *node, FILE *tempFile);
void drawConnections(struct Node_t *node);
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

struct AVL_t  *treeCtor() {
    struct AVL_t *tree = (struct AVL_t*) calloc(1, sizeof(struct AVL_t));
    ON_ERROR(!tree, "Unable to alloc mem", NULL);

    tree->root = NULL;

    return tree;
}

struct Node_t *nodeCtor(Elem_t key) {
    struct Node_t *leaf = (struct Node_t*) calloc(1, sizeof(struct Node_t));
    ON_ERROR(!leaf, "Unable to alloc mem", NULL);

    leaf->height = 0;
    leaf->key    = key;
    leaf->left   = NULL;
    leaf->right  = NULL;

    return leaf;
}

void nodeDtor(struct Node_t *root) {
    ON_ERROR(!root, "Nullptr",);

    if (root->left)  nodeDtor(root->left);
    if (root->right) nodeDtor(root->right);

    free(root);
}

void treeDtor(struct AVL_t *tree) {
    ON_ERROR(!tree, "Nullptr",);

    nodeDtor(tree->root);

    free(tree);
}

void _recountHeight(struct Node_t *node) {
    ON_ERROR(!node, "Nullptr",);

    if (!(node->left)) {
        if (node->right) node->height = node->right->height + 1;
        else node->height = 0;
        return;
    }

    if (!(node->right)) {
        if (node->left) node->height = node->left->height + 1;
        else node->height = 0;
        return;
    }

    node->height = (node->left->height <= node->right->height ? 
                    (node->right->height + 1) : (node->left->height + 1));
}

int _balanceFactor(struct Node_t *node) {
    ON_ERROR(!node, "Nullptr", -100);

    if (!(node->left) || !(node->right)) return 0;

    return node->left->height - node->right->height;
}

struct Node_t *balance(struct Node_t *node) {
    ON_ERROR(!node, "Nullptr", NULL);

    _recountHeight(node);

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

struct Node_t *_addKey(struct Node_t *node, Elem_t key) {
    // leaf case
    if (!node) {
        struct Node_t *newNode = nodeCtor(key);

        return newNode;
    }

    // (node->height)++;
    if (key < node->key) {
        node->left  = _addKey(node->left,  key);
    }
    else if (key == node->key) {
        return node;
    }
    else {
        node->right = _addKey(node->right, key);
    }

    return balance(node);
}

void addKey (struct AVL_t *tree, Elem_t key) {
    ON_ERROR(!tree, "Nullptr",);

    tree->root = _addKey(tree->root, key);
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

void drawConnections(struct Node_t *node, FILE *tempFile) {
    if (!node) return;

    if (node->left) {
        fprintf(tempFile, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", node, node->left);
    }
    if (node->right) {
        fprintf(tempFile, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", node, node->right);
    }

    if (node->left)  drawConnections(node->left, tempFile);
    if (node->right) drawConnections(node->right, tempFile);
}

void graphDump(struct Node_t *node) {
    if (!node) return;

    FILE *tempFile = fopen("temp.dot", "w");
    fprintf(tempFile, "digraph tree {\n");
    fprintf(tempFile, "\trankdir=HR;\n");
    if (!tempFile) return;

    graphNode(node, tempFile);
    drawConnections(node, tempFile);

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
    // controller();
    struct AVL_t *tree = treeCtor();

    addKey(tree, 17);
    addKey(tree, 16);
    addKey(tree, 18);
    addKey(tree, 14);
    addKey(tree, 20);
    addKey(tree, 100);
    addKey(tree, 19);
    addKey(tree, 15);
    addKey(tree, 13);
    addKey(tree, 200);
    addKey(tree, 90);
    addKey(tree, 200);

    graphDump(tree->root);

    treeDtor(tree);

    return 0;
}