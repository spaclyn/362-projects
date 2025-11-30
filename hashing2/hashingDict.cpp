#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <cctype>

class SpellChecker {
private:
    std::unordered_map<std::string, bool> dictionary;
    std::unordered_map<std::string, std::vector<std::string>> prefixMap;
    
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
    
    int calculateEditDistance(const std::string& word1, const std::string& word2) {
        int m = word1.length();
        int n = word2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
        
        for (int i = 0; i <= m; i++) dp[i][0] = i;
        for (int j = 0; j <= n; j++) dp[0][j] = j;
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (word1[i - 1] == word2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = 1 + std::min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
                }
            }
        }
        return dp[m][n];
    }
    
    double calculateSimilarity(const std::string& word1, const std::string& word2) {
        // Simple similarity based on edit distance and common patterns
        int editDistance = calculateEditDistance(word1, word2);
        double maxLen = std::max(word1.length(), word2.length());
        
        if (maxLen == 0) return 1.0;
        return 1.0 - (editDistance / maxLen);
    }

public:
    bool loadDictionary(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error: Could not open dictionary file '" << filename << "'" << std::endl;
            return false;
        }
        
        std::string word;
        int wordCount = 0;
        
        while (std::getline(file, word)) {
            std::string lowerWord = toLower(word);
            
            // Remove carriage return if present (for Windows files)
            if (!lowerWord.empty() && lowerWord.back() == '\r') {
                lowerWord.pop_back();
            }
            
            if (!lowerWord.empty()) {
                // Store in main dictionary
                dictionary[lowerWord] = true;
                
                // Store in prefix map for suggestions (only if word has at least 2 chars)
                if (lowerWord.length() >= 2) {
                    std::string prefix = lowerWord.substr(0, 2);
                    prefixMap[prefix].push_back(lowerWord);
                }
                wordCount++;
            }
        }
        
        file.close();
        std::cout << "Dictionary loaded with " << wordCount << " words" << std::endl;
        return true;
    }
    
    bool wordExists(const std::string& word) {
        return dictionary.find(toLower(word)) != dictionary.end();
    }
    
    std::vector<std::string> getSuggestions(const std::string& word) {
        std::string lowerWord = toLower(word);
        
        // Handle single character words
        if (lowerWord.length() < 2) {
            return {"Word too short for suggestions"};
        }
        
        std::string prefix = lowerWord.substr(0, 2);
        auto it = prefixMap.find(prefix);
        if (it != prefixMap.end()) {
            return it->second;
        }
        
        return {};
    }
    
    std::string findSimilarWord(const std::string& misspelledWord) {
        std::string lowerMisspelled = toLower(misspelledWord);
        
        // Handle very short words
        if (lowerMisspelled.length() < 2) {
            return "";
        }
        
        char firstLetter = lowerMisspelled[0];
        std::string bestMatch;
        double bestSimilarity = 0.0;
        
        // Check all words starting with the same first letter
        for (const auto& entry : dictionary) {
            const std::string& dictWord = entry.first;
            
            if (dictWord.length() >= 2 && dictWord[0] == firstLetter) {
                double similarity = calculateSimilarity(lowerMisspelled, dictWord);
                
                // Bonus for same length
                if (dictWord.length() == lowerMisspelled.length()) {
                    similarity += 0.1;
                }
                
                // Bonus for same first two letters
                if (dictWord.length() >= 2 && lowerMisspelled.length() >= 2 && 
                    dictWord.substr(0, 2) == lowerMisspelled.substr(0, 2)) {
                    similarity += 0.2;
                }
                
                if (similarity > bestSimilarity) {
                    bestSimilarity = similarity;
                    bestMatch = dictWord;
                }
            }
        }
        
        // Only return if similarity is reasonably high
        if (bestSimilarity > 0.6) {
            return bestMatch;
        }
        
        return "";
    }
};

// Function to get user input with prompt
std::string getInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

// Function to display suggestions
void displaySuggestions(const std::vector<std::string>& suggestions) {
    if (suggestions.empty()) {
        std::cout << "No suggestions available" << std::endl;
        return;
    }
    
    if (suggestions[0] == "Word too short for suggestions") {
        std::cout << "Word too short for suggestions" << std::endl;
        return;
    }
    
    std::cout << "Suggestions: ";
    int count = 0;
    for (const auto& word : suggestions) {
        if (count >= 10) { // Limit to 10 suggestions
            std::cout << "... and " << (suggestions.size() - 10) << " more";
            break;
        }
        std::cout << word;
        if (count < std::min(10, (int)suggestions.size() - 1)) {
            std::cout << ", ";
        }
        count++;
    }
    std::cout << std::endl;
}

int main() {
    SpellChecker spellChecker;
    
    std::cout << "=== Spell Checker and Word Suggestion Program ===" << std::endl;
    std::cout << "Loading dictionary..." << std::endl;
    
    if (!spellChecker.loadDictionary("Dictionary.txt")) {
        std::cout << "Failed to load dictionary. Please make sure 'Dictionary.txt' exists." << std::endl;
        return 1;
    }
    
    std::cout << "\nType 'quit' to exit the program\n" << std::endl;
    
    while (true) {
        std::string userInput = getInput("Please enter a word: ");
        
        // Check for exit condition
        if (userInput == "quit") {
            std::cout << "Goodbye!" << std::endl;
            break;
        }
        
        // Handle empty input
        if (userInput.empty()) {
            std::cout << "Please enter a valid word." << std::endl;
            continue;
        }
        
        // Case 1: Word exists in dictionary
        if (spellChecker.wordExists(userInput)) {
            std::cout << "True" << std::endl;
            
            // Get and display suggestions
            std::vector<std::string> suggestions = spellChecker.getSuggestions(userInput);
            displaySuggestions(suggestions);
        }
        // Case 2: Word doesn't exist but might be a misspelling
        else {
            std::string similarWord = spellChecker.findSimilarWord(userInput);
            
            if (!similarWord.empty()) {
                std::string response = getInput("Do you mean '" + similarWord + "'? (yes/no): ");
                
                if (response == "yes" || response == "y") {
                    std::cout << "True" << std::endl;
                    // Show suggestions for the corrected word
                    std::vector<std::string> suggestions = spellChecker.getSuggestions(similarWord);
                    displaySuggestions(suggestions);
                } else {
                    std::cout << "False" << std::endl;
                }
            }
            // Case 3: No similar word found
            else {
                std::cout << "False" << std::endl;
            }
        }
        
        std::cout << std::endl;
    }
    
    return 0;
}