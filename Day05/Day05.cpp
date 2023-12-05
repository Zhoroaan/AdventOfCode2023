#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

struct MappingRange
{
    MappingRange(int64_t InDest, int64_t InSource, int64_t InMapCount)
        : MapCount(InMapCount), SourceMap(InSource), DestMap(InDest)
    {
    }
    int64_t MapCount = 0;
    int64_t SourceMap = 0;
    int64_t DestMap = 0;

    [[nodiscard]] bool IsInSideRange(long long InCheckValue) const
    {
        return InCheckValue >= SourceMap && InCheckValue < SourceMap + MapCount;
    }

    int64_t GetMappedCount(long long InCompareValue) const
    {
        return DestMap + (InCompareValue - SourceMap);
    }
};

struct Category
{
    std::string Name;
    std::vector<MappingRange> Ranges;
};

Category ReadCategory(std::ifstream& InInputFile, bool bInFirstLine = false)
{
    Category returnCategory;
    while (returnCategory.Name.length() < 3)
        std::getline(InInputFile, returnCategory.Name, ':');

    std::string readLine;
    if (!bInFirstLine)
        std::getline(InInputFile, readLine);
    
    while (true)
    {
        if (InInputFile.eof())
            break;
        std::getline(InInputFile, readLine);
        if (readLine.length() < 3)
            break;

        std::stringstream lineStream(readLine);
        int64_t readValue = -1;
        std::vector<int64_t> ranges;
        while (lineStream >> readValue)
        {
            ranges.push_back(readValue);
        }
        if (ranges.size() > 2)
        {
            if (bInFirstLine)
            {
                for (auto startValue : ranges)
                {
                    returnCategory.Ranges.emplace_back(startValue, startValue, 1);
                }
            }
            else
            {
                returnCategory.Ranges.emplace_back(ranges[0], ranges[1], ranges[2]);
            }
        }
    }
    
    return returnCategory;
}


int64_t GetSeedMappingToCategories(const Category& InSeeds, const std::vector<Category>& InCategories)
{
    int64_t lowestLocation = std::numeric_limits<int64_t>::max();

    for (int32_t startIndex = 0; startIndex < static_cast<int32_t>(InSeeds.Ranges.size()); startIndex += 2)
    {
        std ::cout << "Iterating range: " << InSeeds.Ranges[startIndex].SourceMap << " " << InSeeds.Ranges[startIndex + 1].SourceMap << std::endl;
        auto& currentStartValue = InSeeds.Ranges[startIndex];

        const int64_t rangeSize = currentStartValue.SourceMap + InSeeds.Ranges[startIndex + 1].SourceMap;
        for (int64_t currentSourceValue = currentStartValue.SourceMap; currentSourceValue < rangeSize; ++currentSourceValue)
        {
            int64_t currentValue = currentSourceValue;
            for (int32_t categoryIndex = 0; categoryIndex < static_cast<int32_t>(InCategories.size()); ++categoryIndex)
            {
                const auto& category = InCategories[categoryIndex];
                for (const MappingRange& range : category.Ranges)
                {
                    /*if (currentValue == 82)
                    {
                        int breakHere = 0;
                    }*/
                    if (range.IsInSideRange(currentValue))
                    {
                        /*if (currentValue == 82)
                        {
                            std::cout << category.Name << " mapped " << currentValue  << " to " << range.GetMappedCount(currentValue) << std::endl;
                        }*/
                        currentValue = range.GetMappedCount(currentValue);
                        break;
                    }
                }
            }
            if (currentValue < lowestLocation)
                lowestLocation = currentValue;
        }
    }
    return lowestLocation;
}

int main(int argc, char* argv[])
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;
    
    auto seeds = ReadCategory(inputFile, true);
    auto soil = ReadCategory(inputFile);
    auto fertilizer = ReadCategory(inputFile);
    auto water = ReadCategory(inputFile);
    auto light = ReadCategory(inputFile);
    auto temperature = ReadCategory(inputFile);
    auto humidity  = ReadCategory(inputFile);
    auto location  = ReadCategory(inputFile);

    int64_t lowestLocation = GetSeedMappingToCategories(seeds, {soil, fertilizer, water, light, temperature, humidity, location});
    
    std::cout << "The shortest distance is " << lowestLocation << std::endl;
    
    return 0;
}
