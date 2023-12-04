#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

std::vector<int32_t> GetListOfNumbersFromString(std::string InNumberString)
{
    std::vector<int32_t> returnNumbers;
    std::stringstream numbersStream(InNumberString);
    while (!numbersStream.eof())
    {
        int32_t newNumber = -1;
        numbersStream >> newNumber;
        if (newNumber == -1)
            break;
        returnNumbers.push_back(newNumber);
    }
    return returnNumbers;
}

int main(int argc, char* argv[])
{
    
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;

    std::vector<std::string> schematicData;

    int64_t fullCount = 0;
    
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        schematicData.push_back(inputLine);

        std::stringstream lineStream(inputLine);
        std::string card;
        std::getline(lineStream, card, ':');

        std::string winningNumbers;
        std::getline(lineStream, winningNumbers, '|');
        //std::cout << "Wining Numbers for " << card << winningNumbers << std::endl;

        std::vector<int32_t> winningNumbersList = GetListOfNumbersFromString(winningNumbers);

        std::string haveNumbers;
        std::getline(lineStream, haveNumbers);

        std::vector<int32_t> haveNumbersList = GetListOfNumbersFromString(haveNumbers);

        int32_t points = 0;
        for (auto value : haveNumbersList)
        {
            if (std::ranges::find(winningNumbersList, value) != winningNumbersList.end())
            {
                if (points == 0)
                    points = 1;
                else
                    points *= 2;
            }
        }
        fullCount += points;
        //std::cout << "Have Numbers for " << card << haveNumbers << std::endl;
        std::cout << "Points for " << card << ": " << points << std::endl;
    }

    std::cout << "Full scratch card result: " << fullCount << std::endl;
    
    return 0;
}
