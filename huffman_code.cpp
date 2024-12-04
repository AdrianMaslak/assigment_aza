#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <bitset>

using namespace std;

// Structure for a node in the heap
struct Node
{
    char character;
    int frequency;
    int depth; // Depth in the simulated tree
    Node *left, *right;

    // Constructor for leaf nodes
    Node(char c, int freq) : character(c), frequency(freq), depth(0), left(nullptr), right(nullptr) {}

    // Constructor for internal nodes
    Node(int freq, Node *l, Node *r) : character('\0'), frequency(freq), depth(0), left(l), right(r) {}
};

// Comparator for the priority queue (min-heap)
struct Compare
{
    bool operator()(Node *a, Node *b)
    {
        return a->frequency > b->frequency; // Smaller frequency has higher priority
    }
};

// Assign code lengths recursively
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
unordered_map<char, string> generateCanonicalCodes(vector<pair<char, int>> &symbols, unordered_map<char, int> &codeLengths)
{
    // Sort by code length, then lexicographically
    sort(symbols.begin(), symbols.end(), [&](const pair<char, int> &a, const pair<char, int> &b)
         {
        if (codeLengths[a.first] == codeLengths[b.first])
            return a.first < b.first;
        return codeLengths[a.first] < codeLengths[b.first]; });

    unordered_map<char, string> codes;
    int code = 0;
    int currentLength = codeLengths[symbols[0].first];

    for (const auto &sym : symbols)
    {
        // Adjust the code length
        while (currentLength < codeLengths[sym.first])
        {
            code <<= 1; // Left shift to increase code length
            ++currentLength;
        }

        // Convert code to binary string
        codes[sym.first] = bitset<32>(code).to_string().substr(32 - codeLengths[sym.first]);

        // Increment code for the next symbol
        ++code;
    }

    return codes;
}

int main()
{
    // Input: character frequencies
    vector<pair<char, int>> frequencies = {
        {'A', 45},
        {'B', 13},
        {'C', 12},
        {'D', 16},
        {'E', 9},
        {'F', 5}};

    // Create a min-heap (priority queue)
    priority_queue<Node *, vector<Node *>, Compare> minHeap;

    // Insert all characters into the heap
    for (const auto &entry : frequencies)
    {
        minHeap.push(new Node(entry.first, entry.second));
    }

    // Build the Huffman structure using the min-heap
    while (minHeap.size() > 1)
    {
        Node *left = minHeap.top();
        minHeap.pop();
        Node *right = minHeap.top();
        minHeap.pop();

        Node *merged = new Node(left->frequency + right->frequency, left, right);
        minHeap.push(merged);
    }

    // Root of the simulated tree
    Node *root = minHeap.top();

    // Assign code lengths using a recursive function
    unordered_map<char, int> codeLengths;
    assignCodeLengths(root, 0, codeLengths);

    // Generate canonical Huffman codes
    unordered_map<char, string> canonicalCodes = generateCanonicalCodes(frequencies, codeLengths);

    // Print canonical Huffman codes
    cout << "Canonical Huffman Codes:" << endl;
    for (const auto &code : canonicalCodes)
    {
        cout << code.first << ": " << code.second << endl;
    }

    return 0;
}
