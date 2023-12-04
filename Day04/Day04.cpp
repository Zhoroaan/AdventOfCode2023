#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

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

void SetOrAddToMap(int32_t InKey, std::map<int32_t, int64_t>& InMap)
{
    if (InMap.contains(InKey))
        InMap[InKey] += 1;
    else
        InMap[InKey] = 1;
}

int main(int argc, char* argv[])
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;

    std::vector<std::string> schematicData;

    int64_t fullCount = 0;

    int currentScratchCard = 0;
    std::map<int32_t, int64_t> scratchCards;
    
    while (inputFile.is_open() && !inputFile.eof())
    {
        currentScratchCard += 1;

        std::getline(inputFile, inputLine);
        schematicData.push_back(inputLine);
        SetOrAddToMap(currentScratchCard, scratchCards);

        std::stringstream lineStream(inputLine);
        std::string card;
        std::getline(lineStream, card, ':');

        std::string winningNumbers;
        std::getline(lineStream, winningNumbers, '|');

        std::vector<int32_t> winningNumbersList = GetListOfNumbersFromString(winningNumbers);

        std::string haveNumbers;
        std::getline(lineStream, haveNumbers);

        std::vector<int32_t> haveNumbersList = GetListOfNumbersFromString(haveNumbers);

        int32_t points = 0;
        int32_t matchCount = 0;
        for (auto value : haveNumbersList)
        {
            if (std::ranges::find(winningNumbersList, value) != winningNumbersList.end())
            {
                if (points == 0)
                    points = 1;
                else
                    points *= 2;
                matchCount++;
            }
        }
        fullCount += points;

        auto repeatCount = scratchCards[currentScratchCard];
        for (auto otherCardIndex = currentScratchCard + 1; otherCardIndex <= currentScratchCard + matchCount; ++otherCardIndex)
        {
            auto& currentNumber = scratchCards[otherCardIndex];
            for (int repeat = 0; repeat < repeatCount; ++repeat)
                currentNumber++;
        }
    }

    int64_t fullScratchCardCount = 0;
    for (const auto& countForCard : scratchCards)
    {
        fullScratchCardCount += countForCard.second;
    }

    std::cout << "Full scratch card result: " << fullCount << std::endl;
    std::cout << "Total number of scratch cards: " << fullScratchCardCount << std::endl;
    
    return 0;
}
