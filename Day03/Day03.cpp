
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";


bool InternalIsDigit(char InSymbol)
{
    return InSymbol >= '0' && InSymbol <= '9';
}

bool CheckedGetPosition(const std::vector<std::string>& InSchematicData, int InX, int32_t InY)
{
    for (int y = InY - 1; y <= InY + 1; ++y)
    {
        if (InX < 0)
            continue;

        if (y < 0 || y >= static_cast<int>(InSchematicData.size()))
            continue;

        if (InX >= static_cast<int>(InSchematicData[y].size()))
            continue;

        if (InternalIsDigit(InSchematicData[y][InX]))
            continue;

        if (InSchematicData[y][InX] == '.')
            continue;

        return true;
    }
    return false;
}

int main(int argc, char* argv[])
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;

    std::vector<std::string> schematicData;
    
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        schematicData.push_back(inputLine);
    }

    int64_t result = 0;
    for (int32_t schematicLineIndex = 0; schematicLineIndex < static_cast<int32_t>(schematicData.size()); ++schematicLineIndex)
    {
        const auto& schematicLine = schematicData[schematicLineIndex];
        int64_t numCount = 0;
        int64_t currentNumber = 0;
        for (int32_t charIndex = 0; charIndex < static_cast<int32_t>(schematicLine.length()); ++charIndex)
        {
            bool foundDigit = false;
            if (InternalIsDigit(schematicLine[charIndex]))
            {
                ++numCount;
                currentNumber *= 10;
                currentNumber += schematicLine[charIndex] - '0';
                foundDigit = true;
            }
            const bool lastIndex = charIndex == (static_cast<int>(schematicLine.length())- 1);
            if (numCount > 0 && (!foundDigit || lastIndex))
            {
                for (int compareX = charIndex; compareX >= charIndex - numCount + (lastIndex && foundDigit ? 0 : -1); --compareX)
                {
                    if (CheckedGetPosition(schematicData, compareX, schematicLineIndex))
                    {
                        std::cout << currentNumber << std::endl;// << " at " << compareX << ", " << schematicLineIndex << std::endl;
                        result += currentNumber;
                        break;
                    }
                }
                
                numCount = 0;
                currentNumber = 0;
            }
        }
    }
    std::cout << "Result: " << result << std::endl;

    return 0;
}
