#pragma once
#include <functional>
#include <thread>
#include <vector>
#include <utility>
#include <chrono>

#define print(x) std::cout << x << "\n"

#define CoroutineSpeedMs (int)1//Call duration

typedef struct Timer{
    long long timeVal;
public:
    Timer(long long value)
    {
        this->timeVal = value;
    }
    Timer ToSeconds() const
    {
        return Timer(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(this->timeVal)).count());
    }
    Timer ToMinutes() const
    {
        return Timer(std::chrono::duration_cast<std::chrono::minutes>(std::chrono::milliseconds(this->timeVal)).count());
    }
    operator long long()
    {
        return this->timeVal;
    }
    Timer operator ++(int other)
    {
        this->timeVal++;
        return *this;
    }
}
Time;

typedef std::function<void(Time)> CoroutineFunc;

static std::thread* MainThread;
static bool isInitializedThread = false;
static std::vector<std::pair<CoroutineFunc*, Time>> ListCoroutine;

void MainCoroutineLoop()
{
    auto LastTime = std::chrono::system_clock::from_time_t(2), CurrentTime = std::chrono::system_clock::now();
    while (1)
    {
        CurrentTime = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastTime).count() < CoroutineSpeedMs)
            continue;
        for (long long j = 0; j < ListCoroutine.size(); j++)
            (*ListCoroutine[j].first)(ListCoroutine[j].second++);

        LastTime = std::chrono::system_clock::now();
    }
}

void InitializeCoroutine()
{
    if (isInitializedThread)
        return;
    MainThread = new std::thread(MainCoroutineLoop);
    isInitializedThread = true;
}

class Coroutine
{
    CoroutineFunc* Function;
    bool isStopped = true;
public:

    Coroutine(CoroutineFunc Func)
    {
        Function = new CoroutineFunc(Func);
        Play();
    }
    CoroutineFunc* operator&()
    {
        return Function;
    }
    operator bool()
    {
        return isStopped;
    }
    void Stop()
    {
        for (uint16_t j = 0; j < ListCoroutine.size(); j++)
            if (ListCoroutine[j].first == (void*)Function)
            {
                ListCoroutine.erase(ListCoroutine.begin() + j);
                this->isStopped = true;
                break;
            }
    }
    void Play()
    {
        if (!isStopped)
            return;
        ListCoroutine.push_back(std::make_pair(Function, 0));
        this->isStopped = false;
    }
    void Break()
    {
        this->Stop();
        free(Function);
    }
};