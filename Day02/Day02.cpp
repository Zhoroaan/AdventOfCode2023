
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

    int64_t totalProductCount = 0;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        std::stringstream stringstream(inputLine);
        std::string game;
        std::getline(stringstream,  game, ':');
        int32_t gameNumber;
        sscanf_s(game.c_str(), "Game %d", &gameNumber);
        std::map<std::string, int64_t> colorCounter;
        while (!stringstream.eof())
        {
            std::string roundOutput;
            std::getline(stringstream,  roundOutput, ';');

            std::stringstream roundStream(roundOutput);
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
                    colorCounter[color] = std::max(colorCount, colorCounter[color]);
                
            }
                
        }

        int64_t cubeProduct  = colorCounter.empty() ? 0 : 1;
        for (const auto& value : colorCounter)
        {
            cubeProduct *= value.second;
        }
        std::cout << "Game " <<  gameNumber << " product: " << cubeProduct << std::endl;
        totalProductCount += cubeProduct;
    }

    std::cout << "Result: " << totalProductCount << std::endl;
    
    return 0;
}
