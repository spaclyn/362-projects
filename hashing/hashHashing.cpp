#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <random>
#include <set>

class HashTable {
private:
    static const int TABLE_SIZE = 10; // 10 possible first digits (0-9)
    std::vector<std::list<int>> table;
    
    // Hash function
    int hashFunction(int key) {
        return key / 10;
    }
    
public:
    HashTable() : table(TABLE_SIZE) {}
    
    // Insert a value into the hash table
    void insert(int value) {
        int index = hashFunction(value);
        table[index].push_back(value);
    }
    
    // Remove duplicates from all chains
    void removeDuplicates() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (!table[i].empty()) {
                // Use a set to find unique values
                std::set<int> uniqueValues(table[i].begin(), table[i].end());
                table[i].assign(uniqueValues.begin(), uniqueValues.end());
            }
        }
    }
    
    // Display a specific slot
    void displaySlot(int slot) {
        if (slot < 0 || slot >= TABLE_SIZE) {
            std::cout << "Invalid slot number! Please choose between 0 and " << TABLE_SIZE - 1 << std::endl;
            return;
        }
        
        std::cout << "Slot " << slot << " (Numbers starting with digit " << slot << "): ";
        
        if (table[slot].empty()) {
            std::cout << "No entries" << std::endl;
        } else {
            for (int num : table[slot]) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
        }
    }
    
    // debugging/verification
    void displayAllSlots() {
        std::cout << "\nAll slots in the hash table:" << std::endl;
        for (int i = 0; i < TABLE_SIZE; i++) {
            std::cout << "Slot " << i << ": ";
            if (table[i].empty()) {
                std::cout << "Empty";
            } else {
                for (int num : table[i]) {
                    std::cout << num << " ";
                }
            }
            std::cout << std::endl;
        }
    }
    
    // Get table size
    int getTableSize() const {
        return TABLE_SIZE;
    }
};

// Function to generate random integers in range [10, 99]
std::vector<int> generateRandomNumbers(int count) {
    std::vector<int> numbers;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10, 99);
    
    for (int i = 0; i < count; i++) {
        numbers.push_back(dis(gen));
    }
    
    return numbers;
}

int main() {
    const int NUM_COUNT = 1000;
    
    std::cout << "=== Hash Table with Chaining Implementation ===" << std::endl;
    
    // (a) Generate random integers
    std::cout << "Generating " << NUM_COUNT << " random integers in range [10, 99]..." << std::endl;
    std::vector<int> randomNumbers = generateRandomNumbers(NUM_COUNT);
    
    // Create hash table and insert numbers
    HashTable hashTable;
    for (int num : randomNumbers) {
        hashTable.insert(num);
    }
    
    std::cout << "Initial insertion completed." << std::endl;
    
    // (b) Remove duplicates from all chains
    std::cout << "Removing duplicates from hash table..." << std::endl;
    hashTable.removeDuplicates();
    
    // Display table size information
    std::cout << "\nHash table has " << hashTable.getTableSize() << " slots (0-9)" << std::endl;
    
    // Interactive part
    char choice;
    do {
        // (c) Ask user to pick a slot
        int slot;
        std::cout << "\nEnter a slot number (0-9) to display its unique entries: ";
        std::cin >> slot;
        
        // Display the chosen slot
        hashTable.displaySlot(slot);
        
        std::cout << "\nWould you like to view another slot? (y/n): ";
        std::cin >> choice;
        
    } while (choice == 'y' || choice == 'Y');

    // Display all slots at the end
    char showAll;
    std::cout << "\nWould you like to see all slots? (y/n): ";
    std::cin >> showAll;
    if (showAll == 'y' || showAll == 'Y') {
        hashTable.displayAllSlots();
    }
    
    std::cout << "\nThank you for using the hash table program!" << std::endl;
    
    return 0;
}