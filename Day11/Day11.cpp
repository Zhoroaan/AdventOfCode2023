
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

void ExpandMap(std::vector<std::string>& InData)
{
    std::stringstream rowStream;
    for (char column : InData[0])
    {
        rowStream << '.';
    }
    std::string emptyRow = rowStream.str();
    for (auto rowIt = InData.begin(); rowIt != InData.end(); ++rowIt)
    {
        if (std::ranges::find(*rowIt, '#') != rowIt->end())
            continue;

        rowIt = InData.insert(rowIt, emptyRow);
        ++rowIt;
    }

    for (int i = 0; i < InData[0].size(); ++i)
    {
        bool emptyColumn = true;
        for (const auto& row : InData)
        {
            if (row[i] == '#')
            {
                emptyColumn = false;
                break;
            }
        }
        if (!emptyColumn)
            continue;
        
        for (auto& row : InData)
        {
            row.insert(i, 1, '.');
        }
        i++;
    }
}

struct Vector
{
    Vector() = default;
    Vector(int64_t InX, int64_t InY)
        : X(InX)
        , Y(InY)
    {}

    int64_t ManhattanDistance(const Vector& InOther) const
    {
        return std::abs(InOther.X - X) + std::abs(InOther.Y - Y);
    }

    bool operator==(const Vector& InOther) const
    {
        return X == InOther.X && Y == InOther.Y;
    }

    int64_t X = 0;
    int64_t Y = 0;
};

int main(int argc, char* argv[])
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;
    std::vector<std::string> data;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        data.push_back(inputLine);
    }

    ExpandMap(data);

    std::vector<Vector> galaxies;
    for (int64_t y = 0; y < static_cast<int64_t>(data.size()); ++y)
    {
        for (int64_t x = 0; x < static_cast<int64_t>(data[y].size()); ++x)
        {
            if (data[y][x] == '#')
                galaxies.emplace_back(x, y);
        }
    }

    int64_t totalDistance = 0;
    for (auto startGalaxy : galaxies)
    {
        int64_t minPathLength = std::numeric_limits<int64_t>::max();

        for (auto targetGalaxy : galaxies)
        {
            if (startGalaxy == galaxies[4] && targetGalaxy == galaxies[8])
                int breakHere = 0;
            if (targetGalaxy == startGalaxy)
                continue;

            if (startGalaxy.Y > targetGalaxy.Y)
                continue;

            if (startGalaxy.Y == targetGalaxy.Y && startGalaxy.X > targetGalaxy.X)
                continue;

            auto distance = startGalaxy.ManhattanDistance(targetGalaxy);
            totalDistance += distance;
            //if (distance < minPathLength)
            //    minPathLength = distance;
        }
    }

    std::cout << "Day 11 result: " << totalDistance << std::endl;
    
    return 0;
}
