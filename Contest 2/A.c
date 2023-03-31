#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef int Elem_t;

Elem_t DEFAULT_MAX = INT_MAX;

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
Elem_t          getNext(struct Node_t *node, Elem_t cmpEl);

void graphNode(struct Node_t *node, FILE *tempFile);
void drawConnections(struct Node_t *node, FILE *tempFile);
void graphDump(struct Node_t *node);

//=========CONTEST STAFF=============
void controller(struct AVL_t *tree);


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

Elem_t getNext(struct Node_t *node, Elem_t cmpEl) {
    if (!node) return DEFAULT_MAX;

    Elem_t min1 = DEFAULT_MAX;
    Elem_t min2 = DEFAULT_MAX;

    if (node->left)  min1 = getNext(node->left,  cmpEl);
    if (node->right) min2 = getNext(node->right, cmpEl);

    if (node->key >= cmpEl) {
        int mini = (min1 < min2 ? min1 : min2);
        return (mini < node->key ? mini : node->key);
    }

    return (min1 < min2 ? min1 : min2);
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

void controller(struct AVL_t *tree) {
    int n = 0, followedQuest = 0, result = 0;
    INPUT_CHECK(scanf("%d", &n), 1);

    for (int i = 0; i < n; i++) {
        char buffer[10] = "";
        INPUT_CHECK(scanf("%s", buffer), 1);

        switch (buffer[0])
        {
            case '+':
                {
                int value = 0;
                INPUT_CHECK(scanf("%d", &value), 1);

                if (followedQuest) {
                    value = (value + result) % 1000000000;
                    followedQuest = 0;
                }

                addKey(tree, value);
                }
                break;
            case '?':
                {
                int value = 0;
                INPUT_CHECK(scanf("%d", &value), 1);

                int res = getNext(tree->root, value);
                followedQuest = 1;

                if (res == DEFAULT_MAX) {
                    printf("%d\n", -1);
                    break;
                }
                else result = res;

                printf("%d\n", result);
                }
                break;
            default:
                break;
        }
    }
}

int main() {
    struct AVL_t *tree = treeCtor();

    controller(tree);

    // addKey(tree, 1);
    // addKey(tree, 3);
    // addKey(tree, 3);

    // int result = getNext(tree->root, 2);
    // if (result == DEFAULT_MAX) result = -1;
    // printf("%d\n", result);

    // addKey(tree, 1);

    // result = getNext(tree->root, 4);
    // if (result == DEFAULT_MAX) result = -1;
    // printf("%d\n", result);

    // graphDump(tree->root); 

    // treeDtor(tree);

    return 0;
}