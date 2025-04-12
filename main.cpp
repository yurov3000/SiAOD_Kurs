// main.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>

struct Data {
    char a[30];
    short int b;
    char c[22];
    char d[10];
};

struct Node {
    Data data;
    Node* next;
    Node* prev;
};

// Узел дерева поиска
struct TreeNode {
    Data data;
    TreeNode* left;
    TreeNode* right;
    TreeNode* next;  

    TreeNode(Data d) : data(d), left(nullptr), right(nullptr), next(nullptr) {}
};

struct Symbol {
    unsigned char ch;
    int frequency;
    double probability;
    double q;
    std::string code;
};

int compareStrings(const char* str1, const char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    while (len1 > 0 && str1[len1 - 1] == ' ') {
        len1--;
    }

    for (int i = 0; i < len1 && i < len2; i++) {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
    }
    if (len1 < len2) return -1;
    if (len1 > len2) return 1;
    return 0;
}

void sortArrayByName(Data* array, int* indices, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (compareStrings(array[indices[j]].a, array[indices[j + 1]].a) > 0) {
                std::swap(indices[j], indices[j + 1]);
            }
        }
    }
}

void sortArrayByDepartment(Data* array, int* indices, int size) {
    for (int i = 0; i < size; i++) {
        indices[i] = i;
    }

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[indices[j]].b > array[indices[j + 1]].b) {
                std::swap(indices[j], indices[j + 1]);
            }
        }
    }
}
int getHeight(TreeNode* root) {
    if (!root) return 0;
    return 1 + std::max(getHeight(root->left), getHeight(root->right));
}

bool isBalanced(TreeNode* root) {
    if (!root) return true;

    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);

    if (std::abs(leftHeight - rightHeight) <= 1 &&
        isBalanced(root->left) &&
        isBalanced(root->right))
        return true;

    return false;
}

TreeNode* buildOptimalSearchTree(Data* array, int* indices, int start, int end) {
    if (start > end) return nullptr;

    if (start == 0) {
        sortArrayByDepartment(array, indices, end + 1);
    }

    int mid = start + (end - start) / 2;
    TreeNode* root = new TreeNode(array[indices[mid]]);

    std::cout << "Building tree node with department number: " << root->data.b << std::endl;

    root->left = buildOptimalSearchTree(array, indices, start, mid - 1);
    root->right = buildOptimalSearchTree(array, indices, mid + 1, end);

    return root;
}

void printTree(TreeNode* root, int level = 0) {
    if (!root) return;
    printTree(root->right, level + 1);
    std::cout << "Department: " << root->data.b << " | Name: " << root->data.a << " | Date: " << root->data.d << "\n";
    printTree(root->left, level + 1);
}

void findAllDepartments(TreeNode* root, short int departmentNum, TreeNode*& firstFound) {
    if (!root) return;
    findAllDepartments(root->left, departmentNum, firstFound);

    if (root->data.b == departmentNum) {
        if (!firstFound) {
            firstFound = root;
        }
        else {
            TreeNode* duplicate = new TreeNode(root->data);
            duplicate->next = firstFound->next;
            firstFound->next = duplicate;
        }
    }

    findAllDepartments(root->right, departmentNum, firstFound);
}

TreeNode* searchTree(TreeNode* root, short int departmentNum) {
    TreeNode* firstFound = nullptr;

    findAllDepartments(root, departmentNum, firstFound);

    if (firstFound) {
        std::cout << "\n[FOUND DEPARTMENT " << departmentNum << "]:\n";
        std::cout << "Main record: " << firstFound->data.a << " | Date: " << firstFound->data.d << "\n";

        if (firstFound->next != nullptr) {
            std::cout << "\nDuplicate records found:\n";
            TreeNode* duplicate = firstFound->next;
            int count = 1;
            while (duplicate != nullptr) {
                std::cout << count << ". " << duplicate->data.a
                    << " | Date: " << duplicate->data.d << "\n";
                count++;
                duplicate = duplicate->next;
            }
        }
    }

    return firstFound;
}

