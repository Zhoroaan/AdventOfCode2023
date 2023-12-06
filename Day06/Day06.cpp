
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

std::vector<std::pair<std::string, std::vector<int64_t>>> ReadInput(bool bInFirstStar)
{
    std::vector<std::pair<std::string, std::vector<int64_t>>> timeAndDistance;
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        std::stringstream stringstream(inputLine);
        std::pair<std::string, std::vector<int64_t>> input;
        std::getline(stringstream, input.first, ':');

        if (bInFirstStar)
        {
            int32_t readNumber = -1;
            while (stringstream >> readNumber)
            {
                input.second.push_back(readNumber);
            }
        }
        else
        {
            std::string unformatedNumber;
            std::getline(stringstream, unformatedNumber);
            std::erase_if(unformatedNumber, isspace);
            int64_t num = std::atoll(unformatedNumber.c_str());
            input.second.push_back(num);
        }
        timeAndDistance.push_back(input);
    }
    return timeAndDistance;
}

void RunDaySix(bool bInFirstStar)
{
    const std::vector<std::pair<std::string, std::vector<int64_t>>> timeAndDistance = ReadInput(bInFirstStar);
    int64_t result = 1;
    for (size_t gameIndex = 0; gameIndex < timeAndDistance[0].second.size(); ++gameIndex)
    {
        const auto timeForGame = timeAndDistance[0].second[gameIndex];
        int64_t numMatch = 0;
        for (int64_t holdTime = 1; holdTime < timeForGame; ++holdTime)
        {
            auto distanceGone = (timeForGame - holdTime) * holdTime;
            if (distanceGone > timeAndDistance[1].second[gameIndex])
                numMatch++;
        }
        result *= numMatch;
    }

    std::cout << "Result " << (bInFirstStar ? "First Star: " : "Second Star: ") << result << std::endl;
}

int main(int /*argc*/, char* /*argv*/[])
{
    RunDaySix(true);
    RunDaySix(false);
    return 0;
}
