
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

struct Point
{
    Point(int64_t InX, int64_t InY)
        : X(InX)
        , Y(InY)
    {}
    Point() = default;
    int64_t X = 0;
    int64_t Y = 0;
    Point operator-(const Point& InOtherPoint) const
    {
        return {X - InOtherPoint.X, Y - InOtherPoint.Y};
    }

    Point operator+(const Point& InOtherPoint) const
    {
        return {X + InOtherPoint.X, Y + InOtherPoint.Y};
    }

    bool operator==(const Point& InPoint) const
    {
        return X == InPoint.X && Y == InPoint.Y;
    }

    bool operator<(const Point& InPoint) const
    {
        return X < InPoint.X;
    }
};

struct Row
{
    Row() = default;
    Row(const Point& InFirstPoint)
        : Y(InFirstPoint.Y)
    {
        Points.push_back(InFirstPoint);
    }
    

    bool operator<(const Row& InPoint) const
    {
        return Y < InPoint.Y;
    }
    
    int64_t Y = std::numeric_limits<int64_t>::max();
    std::vector<Point> Points;
};

std::vector<Point> Directions =
{
    Point(-1, 0),
    Point(0, -1),
    Point(1, 0),
    Point(0, 1)
};

std::vector<std::vector<std::vector<char>>> DoesTargetAcceptConnections =
{
    { {}, {'S', '|', '7', 'F'}, {} },
    {{'S','-','L','F'}, {}, {'S','-','J','7'}},
    {{}, {'S', '|', 'L', 'J'}, {}},
};

std::vector<std::vector<std::vector<char>>> CanWeMoveTowards = {
    {{}, {'S', '|', 'L', 'J'}, {}},
{{'S','-', '7', 'J'}, {}, {'S','-','L','F'}},
{{}, {'S', '|','7','F'}, {}},
};

bool CanMove(const Point& InFrom, const Point& InTo, const std::vector<std::string>& InGrid)
{
    if (InTo.X < 0 || InTo.X >= static_cast<int64_t>(InGrid[0].length()) || InTo.Y < 0 || InTo.Y >= static_cast<int64_t>(InGrid.size()))
        return false;

    if (InFrom.X < 0 || InFrom.X >= static_cast<int64_t>(InGrid[0].length()) || InFrom.Y < 0 || InFrom.Y >= static_cast<int64_t>(InGrid.size()))
        return false;

    const auto diff = InTo - InFrom;

    if (std::ranges::find(CanWeMoveTowards[diff.Y + 1][diff.X + 1], InGrid[InFrom.Y][InFrom.X]) == CanWeMoveTowards[diff.Y + 1][diff.X + 1].end()) // Is the position next to us pointing towards us?
        return false;

    if (std::ranges::find(DoesTargetAcceptConnections[diff.Y + 1][diff.X + 1], InGrid[InTo.Y][InTo.X]) == DoesTargetAcceptConnections[diff.Y + 1][diff.X + 1].end()) // Is the position next to us pointing towards us?
        return false;
        
    return true;
}

bool DoesRaycastIntersect(const char currentType)
{
    return currentType == '|'
        || currentType == 'J'
        || currentType == 'L'
        || currentType == 'S';
}

int main(int /*argc*/, char* /*argv*/[])
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;
    std::vector<std::string> grid;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        grid.push_back(inputLine);
    }

    Point startPoint;
    for (size_t x = 0; x < grid[0].length(); ++x)
    {
        for (size_t y = 0; y < grid.size(); ++y)
        {
            if (grid[y][x] == 'S')
            {
                startPoint = Point(static_cast<int64_t>(x), static_cast<int64_t>(y));
            }
        }
    }

    Point currenPosition = startPoint;
    Point lastPosition = Point(std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::max());
    std::vector<Point> loopPoints;
    loopPoints.push_back(startPoint);
    for (bool firstMove = true; firstMove || currenPosition != startPoint; firstMove = false)
    {
        for (auto& direction : Directions)
        {
            if (currenPosition + direction == lastPosition)
                continue;
            
            if (CanMove(currenPosition, currenPosition + direction, grid))
            {
                lastPosition = currenPosition;
                currenPosition = currenPosition + direction;
                if (startPoint != currenPosition)
                    loopPoints.push_back(currenPosition);
                break;
            }
        }
    }

    std::vector<Row> rows;
    for (const auto& point : loopPoints)
    {
        bool foundEntry = false;
        for (auto& row : rows)
        {
            if (row.Y == point.Y)
            {
                row.Points.emplace_back(point);
                foundEntry = true;
                break;
            }
        }
        if (!foundEntry)
        {
            rows.emplace_back(point);
        }
    }
    
    int64_t pointsInside = 0;
    for (auto& row : rows)
    {
        std::sort(row.Points.begin(), row.Points.end());
        int32_t countFound = 0;
        for (int64_t x = 0; x < row.Points.back().X; ++x)
        {
            const auto found
                = std::ranges::find_if(row.Points,
                    [point = Point(x, row.Y)](const Point& InPoint)
                    {
                        return InPoint == point;
                    });

            if (found != row.Points.end())
            {
                if (DoesRaycastIntersect(grid[row.Y][x]))
                    countFound++;
                continue;
            }
            if ((countFound % 2) == 1)
                pointsInside++;
        }
    }

    std::cout << "Result Part 1: " << loopPoints.size() / 2 << " is furthest away."<< std::endl;
    std::cout << "Result Part 2: " << pointsInside << " points inside."<< std::endl;
    
    return 0;
}