void printDuplicates(TreeNode* node) {
    if (node == nullptr) {
        std::cout << "Node not found\n";
        return;
    }

    std::cout << "\n[Department " << node->data.b << " Records]:\n";
    std::cout << "Main record: " << node->data.a << " | Date: " << node->data.d << "\n";

    if (node->next != nullptr) {
        std::cout << "\nDuplicate records:\n";
        TreeNode* current = node->next;
        int count = 1;
        while (current != nullptr) {
            std::cout << count << ". " << current->data.a
                << " | Date: " << current->data.d << "\n";
            count++;
            current = current->next;
        }
    }
    else {
        std::cout << "No duplicates found.\n";
    }
}

TreeNode* insertNode(TreeNode* root, Data data) {
    if (root == nullptr) {
        return new TreeNode(data);
    }

    if (data.b == root->data.b) {
        TreeNode* newNode = new TreeNode(data);
        newNode->next = root->next;
        root->next = newNode;
        return root;
    }

    if (data.b < root->data.b) {
        root->left = insertNode(root->left, data);
    }
    else {
        root->right = insertNode(root->right, data);
    }

    return root;
}

void deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

void push_back(Node*& head, Node*& tail, const Data& data) {
    Node* newNode = new Node{ data, nullptr, tail };
    if (tail) {
        tail->next = newNode;
    }
    else {
        head = newNode;
    }
    tail = newNode;
}

Data* listToArray(Node* head, int& size) {
    size = 0;
    Node* current = head;

    while (current) {
        ++size;
        current = current->next;
    }

    Data* array = new Data[size];

    current = head;
    for (int i = 0; i < size; ++i) {
        array[i] = current->data;
        current = current->next;
    }

    return array;
}

int convertDateToInt(const char* date) {
    std::string cleanedDate;
    for (int i = 6; date[i] != '\0'; ++i) {
        if (date[i] != '-') {
            cleanedDate += date[i];
        }
    }
    return std::atoi(cleanedDate.c_str());
}

void binarySearchAll(Data* array, int target, int size, int*& foundIndices, int& foundCount) {
    foundCount = 0;
    int left = 0;
    int right = size - 1;
    int mid = -1;

    while (left < right) {
        mid = left + (right - left) / 2;
        int midDate = convertDateToInt(array[mid].d);

        if (midDate < target) {
            left = mid + 1;
        }
        else {
            right = mid;
        }
    }
    if (convertDateToInt(array[left].d) != target) {
        foundIndices = nullptr;
        return;
    }

    int leftIdx = left;
    int rightIdx = left;

    while (rightIdx < size && convertDateToInt(array[rightIdx].d) == target) {
        rightIdx++;
    }

    foundCount = rightIdx - leftIdx;
    foundIndices = new int[foundCount];
    for (int i = 0; i < foundCount; ++i) {
        foundIndices[i] = leftIdx + i;
    }
}

void createAndPrintQueue(Data* array, int size, int*& foundIndices, int& foundCount) {
    Node* tempHead = nullptr;
    Node* tempTail = nullptr;
    std::cout << "\n";

    for (int i = 0; i < foundCount; ++i) {
        push_back(tempHead, tempTail, array[foundIndices[i]]);
    }

    int index = 1;
    Node* current = tempHead;
    while (current != nullptr) {
        std::cout << "| " << index++ << ". " << current->data.a << "\t" << current->data.b
            << "\t" << current->data.c << "\t" << current->data.d << " |" << "\n";
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

void readAndCountFrequencies(const char* filename, std::vector<Symbol>& symbols, int& totalChars) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }
    symbols.resize(256);
    for (int i = 0; i < 256; i++) {
        symbols[i].ch = static_cast<unsigned char>(i);
        symbols[i].frequency = 0;
    }

    unsigned char ch;
    totalChars = 0;
    while (file.read(reinterpret_cast<char*>(&ch), sizeof(ch))) {
        symbols[ch].frequency++;
        totalChars++;
    }

    file.close();
}

