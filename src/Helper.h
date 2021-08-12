#ifndef HELPER_H
#define HELPER_H

#include <ctime>
#include <sys/types.h>
#include <unistd.h>

namespace Helper{
    /* Alternative solution for seeding rand, instead with using only the current time on the machine */
    int GenerateSeedForRand();
}

#endif