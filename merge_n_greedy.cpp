#include <iostream>
#include <vector>
using namespace std;

void swapInts(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}
// Function to heapify a subtree rooted at index 'i' in the heap array
void heapify(vector<int> &heap, int size, int i)
{
    int smallest = i;      // Initialize smallest as root
    int left = 2 * i + 1;  // Left child index
    int right = 2 * i + 2; // Right child index
    // If left child exists and is smaller than root
    if (left < size && heap[left] < heap[smallest])
        smallest = left;
    // If right child exists and is smaller than the smallest so far
    if (right < size && heap[right] < heap[smallest])
        smallest = right;
    // If smallest is not root
    if (smallest != i)
    {
        swapInts(heap[i], heap[smallest]);
        heapify(heap, size, smallest);
    }
}
// Function to build a min-heap from an unsorted array
void buildMinHeap(vector<int> &heap)
{
    int size = heap.size();
    // Start from the last non-leaf node and heapify each node
    for (int i = size / 2 - 1; i >= 0; i--)
        heapify(heap, size, i);
}
// Function to insert a new key into the heap
void insertHeap(vector<int> &heap, int key)
{
    heap.push_back(key); // Insert the new key at the end
    int i = heap.size() - 1;
    while (i != 0 && heap[(i - 1) / 2] > heap[i])
    {
        swapInts(heap[i], heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}
int extractMin(vector<int> &heap)
{
    int size = heap.size();
    if (size <= 0)
    {
        cout << "Heap underflow. No elements to extract.\n";
        return -1;
    }
    if (size == 1)
    {
        int root = heap[0];
        heap.pop_back();
        return root;
    }
    int root = heap[0];
    heap[0] = heap[size - 1];
    heap.pop_back();
    heapify(heap, heap.size(), 0);
    return root;
}
void greedyMerge(vector<int> files, int n)
{
    vector<int> heap = files;
    buildMinHeap(heap);
    int totalRecordMoves = 0;
    cout << "Merge Steps:\n";
    cout << "---------------------------\n";
    while (heap.size() > 1)
    {
        int first = extractMin(heap);
        int second = extractMin(heap);
        int merged = first + second;
        totalRecordMoves += merged;
        cout << "Merged " << first << " and " << second << " into " << merged
             << " (Total Record Moves: " << totalRecordMoves << ")\n";
        insertHeap(heap, merged);
    }
    cout << "---------------------------\n";
    cout << "Total Record Moves: " << totalRecordMoves << "\n\n";
}
int main()
{
    int n;
    cout << "Enter the number of files: ";
    cin >> n;
    if (n <= 0)
    {
        cout << "Number of files must be positive.\n";
        return 1;
    }
    vector<int> userFiles(n);
    cout << "Enter the sizes of the files:\n";
    for (int i = 0; i < n; ++i)
    {
        cout << "Size of file " << i + 1 << ": ";
        cin >> userFiles[i];
    }
    greedyMerge(userFiles, n);
    return 0;
}