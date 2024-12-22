

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define the AVL node structure
typedef struct AVLNode {
    char station_id[20];
    long capacity;
    long consumption;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

// Function prototypes
AVLNode *create_node(const char *station_id, long capacity, long consumption);
int get_height(AVLNode *node);
int get_balance(AVLNode *node);
AVLNode *rotate_right(AVLNode *y);
AVLNode *rotate_left(AVLNode *x);
AVLNode *insert(AVLNode *node, const char *station_id, long capacity, long consumption);
void in_order(AVLNode *root, FILE *output_file);
void free_tree(AVLNode *root);

// Main function
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    const char *input_file = argv[1];
    const char *output_file = argv[2];

    FILE *input = fopen(input_file, "r");
    if (!input) {
        perror("Error opening input file");
        return 1;
    }

    FILE *output = fopen(output_file, "w");
    if (!output) {
        perror("Error opening output file");
        fclose(input);
        return 1;
    }

    AVLNode *root = NULL;
    char line[256];

    while (fgets(line, sizeof(line), input)) {
        char station_id[20];
        long capacity = 0;
        long consumption = 0;

        if (sscanf(line, "%[^;];%ld;%ld", station_id, &capacity, &consumption) == 3) {
            root = insert(root, station_id, capacity, consumption);
        }
    }

    fprintf(output, "Station ID:Capacity:Consumption\n");
    in_order(root, output);

    free_tree(root);
    fclose(input);
    fclose(output);

    return 0;
}

// Create a new AVL node
AVLNode *create_node(const char *station_id, long capacity, long consumption) {
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    strcpy(node->station_id, station_id);
    node->capacity = capacity;
    node->consumption = consumption;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

// Get height of a node
int get_height(AVLNode *node) {
    return node ? node->height : 0;
}

// Get balance factor of a node
int get_balance(AVLNode *node) {
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

// Right rotate
AVLNode *rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + ((get_height(y->left) > get_height(y->right)) ? get_height(y->left) : get_height(y->right));
    x->height = 1 + ((get_height(x->left) > get_height(x->right)) ? get_height(x->left) : get_height(x->right));

    return x;
}

// Left rotate
AVLNode *rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + ((get_height(x->left) > get_height(x->right)) ? get_height(x->left) : get_height(x->right));
    y->height = 1 + ((get_height(y->left) > get_height(y->right)) ? get_height(y->left) : get_height(y->right));

    return y;
}

// Insert a node into the AVL tree
AVLNode *insert(AVLNode *node, const char *station_id, long capacity, long consumption) {
    if (!node) {
        return create_node(station_id, capacity, consumption);
    }

    if (strcmp(station_id, node->station_id) < 0) {
        node->left = insert(node->left, station_id, capacity, consumption);
    } else if (strcmp(station_id, node->station_id) > 0) {
        node->right = insert(node->right, station_id, capacity, consumption);
    } else {
        node->consumption += consumption;
        return node;
    }

    node->height = 1 + ((get_height(node->left) > get_height(node->right)) ? get_height(node->left) : get_height(node->right));

    int balance = get_balance(node);

    if (balance > 1 && strcmp(station_id, node->left->station_id) < 0) {
        return rotate_right(node);
    }

    if (balance < -1 && strcmp(station_id, node->right->station_id) > 0) {
        return rotate_left(node);
    }

    if (balance > 1 && strcmp(station_id, node->left->station_id) > 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    if (balance < -1 && strcmp(station_id, node->right->station_id) < 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

// In-order traversal
void in_order(AVLNode *root, FILE *output_file) {
    if (root) {
        in_order(root->left, output_file);
        fprintf(output_file, "%s:%ld:%ld\n", root->station_id, root->capacity, root->consumption);
        in_order(root->right, output_file);
    }
}

// Free AVL tree
void free_tree(AVLNode *root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}
