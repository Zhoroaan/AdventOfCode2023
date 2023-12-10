
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

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
    if (InTo.X < 0 || InTo.X >= InGrid[0].length() || InTo.Y < 0 || InTo.Y >= InGrid.size())
        return false;

    if (InFrom.X < 0 || InFrom.X >= InGrid[0].length() || InFrom.Y < 0 || InFrom.Y >= InGrid.size())
        return false;

    const auto diff = InTo - InFrom;

    if (std::ranges::find(CanWeMoveTowards[diff.Y + 1][diff.X + 1], InGrid[InFrom.Y][InFrom.X]) == CanWeMoveTowards[diff.Y + 1][diff.X + 1].end()) // Is the position next to us pointing towards us?
        return false;

    if (std::ranges::find(DoesTargetAcceptConnections[diff.Y + 1][diff.X + 1], InGrid[InTo.Y][InTo.X]) == DoesTargetAcceptConnections[diff.Y + 1][diff.X + 1].end()) // Is the position next to us pointing towards us?
        return false;
    
    //if (DoesTargetAcceptConnections[diff.X + 1][diff.Y + 1].empty()) // Is the position next to us pointing towards us?
    //    return false;
    
    return true;
}

int main(int argc, char* argv[])
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
                startPoint = Point(x, y);
            }
        }
    }

    std::vector<Point> directions =
    {
        Point(-1, 0),
        Point(0, -1),
        Point(1, 0),
        Point(0, 1)
    };

    int64_t sizeOfLoop = 0;
    Point currenPosition = startPoint;
    bool firstMove = true;
    Point lastPosition = Point(std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::max());

    while (firstMove || currenPosition != startPoint)
    {
        firstMove = false;
        for (auto direction : directions)
        {
            if (currenPosition + direction == lastPosition)
                continue;
            
            if (CanMove(currenPosition, currenPosition + direction, grid))
            {
                std::cout << "Can move to " << (currenPosition + direction).X << ", " << (currenPosition + direction).Y << std::endl;
                lastPosition = currenPosition;
                currenPosition = currenPosition + direction;
                sizeOfLoop++;
                break;
            }
        }
    }

    std::cout << "Result: " << sizeOfLoop / 2 << std::endl;
    
    return 0;
}
