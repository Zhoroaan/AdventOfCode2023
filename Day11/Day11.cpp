#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

void ExpandMap(std::vector<std::string>& InData, std::vector<int64_t>& InEmptyColums, std::vector<int64_t>& InEmptyRows)
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
        
        InEmptyRows.push_back(std::distance(InData.begin(), rowIt));
    }

    for (size_t columnIndex = 0; columnIndex < InData[0].size(); ++columnIndex)
    {
        bool emptyColumn = true;
        for (const auto& row : InData)
        {
            if (row[columnIndex] == '#')
            {
                emptyColumn = false;
                break;
            }
        }
        if (!emptyColumn)
            continue;
        InEmptyColums.push_back(columnIndex);
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

int64_t CalculateDistance(std::vector<std::string> InData, std::vector<int64_t> InEmptyColumns,
        std::vector<int64_t> InEmptyRows, int64_t InEmptyOffset = 1000000)
{
    std::vector<Vector> galaxies;
    int64_t y = 0;
    for (int64_t row = 0; row < static_cast<int64_t>(InData.size()); ++row)
    {
        int64_t x = 0;
        for (int64_t column = 0; column < static_cast<int64_t>(InData[row].size()); ++column)
        {
            if (InData[row][column] == '#')
                galaxies.emplace_back(x, y);
            x += std::ranges::find(InEmptyColumns, column) != InEmptyColumns.end() ? InEmptyOffset : 1;
        }
        y += std::ranges::find(InEmptyRows, row) != InEmptyRows.end() ? InEmptyOffset : 1;
    }

    int64_t totalDistance = 0;
    for (auto startGalaxy : galaxies)
    {
        for (auto targetGalaxy : galaxies)
        {
            if (targetGalaxy == startGalaxy)
                continue;

            if (startGalaxy.Y > targetGalaxy.Y)
                continue;

            if (startGalaxy.Y == targetGalaxy.Y && startGalaxy.X > targetGalaxy.X)
                continue;

            const auto distance = startGalaxy.ManhattanDistance(targetGalaxy);
            totalDistance += distance;
        }
    }
    return totalDistance;
}

int main(int /*argc*/, char* /*argv*/[])
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

    std::vector<int64_t> emptyColumns, emptyRows;
    ExpandMap(data, emptyColumns, emptyRows);

    std::cout << "Day 11 result empty space 2: " << CalculateDistance(data, emptyColumns, emptyRows, 2)  << std::endl;
    std::cout << "Day 11 result empty space 1000000: " << CalculateDistance(data, emptyColumns, emptyRows, 1000000) << std::endl;
    
    return 0;
}
