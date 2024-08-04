
#include <iostream>
#include <string>

struct SimResult
{
    bool switched;
    bool success;
};

struct ResultTally
{
    int switchSuccesses;
    int switchAttempts;
    int staySuccesses;
    int stayAttempts;
    int numSim;
};

static SimResult RunSim()
{
    SimResult result = {};
    bool doors[3] = {};
    int num = rand();
    doors[0] = (num % 3) == 0;
    doors[1] = ((num + 1) % 3) == 0;
    doors[2] = ((num + 2) % 3) == 0;
    int playerDoor = rand() % 3;
    int openedDoor = 0;
    for (int i = 0; i < 3; i++)
    {
        if (i != playerDoor && !doors[i])
        {
            openedDoor = i;
        }
    }
    if (rand() % 2)
    {
        result.switched = true;
        for (int i = 0; i < 3; i++)
        {
            if (i != playerDoor && i != openedDoor)
                result.success = doors[i];
        }
    }
    else
    {
        result.switched = false;
        result.success = doors[playerDoor];
    }

    return result;
}

static void TallyResult(SimResult result, ResultTally& tally)
{
    tally.stayAttempts += !result.switched;
    tally.switchAttempts += result.switched;
    tally.staySuccesses += !result.switched * result.success;
    tally.switchSuccesses += result.switched * result.success;
    tally.numSim++;
}

int main(int argc, char **argv)
{
    srand((unsigned int)time(0));
    if (argc < 2)
    {
        std::cout << "The number of simulations must be specified.\n";
        return -1;
    }
    std::string argString(argv[1]);
    if (strspn(argString.c_str(), "0123456789") != argString.size())
    {
        std::cout << "Argument was not a positive integer.\n";
        return -1;
    }

    int numSim = std::stoi(argString);
    ResultTally tally = {};
    tally.numSim = numSim;

    // Run the simulation numSim times, tallying every result.
    for (int i = 0; i < numSim; i++)
    {
        TallyResult(RunSim(), tally);
    }

    std::cout << "Switch Success Rate was: " << 100.0 * (double)tally.switchSuccesses / (double)tally.switchAttempts << "%\n";
    std::cout << "Stay Success Rate was: " << 100.0 * (double)tally.staySuccesses / (double)tally.stayAttempts << "%\n";
}
