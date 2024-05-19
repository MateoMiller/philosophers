#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

const int number_of_Philosophers = 5;
const int eatingDuration = 1000;
const int thinkingDuration = 1000;
const int iterationsLimit = 15;

std::vector<std::mutex> forks(number_of_Philosophers);
std::vector<std::thread> philosophers(number_of_Philosophers);
std::vector<int> eatingTime(number_of_Philosophers);
std::vector<int> thinkingTime(number_of_Philosophers);


void philosopher(int id)
{
    int leftFork = id;
    int rightFork = (id + 1) % number_of_Philosophers;

    int i = iterationsLimit;

    while (i > 0)
    {
        std::cout << "Philosopher " << id << " is thinking." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(thinkingDuration));

        forks[leftFork].lock();
        forks[rightFork].lock();

        std::cout << "Philosopher " << id << " started eating." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(eatingDuration));

        forks[leftFork].unlock();
        forks[rightFork].unlock();

        std::cout << "Philosopher " << id << " finished eating." << std::endl;

        i--;
    }
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

    return 0;
}