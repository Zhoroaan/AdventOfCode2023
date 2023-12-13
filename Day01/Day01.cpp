#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <string>
#include <regex>
std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

int main(int argc, char* argv[])
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;
    int64_t totalNum = 0;
    std::map<int64_t, std::string> numbers = {
        {0, "zero"},
        {1, "one"},
        {2, "two"},
        {3, "three"},
        {4, "four"},
        {5, "five"},
        {6, "six"},
        {7, "seven"},
        {8, "eight"},
        {9, "nine"}
    };
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        int64_t count = 0;

        int64_t firstFound = 0;
        int64_t lastFound = 0;
        int64_t firstFoundIndex = std::numeric_limits<int64_t>::max();
        int64_t lastFoundIndex = -1;

        for (const auto& number : numbers)
        {
            auto foundIndex = (inputLine.find(number.second));
            if (foundIndex != std::string::npos && static_cast<int64_t>(foundIndex) < firstFoundIndex)
            {
                firstFoundIndex = static_cast<int64_t>(foundIndex);
                firstFound = number.first;
            }
            foundIndex = (inputLine.rfind(number.second));
            if (foundIndex != std::string::npos && static_cast<int64_t>(foundIndex) > lastFoundIndex)
            {
                lastFoundIndex = static_cast<int64_t>(foundIndex);
                lastFound = number.first;
            }
        }

        for (auto startIt = inputLine.begin(); startIt != inputLine.end(); ++startIt)
        {
            if (std::distance(inputLine.begin(), startIt) > firstFoundIndex)
            {
                count = firstFound * 10;
                std::cout << "Fixed start Digit: " << firstFound;
                break;
            }
           if (std::isdigit(*startIt))
           {
               count = (*startIt - '0') * 10;
               std::cout << "Start Digit: " << *startIt;
               break;
           }
        }

        for (auto endIt = inputLine.rbegin(); endIt != inputLine.rend(); ++endIt)
        {
            int64_t diff = std::distance(inputLine.rbegin(), endIt);
            if (diff >= inputLine.size() - lastFoundIndex)
            {
                count += lastFound;
                std::cout << " Fixed end Digit: " << lastFound << std::endl;
                break;
            }
            
            if (std::isdigit(*endIt))
            {
                count += (*endIt - '0');
                std::cout << " End Digit: " << *endIt << std::endl;
                break;
            }
        }

        totalNum += count;
    }

    std::cout << "Total count: " << totalNum << std::endl;
    
    return 0;
}
