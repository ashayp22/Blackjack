// Blackjack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <array>
#include <random>
#include <ctime>




//constants 
constexpr int maximumScore{ 21 }; //maximum score that a player can have
constexpr int minimumDealerScore{ 17 }; //the minimum score a dealer will have
constexpr int shuffleMinimum{ 46 }; //the minimum score a dealer will have

//Enumerator for a Card's Rank
enum class CardsRanks {
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
    RANK_9,
    RANK_10,
    RANK_JACK,
    RANK_QUEEN,
    RANK_KING,
    RANK_ACE,
    MAX_RANKS
};


//Enumerator for a Card's Suit
enum class CardsSuits {
    SUIT_CLUBS,
    SUIT_DIAMONDS,
    SUIT_HEARTS,
    SUIT_SPADES,
    MAX_SUITS
};

class Card {

private:
    CardsRanks m_rank{};
    CardsSuits m_suit{};

public:

    //Default Constructor
    Card() = default;

    Card(CardsRanks rank, CardsSuits suit) {
        m_rank = rank;
        m_suit = suit;
    }

    //Returns a card's suit and rank as a string with length 2
    std::string printCard() const {

        std::string code{ "" };

        switch (m_rank)
        {
        case CardsRanks::RANK_2:
            code = "2 of";
            break;
        case CardsRanks::RANK_3:
            code = "3 of";
            break;
        case CardsRanks::RANK_4:
            code = "4 of";
            break;
        case CardsRanks::RANK_5:
            code = "5 of";
            break;
        case CardsRanks::RANK_6:
            code = "6 of";
            break;
        case CardsRanks::RANK_7:
            code = "7 of";
            break;
        case CardsRanks::RANK_8:
            code = "8 of";
            break;
        case CardsRanks::RANK_9:
            code = "9 of";
            break;
        case CardsRanks::RANK_10:
            code = "10 of";
            break;
        case CardsRanks::RANK_JACK:
            code = "Jack of";
            break;
        case CardsRanks::RANK_QUEEN:
            code = "Queen of";
            break;
        case CardsRanks::RANK_KING:
            code = "King of";
            break;
        case CardsRanks::RANK_ACE:
            code = "Ace of";
            break;
        default:
            break;
        }

        switch (m_suit) {
        case CardsSuits::SUIT_CLUBS:
            code += " Clubs";
            break;
        case CardsSuits::SUIT_DIAMONDS:
            code += " Diamonds";
            break;
        case CardsSuits::SUIT_HEARTS:
            code += " Hearts";
            break;
        case CardsSuits::SUIT_SPADES:
            code += " Spades";
            break;
        default:
            break;
        }

        return code;
    }

    //returns a card's value
    //const member function is a member function that guarantees it will not modify the object or call any non-const member functions (as they may modify the object)
    int getCardValue() const {
        if (m_rank <= CardsRanks::RANK_10) {
            return (static_cast<int>(m_rank) + 2); //since RANK_2 is 0, adding 2 will get it's actual value
        }

        switch (m_rank) {
        case CardsRanks::RANK_JACK:
        case CardsRanks::RANK_QUEEN:
        case CardsRanks::RANK_KING:
            return 10; //jack, queen, and king are all worth 10
        case CardsRanks::RANK_ACE: //Ace's are worth 11
            return 11;
        default:
            return 0;
        }
    }


};

class Deck {
public:
    // aliases that will be used throughout the class
    using deck_type = std::array<Card, 52>;
    using index_type = deck_type::size_type;
private:
    deck_type m_deck{};
    index_type m_cardIndex{ 0 };
public:
    Deck() {

        //create a deck of 52 cards
        index_type nextIndex{ 0 };

        //convert enum into int to get the total number of suits and ranks
        auto suits{ static_cast<int>(CardsSuits::MAX_SUITS) };
        auto ranks{ static_cast<int>(CardsRanks::MAX_RANKS) };

        for (int suit = 0; suit < suits; suit++) {
            for (int rank = 0; rank < ranks; rank++) {
                //passes a rank and suit that we casted from an integer into the constructor
                m_deck[nextIndex] = { static_cast<CardsRanks>(rank), static_cast<CardsSuits>(suit) };
                //next card
                nextIndex++;
            }
        }

    }

    //prints an entire deck
    void printDeck() { //passed by reference

        for (const Card card : m_deck) {
            std::cout << card.printCard() << "\n";
        }

    }

