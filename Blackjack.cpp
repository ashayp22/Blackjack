// Blackjack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <array>
#include <random>
#include <ctime>


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

//Structure for a single card
struct Card {
    CardsRanks rank{};
    CardsSuits suit{};
};

//Structure for a player
struct Player {
    int score;
};

//Returns a card's suit and rank as a string with length 2
std::string printCard(const Card &card) {

    std::string code{ "" };

    switch (card.rank)
    {
    case CardsRanks::RANK_2:
        code += '2';
        break;
    case CardsRanks::RANK_3:
        code += '3';
        break;
    case CardsRanks::RANK_4:
        code += '4';
        break;
    case CardsRanks::RANK_5:
        code += '5';
        break;
    case CardsRanks::RANK_6:
        code += '6';
        break;
    case CardsRanks::RANK_7:
        code += '7';
        break;
    case CardsRanks::RANK_8:
        code += '8';
        break;
    case CardsRanks::RANK_9:
        code += '9';
        break;
    case CardsRanks::RANK_10:
        code += 'T';
        break;
    case CardsRanks::RANK_JACK:
        code += 'J';
        break;
    case CardsRanks::RANK_QUEEN:
        code += 'Q';
        break;
    case CardsRanks::RANK_KING:
        code += 'K';
        break;
    case CardsRanks::RANK_ACE:
        code += 'A';
        break;
    default:
        break;
    }

    switch (card.suit) {
        case CardsSuits::SUIT_CLUBS:
            code += "C";
            break;
        case CardsSuits::SUIT_DIAMONDS:
            code += "D";
            break;
        case CardsSuits::SUIT_HEARTS:
            code += "H";
            break;
        case CardsSuits::SUIT_SPADES:
            code += "S";
            break;
        default:
            break;
    }

    return code;
}

// aliases that will be used throughout the program
using deck_type = std::array<Card, 52>;
using index_type = deck_type::size_type;

//constants 
constexpr int maximumScore{ 21 }; //maximum score that a player can have
constexpr int minimumDealerScore{ 17 }; //the minimum score a dealer will have
constexpr int shuffleMinimum{ 46 }; //the minimum score a dealer will have

//creates and returns a deck of 52 cards
deck_type createDeck() {

    deck_type deck{};

    index_type nextIndex{ 0 };

    //convert enum into int
    auto suits{ static_cast<int>(CardsSuits::MAX_SUITS) };
    auto ranks{ static_cast<int>(CardsRanks::MAX_RANKS) };
    
    for (int suit = 0; suit < suits; suit++) {
        for (int rank = 0; rank < ranks; rank++) {
            deck[nextIndex].suit = static_cast<CardsSuits>(suit);
            deck[nextIndex].rank = static_cast<CardsRanks>(rank);
            nextIndex++;
        }
    }

    return deck;
}

//prints an entire deck
void printDeck(const deck_type &deck) { //passed by reference

    for (const Card card: deck) {
        std::cout << printCard(card) << "\n";
    }

}

//shuffles a deck
void shuffleDeck(deck_type& deck) { //passed by reference
    // mt is static so it only gets seeded once.
    static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

    std::shuffle(deck.begin(), deck.end(), mt);
}

//returns a card's value
int getCardValue(const Card& card) {
    if (card.rank <= CardsRanks::RANK_10) { 
        return (static_cast<int>(card.rank) + 2); //since RANK_2 is 0, adding 2 will get it's actual value
    }

    switch (card.rank) {
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
bool playerTurn(deck_type& deck, Player &player, index_type& nextIndex) {
    
    //player continues playing as long as they haven't exceeded the maximum score and they choose to hit
    while (player.score <= maximumScore && playerHit()) { 
        Card newCard = deck[nextIndex++];
        player.score += getCardValue(newCard); //adds the new card's value to the player's score
        std::cout << "You pulled a " << printCard(newCard) << ". You now have " << player.score << ".\n";
    }
    return player.score > maximumScore; //returns true if the player has busted
}

//Plays the dealer's turn
bool dealerTurn(deck_type& deck, Player& dealer, index_type &nextIndex) {
    while (dealer.score < minimumDealerScore) { //dealer plays as long as their score hasn't surpassed their minimum score requirement
        Card newCard = deck[nextIndex++];
        dealer.score += getCardValue(newCard); //adds the new card's value to the dealer's score
        std::cout << "The dealer pulled a " << printCard(newCard) << ". He now has " << dealer.score << ".\n";
    }

    return dealer.score > maximumScore; //returns true if the dealer has busted
}

//Main function for the blackjack game
bool playBlackjack(deck_type& deck, index_type& nextIndex) {

    //reshuffles the deck and resets the card's next index if the game is running out of cards
    if (nextIndex >= shuffleMinimum) {
        shuffleDeck(deck);
        nextIndex = 0;
    }

    //gives the player and dealer their cards
    Player dealer{ getCardValue(deck[nextIndex++]) };

    Player player{ getCardValue(deck[nextIndex]) + getCardValue(deck[nextIndex +1]) };
    nextIndex += 2;

    std::cout << "The dealer has: " << dealer.score << ".\n";
    std::cout << "You have: " << player.score << ".\n";

    //player goes first
    if (playerTurn(deck, player, nextIndex)) {
        return false; //player has busted
    }

    //dealer goes next
    if (dealerTurn(deck, dealer, nextIndex))
    {
        return true; //dealer has busted
    }
    
    //whoever has the higher score wins
    return player.score > dealer.score;
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
    //first start by creating and shuffling the deck
    deck_type mainDeck = createDeck();
    shuffleDeck(mainDeck);
    index_type nextIndex = 0;

    do {
        if (playBlackjack(mainDeck, nextIndex)) { 
            std::cout << "You win!" << "\n"; //player has won
        }
        else {
            std::cout << "You lose!" << "\n"; //player has lost
        }
        std::cout << "\n" << "=============================================" << "\n";
    } while (true);

    return 0;
}

