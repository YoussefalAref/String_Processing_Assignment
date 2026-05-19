#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <unordered_map>

using namespace std;

// Structure to store search statistics
struct Statistics {
    long long comparisons;
    int matchCount;
    double executionTime;
    string algorithm;
};

// Global variables
string text = "";
Statistics lastStats;

// ============ BOYER-MOORE ALGORITHM ============

class BoyerMoore {
private:
    unordered_map<char, int> badCharTable;
    
    void buildBadCharTable(const string& pattern) {
        badCharTable.clear();
        for (int i = 0; i < pattern.length(); i++) {
            badCharTable[pattern[i]] = pattern.length() - i - 1;
        }
    }
    
public:
    vector<int> search(const string& text, const string& pattern, long long& comparisons) {
        vector<int> matches;
        comparisons = 0;
        
        if (pattern.empty() || text.length() < pattern.length()) {
            return matches;
        }
        
        buildBadCharTable(pattern);
        
        int n = text.length();
        int m = pattern.length();
        int shift = 0;
        
        while (shift <= n - m) {
            int j = m - 1;
            
            // Compare pattern from right to left
            while (j >= 0 && pattern[j] == text[shift + j]) {
                comparisons++;
                j--;
            }
            comparisons++;  // One more comparison when mismatch occurs
            
            if (j < 0) {
                // Pattern found
                matches.push_back(shift);
                // Shift to find overlapping patterns
                shift += (shift + m < n) ? 1 : 1;
            } else {
                // Use bad character rule to shift
                char badChar = text[shift + j];
                if (badCharTable.find(badChar) != badCharTable.end()) {
                    shift += badCharTable[badChar];
                } else {
                    shift += j + 1;
                }
            }
        }
        
        return matches;
    }
};

// ============ RABIN-KARP ALGORITHM ============

class RabinKarp {
private:
    const long long PRIME = 101;
    const long long BASE = 256;
    
    long long computeHash(const string& str, int len) {
        long long hash = 0;
        long long pow = 1;
        
        for (int i = len - 1; i >= 0; i--) {
            hash = (hash + (long long)str[i] * pow) % PRIME;
            pow = (pow * BASE) % PRIME;
        }
        
        return hash;
    }
    
    long long updateHash(const string& text, int oldIdx, int newIdx, long long oldHash, 
                         long long pow, int patternLen) {
        long long newHash = (oldHash - (long long)text[oldIdx] * pow) % PRIME;
        newHash = (newHash * BASE + (long long)text[newIdx]) % PRIME;
        
        if (newHash < 0) {
            newHash += PRIME;
        }
        
        return newHash;
    }
    
public:
    vector<int> search(const string& text, const string& pattern, long long& comparisons) {
        vector<int> matches;
        comparisons = 0;
        
        if (pattern.empty() || text.length() < pattern.length()) {
            return matches;
        }
        
        int n = text.length();
        int m = pattern.length();
        long long pow = 1;
        
        // Compute base power
        for (int i = 0; i < m - 1; i++) {
            pow = (pow * BASE) % PRIME;
        }
        
        long long patternHash = computeHash(pattern, m);
        long long textHash = computeHash(text, m);
        
        // Sliding window search
        for (int i = 0; i <= n - m; i++) {
            // Compare hashes
            comparisons++;
            
            if (patternHash == textHash) {
                // Hash match, verify character by character
                bool match = true;
                for (int j = 0; j < m; j++) {
                    comparisons++;
                    if (text[i + j] != pattern[j]) {
                        match = false;
                        break;
                    }
                }
                
                if (match) {
                    matches.push_back(i);
                }
            }
            
            // Update hash for next window
            if (i < n - m) {
                textHash = updateHash(text, i, i + m, textHash, pow, m);
            }
        }
        
        return matches;
    }
};

// ============ FILE I/O FUNCTIONS ============

void loadFile() {
    string filename;
    cout << "\nEnter filename (e.g., input.txt): ";
    cin.ignore();
    getline(cin, filename);
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'" << endl;
        return;
    }
    
    text = "";
    string line;
    while (getline(file, line)) {
        if (!text.empty()) text += "\n";
        text += line;
    }
    
    file.close();
    cout << "File loaded successfully! Text length: " << text.length() << " characters." << endl;
}

