#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef int Elem_t;

int mode = 0;

struct Node_t {
    Elem_t key;
    Elem_t prior;
    
    struct Node_t* left;
    struct Node_t* right;
    struct Node_t* parent;
};

struct Treap_t {
    struct Node_t* root;

    int ableToBuild;
};

//==========TREAP_METHODS============
struct Treap_t *treeCtor();
struct Node_t *nodeCtor(Elem_t key, Elem_t prior, struct Node_t *prev);
void nodeDtor(struct Node_t *root);
void treeDtor(struct Treap_t *tree);

// USE ONLY IF KEYS SORTED!!!
struct Treap_t *bldTreeO_N(Elem_t *keys, Elem_t *priors, size_t size);
struct Node_t *_bldNodeO_N(struct Treap_t *tree, Elem_t *keys, Elem_t *priors, size_t n);

void graphNode(struct Node_t *node, FILE *tempFile);
void drawConnections(struct Node_t *node, FILE *tempFile);
void graphDump(struct Node_t *node, int id);

//=========CONTEST STAFF=============
void dfs(struct Node_t *node);


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

struct Treap_t *treeCtor() {
    struct Treap_t *tree = (struct Treap_t*) calloc(1, sizeof(struct Treap_t));
    ON_ERROR(!tree, "Unable to alloc mem", NULL);

    tree->root        = NULL;
    tree->ableToBuild = 1;

    return tree;
}

struct Node_t *nodeCtor(Elem_t key, Elem_t prior, struct Node_t *prev) {
    struct Node_t *leaf = (struct Node_t*) calloc(1, sizeof(struct Node_t));
    if(!leaf) return NULL;

    leaf->key    = key;
    leaf->prior  = prior;

    leaf->left   = NULL;
    leaf->right  = NULL;
    leaf->parent = prev;

    return leaf;
}

void nodeDtor(struct Node_t *root) {
    if(!root) return;

    if (root->left)  nodeDtor(root->left);
    if (root->right) nodeDtor(root->right);

    free(root);
}

void treeDtor(struct Treap_t *tree) {
    ON_ERROR(!tree, "Nullptr", );

    nodeDtor(tree->root);

    free(tree);
}

// USE ONLY IF KEYS SORTED!!!
struct Treap_t *bldTreeO_N(Elem_t *keys, Elem_t *priors, size_t size) {
    ON_ERROR(!keys || !priors || !size, "Nullptr", NULL);

    struct Treap_t *tree = treeCtor();
    ON_ERROR(!tree, "Nullptr", NULL);

    tree->root = nodeCtor(keys[0], priors[0], NULL);

    struct Node_t *prev = tree->root;
    for (size_t i = 1; i < size; i++) {
        if (priors[i] >= prev->prior) {
            prev->right = nodeCtor(keys[i], priors[i], prev);
            prev = prev->right;
        } else {
            struct Node_t *cur = prev;

            while (priors[i] < cur->prior && cur->parent) {
                cur = cur->parent;
            }

            if (priors[i] < cur->prior) {
                prev        = nodeCtor(keys[i], priors[i], NULL);  // new root node
                prev->left  = cur;
                cur->parent = prev;
            } else {
                prev        = nodeCtor(keys[i], priors[i], cur);
                prev->left  = cur->right;
                cur->right->parent = prev;

                cur->right  = prev;
            }
        }
    }

    while (tree->root->parent)
    {
        tree->root = tree->root->parent;
    }
    
    return tree;
}

void graphNode(struct Node_t *node, FILE *tempFile) {
    fprintf(
                tempFile, 
                "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{ {val: %d | prior: %d ",
                node,
                node->key,
                node->prior
            );  

    fprintf(
                tempFile, 
                " | {left: %p | right: %p | parent: %p | cur: %p} }}\"];\n", 
                node->left,
                node->right,
                node->parent,
                node
            );

    if (node->parent) {
        fprintf(tempFile, "\tlabel%p->label%p [color=\"blue\", style=\"dashed\",arrowhead=\"none\"]", node->parent, node);
    }

    if (node->left)  graphNode(node->left, tempFile);
    if (node->right) graphNode(node->right, tempFile);
}

void drawConnections(struct Node_t *node, FILE *tempFile) {
    if (!node) return;

    if (node->left) {
        fprintf(tempFile, "\tlabel%p->label%p [color=\"green\", style=\"dashed\",arrowhead=\"none\"]", node, node->left);
    }
    if (node->right) {
        fprintf(tempFile, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", node, node->right);
    }

    if (node->left)  drawConnections(node->left, tempFile);
    if (node->right) drawConnections(node->right, tempFile);
}

void graphDump(struct Node_t *node, int id) {
    if (!node) return;

    FILE *tempFile = fopen("temp.dot", "w");
    fprintf(tempFile, "digraph tree {\n");
    fprintf(tempFile, "\trankdir=HR;\n");
    if (!tempFile) return;

    graphNode(node, tempFile);
    drawConnections(node, tempFile);

    fprintf(tempFile, "}");
    fclose(tempFile);

    char command[2096] = "";
    sprintf(command, "dot -Tsvg temp.dot > graph_%d.png && xdg-open graph_%d.png", id, id);
    system(command);
}

void dfs(struct Node_t *node) {
    if (!node) return;

    if (node->left)  dfs(node->left);

    if (node->parent) printf("%d ", node->parent->key + 1);
    else              printf("%d ", 0);

    if (node->left)  printf("%d ", node->left->key + 1);
    else             printf("%d ",  0);

    if (node->right) printf("%d", node->right->key + 1);
    else             printf("%d",  0);

    printf("\n");

    if (node->right) dfs(node->right);
}

int main() {
    int n = 0;
    INPUT_CHECK(scanf("%d", &n), 1);

    int *keys = (int*) calloc(n, sizeof(int));
    int *prio = (int*) calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        int val1 = 0, val2 = 0;
        INPUT_CHECK(scanf("%d %d", &val1, &val2), 2);

        keys[i] = i;
        prio[i] = val2;
    }

    struct Treap_t *tree = bldTreeO_N(keys, prio, n);

    printf("YES\n");
    // graphDump(tree->root, 0);
    dfs(tree->root);

    treeDtor(tree);

    return 0;
}