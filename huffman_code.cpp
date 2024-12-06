#include <iostream>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <cmath>

using namespace std;

struct Node
{
    char character;
    int frequency;
    int depth;
    Node *left, *right;

    Node(char c, int freq) : character(c), frequency(freq), depth(0), left(nullptr), right(nullptr) {}
    Node(int freq, Node *l, Node *r) : character('\0'), frequency(freq), depth(0), left(l), right(r) {}
};

struct MinHeap
{
    vector<Node *> heap;

    void push(Node *node)
    {
        heap.push_back(node);
        size_t idx = heap.size() - 1;
        while (idx > 0)
        {
            size_t parent = (idx - 1) / 2;
            if (heap[parent]->frequency <= heap[idx]->frequency)
                break;
            swap(heap[parent], heap[idx]);
            idx = parent;
        }
    }

    Node *pop()
    {
        if (heap.empty())
            return nullptr;
        Node *minNode = heap[0];
        heap[0] = heap.back();
        heap.pop_back();

        size_t idx = 0;
        while (true)
        {
            size_t left = 2 * idx + 1, right = 2 * idx + 2, smallest = idx;
            if (left < heap.size() && heap[left]->frequency < heap[smallest]->frequency)
                smallest = left;
            if (right < heap.size() && heap[right]->frequency < heap[smallest]->frequency)
                smallest = right;
            if (smallest == idx)
                break;
            swap(heap[idx], heap[smallest]);
            idx = smallest;
        }
        return minNode;
    }

    bool empty()
    {
        return heap.empty();
    }
};

void bubbleSort(vector<pair<char, int>> &symbols, unordered_map<char, int> &codeLengths)
{
    for (size_t i = 0; i < symbols.size(); ++i)
    {
        for (size_t j = 0; j < symbols.size() - i - 1; ++j)
        {
            bool condition = (codeLengths[symbols[j].first] > codeLengths[symbols[j + 1].first]) ||
                             (codeLengths[symbols[j].first] == codeLengths[symbols[j + 1].first] &&
                              symbols[j].first > symbols[j + 1].first);
            if (condition)
            {
                swap(symbols[j], symbols[j + 1]);
            }
        }
    }
}

void assignCodeLengths(Node *node, int depth, unordered_map<char, int> &codeLengths)
{
    if (!node)
        return;

    if (!node->left && !node->right)
    { // Leaf node
        codeLengths[node->character] = depth;
    }

    assignCodeLengths(node->left, depth + 1, codeLengths);
    assignCodeLengths(node->right, depth + 1, codeLengths);
}

// Generate canonical codes
unordered_map<char, string> CanonicalCode(vector<pair<char, int>> &symbols, unordered_map<char, int> &codeLengths)
{
    bubbleSort(symbols, codeLengths);

    unordered_map<char, string> codes;
    int code = 0;
    int currentLength = codeLengths[symbols[0].first];

    for (const auto &sym : symbols)
    {
        while (currentLength < codeLengths[sym.first])
        {
            code <<= 1; // Left shift to increase code length
            ++currentLength;
        }

        codes[sym.first] = bitset<32>(code).to_string().substr(32 - codeLengths[sym.first]);
        ++code;
    }

    return codes;
}

unordered_map<char, string> FreqApprox(vector<pair<char, int>> &symbols, unordered_map<char, int> &approxCodeLengths)
{
    bubbleSort(symbols, approxCodeLengths);

    unordered_map<char, string> approxCodes;
    int code = 0;
    int currentLength = 0;

    for (const auto &sym : symbols)
    {
        while (currentLength < approxCodeLengths[sym.first])
        {
            code <<= 1; // Left shift to increase code length
            ++currentLength;
        }

        approxCodes[sym.first] = bitset<32>(code).to_string().substr(32 - approxCodeLengths[sym.first]);
        ++code;
    }

    return approxCodes;
}

int main()
{
    vector<pair<char, int>> frequencies = {
        {'A', 45},
        {'B', 13},
        {'C', 12},
        {'D', 16},
        {'E', 9},
        {'F', 5}};

    int totalFrequency = 0;
    for (const auto &entry : frequencies)
    {
        totalFrequency += entry.second;
    }

    // Assign code lengths using ceil(-log2(probability))
    unordered_map<char, int> approxCodeLengths;
    for (const auto &entry : frequencies)
    {
        double probability = static_cast<double>(entry.second) / totalFrequency;
        approxCodeLengths[entry.first] = static_cast<int>(ceil(-log2(probability)));
    }

    MinHeap minHeap;
    for (const auto &entry : frequencies)
    {
        minHeap.push(new Node(entry.first, entry.second));
    }

    while (minHeap.heap.size() > 1)
    {
        Node *left = minHeap.pop();
        Node *right = minHeap.pop();
        Node *merged = new Node(left->frequency + right->frequency, left, right);
        minHeap.push(merged);
    }

    Node *root = minHeap.pop();

    unordered_map<char, int> trueCodeLengths;
    assignCodeLengths(root, 0, trueCodeLengths);

    unordered_map<char, string> canonicalCodes = CanonicalCode(frequencies, trueCodeLengths);

    unordered_map<char, string> approxCodes = FreqApprox(frequencies, approxCodeLengths);

    cout << "CHAR\tFREQ\tCANONICAL\tFREQ BASED" << endl;

    for (const auto &entry : frequencies)
    {
        char character = entry.first;
        cout << character << '\t'
             << entry.second << '\t'
             << canonicalCodes[character] << '\t'
             << approxCodes[character] << endl;
    }

    return 0;
}
