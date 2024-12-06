#include <iostream>
#include <vector>

using namespace std;

struct Job
{
    int job;
    int profit;
    int deadline;
};

void sort(vector<Job> &jobs)
{
    int n = jobs.size();
    for (int i = 0; i < n - 1; i++)
    {
        int maxIndex = i;
        for (int j = i + 1; j < n; j++)
        {
            if (jobs[j].profit > jobs[maxIndex].profit)
            {
                maxIndex = j;
            }
        }
        if (maxIndex != i)
        {
            swap(jobs[i], jobs[maxIndex]);
        }
    }
}

vector<int> parent;

// Function to find the representative (root) of the set that contains 'x'
int findSet(int x)
{
    if (parent[x] != x)
        parent[x] = findSet(parent[x]);
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

int scheduleJobs(vector<Job> &jobs)
{
    sort(jobs);

    int maxDeadline = 0;
    for (auto job : jobs)
        if (job.deadline > maxDeadline)
            maxDeadline = job.deadline;

    parent.resize(maxDeadline + 1);
    for (int i = 0; i <= maxDeadline; i++)
    {
        parent[i] = i;
    }

    int profit = 0;
    for (auto job : jobs)
    {
        // Find the latest available slot for this job
        // min(job.deadline, maxDeadline) ensures we dont exceed the maximum deadline
        int availableSlot = findSet(min(job.deadline, maxDeadline));

        if (availableSlot > 0)
        {
            profit += job.profit;

            // Merge this slot with the previous slot to mark it as occupied
            unionSet(availableSlot, availableSlot - 1);
        }
    }

    return profit;
}

int main()
{
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
    int profit = scheduleJobs(jobs);
    cout << "Max profit (Disjoint Set algorithm): " << profit << endl;

    return 0;
}
