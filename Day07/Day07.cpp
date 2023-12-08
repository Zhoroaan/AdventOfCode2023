
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
        if (InOther.Cards == "JJJJJ")
            return 7 * 10;
        
        std::vector<std::pair<char, int32_t>> cardCounts;
        int numJ = 0;
        for (char cardValue : InOther.Cards)
        {
            if (cardValue == 'J')
            {
                numJ++;
                continue;
            }
            bool foundValue = false;
            for (std::pair<char, int32_t>& savedCards : cardCounts)
            {
                if (savedCards.first != cardValue)
                    continue;
                savedCards.second++;
                foundValue = true;
                break;
            }
            if (!foundValue)
            {
                std::pair<char, int32_t> newCard;
                newCard.first = cardValue;
                newCard.second = 1;
                cardCounts.push_back(newCard);
            }
        }

        int num3 = 0;
        int num2 = 0;

        std::sort(cardCounts.begin(), cardCounts.end(), [](const std::pair<const char, int>& InA, const std::pair<const char, int>& InB)
        {
            return InA.second > InB.second;
        });

        cardCounts[0].second += numJ;
        
        for (const std::pair<char, int32_t>& cardCount : cardCounts)
        {
            if (cardCount.second == 5)
                return 7 * 10;
            if (cardCount.second == 4)
                return 6 * 10;
            if (cardCount.second == 3)
                num3++;
            if (cardCount.second == 2)
                num2++;
        }
        
        if (num3 == 1 && num2 == 1)
            return 5 * 10;
        if (num3 == 1)
            return 4 * 10;
        if (num2 > 1)
            return 3 * 10;
        if (num2 == 1)
            return 2 * 10;
        
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
            return 1;
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
        if (Cards == "QQQJA" && other.Cards == "KTJJT")
        {
            int b = 2;
        }
        if (Cards == "KTJJT" && other.Cards == "QQQJA")
        {
            int b = 2;
        }
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
