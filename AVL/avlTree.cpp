#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <string>

using namespace std;

struct TreeNode {
    int key;
    TreeNode* left;
    TreeNode* right;
    int height;
    
    TreeNode(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    TreeNode* root;

    // Get height of node
    int getHeight(TreeNode* node) {
        return node ? node->height : 0;
    }

    // Get balance factor
    int getBalance(TreeNode* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    // Update height
    void updateHeight(TreeNode* node) {
        if (node) {
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        }
    }

    // Right rotation
    TreeNode* rightRotate(TreeNode* y) {
        TreeNode* x = y->left;
        TreeNode* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // Left rotation
    TreeNode* leftRotate(TreeNode* x) {
        TreeNode* y = x->right;
        TreeNode* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        updateHeight(x);
        updateHeight(y);

        return y;
    }

    // Insert a key into AVL tree
    TreeNode* insert(TreeNode* node, int key) {
        // 1. Perform normal BST insertion
        if (!node) return new TreeNode(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else // Duplicate keys not allowed
            return node;

        // Update height of current node
        updateHeight(node);

        // Get balance factor
        int balance = getBalance(node);

        // Perform rotations if unbalanced

        // Left Left Case
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Find minimum value node
    TreeNode* minValueNode(TreeNode* node) {
        TreeNode* current = node;
        while (current && current->left)
            current = current->left;
        return current;
    }

    // Delete a key from AVL tree
    TreeNode* deleteNode(TreeNode* root, int key) {
        // 1. Perform standard BST deletion
        if (!root) return root;

        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
            // Node with only one child or no child
            if (!root->left || !root->right) {
                TreeNode* temp = root->left ? root->left : root->right;

                // No child case
                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else { // One child case
                    *root = *temp; // Copy contents
                }
                delete temp;
            } else {
                // Node with two children
                TreeNode* temp = minValueNode(root->right);
                root->key = temp->key;
                root->right = deleteNode(root->right, temp->key);
            }
        }

        // If tree had only one node then return
        if (!root) return root;

        // Update height
        updateHeight(root);

        // Get balance factor
        int balance = getBalance(root);

        // Perform rotations if unbalanced

        // Left Left Case
        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);

        // Left Right Case
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // Right Right Case
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);

        // Right Left Case
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    // Print tree with ASCII
    void printTreeVisual(TreeNode* root, string prefix = "", bool isLeft = true) {
        if (root == nullptr) {
            cout << prefix << (isLeft ? "|-- " : "\\-- ") << "null" << endl;
            return;
        }

        cout << prefix;
        cout << (isLeft ? "|-- " : "\\-- ");
        cout << root->key << "(h:" << root->height << ", b:" << getBalance(root) << ")" << endl;

        // Create appropriate prefix for children
        string childPrefix = prefix + (isLeft ? "|   " : "    ");
        printTreeVisual(root->left, childPrefix, true);
        printTreeVisual(root->right, childPrefix, false);
    }

    // Simple string conversion
    string nodeToString(TreeNode* node) {
        if (!node) return "null";
        return to_string(node->key) + "(h:" + to_string(node->height) + 
               ", b:" + to_string(getBalance(node)) + ")";
    }

    // Print tree in level order with better formatting
    void printLevelOrder(TreeNode* root) {
        if (!root) {
            cout << "Tree is empty!" << endl;
            return;
        }

        queue<TreeNode*> q;
        q.push(root);
        int level = 0;

        while (!q.empty()) {
            int levelSize = q.size();
            cout << "Level " << level << ": ";
            
            for (int i = 0; i < levelSize; i++) {
                TreeNode* current = q.front();
                q.pop();
                
                if (current) {
                    cout << nodeToString(current);
                    q.push(current->left);
                    q.push(current->right);
                } else {
                    cout << "null";
                }
                if (i < levelSize - 1) cout << " | ";
            }
            cout << endl;
            level++;
        }
    }

    // Simple indented tree print (alternative visualization)
    void printTreeIndented(TreeNode* node, int depth = 0, string prefix = "") {
        if (!node) {
            // Don't print null nodes to reduce clutter
            return;
        }

        // Print right subtree first (so it appears on top in console)
        printTreeIndented(node->right, depth + 1, "/---");

        // Print current node
        string indent = string(depth * 4, ' ');
        cout << indent << prefix << node->key << "(h:" << node->height << ")" << endl;

        // Print left subtree
        printTreeIndented(node->left, depth + 1, "\\---");
    }

    // In-order traversal
    void inOrder(TreeNode* node, vector<int>& result) {
        if (!node) return;
        inOrder(node->left, result);
        result.push_back(node->key);
        inOrder(node->right, result);
    }

public:
    AVLTree() : root(nullptr) {}

    // Public insert method
    void insert(int key) {
        root = insert(root, key);
    }

    // Public delete method
    void deleteKey(int key) {
        root = deleteNode(root, key);
    }

    // Print the tree with ASCII characters
    void printTree() {
        cout << "\nAVL Tree Structure (ASCII):" << endl;
        if (!root) {
            cout << "Tree is empty!" << endl;
            return;
        }
        printTreeVisual(root);
        cout << endl;
    }

    // Alternative tree printing
    void printTreeSideways() {
        cout << "\nAVL Tree Structure (Sideways):" << endl;
        if (!root) {
            cout << "Tree is empty!" << endl;
            return;
        }
        printTreeIndented(root);
        cout << endl;
    }

    // Print level order
    void printLevelOrder() {
        cout << "Level Order Traversal:" << endl;
        printLevelOrder(root);
        cout << endl;
    }

    // Print in-order traversal
    void printInOrder() {
        vector<int> result;
        inOrder(root, result);
        cout << "In-order traversal: ";
        for (size_t i = 0; i < result.size(); i++) {
            cout << result[i];
            if (i < result.size() - 1) cout << " ";
        }
        cout << endl;
    }

    // Build tree from array of keys
    void buildTree(const vector<int>& keys) {
        for (int key : keys) {
            insert(key);
        }
    }
};

int main() {
    vector<int> keys = {3, 2, 1, 4, 5, 6, 7, 16, 15, 14, 13, 12, 11, 10, 8, 9};
    
    cout << "=== AVL Tree Operations ===" << endl;
    cout << "Inserting keys: ";
    for (int key : keys) cout << key << " ";
    cout << endl << endl;
    
    // Create AVL tree and insert all keys
    AVLTree tree;
    tree.buildTree(keys);
    
    cout << "After all insertions:" << endl;
    tree.printTree();
    tree.printLevelOrder();
    tree.printInOrder();
    
    cout << "\nAlternative visualization:" << endl;
    tree.printTreeSideways();
    
    cout << "\n" << string(60, '=') << endl;
    
    // Delete key 6
    cout << "Deleting key 6..." << endl;
    tree.deleteKey(6);
    tree.printTree();
    tree.printLevelOrder();
    tree.printInOrder();
    
    cout << "\n" << string(60, '=') << endl;
    
    // Delete key 15
    cout << "Deleting key 15..." << endl;
    tree.deleteKey(15);
    tree.printTree();
    tree.printLevelOrder();
    tree.printInOrder();
    
    return 0;
}