void manualInput() {
    cout << "\nEnter text (press Enter twice when done):\n";
    cin.ignore();
    
    text = "";
    string line;
    string temp;
    
    while (getline(cin, temp)) {
        if (temp.empty()) {
            getline(cin, temp);
            if (temp.empty()) break;
        }
        if (!text.empty()) text += "\n";
        text += temp;
    }
    
    cout << "Text entered successfully! Text length: " << text.length() << " characters." << endl;
}

// ============ SEARCH FUNCTIONS ============

void highlightMatches(const string& text, const string& pattern, const vector<int>& matches) {
    if (matches.empty()) {
        cout << "\nText: " << text << endl;
        cout << "No matches found." << endl;
        return;
    }
    
    cout << "\nText with highlighted matches:\n";
    string result = text;
    
    // Sort matches in reverse order to avoid index shifting
    vector<int> sortedMatches = matches;
    sort(sortedMatches.rbegin(), sortedMatches.rend());
    
    for (int pos : sortedMatches) {
        result.insert(pos, "[");
        result.insert(pos + pattern.length() + 1, "]");
    }
    
    cout << result << endl;
}

void printStatistics(const string& algorithm, const string& pattern) {
    cout << "\n========== SEARCH STATISTICS ==========" << endl;
    cout << "Algorithm: " << algorithm << endl;
    cout << "Pattern: " << pattern << endl;
    cout << "Number of Comparisons: " << lastStats.comparisons << endl;
    cout << "Number of Matches: " << lastStats.matchCount << endl;
    cout << "Execution Time: " << lastStats.executionTime << " ms" << endl;
    cout << "======================================\n" << endl;
}

void boyerMooreSearch() {
    if (text.empty()) {
        cout << "\nError: Please load or enter text first!" << endl;
        return;
    }
    
    string pattern;
    cout << "\nEnter pattern to search: ";
    cin.ignore();
    getline(cin, pattern);
    
    if (pattern.empty()) {
        cout << "Error: Pattern cannot be empty!" << endl;
        return;
    }
    
    BoyerMoore bm;
    
    auto start = chrono::high_resolution_clock::now();
    vector<int> matches = bm.search(text, pattern, lastStats.comparisons);
    auto end = chrono::high_resolution_clock::now();
    
    lastStats.matchCount = matches.size();
    lastStats.executionTime = chrono::duration<double, milli>(end - start).count();
    lastStats.algorithm = "Boyer-Moore";
    
    cout << "\n--- Boyer-Moore Search Results ---" << endl;
    if (!matches.empty()) {
        cout << "Matches found at indices: ";
        for (int idx : matches) {
            cout << idx << " ";
        }
        cout << endl;
    }
    
    highlightMatches(text, pattern, matches);
    printStatistics("Boyer-Moore", pattern);
}

void rabinKarpSearch() {
    if (text.empty()) {
        cout << "\nError: Please load or enter text first!" << endl;
        return;
    }
    
    string pattern;
    cout << "\nEnter pattern to search: ";
    cin.ignore();
    getline(cin, pattern);
    
    if (pattern.empty()) {
        cout << "Error: Pattern cannot be empty!" << endl;
        return;
    }
    
    RabinKarp rk;
    
    auto start = chrono::high_resolution_clock::now();
    vector<int> matches = rk.search(text, pattern, lastStats.comparisons);
    auto end = chrono::high_resolution_clock::now();
    
    lastStats.matchCount = matches.size();
    lastStats.executionTime = chrono::duration<double, milli>(end - start).count();
    lastStats.algorithm = "Rabin-Karp";
    
    cout << "\n--- Rabin-Karp Search Results ---" << endl;
    if (!matches.empty()) {
        cout << "Matches found at indices: ";
        for (int idx : matches) {
            cout << idx << " ";
        }
        cout << endl;
    }
    
    highlightMatches(text, pattern, matches);
    printStatistics("Rabin-Karp", pattern);
}

