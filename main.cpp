#include <iostream>
#include "Coroutine.hpp"


int main()
{
    {//Example code
        InitializeCoroutine();
        Coroutine b = (Coroutine) [&](Time x)
        {
            print("MAIN PROCESS");
        };
        Coroutine c = (Coroutine) [&](Time x)
        {
            if (x.ToSeconds()>2)
            {
                b.Stop();
                print("STOPPED PROCESS");
            }
            if (x==7)
            {
                b.Play();
                print("STARTED PROCESS");
            }
                

        };
        while (1);
    }

}