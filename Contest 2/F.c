#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>

typedef long long int ll;
typedef ll Elem_t;

const int MAX_COM_LENGTH = 5;

const char ADD_COM[] = "+";
const char SUM_COM[] = "?";

#define true 1;
#define false 0;

const Elem_t DEFAULT_NONE = INT_MAX;
const Elem_t MIN_VALUE    = INT_MIN;

struct Node_t
{
    Elem_t key;
    int prior;

    ll treeSum; 
    ll treeMin;
    ll treeMax;

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

ll     getNodeMin(struct Node_t *node);
ll     getNodeMax(struct Node_t *node);
ll     getNodeSum(struct Node_t *node);
void   updNodeSum(struct Node_t *node);

struct Node_t *_merge(struct Node_t *left, struct Node_t *right);
void   _split(Elem_t key, struct Node_t *current, struct Node_t **outLeft, struct Node_t **outRight);

struct Node_t *_insert(struct Node_t *node, Elem_t key, int priority);
void   insert (struct Treap_t *treap, Elem_t key);

int _exists(struct Node_t  *node,  Elem_t key);
int  exists(struct Treap_t *treap, Elem_t key);

Elem_t _next(struct Node_t  *node,  Elem_t cmpEl);
Elem_t  next(struct Treap_t *treap, Elem_t cmpEl);

Elem_t _prev(struct Node_t  *node,  Elem_t cmpEl);
Elem_t  prev(struct Treap_t *treap, Elem_t cmpEl);

// DEBUG
void graphNode(struct Node_t *node, FILE *tempFile);
void drawConnections(struct Node_t *node, FILE *tempFile);
void graphDump(struct Node_t *node, int id);
void printInOrder(struct Node_t* node);

#define MIN(val1, val2) val1 < val2 ? val1 : val2

#define MAX(val1, val2) val1 < val2 ? val2 : val1

//=========CONTEST STAFF=============
ll sumLR(struct Node_t *node, int l, int r);
void parser();

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

    node->left  = left;
    node->right = right;

    updNodeSum(node);

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

// MERGE & SPLIT

ll getNodeMin(struct Node_t *node) {
    return (node == NULL) ? DEFAULT_NONE : node->treeMin;
}

ll getNodeMax(struct Node_t *node) {
    return (node == NULL) ? MIN_VALUE    : node->treeMax;
}

ll getNodeSum(struct Node_t *node) {
    return (node == NULL) ? 0 : node->treeSum;
}

void updNodeSum(struct Node_t *node) {
    if (!node) return;

    node->treeSum = getNodeSum(node->left) + getNodeSum(node->right) + node->key;

    ll newMin = MIN(getNodeMin(node->left), getNodeMin(node->right));
    node->treeMin = node->key < newMin ? node->key : newMin;

    ll newMax = MAX(getNodeMax(node->left), getNodeMax(node->right));
    node->treeMax = node->key < newMax ? newMax : node->key;
}

struct Node_t *_merge(struct Node_t *left, struct Node_t *right) {
    if (!left)  return right;
    if (!right) return left;

    if (left->prior > right->prior) {
        left->right = _merge(left->right, right);

        updNodeSum(left);
        return left;
    }

    // else
    right->left = _merge(left, right->left);

    updNodeSum(right);
    return right;
}

void _split(Elem_t key, struct Node_t *current, struct Node_t **outLeft, struct Node_t **outRight) {
    struct Node_t *newNode = NULL;

    if (key >= current->key) {

        if (!(current->right)) *outRight = NULL;
        else _split(key, current->right, &newNode, outRight);

        current->right = newNode;
        *outLeft = current;

        updNodeSum(current);
    } else {
        if (!(current->left)) *outLeft = NULL;
        else _split(key, current->left, outLeft, &newNode);

        current->left = newNode;
        *outRight = current;

        updNodeSum(current);
    }
}

// INSERT SECTION

struct Node_t *_insert(struct Node_t *node, Elem_t key, int priority) {
    if (!node) return nodeCtor(key, priority, NULL, NULL);

    if (node->prior >= priority) {
        if (key < node->key) {
            node->left  = _insert(node->left, key, priority);
        } else {
            node->right = _insert(node->right, key, priority);
        }

        updNodeSum(node);

        return node;
    }

    struct Node_t *outLeft  = NULL;
    struct Node_t *outRight = NULL;

    _split(key, node, &outLeft, &outRight);

