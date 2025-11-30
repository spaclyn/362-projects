#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class BalancedBST {
private:
    TreeNode* root;
    int count;

    // Helper function to build balanced BST from array
    TreeNode* buildBalancedBST(vector<int>& nums, int start, int end) {
        if (start > end) return nullptr;
        
        int mid = start + (end - start) / 2;
        TreeNode* node = new TreeNode(nums[mid]);
        
        node->left = buildBalancedBST(nums, start, mid - 1);
        node->right = buildBalancedBST(nums, mid + 1, end);
        
        return node;
    }

    //Ordered traversal to find k-th smallest element
    void inOrderKth(TreeNode* node, int k, int& result, int& current) {
        if (!node || current >= k) return;
        
        inOrderKth(node->left, k, result, current);
        
        current++;
        if (current == k) {
            result = node->val;
            return;
        }
        
        inOrderKth(node->right, k, result, current);
    }

    //Helper for level order tree printing
    void printLevelOrder(TreeNode* root) {
        if (!root) return;
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            int levelSize = q.size();
            for (int i = 0; i < levelSize; i++) {
                TreeNode* current = q.front();
                q.pop();
                
                if (current) {
                    cout << current->val << " ";
                    q.push(current->left);
                    q.push(current->right);
                } else {
                    cout << "null ";
                }
            }
            cout << endl;
        }
    }

public:
    BalancedBST(vector<int>& nums) {
        //Sort Array
        vector<int> sortedNums = nums;
        sort(sortedNums.begin(), sortedNums.end());
        
        //Build Balanced BST
        root = buildBalancedBST(sortedNums, 0, sortedNums.size() - 1);
        count = sortedNums.size();
    }

    //k-th smallest element (1-based index)
    int findKthSmallest(int k) {
        if (k < 1 || k > count) {
            throw out_of_range("k is out of range");
        }
        
        int result = -1;
        int current = 0;
        inOrderKth(root, k, result, current);
        return result;
    }

    //Print in Level Order
    void printBST() {
        cout << "BST Level Order Traversal:" << endl;
        printLevelOrder(root);
        cout << endl;
    }

    //Print The best way to visualize a BST
    void printBSTPretty() {
        cout << "BST Structure (in-order sorted):" << endl;
        vector<int> inOrderResult;
        getInOrder(root, inOrderResult);
        
        for (int val : inOrderResult) {
            cout << val << " ";
        }
        cout << endl << endl;
        
        cout << "BST Level by Level:" << endl;
        printLevelOrderPretty();
    }

private: //Helper for pretty in-order traversal, private
    void getInOrder(TreeNode* node, vector<int>& result) {
        if (!node) return;
        getInOrder(node->left, result);
        result.push_back(node->val);
        getInOrder(node->right, result);
    }

    void printLevelOrderPretty() {
        if (!root) return;
        
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
                    cout << current->val << " ";
                    q.push(current->left);
                    q.push(current->right);
                }
            }
            cout << endl;
            level++;
        }
    }
};

int main() {
    vector<int> nums = {6, 17, 20, 41, 45, 52, 57, 65, 71, 76, 79, 87, 92, 95, 99}; //required input array
    
    //Create balanced BST
    BalancedBST bst(nums);
    
    //Print the BST
    bst.printBSTPretty();
    
    //User Input for k
    int k;
    cout << "Enter k to find k-th smallest element (1-based): ";
    cin >> k;
    
    try {
        int kthSmallest = bst.findKthSmallest(k);
        cout << "The " << k << "-th smallest element is: " << kthSmallest << endl;
    } catch (const out_of_range& e) {
        cout << "Error: " << e.what() << endl;
    }
    
    return 0;
}