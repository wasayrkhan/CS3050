// WASAY KHAN
// WRKGCH
// OCTOBER 29TH, 2021
// PROGRAMMING ASSIGNMENT 2 - RED AND BLACK TREE CONSTRUCTION

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    char color;
    struct node *left, *right, *parent;
} node;

//Function Prototypes
void leftRotate(struct node **, struct node *);
void rightRotate(struct node **, struct node *);
void insertFixUP(struct node **, struct node **);
void insert(struct node **, int data);
void inOrder(struct node *);
//int OSSelect(struct node *, struct node *, int data);
//int size(struct node *, struct node*);

int main(){
    struct node *root = NULL;

    int input; 
    while (input!= -1)
    {
        printf("Enter number(Greater than -1. Enter -1 to exit) ");
        scanf("%d", &input);
        insert(&root, input);
    }
    printf("Inorder Traversal: ");
    inOrder(root);

    //root = OSSelect(root, root->data, size(root, root->data)-1);

    printf("\n");
 
    return 0;
}

//Left Rotation
void leftRotate(struct node** root, struct node *x) {
    
    if(!x || !x->right){
        return;
    }
    // y stored pointer - right child of x
    struct node *y = x->right;
    //store y's left subtree's pointer as x's right child
    x->right = y->left;

    //update y's parent pointer of x's right
    if (x->right != NULL){
        x->right->parent = x;
    }

    //update y's parent pointer
    y->parent = x->parent;
    
    //if x's parent is null, make y root of tree
    if (x->parent == NULL){
        (*root) = y;
    }

    //store y at place of x
    else if (x == x->parent->left){
        x->parent->left = y;
    }

    else {
        x->parent->right = y;
    }

    //make x as left child of y
    y->left = x;

    //update parent pointer of x
    x->parent = y;
}

//Right Rotation (Similar to leftRotate)
void rightRotate(struct node **root, struct node *y) {

    if(!y || !y->left){
        return ;
    }

    struct node *x = y->left;
    y->left = x ->right;

    if (x->right != NULL){
        x->right->parent = y;
    }
    x->parent = y->parent;

    if (x->parent == NULL){
        (*root) = x;
    }

    else if (y== y->parent->left){
        y->parent->left = x;
    }
    
    else {
        y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
};

// Utility function to fixup the Red-Black tree after standard BST insertion
void insertFixUp(struct node **root,struct node *z)
{
    // iterate until z is not the root and z's parent color is red
    while (z != *root && z != (*root)->left && z != (*root)->right && z->parent->color == 'R')
    {
        struct node *y;

        // Find uncle and store uncle in y
        if (z->parent && z->parent->parent && z->parent == z->parent->parent->left)
            y = z->parent->parent->right;
        else
            y = z->parent->parent->left;

        // If uncle is RED, do following
        // (i)  Change color of parent and uncle as BLACK
        // (ii) Change color of grandparent as RED
        // (iii) Move z to grandparent
        if (!y)
            z = z->parent->parent;
        else if (y->color == 'R')
        {
            y->color = 'B';
            z->parent->color = 'B';
            z->parent->parent->color = 'R';
            z = z->parent->parent;
        }

        // Uncle is BLACK, there are four cases (LL, LR, RL and RR)
        else
        {
            // Left-Left (LL) case, do following
            // (i)  Swap color of parent and grandparent
            // (ii) Right Rotate Grandparent
            if (z->parent == z->parent->parent->left &&
                z == z->parent->left)
            {
                char ch = z->parent->color ;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                rightRotate(root,z->parent->parent);
            }

            // Left-Right (LR) case, do following
            // (i)  Swap color of current node  and grandparent
            // (ii) Left Rotate Parent
            // (iii) Right Rotate Grand Parent
            if (z->parent && z->parent->parent && z->parent == z->parent->parent->left &&
                z == z->parent->right)
            {
                char ch = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                leftRotate(root,z->parent);
                rightRotate(root,z->parent->parent);
            }

            // Right-Right (RR) case, do following
            // (i)  Swap color of parent and grandparent
            // (ii) Left Rotate Grandparent
            if (z->parent && z->parent->parent &&
                z->parent == z->parent->parent->right &&
                z == z->parent->right)
            {
                char ch = z->parent->color ;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                leftRotate(root,z->parent->parent);
            }

            // Right-Left (RL) case, do following
            // (i)  Swap color of current node  and grandparent
            // (ii) Right Rotate Parent
            // (iii) Left Rotate Grand Parent
            if (z->parent && z->parent->parent && z->parent == z->parent->parent->right &&
                z == z->parent->left)
            {
                char ch = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                rightRotate(root,z->parent);
                leftRotate(root,z->parent->parent);
            }
        }
    }
    (*root)->color = 'B'; //keep root always black
}

// Utility function to insert newly node in RedBlack tree
void insert(struct node **root, int data)
{
    // Allocate memory for new node
    struct node *z = (struct node*)malloc(sizeof(struct node));
    z->data = data;
    z->left = z->right = z->parent = NULL;

     //if root is null make z as root
    if (*root == NULL) {
        z->color = 'B';
        (*root) = z;
    }
    else {
        struct node *y = NULL;
        struct node *x = (*root);

        // Follow standard BST insert steps to first insert the node
        while (x != NULL) {
            y = x;
            if (z->data < x->data){
                x = x->left;
            }
            else{
                x = x->right;
            }
        }
        z->parent = y;
        if (z->data > y->data){
            y->right = z;
        }
        else {
            y->left = z;
        }
        z->color = 'R';

        // call insertFixUp to fix reb-black tree's property if it
        // is voilated due to insertion.
        insertFixUp(root,z);
    }
}

// A utility function to traverse Red-Black tree in inorder fashion
void inOrder(struct node *root)
{
    static int last = 0;
    if (root == NULL) {
        return;
    }
    inOrder(root->left);
    printf("%d ", root->data);
    last = root->data;
    inOrder(root->right);
}

/*int OSSelect(struct node* tree, node* root, int i)
{
    int r = size(tree, root->left) +1;
    r = x->left->size-1;
    if (i == r)
        return x;
    else if (i < r)
        return OSSelect(x->left, i);
    else   
        return OSSelect(x->right, i-r);
}

int size(struct node* tree, struct node* root){
    if(root == tree->NIL){
        return 0;
    }
    else {
        return(size(tree, node->left)+1+size(tree, node->right));
    }
}
*/