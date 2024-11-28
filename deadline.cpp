#include <iostream>
#include <vector>

using namespace std;

struct Job
{
    int job;
    int profit;
    int deadline;
};

void sortJobsByProfit(vector<Job> &jobs)
{
    int n = jobs.size();
    for (int i = 0; i < n - 1; i++)
    {
        // Assume the current job is the highest
        int maxIndex = i;
        for (int j = i + 1; j < n; j++)
        {
            if (jobs[j].profit > jobs[maxIndex].profit)
            {
                maxIndex = j;
            }
        }
        // Swap the found job with the first unsorted job
        if (maxIndex != i)
        {
            swap(jobs[i], jobs[maxIndex]);
        }
    }
}

int scheduleJobs(vector<Job> &jobs)
{

    sortJobsByProfit(jobs);
    int maxDeadline = 0;
    for (auto job : jobs)
    {
        if (job.deadline > maxDeadline)
        {
            maxDeadline = job.deadline;
        }
    }
    // cout << maxDeadline;

    vector<bool> slots(maxDeadline + 1, false); // Slots indexed from 1 to maxDeadline
    int profit = 0;

    for (auto job : jobs)
    {
        for (int j = job.deadline; j > 0; j--)
        {
            if (slots[j] == false)
            {
                slots[j] = true;
                profit += job.profit;
                break;
            }
        }
    }
    return profit;
}

int main()
{
    // vector of jobs
    vector<Job> jobs = {
        {1, 40, 2},
        {2, 15, 4},
        {3, 60, 3},
        {4, 20, 2},
        {5, 10, 3},
        {6, 45, 1},
        {7, 55, 1}};

    // Print sorted jobs
    for (auto job : jobs)
    {
        cout << "Job: " << job.job << ", Profit: " << job.profit << ", Deadline: " << job.deadline << endl;
    }

    cout << "Max profit: " << scheduleJobs(jobs) << endl;
    return 0;
}
