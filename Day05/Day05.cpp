#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

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


std::map<int64_t, int64_t> GetSeedMappingToCategories(const Category& InSeeds, const std::vector<Category>& InCategories)
{
    std::map<int64_t, int64_t> returnMap;
    std::map<int64_t, int32_t> appliedMappingIndex;
    for (auto startSeedLocation : InSeeds.Ranges)
    {
        returnMap[startSeedLocation.DestMap] = startSeedLocation.DestMap;
        appliedMappingIndex[startSeedLocation.DestMap] = -1;
    }
    
    for (int32_t categoryIndex = 0; categoryIndex < static_cast<int32_t>(InCategories.size()); ++categoryIndex)
    {
        const auto& category = InCategories[categoryIndex];
        for (const MappingRange& range : category.Ranges)
        {
            for (auto& currentMappingValue : returnMap)
            {
                if (range.IsInSideRange(currentMappingValue.second) && appliedMappingIndex[currentMappingValue.second] < categoryIndex)
                {
                    if (currentMappingValue.first == 14)
                    {
                        std::cout << category.Name << " mapped " << currentMappingValue.second  << " to " << range.GetMappedCount(currentMappingValue.second) << std::endl;
                    }
                    currentMappingValue.second = range.GetMappedCount(currentMappingValue.second);
                    appliedMappingIndex[currentMappingValue.second] = categoryIndex;
                }
            }
        }
    }
    return returnMap;
}

int main(int argc, char* argv[])
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;
    
    //std::getline(inputFile, inputLine);
    auto seeds = ReadCategory(inputFile, true);
    auto soil = ReadCategory(inputFile);
    auto fertilizer = ReadCategory(inputFile);
    auto water = ReadCategory(inputFile);
    auto light = ReadCategory(inputFile);
    auto temperature = ReadCategory(inputFile);
    auto humidity  = ReadCategory(inputFile);
    auto location  = ReadCategory(inputFile);

    std::map<int64_t, int64_t> seedMap = GetSeedMappingToCategories(seeds, {soil, fertilizer, water, light, temperature, humidity, location});

    int64_t lowestLocation = std::numeric_limits<int64_t>::max();

    for (const auto& seedMapping : seedMap)
    {
        if (seedMapping.second < lowestLocation)
            lowestLocation = seedMapping.second;
    }
    
    std::cout << "The shortest distance is " << lowestLocation << std::endl;
    
    return 0;
}
