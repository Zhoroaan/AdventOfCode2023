#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

int main(int argc, char* argv[])
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;

    std::getline(inputFile, inputLine);
    std::string instructionSequence = inputLine;
    std::getline(inputFile, inputLine); // skip the empty line
    std::unordered_map<std::string, std::pair<std::string,std::string>> movementMapping;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        
        char from[4], left[4], right[4];
        int result = sscanf_s(inputLine.c_str(),"%3s = (%3s, %3s)",
            from, static_cast<unsigned>(_countof(from)),
            left, static_cast<unsigned>(_countof(left)),
            right, static_cast<unsigned>(_countof(right)));
        movementMapping[from] = std::make_pair(left, right);
    }

    std::string currentEntry = "AAA";
    for (int64_t steps = 1; ;steps++)
    {
        if (steps % 100000000 == 0)
            std::cout << "Calculating at: " << steps << std::endl;
        if (instructionSequence[(steps - 1) % instructionSequence.length()] == 'L')
            currentEntry = movementMapping[currentEntry].first;
        else
            currentEntry = movementMapping[currentEntry].second;

        if (currentEntry == "ZZZ")
        {
            std::cout << "Day 8 Part 1: " << steps << " steps needed" << std::endl;
            break;
        }
    }

    //std::cout << "Day 8" << std::endl;
    
    return 0;
}