    //shuffles a deck
    void shuffleDeck() { //passed by reference

        //reset next card index
        m_cardIndex = 0;

        // mt is static so it only gets seeded once.
        static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

        std::shuffle(m_deck.begin(), m_deck.end(), mt);

        std::cout << "***********************************************The deck has been shuffled\n\n*******************************************\n\n";
    }

    //deals a card to the user 
    const Card& dealCard() {
        
        if (m_cardIndex >= m_deck.size()) { //we have run out of cards to deal so we must reshuffle the deck
            shuffleDeck();
            return m_deck[m_cardIndex++];
        }
        return m_deck[m_cardIndex++]; //returns a reference to the next card in the deck
    }


};

class Player {
private:
    int m_score{ 0 };
    int m_record{ 0 };
public:
    //deals the player one card from the deck
    std::string drawCard(Deck& deck) {

        Card drawnCard = deck.dealCard(); //get the dealed card

        m_score += drawnCard.getCardValue(); //increment the player's scire

        return drawnCard.printCard(); //returns the card name
    }

    //returns true if the player's score exceeds the maximum score
    bool isBust() const {
        return m_score > maximumScore;
    }

    int getScore() const {
        return m_score;
    }

    void clearScore() {
        m_score = 0;
    }

    void incrementRecord() {
        m_record += 1;
    }

    int getRecord() const {
        return m_record;
    }
};


//checks if a player wants to hit
bool playerHit() {
    std::cout << "Do you want to hit (h) or stand (s)?" << "\n";
    char ch{};
    std::cin >> ch;

    switch (ch)
    {
    case 'h':
        return true;
    case 's':
        return false;
    default:
        return false;
    }
}


//Plays the player's turn
bool playerTurn(Deck& deck, Player &player) {
    
    //player continues playing as long as they haven't exceeded the maximum score and they choose to hit
    while (!player.isBust() && playerHit()) { 
        std::string drawnCardName = player.drawCard(deck);
        std::cout << "You pulled a " << drawnCardName << ". You now have " << player.getScore() << ".\n";
    }
    return player.isBust(); //returns true if the player has busted
}

//Plays the dealer's turn
bool dealerTurn(Deck& deck, Player& dealer) {
    while (dealer.getScore() < minimumDealerScore) { //dealer plays as long as their score hasn't surpassed their minimum score requirement
        std::string drawnCardName = dealer.drawCard(deck);
        std::cout << "The dealer pulled a " << drawnCardName << ". He now has " << dealer.getScore() << ".\n";
    }

    return dealer.isBust(); //returns true if the dealer has busted
}

//Main function for the blackjack game
bool playBlackjack(Deck& deck, Player& player, Player& dealer) {

    //clear player's scores
    player.clearScore();
    dealer.clearScore();

    //gives the player and dealer their cards
    dealer.drawCard(deck);

    player.drawCard(deck);
    player.drawCard(deck);

    std::cout << "The dealer has: " << dealer.getScore() << ".\n";
    std::cout << "You have: " << player.getScore() << ".\n";

    //player goes first
    if (playerTurn(deck, player)) {
        return false; //player has busted
    }

    //dealer goes next
    if (dealerTurn(deck, dealer))
    {
        return true; //dealer has busted
    }
    
    //whoever has the higher score wins
    return player.getScore() > dealer.getScore();
}

//Returns true if the user chooses to play again
bool playAgain() {
    std::cout << "Do you want to play again, yes (y) or no (n)" << "\n";
    char ch{};
    std::cin >> ch;

    switch (ch)
    {
    case 'y':
        return true;
    case 'n':
        return false;
    default:
        return false;
    }
}

int main()
{
    Deck deck{};
    std::cout << "Entire shuffled deck: " << "\n";
    deck.shuffleDeck();
    deck.printDeck();

    std::cout << "\n\n===========================================================\n\nThe game has started!\n\n";

    //players
    Player player{};
    Player dealer{};

    do {
        if (playBlackjack(deck, player, dealer)) { 
            player.incrementRecord();
            std::cout << "\nYou win!" << "\n"; //player has won
        }
        else {
            dealer.incrementRecord();
            std::cout << "\nYou lose!" << "\n"; //player has lost
        }
        std::cout << "You record is now " << player.getRecord() << " - " << dealer.getRecord();
        std::cout << "\n" << "=============================================" << "\n\n";
    } while (true);

    return 0;
}

