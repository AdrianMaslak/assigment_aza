#include <iostream>
#include <vector>

using namespace std;

// Structure to represent a job
struct Job
{
    int job;      // Unique identifier for the job
    int profit;   // Profit associated with the job
    int deadline; // Deadline by which the job should be completed
};

// Function to sort jobs in descending order of profit using Selection Sort
void sort(vector<Job> &jobs)
{
    int n = jobs.size();
    // Iterate through each job
    for (int i = 0; i < n - 1; i++)
    {
        // Assume the current job has the highest profit
        int maxIndex = i;
        // Compare with the remaining jobs to find the one with the highest profit
        for (int j = i + 1; j < n; j++)
        {
            if (jobs[j].profit > jobs[maxIndex].profit)
            {
                maxIndex = j;
            }
        }
        // Swap the job with the highest profit with the current job
        if (maxIndex != i)
        {
            swap(jobs[i], jobs[maxIndex]);
        }
    }
}

// Global vector to store the parent of each slot in the Disjoint Set
vector<int> parent;

// Function to find the representative (root) of the set that contains 'x'
// Implements path compression for optimization
int findSet(int x)
{
    if (parent[x] != x)
        parent[x] = findSet(parent[x]); // Path compression
    return parent[x];
}

// Function to perform the union of two sets containing 'x' and 'y'
void unionSet(int x, int y)
{
    int xset = findSet(x); // Find the root of set containing 'x'
    int yset = findSet(y); // Find the root of set containing 'y'
    if (xset != yset)
        parent[xset] = yset; // Merge the two sets by updating the parent
}

// Function to schedule jobs using the Disjoint Set (Union-Find) approach
int scheduleJobs(vector<Job> &jobs)
{
    // Step 1: Sort all jobs in descending order of profit
    sort(jobs);

    // Step 2: Find the maximum deadline among all jobs to determine the number of slots
    int maxDeadline = 0;
    for (auto job : jobs)
        if (job.deadline > maxDeadline)
            maxDeadline = job.deadline;

    // Step 3: Initialize the parent array where each slot is its own parent initially
    parent.resize(maxDeadline + 1);
    for (int i = 0; i <= maxDeadline; i++)
    {
        parent[i] = i;
    }

    int profit = 0; // Variable to store the total profit from scheduled jobs

    // Step 4: Iterate through each job and schedule it
    for (auto job : jobs)
    {
        // Find the latest available slot for this job
        // min(job.deadline, maxDeadline) ensures we don't exceed the maximum deadline
        int availableSlot = findSet(min(job.deadline, maxDeadline));

        // If there is a free slot available (slot > 0)
        if (availableSlot > 0)
        {
            // Assign the job to this slot and add its profit to the total profit
            profit += job.profit;

            // Merge this slot with the previous slot to mark it as occupied
            // This effectively updates the available slots for future jobs
            unionSet(availableSlot, availableSlot - 1);
        }
        // If no slot is available, the job is rejected (implicitly by not adding its profit)
    }

    return profit; // Return the total profit from all scheduled jobs
}

int main()
{
    // Define a list of jobs with their job ID, profit, and deadline
    vector<Job> jobs = {
        {1, 40, 2},
        {2, 15, 4},
        {3, 60, 3},
        {4, 20, 2},
        {5, 10, 3},
        {6, 45, 1},
        {7, 55, 1}};

    for (auto job : jobs)
    {
        cout << "Job: " << job.job << ", Profit: " << job.profit << ", Deadline: " << job.deadline << endl;
    }
    // Calculate the maximum profit using the Disjoint Set algorithm
    int profit = scheduleJobs(jobs);
    cout << "Max profit (Disjoint Set algorithm): " << profit << endl;

    return 0;
}
