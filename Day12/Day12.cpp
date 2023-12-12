
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

bool IsValidInput(const std::string& InInputData, const std::vector<int64_t>& InGroups)
{
    bool isInGroup = false;
    int64_t currentGroupSize = 0;
    int64_t currentGroupIndex = 0;
    //std::cout << "Testing permutation: " << InInputData << std::endl;
    for (int a = 0; a < InInputData.size(); ++a)
    {
        if (isInGroup && (InInputData[a] == '.' || InInputData[a] == '?'))
        {
            isInGroup = false;
            if (static_cast<int64_t>(InGroups.size()) <= currentGroupIndex)
                return false;

            if (InGroups[currentGroupIndex] != currentGroupSize)
                return false;
            
            currentGroupSize = 0;
            currentGroupIndex += 1;
        }
        else if (InInputData[a] == '#')
        {
            currentGroupSize++;
            isInGroup = true;
        }

        if (a == static_cast<int64_t>(InInputData.size()) - 1 && !isInGroup)
        {
            return true;
        }
    }
    if (static_cast<int64_t>(InGroups.size()) <= currentGroupIndex)
        return false;

    if (InGroups[currentGroupIndex] != currentGroupSize)
        return false;
    
    return true;
}

int64_t CountNumberOfPermutations(const std::string& InInputData, const std::vector<int64_t>& InGroups, int64_t InTotalNumberMissingSprings, int64_t InMinIndex = 0)
{
    int64_t numPermutations = 0;
    if (InTotalNumberMissingSprings == 0 && IsValidInput(InInputData, InGroups))
    {
        //std::cout << "Matching permutation: " << InInputData << std::endl;
        return 1;
    }
    
    std::string workData;
    for (int a = InMinIndex; a < InInputData.size(); ++a)
    {
        if (InInputData[a] != '?')
            continue;

        workData = InInputData;
        workData[a] = '#';
        numPermutations += CountNumberOfPermutations(workData, InGroups, InTotalNumberMissingSprings - 1, a + 1);
    }
    return numPermutations;
}

int main(int argc, char* argv[])
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;
    int64_t totalSumPermutations = 0;
    while (inputFile.is_open() && !inputFile.eof())
    {
        bool b = IsValidInput(".?#..#?...###.", {1,1,3});
        std::getline(inputFile, inputLine);
        std::stringstream ss(inputLine);
        std::string springCondition;
        std::getline(ss, springCondition, ' ');

        std::vector<int64_t> numberGroups;
        int64_t totalNumberOfBrokenSprings = 0;
        for (int64_t group = -1; ss >> group;)
        {
            numberGroups.push_back(group);
            ss.ignore(1, ',');
            totalNumberOfBrokenSprings += group;
        }
        for (auto spring : springCondition)
        {
            if (spring == '#')
                totalNumberOfBrokenSprings--;
        }
        int64_t numPermutations = CountNumberOfPermutations(springCondition, numberGroups, totalNumberOfBrokenSprings);
        totalSumPermutations += numPermutations;
    }

    std::cout << "Day 12 result: " << totalSumPermutations << std::endl;
    
    return 0;
}
