#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef int Elem_t;

struct Node_t
{
    Elem_t key;
    int prior;

    size_t childrenCount; 

    struct Node_t *left;
    struct Node_t *right;
};


struct Treap_t {
    struct Node_t *root;
};

//========TREAP FUNCTIONS===========
struct Treap_t *treapCtor();
struct Node_t  *nodeCtor (Elem_t key, int prior, struct Node_t *left, struct Node_t *right);
void   nodeDtor (struct Node_t  *node);
void   treapDtor(struct Treap_t *treap);

struct Node_t *_merge(struct Node_t *left, struct Node_t *right);
void   _split(Elem_t key, struct Node_t *current, struct Node_t **outLeft, struct Node_t **outRight);
struct Node_t *_insert(struct Node_t *node, Elem_t key, int priority);
void   insert (struct Treap_t *treap, Elem_t key);

void graphNode(struct Node_t *node, FILE *tempFile);
void drawConnections(struct Node_t *node, FILE *tempFile);
void graphDump(struct Node_t *node, int id);

//=========CONTEST STAFF=============


#define INPUT_CHECK(readFunc, res) {        \
    int factRes = readFunc;                  \
    if (factRes != res) abort();              \
}                                              \


#define ON_ERROR(expr, errStr, retVal) {                                  \
    if (expr) {                                                            \
        fprintf(stderr, "FATAL ERROR: %s in %d\n", errStr, __LINE__);       \
        return retVal;                                                       \
    }                                                                         \
}                                                                              \
//=========================================== 

struct Treap_t *treapCtor() {
    struct Treap_t *treap = (struct Treap_t *) calloc(1, sizeof(struct Treap_t));
    ON_ERROR(!treap, "Unable to alloc mem", NULL);

    treap->root = NULL;

    return treap;
}

struct Node_t  *nodeCtor (Elem_t key, int prior, struct Node_t *left, struct Node_t *right) {
    struct Node_t *node = (struct Node_t *) calloc(1, sizeof(struct Node_t));
    ON_ERROR(!node, "Unable to alloc mem", NULL);

    node->key           = key;
    node->prior         = prior;
    node->childrenCount = 0;

    node->left  = left;
    node->right = right;

    return node;
}

void nodeDtor (struct Node_t *node) {
    if(!node) return;

    if (node->left)  nodeDtor(node->left);
    if (node->right) nodeDtor(node->right);

    free(node);
}

void treapDtor(struct Treap_t *treap) {
    ON_ERROR(!treap, "Nullptr",);

    nodeDtor(treap->root);
    free(treap);
}

struct Node_t *_merge(struct Node_t *left, struct Node_t *right) {
    if (!left)  return right;
    if (!right) return left;

    if (left->prior > right->prior) {
        left->right = _merge(left->right, right);
        return left;
    }

    // else
    right->left = _merge(left, right->left);
    return right;
}

void _split(Elem_t key, struct Node_t *current, struct Node_t **outLeft, struct Node_t **outRight) {
    struct Node_t *newNode = NULL;

    if (key >= current->key) {

        if (!(current->right)) *outRight = NULL;
        else _split(key, current->right, &newNode, outRight);

        current->right = newNode;
        *outLeft = current;
    } else {
        if (!(current->left)) *outLeft = NULL;
        else _split(key, current->left, outLeft, &newNode);

        current->left = newNode;
        *outRight = current;
    }
}

int count = 100;

struct Node_t *_insert(struct Node_t *node, Elem_t key, int priority) {
    if (!node) return nodeCtor(key, priority, NULL, NULL);

    if (node->prior >= priority) {
        if (key < node->key) {
            node->left  = _insert(node->left, key, priority);
        } else {
            node->right = _insert(node->right, key, priority);
        }

        // graphDump(node, count++);
        return node;
    }

    struct Node_t *outLeft  = NULL;
    struct Node_t *outRight = NULL;
    _split(key, node, &outLeft, &outRight);

    // printf("%p %p\n", outLeft, outRight);

    // graphDump(outLeft, count++);
    // graphDump(outRight, count++);

    return nodeCtor(key, priority, outLeft, outRight);
}

void insert(struct Treap_t *treap, Elem_t key) {
    ON_ERROR(!treap, "Nullptr", );

    treap->root = _insert(treap->root, key, rand() % 100);
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
                " | {left: %p | right: %p | cur: %p} }}\"];\n", 
                node->left,
                node->right,
                node
            );

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
    ON_ERROR(!node, "Nullptr", );

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

void inorder(struct Node_t* root) { 
    if (root != NULL) { 
        inorder(root->left); 
        printf("key: %d priority: %d\n", root->key, root->prior); 
        inorder(root->right); 
    } 
} 

int main() {
    srand((unsigned int) time(NULL));

    struct Treap_t *treap = treapCtor();

    // treap->root = nodeCtor(2, rand() % 100, NULL, NULL);

    insert(treap, 1);
    // inorder(treap->root);
    // graphDump(treap->root, 0);
    insert(treap, 90);
    // graphDump(treap->root, 1);
    insert(treap, 17);
    // graphDump(treap->root, 2);
    insert(treap, 18);
    insert(treap, -10);
    insert(treap, 19);
    insert(treap, 45);
    insert(treap, 8);
    graphDump(treap->root, 3);

    inorder(treap->root);

    treapDtor(treap);

    return 0;
}