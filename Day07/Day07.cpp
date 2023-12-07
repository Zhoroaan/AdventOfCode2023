
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

struct Hand
{
    std::string Cards;
    int64_t Bid;

    static int32_t CalculatePoints(const Hand& InOther)
    {
        std::map<char, int32_t> cardCounts;
        for (char cardValue : InOther.Cards)
        {
            if (cardCounts.contains(cardValue))
                cardCounts[cardValue]++;
            else
                cardCounts[cardValue] = 1;
        }
        int num3 = 0;
        int num2 = 0;

        for (const std::pair<const char, int>& cardCount : cardCounts)
        {
            if (cardCount.second == 5)
                return 7;
            if (cardCount.second == 4)
                return 6;
            if (cardCount.second == 3)
                num3++;
            if (cardCount.second == 2)
                num2++;
        }
        if (num3 == 1 && num2 == 1)
            return 5;
        if (num3 == 1)
            return 4;
        if (num2 > 1)
            return 3;
        if (num2 == 1)
            return 2;
        return 1;
    }

    static int32_t CardWorth(char InCard)
    {
        switch (InCard)
        {
        case 'A':
            return 14;
        case 'K':
            return 13;
        case 'Q':
            return 12;
        case 'J':
            return 11;
        case 'T':
            return 10;
        default:
            return InCard - '0';
        }
    }

    bool IsHandOrderLower(const Hand& InOther) const
    {
        for (size_t a = 0; a < InOther.Cards.length(); ++a)
        {
            if (Cards[a] == InOther.Cards[a])
                continue;

            return CardWorth(Cards[a]) < CardWorth(InOther.Cards[a]);
        }
        return false;
    }

    bool operator < (const Hand& other) const
    {
        /*bool result = (CalculatePoints(*this) < CalculatePoints(other)) || IsHandOrderLower(other);

        if (result)
            std::cout << Cards << " " << Bid << " < " << other.Cards << " " << other.Bid << std::endl;
        else
            std::cout << Cards << " " << Bid << " >= " << other.Cards << " " << other.Bid << std::endl;
        
        return result;*/
        if (CalculatePoints(*this) < CalculatePoints(other))
            return true;

        if (CalculatePoints(*this) > CalculatePoints(other))
            return false;

        return IsHandOrderLower(other);
        //return (CalculatePoints(*this) < CalculatePoints(other)) || IsHandOrderLower(other);
    }
};

int main(int argc, char* argv[])
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;
    std::vector<Hand> hands;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        if (inputLine.length() < 4)
            break;
        std::stringstream ss(inputLine);
        Hand newHand;
        ss >> newHand.Cards >> newHand.Bid;
        hands.push_back(newHand);
    }
    std::sort(hands.begin(), hands.end());

    int64_t result = 0;
    for (size_t a = 0; a < hands.size(); ++a)
    {
        result += (a + 1) * hands[a].Bid;
    }

    std::cout << "Day 7 result: " << result << std::endl;
    
    return 0;
}