void compareAlgorithms() {
    if (text.empty()) {
        cout << "\nError: Please load or enter text first!" << endl;
        return;
    }
    
    string pattern;
    cout << "\nEnter pattern to search: ";
    cin.ignore();
    getline(cin, pattern);
    
    if (pattern.empty()) {
        cout << "Error: Pattern cannot be empty!" << endl;
        return;
    }
    
    BoyerMoore bm;
    RabinKarp rk;
    long long bm_comparisons, rk_comparisons;
    
    // Boyer-Moore search
    auto start_bm = chrono::high_resolution_clock::now();
    vector<int> bm_matches = bm.search(text, pattern, bm_comparisons);
    auto end_bm = chrono::high_resolution_clock::now();
    double bm_time = chrono::duration<double, milli>(end_bm - start_bm).count();
    
    // Rabin-Karp search
    auto start_rk = chrono::high_resolution_clock::now();
    vector<int> rk_matches = rk.search(text, pattern, rk_comparisons);
    auto end_rk = chrono::high_resolution_clock::now();
    double rk_time = chrono::duration<double, milli>(end_rk - start_rk).count();
    
    cout << "\n========== ALGORITHM COMPARISON ==========" << endl;
    cout << "Pattern: " << pattern << endl;
    cout << "Text length: " << text.length() << " characters\n" << endl;
    
    cout << "Boyer-Moore Algorithm:" << endl;
    cout << "  Comparisons: " << bm_comparisons << endl;
    cout << "  Matches: " << bm_matches.size() << endl;
    cout << "  Execution Time: " << bm_time << " ms\n" << endl;
    
    cout << "Rabin-Karp Algorithm:" << endl;
    cout << "  Comparisons: " << rk_comparisons << endl;
    cout << "  Matches: " << rk_matches.size() << endl;
    cout << "  Execution Time: " << rk_time << " ms\n" << endl;
    
    cout << "Winner (Fewer Comparisons): ";
    if (bm_comparisons < rk_comparisons) {
        cout << "Boyer-Moore (" << bm_comparisons << " vs " << rk_comparisons << ")" << endl;
    } else if (rk_comparisons < bm_comparisons) {
        cout << "Rabin-Karp (" << rk_comparisons << " vs " << bm_comparisons << ")" << endl;
    } else {
        cout << "Tie (" << bm_comparisons << " comparisons)" << endl;
    }
    
    cout << "Winner (Faster Execution): ";
    if (bm_time < rk_time) {
        cout << "Boyer-Moore (" << bm_time << "ms vs " << rk_time << "ms)" << endl;
    } else if (rk_time < bm_time) {
        cout << "Rabin-Karp (" << rk_time << "ms vs " << bm_time << "ms)" << endl;
    } else {
        cout << "Tie (" << bm_time << "ms)" << endl;
    }
    cout << "=========================================\n" << endl;
}

// ============ MAIN MENU ============

void displayMenu() {
    cout << "\n========== STRING MATCHING ALGORITHMS ==========" << endl;
    cout << "1. Load Text File" << endl;
    cout << "2. Enter Text Manually" << endl;
    cout << "3. Search Using Boyer-Moore" << endl;
    cout << "4. Search Using Rabin-Karp" << endl;
    cout << "5. Compare Algorithms" << endl;
    cout << "6. Exit" << endl;
    cout << "===============================================" << endl;
    
    if (!text.empty()) {
        cout << "Current text: " << (text.length() > 50 ? text.substr(0, 47) + "..." : text) << endl;
        cout << "Text length: " << text.length() << " characters" << endl;
    }
}

int main() {
    int choice;
    
    cout << "===== STRING PROCESSING ASSIGNMENT - ASSIGNMENT 4 =====" << endl;
    cout << "String Matching using Boyer-Moore and Rabin-Karp Algorithms" << endl;
    cout << "=========================================================\n" << endl;
    
    while (true) {
        displayMenu();
        cout << "\nEnter your choice (1-6): ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                loadFile();
                break;
            case 2:
                manualInput();
                break;
            case 3:
                boyerMooreSearch();
                break;
            case 4:
                rabinKarpSearch();
                break;
            case 5:
                compareAlgorithms();
                break;
            case 6:
                cout << "\nThank you for using the String Matching Program!" << endl;
                cout << "Assignment 4 completed successfully." << endl;
                return 0;
            default:
                cout << "\nInvalid choice! Please enter a number between 1 and 6." << endl;
        }
    }
    
    return 0;
}
