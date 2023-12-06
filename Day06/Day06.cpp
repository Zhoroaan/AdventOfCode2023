
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <string>
#include <regex>
#include <sstream>
//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

std::vector<std::pair<std::string, std::vector<int32_t>>> ReadInput()
{
    std::vector<std::pair<std::string, std::vector<int32_t>>> timeAndDistance;
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        std::stringstream stringstream(inputLine);
        std::pair<std::string, std::vector<int32_t>> input;
        std::getline(stringstream, input.first, ':');
        int32_t readNumber = -1;
        while (stringstream >> readNumber)
        {
            input.second.push_back(readNumber);
        }
        timeAndDistance.push_back(input);
    }
    return timeAndDistance;
}

int main(int /*argc*/, char* /*argv*/[])
{
    const std::vector<std::pair<std::string, std::vector<int32_t>>> timeAndDistance = ReadInput();
    int64_t result = 1;
    for (size_t gameIndex = 0; gameIndex < timeAndDistance[0].second.size(); ++gameIndex)
    {
        const auto timeForGame = timeAndDistance[0].second[gameIndex];
        int32_t numMatch = 0;
        for (int32_t holdTime = 1; holdTime < timeForGame; ++holdTime)
        {
            auto distanceGone = (timeForGame - holdTime) * holdTime;
            if (distanceGone > timeAndDistance[1].second[gameIndex])
                numMatch++;
        }
        result *= numMatch;
        std::cout << "Game " << gameIndex + 1 << " matches: " << numMatch << std::endl;
    }

    std::cout << "Result: " << result << std::endl;
    
    return 0;
}
