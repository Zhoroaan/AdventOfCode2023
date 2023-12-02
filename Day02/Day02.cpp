
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

int main(int argc, char* argv[])
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;
    std::map<std::string, int64_t> availableCubes =
        {
        {"red", 12},
        {"green", 13},
        {"blue", 14}
        };
    int32_t validGameNumbers = 0;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        std::stringstream stringstream(inputLine);
        std::string game;
        std::getline(stringstream,  game, ':');
        std::cout << game << std::endl;
        int32_t gameNumber;
        sscanf_s(game.c_str(), "Game %d", &gameNumber);
        std::cout << "Game number: " << gameNumber << std::endl;
        bool bGamePossible = true;
        while (!stringstream.eof())
        {
            std::string roundOutput;
            std::getline(stringstream,  roundOutput, ';');

            std::stringstream roundStream(roundOutput);
            std::map<std::string, int64_t> colorCounter;
            while (!roundStream.eof())
            {
                std::string colorOutput;
                std::getline(roundStream,  colorOutput, ',');
                int64_t colorCount = -1;
                std::string color;
                std::stringstream colorStream(colorOutput);
                colorStream >> colorCount;
                colorStream >> color;
                if (!colorCounter.contains(color))
                    colorCounter[color] = colorCount;
                else
                    colorCounter[color] += colorCount;
                
            }
            for (const auto& value : colorCounter)
            {
                if (value.second > availableCubes[value.first])
                {
                    bGamePossible = false;
                    break;
                }
            }
                
        }
        if (bGamePossible)
            validGameNumbers += gameNumber;
    }

    std::cout << "Result: " << validGameNumbers << std::endl;
    
    return 0;
}
