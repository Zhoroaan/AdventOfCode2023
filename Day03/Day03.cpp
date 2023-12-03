
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

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

int32_t GetNumAtPosition(const std::vector<std::string>& InSchematicData, int32_t InX, int32_t InY)
{
    int32_t result = 0;
    if (InY < 0 || InY >= static_cast<int>(InSchematicData.size()))
        return 0;

    if (InX < 0 ||InX >= static_cast<int>(InSchematicData[InY].size()))
        return 0;

    if (!InternalIsDigit(InSchematicData[InY][InX]))
        return 0;
    
    int firstDigitX = InX;

    for (int x = InX - 1; x >= 0 && InternalIsDigit(InSchematicData[InY][x]); --x)
        firstDigitX = x;
    
    for (int x = firstDigitX; ;  ++x)
    {
        if (x < 0)
            continue;

        if (x >= static_cast<int>(InSchematicData[0].length()))
            break;

        if (InternalIsDigit(InSchematicData[InY][x]))
        {
            result *= 10;
            result += InSchematicData[InY][x] - '0';
        }
        else
            break;
    }
    return result;
}

void RunFirstStar()
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
                // Special handling of number at end of line
                for (int compareX = charIndex; compareX >= charIndex - numCount + (lastIndex && foundDigit ? 0 : -1); --compareX)
                {
                    if (CheckedGetPosition(schematicData, compareX, schematicLineIndex))
                    {
                        result += currentNumber;
                        break;
                    }
                }
                
                numCount = 0;
                currentNumber = 0;
            }
        }
    }
    std::cout << "Result First Star: " << result << std::endl;
}

void RunSecondStar()
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
        for (int32_t charIndex = 0; charIndex < static_cast<int32_t>(schematicLine.length()); ++charIndex)
        {
           if (schematicData[schematicLineIndex][charIndex] != '*')
               continue;

            int checkPositions[9] = {0};
            
            for (int y = -1; y <= 1; ++y)
                for (int x = -1; x <= 1; ++x)
                    checkPositions[(y + 1) * 3 + (x + 1)] = GetNumAtPosition(schematicData, charIndex + x, schematicLineIndex + y);
            
            int countGears = 0;
            int64_t gearRatio = 1;
            for (int y = 0; y <= 2; ++y)
            {
                for (int x = 0; x <= 2; ++x)
                {
                    if (checkPositions[y * 3 + x] != 0)
                    {
                        ++countGears;
                        gearRatio*= checkPositions[y * 3 + x];
                        // Same number in all three slots
                        if (x == 0 && checkPositions[y * 3 + x + 1] != 0 && checkPositions[y * 3 + x + 2] != 0)
                            break;

                        // different numbers furthers from the left and right
                        x++;
                    }
                }
            }
            if (countGears == 2)
                result += gearRatio;
        }
    }
    std::cout << "Result Second Star: " << result << std::endl;
}

int main(int argc, char* argv[])
{
    RunFirstStar();
    RunSecondStar();

    return 0;
}
