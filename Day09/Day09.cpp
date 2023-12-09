
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

int64_t CalculateNextNumber(const std::vector<int64_t>& InNumbers, bool bInFirstPart)
{
    if (InNumbers.size() < 2)
        return 0;

    bool allZeros = true;
    for (const auto number : InNumbers)
    {
        if (number != 0)
        {
            allZeros = false;
            break;
        }
    }
    if (allZeros)
        return 0;

    std::vector<int64_t> diffSequence;
    for (size_t a = 0; a < InNumbers.size() - 1; ++a)
    {
        diffSequence.push_back(InNumbers[a + 1] - InNumbers[a]);
    }
    if (bInFirstPart)
        return InNumbers.back() + CalculateNextNumber(diffSequence, bInFirstPart);
    return InNumbers[0] - CalculateNextNumber(diffSequence, bInFirstPart);
}

void PrintDay9Result(std::vector<std::vector<int64_t>> allSequences, bool bInFirstPart)
{
    int64_t result = 0;
    for (const std::vector<int64_t>& sequence : allSequences)
    {
        result +=  CalculateNextNumber(sequence, bInFirstPart);
    }
    std::cout << "Result " << (bInFirstPart ? "first star: " : "second star: ") << result << std::endl;
}

int main(int argc, char* argv[])
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;
    std::vector<std::vector<int64_t>> allSequences;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        std::stringstream ss(inputLine);
        std::vector<int64_t> sequence;
        int64_t inputNumber;
        while (ss >> inputNumber)
        {
            sequence.push_back(inputNumber);
        }
        if (!sequence.empty())
            allSequences.push_back(sequence);
    }

    PrintDay9Result(allSequences, true);
    PrintDay9Result(allSequences, false);
    
    return 0;
}
