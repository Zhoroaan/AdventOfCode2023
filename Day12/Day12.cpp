#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <execution>
std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

bool IsValidInput(const std::string& InInputData, const std::vector<int64_t>& InGroups, int64_t& OutBrokeAtIndex)
{
    bool isInGroup = false;
    int64_t currentGroupSize = 0;
    int64_t currentGroupIndex = 0;
    //std::cout << "Testing permutation: " << InInputData << std::endl;
    for (int a = 0; a < InInputData.size(); ++a)
    {
        OutBrokeAtIndex = a;
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
            OutBrokeAtIndex = -1;
            return true;
        }
    }
    if (static_cast<int64_t>(InGroups.size()) <= currentGroupIndex)
        return false;

    if (InGroups[currentGroupIndex] != currentGroupSize)
        return false;
    
    OutBrokeAtIndex = -1;
    return true;
}

int64_t CountNumberOfPermutations(std::vector<std::string>& bufferData, const std::string& InInputData, const std::vector<int64_t>& InGroups, int64_t InTotalNumberMissingSprings, int64_t& OutBrokeAt, int64_t InMinIndex = 0, int64_t InDepth = 0)
{
    int64_t numPermutations = 0;
    if (InTotalNumberMissingSprings == 0 && IsValidInput(InInputData, InGroups, OutBrokeAt))
    {
        //std::cout << "Matching permutation: " << InInputData << std::endl;
        return 1;
    }
    else if (InTotalNumberMissingSprings <= 0)
        return 0;
    
    if (bufferData.size() <= InDepth)
        bufferData.emplace_back();

    std::string& workData = bufferData[InDepth];
    for (int a = InMinIndex; a < InInputData.size(); ++a)
    {
        if (InInputData[a] != '?')
            continue;

        workData = InInputData;
        workData[a] = '#';
        numPermutations += CountNumberOfPermutations(bufferData, workData, InGroups, InTotalNumberMissingSprings - 1, OutBrokeAt, a + 1, InDepth + 1);
        if (OutBrokeAt != -1 && a >= OutBrokeAt)
            return numPermutations;
    }
    return numPermutations;
}

struct RowInput
{
    std::string FullString;
    std::string SpringCondition;
    std::vector<int64_t> FullNumberGroup;
    int64_t TotalNumberOfBrokenSprings = 0;
};

int main(int argc, char* argv[])
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;
    std::vector<RowInput> rowInputs;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        std::stringstream ss(inputLine);
        RowInput rowInput;
        std::getline(ss, rowInput.SpringCondition, ' ');
        
        for (int a = 0; a < 5; ++a)
        {
            rowInput.FullString += rowInput.SpringCondition;
            if (a != 4)
                rowInput.FullString += '?';
        }

        std::vector<int64_t> numberGroups;
        for (int64_t group = -1; ss >> group;)
        {
            numberGroups.push_back(group);
            ss.ignore(1, ',');
            rowInput.TotalNumberOfBrokenSprings += group;
        }
        for (int a = 0; a < 5; ++a)
        {
            for (auto count : numberGroups)
            {
                rowInput.FullNumberGroup.push_back(count);
            }
        }

        rowInput.TotalNumberOfBrokenSprings *= 5;
        
        for (auto spring : rowInput.FullString)
        {
            if (spring == '#')
                rowInput.TotalNumberOfBrokenSprings--;
        }
        rowInputs.push_back(rowInput);
    }

    std::atomic_int64_t totalSumPermutations = 0;
    std::atomic_int64_t numFinished = 0;
    /*std::for_each(
    std::execution::par_unseq,
    rowInputs.begin(),
    rowInputs.end(),
    [&totalSumPermutations](auto&& rowInput)*/
    #pragma omp parallel for
    for (int i = 0; i  < rowInputs.size(); ++i)
    {
        auto& rowInput = rowInputs[i];
        std::vector<std::string> stringBuffer;
        stringBuffer.reserve(256);
        //do stuff with item
        std::cout << "Working on [" << i + 1 << "/" << rowInputs.size() << "] " << rowInput.SpringCondition << std::endl;
        int64_t outBrokeAt = -1;
        int64_t numPermutations = CountNumberOfPermutations(stringBuffer, rowInput.FullString, rowInput.FullNumberGroup, rowInput.TotalNumberOfBrokenSprings, outBrokeAt);
    
        totalSumPermutations += numPermutations;
        numFinished+= 1;
        std::cout << "Finished " << numFinished << std::endl;
    }//);

    std::cout << "Day 12 result: " << totalSumPermutations << std::endl;
    
    return 0;
}
