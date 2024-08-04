
#include <iostream>
#include <string>
#include <cstring>

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

    // Choose a random door to have the prize behind (true means prize, false means empty)
    doors[0] = (num % 3) == 0;
    doors[1] = ((num + 1) % 3) == 0;
    doors[2] = ((num + 2) % 3) == 0;

    // Pick the player's originally chosen door
    int playerDoor = rand() % 3;
    int openedDoor = 0;

    // Choose the door opened by the gameshow host as the first door found that is empty and not chosen by the player
    for (int i = 0; i < 3; i++)
    {
        if (i != playerDoor && !doors[i])
        {
            openedDoor = i;
        }
    }

    // Randomly decide whether to switch doors or keep doors
    if (rand() % 2)
    {
        result.switched = true;
        // Find the door to switch to and set success if the prize is behind it
        for (int i = 0; i < 3; i++)
        {
            if (i != playerDoor && i != openedDoor)
                result.success = doors[i];
        }
    }
    else
    {
	// Stick with the player's first door and set success if the prize is behind it
        result.switched = false;
        result.success = doors[playerDoor];
    }

    return result;
}

// Add the results of one simulation to the tally. Only add another stay attempt if result.switched is false, and only add another switch attempt if result.switched is true, same for the success value
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
    // Seed the random number generator
    srand((unsigned int)time(0));
    // Check the command line arguments
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

    // Report the success rates as a fraction of the successes/attempts multiplied by 100 (percentage) unless there were no attempts of that type made.
    if (tally.switchAttempts < 1)
        std::cout << "No simulations where the player switched their choice were performed.\n";
    else
        std::cout << "Switch Success Rate was: " << 100.0 * (double)tally.switchSuccesses / (double)tally.switchAttempts << "%\n";

    if (tally.stayAttempts < 1)
        std::cout << "No simulations where the player kept their choice were performed.\n";
    else
        std::cout << "Stay Success Rate was: " << 100.0 * (double)tally.staySuccesses / (double)tally.stayAttempts << "%\n";
}
