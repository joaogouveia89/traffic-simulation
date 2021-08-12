#include "Helper.h"

int Helper::GenerateSeedForRand(){
    time_t currentTime = std::time(0);
    // thanks to https://www.systutorials.com/how-to-get-the-running-process-pid-in-c-c/
    pid_t pid = getpid();

    return currentTime+ 1000 * pid;
}
