#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

//B-tree node structure for a 5-way B-tree (order 5)
class BTreeNode {
public:
    std::vector<int> keys;          //Store keys (max 4 keys in a node)
    std::vector<BTreeNode*> children; //Store child pointers (max 5 children)
    bool isLeaf;
    
    BTreeNode(bool leaf) {
        isLeaf = leaf;
        keys.reserve(4);      //Maximum 4 keys
        children.reserve(5);  //Maximum 5 children
    }
    
    ~BTreeNode() {
        for (auto child : children) {
            delete child;
        }
    }
};

// B-tree class
class BTree {
private:
    BTreeNode* root;
    int t;
    
    //Helper to insert key in a non-full node
    void insertNonFull(BTreeNode* node, int key) {
        int i = node->keys.size() - 1;
        
        if (node->isLeaf) {
            //Insert key in sorted position in leaf node
            node->keys.push_back(0); // Add placeholder
            while (i >= 0 && key < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = key;
        } else {
            while (i >= 0 && key < node->keys[i]) {
                i--;
            }
            i++;
            
            if (node->children[i]->keys.size() == 4) {
                splitChild(node, i, node->children[i]);
                

                if (key > node->keys[i]) {
                    i++;
                }
            }
            insertNonFull(node->children[i], key);
        }
    }
    
    //Helper to split a full child
    void splitChild(BTreeNode* parent, int i, BTreeNode* child) {
        BTreeNode* newChild = new BTreeNode(child->isLeaf);
        
        //Move the last (t-1) keys from child to newChild
        for (int j = 0; j < 2; j++) {
            newChild->keys.push_back(child->keys[j + 3]);
        }
        
        //Move the last t children from child to newChild if not leaf
        if (!child->isLeaf) {
            for (int j = 0; j < 3; j++) {
                newChild->children.push_back(child->children[j + 2]);
            }
            child->children.resize(2);
        }
        
        child->keys.resize(2);
        
        //Move middle key up to parent
        int middleKey = child->keys[2];
        child->keys.pop_back(); //Remove the middle from child
        
        //Insert newChild into parent's children
        parent->children.insert(parent->children.begin() + i + 1, newChild);
        
        //Insert middle key into parent
        parent->keys.insert(parent->keys.begin() + i, middleKey);
    }
    
    // Helper for range search
    void rangeSearchHelper(BTreeNode* node, int low, int high, std::vector<int>& result) {
        if (node == nullptr) return;
        
        int i = 0;
        //Traverse through all keys and children
        while (i < node->keys.size()) {
            //If not leaf, then traverse the subtree rooted with child[i]
            if (!node->isLeaf) {
                rangeSearchHelper(node->children[i], low, high, result);
            }
            
            //If current key is in range, add to result
            if (node->keys[i] >= low && node->keys[i] <= high) {
                result.push_back(node->keys[i]);
            }
            
            i++;
        }
        
        //Traverse the subtree rooted with last child
        if (!node->isLeaf) {
            rangeSearchHelper(node->children[i], low, high, result);
        }
    }
    
    //Helper prints tree (in-order traversal)
    void printTreeHelper(BTreeNode* node, int level) {
        if (node == nullptr) return;
        
        std::cout << "Level " << level << ": ";
        for (int key : node->keys) {
            std::cout << key << " ";
        }
        std::cout << std::endl;
        
        if (!node->isLeaf) {
            for (auto child : node->children) {
                printTreeHelper(child, level + 1);
            }
        }
    }
    
public:
    BTree() {
        root = nullptr;
        t = 2;  //For 5-way B-tree (2*t-1 = 3 max keys, but will allow 4)
    }
    
    ~BTree() {
        delete root;
    }
    
    //Insert a key into the B-tree
    void insert(int key) {
        if (root == nullptr) {
            root = new BTreeNode(true);
            root->keys.push_back(key);
        } else {
            //If root is full, split it
            if (root->keys.size() == 4) {
                BTreeNode* newRoot = new BTreeNode(false);
                newRoot->children.push_back(root);
                splitChild(newRoot, 0, root);
                
                //New root has two children now, decide which child gets the new key
                int i = 0;
                if (key > newRoot->keys[0]) {
                    i++;
                }
                insertNonFull(newRoot->children[i], key);
                
                root = newRoot;
            } else {
                insertNonFull(root, key);
            }
        }
    }
    
    //Search for keys in range [low, high]
    std::vector<int> rangeSearch(int low, int high) {
        std::vector<int> result;
        rangeSearchHelper(root, low, high, result);
        return result;
    }
    
    //Print the entire tree structure
    void printTree() {
        if (root == nullptr) {
            std::cout << "Tree is empty" << std::endl;
        } else {
            std::cout << "B-tree structure:" << std::endl;
            printTreeHelper(root, 0);
        }
    }
    
    // Get the root node
    BTreeNode* getRoot() {
        return root;
    }
};

//Function to generate random integers in range [0, 3*N]
std::vector<int> generateRandomKeys(int N) {
    std::vector<int> keys;
    keys.reserve(N);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 3 * N);
    
    for (int i = 0; i < N; i++) {
        keys.push_back(dist(gen));
    }
    
    return keys;
}

int main() {
    int N;
    
    //Get input N from user
    std::cout << "Enter N (>= 400): ";
    std::cin >> N;
    
    if (N < 400) {
        std::cout << "Error: N must be >= 400" << std::endl;
        return 1;
    }
    
    //Generate random keys
    std::cout << "Generating " << N << " random integers in range [0, " << 3*N << "]..." << std::endl;
    std::vector<int> keys = generateRandomKeys(N);
    
    // Build B-tree; insert keyys into B-tree
    std::cout << "Building 5-way B-tree..." << std::endl;
    BTree tree;
    for (int key : keys) {
        tree.insert(key);
    }
    
    //Output a: Print the tree structure
    std::cout << "\n=== OUTPUT (a): TREE STRUCTURE ===" << std::endl;
    tree.printTree();
    
    //Output b: Search for keys in range [N, 2*N]
    std::cout << "\n=== OUTPUT (b): KEYS FOUND IN RANGE [" << N << ", " << 2*N << "] ===" << std::endl;
    std::vector<int> foundKeys = tree.rangeSearch(N, 2*N);
    
    if (foundKeys.empty()) {
        std::cout << "No keys found in the specified range." << std::endl;
    } else {
        std::cout << "Found " << foundKeys.size() << " keys: ";
        for (int key : foundKeys) {
            std::cout << key << " ";
        }
        std::cout << std::endl;
    }
    
    //Additional statistics
    std::cout << "\n=== STATISTICS ===" << std::endl;
    std::cout << "Total keys inserted: " << N << std::endl;
    std::cout << "Keys found in range [" << N << ", " << 2*N << "]: " << foundKeys.size() << std::endl;
    std::cout << "Percentage of keys in range: " 
              << (static_cast<double>(foundKeys.size()) / N * 100) << "%" << std::endl;
    
    return 0;
}