#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <numeric>
std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

uint64_t LCM(const std::vector<uint64_t>& InNumbers)
{
    if (InNumbers.empty())
        return 0;

    if (InNumbers.size() == 1)
        return InNumbers[0];

    uint64_t result = InNumbers[0];
    for (uint64_t i = 1; i < InNumbers.size(); i++)
        result = std::lcm(result, InNumbers[i]);
    return result;
}

int main(int /*argc*/, char* /*argv*/[])
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;

    std::getline(inputFile, inputLine);
    std::string instructionSequence = inputLine;
    std::getline(inputFile, inputLine); // skip the empty line
    std::unordered_map<std::string, std::pair<std::string, std::string>> movementMapping;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        
        char from[4], left[4], right[4];
        sscanf_s(inputLine.c_str(),"%3s = (%3s, %3s)",
                 from, static_cast<unsigned>(_countof(from)),
                 left, static_cast<unsigned>(_countof(left)),
                 right, static_cast<unsigned>(_countof(right)));
        movementMapping[from] = std::make_pair(left, right);
    }

    std::vector<uint64_t> stepCount;
    // These steps are cyclical so we only need to know when they all align
    for (const auto& movement : movementMapping)
    {
        if (movement.first[2] != 'A')
            continue;

        std::string node = movement.first;

        for (int64_t steps = 1; ;steps++)
        {
            if (instructionSequence[(steps - 1) % instructionSequence.length()] == 'L')
                node = movementMapping[node].first;
            else
                node = movementMapping[node].second;

            if (node[2] != 'Z')
                continue;

            stepCount.push_back(steps);
            break;
        }
    }

    std::cout << "Result: " << LCM(stepCount) << std::endl;
    
    return 0;
}