void calculateProbabilities(std::vector<Symbol>& symbols, int totalChars) {
    for (auto& symbol : symbols) {
        symbol.probability = static_cast<double>(symbol.frequency) / totalChars;
    }

    std::sort(symbols.begin(), symbols.end(), [](const Symbol& a, const Symbol& b) {
        return a.ch < b.ch;
        });
}

void calculateCumulativeProbabilities(std::vector<Symbol>& symbols) {
    double cumulative = 0.0;
    for (auto& symbol : symbols) {
        symbol.q = cumulative + (symbol.probability / 2.0);
        cumulative += symbol.probability;
    }
}

double calculateEntropy(const std::vector<Symbol>& symbols) {
    double entropy = 0.0;
    for (const auto& symbol : symbols) {
        if (symbol.probability > 0) {
            entropy -= symbol.probability * std::log2(symbol.probability);
        }
    }
    return entropy;
}

double calculateAverageLength(const std::vector<Symbol>& symbols) {
    double avgLength = 0.0;
    for (const auto& symbol : symbols) {
        avgLength += symbol.probability * symbol.code.length();
    }
    return avgLength;
}

void generateGilbertMooreCodes(std::vector<Symbol>& symbols) {
    if (symbols.empty()) return;

    int totalChars = 0;
    for (const auto& symbol : symbols) {
        totalChars += symbol.frequency;
    }

    calculateProbabilities(symbols, totalChars);

    calculateCumulativeProbabilities(symbols);

    for (auto& symbol : symbols) {
        double q = symbol.q;
        symbol.code = "";
        int codeLength = static_cast<int>(-std::log2(symbol.probability) + 2);

        for (int i = 0; i < codeLength; ++i) {
            q *= 2;
            if (q >= 1.0) {
                symbol.code += "1";
                q -= 1.0;
            }
            else {
                symbol.code += "0";
            }
        }
    }

    double entropy = calculateEntropy(symbols);
    double avgLength = calculateAverageLength(symbols);
    double redundancy = avgLength - entropy;
    std::cout << "\nGilbert-Moore Codes (Alphabetically sorted):\n";
    std::cout << "Symbol\tFreq\tProb\t\tCode\t\tLength\n";
    std::cout << "--------------------------------------------------------\n";

    for (const auto& symbol : symbols) {
        std::cout << symbol.ch << "\t"
            << symbol.frequency << "\t"
            << std::fixed << std::setprecision(6) << symbol.probability << "\t"
            << symbol.code << "\t\t"
            << symbol.code.length() << "\n";
    }

    std::cout << "\nCode Characteristics:\n";
    std::cout << "--------------------------------------------------------\n";
    std::cout << "Entropy (H): " << std::fixed << std::setprecision(6) << entropy << " bits\n";
    std::cout << "Average Code Length (L): " << avgLength << " bits\n";
    std::cout << "Redundancy (L - H): " << redundancy << " bits\n";
    std::cout << "Relative Redundancy ((L - H)/H): " << (redundancy / entropy) * 100 << "%\n\n";
}

void processGilbertMooreCoding(const char* filename) {
    std::vector<Symbol> symbols;
    int totalChars = 0;
    readAndCountFrequencies(filename, symbols, totalChars);
    symbols.erase(
        std::remove_if(symbols.begin(), symbols.end(),
            [](const Symbol& s) { return s.frequency == 0; }),
        symbols.end()
    );

    generateGilbertMooreCodes(symbols);
}