    return nodeCtor(key, priority, outLeft, outRight);
}

void insert(struct Treap_t *treap, Elem_t key) {
    ON_ERROR(!treap, "Nullptr", );

    if (exists(treap, key)) return;

    treap->root = _insert(treap->root, key, rand());
}

// EXISTS SECTION
int _exists(struct Node_t *node,  Elem_t key) {
    struct Node_t *cur = node;

    while (cur)
    {
        if (cur->key == key) return true;

        if (key < cur->key) cur = cur->left;
        else                cur = cur->right;
    }
    

    return false;
}

int exists(struct Treap_t *treap, Elem_t key) {
    ON_ERROR(!treap, "Nullptr", -1);

    return _exists(treap->root, key);
}

// NEXT SECTION

Elem_t _next(struct Node_t  *node,  Elem_t cmpEl) {
    struct Node_t *cur = node;
    struct Node_t *res = NULL;

    while (cur)
    {
        if(cur->key > cmpEl) {
            res = cur;
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }

    if (res) return res->key;
    
    return DEFAULT_NONE;
}

Elem_t next(struct Treap_t *treap, Elem_t cmpEl) {
    ON_ERROR(!treap, "Nullptr", DEFAULT_NONE);

    return _next(treap->root, cmpEl);
}

// PREV SECTION

Elem_t _prev(struct Node_t  *node,  Elem_t cmpEl) {
    struct Node_t *cur = node;
    struct Node_t *res = NULL;

    while (cur)
    {
        if(cur->key >= cmpEl) {
            cur = cur->left;
        } else {
            res = cur;
            cur = cur->right;
        }
    }

    if (res) return res->key;
    
    return DEFAULT_NONE;
}

Elem_t  prev(struct Treap_t *treap, Elem_t cmpEl) {
    ON_ERROR(!treap, "Nullptr", DEFAULT_NONE);

    return _prev(treap->root, cmpEl);
}

// GRAPH DUMPS

void graphNode(struct Node_t *node, FILE *tempFile) {
    fprintf(
                tempFile, 
                "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{ {val: %lld | prior: %d ",
                node,
                node->key,
                node->prior
            );  

    fprintf(
                tempFile, 
                " | {min: %lld | max: %lld | summa: %lld | cur: %p} }}\"];\n", 
                node->treeMin,
                node->treeMax,
                node->treeSum,
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

void printInOrder(struct Node_t* node) { 
    if (node) { 
        printInOrder(node->left); 
        printf("key: %lld priority: %d\n", node->key, node->prior); 
        printInOrder(node->right); 
    } 
} 

ll sumLR(struct Node_t *node, int l, int r) {
    if (!node) return 0;

    ll mini = node->treeMin;
    ll maxi = node->treeMax;

    // out of segment
    if (r < mini || l > maxi) return 0;

    // fully in segment
    if (l > mini && r < maxi) {
        if (l <= node->key && node->key <= r) {
            return node->key + sumLR(node->left, l, r) + sumLR(node->right, l, r);
        }

        return sumLR(node->left, l, r) + sumLR(node->right, l, r);
    } else if (l <= mini && r >= maxi) {
        return node->treeSum;
    } else {
        // partly inside segment
        if (l <= node->key && node->key <= r) {
            return node->key + sumLR(node->left, l, r) + sumLR(node->right, l, r);
        }

        return sumLR(node->left, l, r) + sumLR(node->right, l, r);
    }

    return 0;
}

void parser() {
    struct Treap_t *treap = treapCtor();

    int n = 0, questPrev = false;
    ll lastRes = 0;
    char readBuf[2];
    INPUT_CHECK(scanf("%d", &n), 1);

    for (int i = 0; i < n; i++) {
        INPUT_CHECK(scanf("%s", readBuf), 1);

        if (readBuf[0] == '+') {
            int addVal = 0;
            INPUT_CHECK(scanf("%d", &addVal), 1);

            if (questPrev) {
                insert(treap, (addVal + lastRes) % ((ll) 1e9));

                questPrev = false;
            }
            else insert(treap, addVal);
        }
        if (readBuf[0] == '?') {
            int l = 0, r = 0;
            INPUT_CHECK(scanf("%d %d", &l, &r), 2);

            ll result = sumLR(treap->root, l, r);
            printf("%lld\n", result);

            questPrev = true;
            lastRes = result;
        }
    }
    
}

int main() {
    srand((unsigned int) time(NULL));

    parser();

    // struct Treap_t *treap = treapCtor();

    // insert(treap, 15);
    // insert(treap, 16);
    // insert(treap, 90);
    // insert(treap, -10);

    // printf("%lld\n", sumLR(treap->root, 15, 91));

    // graphDump(treap->root, 0);

    return 0;
}