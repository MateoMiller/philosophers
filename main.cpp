#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>

const int number_of_Philosophers = 10;
const int eatingDuration = 100;
const int thinkingDuration = 100;
const int iterationsLimit = 250;

enum PhilosopherState{
    Waiting,
    Eating
};

std::vector<std::mutex> forks(number_of_Philosophers);
std::vector<std::thread> philosophers(number_of_Philosophers);
std::vector<PhilosopherState> philosophersStates(number_of_Philosophers);
std::vector<std::vector<double>> elapsedForPhilosopher(number_of_Philosophers);
std::mutex waiter;

void WaitForMyOrder(int id, int leftForkId, int rightForkId){

    auto& state = philosophersStates.at(id);
    auto& left = philosophersStates.at(leftForkId);
    auto& right = philosophersStates.at(rightForkId);
    while(state != Eating){
        waiter.lock();
        if (left == Waiting || right == Waiting) {
            state = Eating;
        }
        waiter.unlock();
    }
}

void philosopher(int id)
{
    auto& MyState = philosophersStates.at(id);
    int leftFork = id;
    int rightFork = (id + 1) % number_of_Philosophers;

    int i = 0;
    std::vector<double> iterationTime(iterationsLimit);

    while (i < iterationsLimit)
    {
        auto startTime = std::chrono::steady_clock::now();
        std::cout << "P " << id << " th" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(thinkingDuration));


        WaitForMyOrder(id, leftFork, rightFork);
        forks[leftFork].lock();
        forks[rightFork].lock();

        std::cout << "P " << id << " eat" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(eatingDuration));

        forks[leftFork].unlock();
        forks[rightFork].unlock();
        MyState = Waiting;


        std::cout << "P " << id << " fin" << std::endl;

        auto endTime = std::chrono::steady_clock::now();
        iterationTime[i] = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        i++;
    }
    elapsedForPhilosopher.at(id) = iterationTime;
}


int main()
{
    for (int i = 0; i < number_of_Philosophers; ++i)
    {
        philosophers[i] = std::thread(philosopher, i);
    }

    for (int i = 0; i < number_of_Philosophers; ++i)
    {
        philosophers[i].join();
    }

    for (int i = 0; i < elapsedForPhilosopher.size(); i++){
        std::vector<double> elapsed = elapsedForPhilosopher.at(i);
        double min_value = *std::min_element(elapsed.begin(), elapsed.end());

        double max_value = *std::max_element(elapsed.begin(), elapsed.end());

        double sum = std::accumulate(elapsed.begin(), elapsed.end(), 0.0);
        double average_value = sum / elapsed.size();

        std::cout << "Philosopher: " << i << std::endl;
        std::cout << "Min time: " << min_value << std::endl;
        std::cout << "Max time: " << max_value << std::endl;
        std::cout << "Average time: " << average_value << std::endl;
    }

    return 0;
}

double avg(const std::vector<double>& vector)
{
    double total = 0;
    for (double i : vector)
    {
        total += i;
    }

    return total / vector.size();
}