void showPage(Node* head) {
    Node* it = head;
    char choice;
    const int pageSize = 20;
    int size = 0;
    char searchTarget[10];
    Data* array = listToArray(head, size);
    int processedDate;
    int countSymbols;

    std::vector<Symbol> symbols;
    int totalChars = 0;
    double entropy;
    double avgLength;

    while (true) {
        for (int i = 0; i < pageSize && it != nullptr; ++i) {
            std::cout << it->data.a << "\t" << it->data.b << "\t"
                << it->data.c << "\t" << it->data.d << "\n";
            it = it->next;
        }

        std::cout << "N/n - next page, P/p - previous page, Q/q - quit, S/s - search, G/g - show codes" << std::endl;
        std::cin >> choice;

        switch (choice) {
        case 'N':
        case 'n':
            if (it == nullptr) it = head;
            break;
        case 'P':
        case 'p':
            for (int i = 0; i < pageSize && it != nullptr; ) {
                if (it->prev == nullptr) break;
                it = it->prev;
                ++i;
            }
            break;
        case 'G':
        case 'g':
            processGilbertMooreCoding("testBase2.dat");
            break;
        case 'S':
        case 's': {
            std::cout << "[INPUT YEAR]: ";
            std::cin >> searchTarget;

            processedDate = convertDateToInt(searchTarget);
            int* foundIndices = nullptr;
            int foundCount = 0;
            binarySearchAll(array, processedDate, size, foundIndices, foundCount);

            if (foundCount > 0) {
                std::cout << "\n[RECORDS FOUND]:\n";
                std::cout << "===================================================================================";
                createAndPrintQueue(array, size, foundIndices, foundCount);
                std::cout << "===================================================================================";
                std::cout << "\n\n";
                TreeNode* root = buildOptimalSearchTree(array, foundIndices, 0, foundCount - 1);
                std::cout << "\n[OPTIMAL SEARCH TREE]:\n";
                printTree(root);
                std::cout << "\n";
                short int searchDepartment;
                std::cout << "Enter the department number:\n";
                std::cin >> searchDepartment;

                TreeNode* result = searchTree(root, searchDepartment);
                if (result) {
                    std::cout << "\n[FOUND]:\n";
                    std::cout << "|" << result->data.a << " " << result->data.d << "|\n\n";
                    printDuplicates(result);
                }
                else {
                    std::cout << "\n[NOT FOUND]: No department with the number '" << searchDepartment << "' exists in the tree.\n";
                }
                deleteTree(root);
                delete[] foundIndices;
            }
            else {
                std::cout << "[NO RECORDS FOUND FOR DATE]: " << searchTarget << "\n\n";
            }
            break;
        }
        case 'Q':
        case 'q':
            delete[] array;
            return;
        default:
            break;
        }

        if (it == nullptr) it = head;
    }
}

void extractDate(const char* date, int& day, int& month, int& year) {
    day = (date[0] - '0') * 10 + (date[1] - '0');
    month = (date[3] - '0') * 10 + (date[4] - '0');
    year = (date[6] - '0') * 10 + (date[7] - '0');
}

void DigitalSort(Node** head) {
    if (*head == nullptr) return;

    Node* buckets[100] = { nullptr };
    Node* tails[100] = { nullptr };

    for (int j = 0; j < 3; j++) {
        Node* current = *head;

        while (current != nullptr) {
            Node* next = current->next;

            int day, month, year;
            extractDate(current->data.d, day, month, year);

            int key;
            if (j == 0) {
                key = day;
            }
            else if (j == 1) {
                key = month;
            }
            else {
                key = year;
            }

            if (buckets[key] == nullptr) {
                buckets[key] = current;
            }
            else {
                tails[key]->next = current;
                current->prev = tails[key];
            }
            tails[key] = current;
            current->next = nullptr;

            current = next;
        }
        Node* newHead = nullptr;
        Node* newTail = nullptr;

        for (int i = 0; i < 100; ++i) {
            if (buckets[i] != nullptr) {
                if (newHead == nullptr) {
                    newHead = buckets[i];
                }
                else {
                    newTail->next = buckets[i];
                    buckets[i]->prev = newTail;
                }
                newTail = tails[i];
                buckets[i] = tails[i] = nullptr;
            }
        }

        *head = newHead;
    }
}



int main() {

    std::ifstream file("testBase2.dat", std::ios::binary);
    if (!file.is_open()) {
        std::cout << "File isn't open!" << std::endl;
        return 1;
    }

    Node* head = nullptr;
    Node* tail = nullptr;

    while (true) {
        Data temp;
        file.read(reinterpret_cast<char*>(&temp), sizeof(Data));
        if (file.gcount() == 0) break;
        push_back(head, tail, temp);
    }

    DigitalSort(&head);
    showPage(head);

    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }

    file.close();
    return 0;
}
