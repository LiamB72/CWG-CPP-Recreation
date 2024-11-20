/*
// This file is custom-made by my hands.
// It features some of the different functions I'd use
// in the main.cpp file. Just to avoid overloading it and is a bit cleaner. :p
*/


#pragma once

#include "SFML/Graphics.hpp"

#include <iostream>
#include <iomanip>

#include <algorithm>
#include <random>

#include <string>
#include <map>
#include <vector>

using namespace std;

#ifndef CWG_MODULE_H
#define CWG_MODULE_H



struct EditableSliderState; // Helper to hold an int if an editable slider has entered edit mode.
struct CardStruct;          // Structure to define how a card is made. Contains a function give its name.
struct DeckStruct;          // Helper functions to manipulate a deck (array) of cards (CardStruct).
struct GameDeckStruct;      // Helper to get the card ID (int) by the card's name (string) or the card itself (CardStruct). Also, you can get a card (CardStruct) by giving an ID (int).
struct CardGraphicsStruct;  // Stores all the graphical images and functions.
struct PlayerStruct;        // Holds multiple structures to be manipulated with the functions provided within said struct.

#ifndef CARD_VALUES
const vector<string> CARD_VALUES = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace" };
#endif // !CARD_VALUES
#ifndef CARD_SUITS
const vector<string> CARD_SUITS = { "Clubs", "Spade", "Heart", "Diamonds" };
#endif // !CARD_SUITS
#ifndef DEFAULT_CARD_ID
constexpr int DEFAULT_CARD_ID = 52;
#endif // !DEFAULT_CARD_ID;


/*
//  Description:
//      Takes in a player, the graphic card and a deck.
//      It uses those classes and structure to display a deck in its entirety.
//      Moreover, when one card is clicked, its information is printed
//      onto the command line.
//      BOOL changeCard IS TEMPORARY AND ONLY FOR DEBUG PURPOSES.
//      Returns nothing.
*/
void displayPlayerDeck(PlayerStruct& player, const CardGraphicsStruct& cardGraphics, const DeckStruct& deck, bool changeCard = true);

/*
//  Description:
//      Takes in two players, one supposed round winner and the other, the round loser.
//      This function acts on both players' values. It removes the current card of
//      both players to put them inside the winner's waiting deck.
//
//      Moreover, the winner's victory count is increased by one.
//
//      Returns nothing.
*/
void awardRound(PlayerStruct& winner, PlayerStruct& loser, const CardGraphicsStruct& cardGraphics, const bool& draw, int& battleTime, DeckStruct* battleDeck);

/*
//  Description:
//      Takes in parameters:
//      A player class,
//      an int.
//
//      Function as it takes the cardID, creates a temporary
//      cardStruct item with it. To apply said cardStruct to the
//      dest (destination).
//
//      Return 0 if the cardID isn't DEFAULT_CARD_ID (52).
//      Return 1 if the cardID is DEFAULT_CARD_ID (52).
*/
int applyNewCard(PlayerStruct& dest, const int& cardID);

/*
//  Description:
// Creates a player tab for the playerMenu debug menu.
*/
void playerTab(PlayerStruct& player, const CardGraphicsStruct& cardGraphics, int& newCardID, bool& showPlayerActionMenu);


int EditableSliderInt(const char* label, int* value, int min, int max, EditableSliderState& state);


int EditableSliderFloat(const char* label, float* value, float min, float max, EditableSliderState& state);


string zeroPaddingInt(int num);


void showTooltip(int cardIndex);


void setVictoryScreen(PlayerStruct& winner, PlayerStruct& notWinner, sf::Text& previousWinner, bool actionMenu0, bool actionMenu1);

struct EditableSliderState {
    bool editMode = false;
};


struct CardStruct {
    int value;
    int suit;

    CardStruct(const int v = -1, const int s = -1) : value(v), suit(s) {}

    string getName() const {
        return CARD_VALUES[value] + " of " + CARD_SUITS[suit];
    }

    bool operator==(const CardStruct& otherCard) const {
        return value == otherCard.value;
    }

    bool operator<(const CardStruct& otherCard) const {
        // Defines ordering based on `value` first, then `suit`
        if (value != otherCard.value) {
            return value < otherCard.value;
        }
        return suit < otherCard.suit;
    }
};


extern CardStruct DEFAULT_CARD;
extern map<CardStruct, string> IMAGES;


/*
//  Definition:
// The DeckStruct is a struct that handles the entirety of the different action
// permitted upon a 'deck' of cards.
// It holds the main functions, often used to manipulate the insides of a deck.
*/
struct DeckStruct {
    vector<CardStruct> hand;

    CardStruct takeCard(CardStruct& card = DEFAULT_CARD) {
        if (!isDeckEmpty()) {

            // If the card isn't specified, then take the first to come
            // PS: THIS IS SUPPOSED TO BE THE ACTUAL WAY TO PLAY
            //     YOU DON'T NORMALLY CHOOSE WHICH ONE YOU ARE GOING TO PLAY!
            //     BUT RIGHT NOW IT IS FOR TESTING PURPOSE, SO BEAR IN MIND THAT!!!
            if (card == DEFAULT_CARD) {
                card = hand[0];
                hand.erase(hand.begin());
                return card;
            }
            // Searching through the hand for the vector (card)
            for (int i = 0; i < hand.size(); i++) {
                if (card == hand[i]) {
                    hand.erase(hand.begin() + i);
                    return card;
                }
            }
        }
        cout << "no more card!\n";
        return card;
    }

    void removeCard(const CardStruct& card) {
        for (int i = 0; i < hand.size(); i++) {
            if (hand[i] == card) {
                hand.erase(hand.begin() + i);
                break;
            }
        }
    }

    void addCard(const CardStruct& card) {
        hand.push_back(card);
    }

    bool isDeckEmpty() const {
        return hand.empty();
    }

    int getHandSize() const {
        /* The C way: return (int)(hand.size());*/
        return static_cast<int> (hand.size());
    }

    /*Static member functions:
        Declared inside a class, a static member function can be called without creating an object of the class.
        It has access only to other static members and not to non-static members (instance variables and methods).
        Static member functions are essentially global functions that happen to be part of a class
    
    static void printCard(const CardStruct& card) {
        printf("{%d, %d}", card.value, card.suit);
    }
    

    void printDeck() const {
        
        // The C ways to do it:
        //  for (int i = 0; i < hand.size(); i++) {
        //      printf("%s, ", hand[i].getName());
        //  }
         
        for (auto& i : hand) {
            cout << i.getName() << ", ";
        }
    }
    */

};

/*
//  Definition:
//      The GameDeckClass is a class that is used as the main frame of value
// storing and card evaluation and manipulation.
*/

struct GameDeckStruct {
    DeckStruct deck;

    void shuffleDeck() {
        // Uses a random int generator to shuffle the cards
        random_device rd;
        default_random_engine rng(rd());
        for (int i = 0; i < IMAGES.size(); i++) {
            shuffle(deck.hand.begin(), deck.hand.end(), rng);
        }
    }

    static int getCardID(const CardStruct& playerCard) {
        return stoi(IMAGES.at(playerCard));
    }

    static CardStruct getCardByID(const int cardID) {
        if (cardID != 52) {
            CardStruct cardFound;
            for (const auto& pair : IMAGES) {
                if (pair.second == zeroPaddingInt(cardID)) {
                    cardFound.suit = pair.first.suit;
                    cardFound.value = pair.first.value;
                    return cardFound;
                }
            }
            return cardFound;
        }
        return { -1, -1 };
    }

    static CardStruct getCardByName(string& cardName) {
        // Example input format: "Ace of Spade"
        CardStruct result;

        // Find the position of " of " to split the string
        const size_t pos = cardName.find(" of ");
        if (pos == string::npos) {
            return result;
        }

        // Extract the value and colour parts
        const string valueStr = cardName.substr(0, pos);
        const string colorStr = cardName.substr(pos + 4);

        // Find value in cardValue vector
        const auto valueIt = find(CARD_VALUES.begin(), CARD_VALUES.end(), valueStr);
        if (valueIt == CARD_VALUES.end()) {
            return result;
        }
        const int valueIndex = static_cast<int> (distance(CARD_VALUES.begin(), valueIt));

        // Find colour in cardColour vector
        const auto colourIt = find(CARD_SUITS.begin(), CARD_SUITS.end(), colorStr);
        if (colourIt == CARD_SUITS.end()) {
            return result;
        }
        const int suitIndex = static_cast<int> (distance(CARD_SUITS.begin(), colourIt));

        // Construct the result vector {valueIndex, suitIndex}
        result.value = valueIndex;
        result.suit = suitIndex;

        return result;
    }

    GameDeckStruct() {
        for (int i = 0; i < CARD_SUITS.size(); i++) {
            for (int j = 0; j < CARD_VALUES.size(); j++) {
                deck.addCard({ j, i });
            }
        }
    }
};

/*
//  Definition:
// The CardClass is a class destined to be used as a placeholder for
// sprite and texture manipulation onto the screen.
// Its purpose is purely graphical and holds no technical functionality
// in relation to the deck of cards.
*/
struct CardGraphicsStruct {
    static constexpr int NUM_CARDS = 53;

    sf::Texture cardTextures[NUM_CARDS];
    sf::Sprite cardSprites[NUM_CARDS];

    sf::Sprite getCardSpriteAt(const int index) const {
        if (index < NUM_CARDS) {
            return cardSprites[index];
        }

        return cardSprites[NUM_CARDS - 1];
    }

    CardGraphicsStruct() {
        for (int i = 0; i < NUM_CARDS; i++) {
            string cardLoaded = "./data/textures/cards/" + zeroPaddingInt(i) + ".png";
            cardTextures[i].loadFromFile(cardLoaded);
            cardSprites[i].setTexture(cardTextures[i]);
        }
    }
};

/*
//  Definition:
// The PlayerClass is a class that has a purpose in quantifying the different elements
// required to play a game of war.
// It also holds useful functions allowing the player to easily interfere with its deck.
*/
struct PlayerStruct {

    DeckStruct playerDeck;
    DeckStruct waitingDeck;
    CardStruct currentCard = DEFAULT_CARD;
    int cardID;
    int playerID;
    int victoryCount = 0;
    bool confirmedCard = false;
    sf::Sprite cardSprite;


    void checkDeck() {
        if (playerDeck.hand.size() < 6) {
            int waitingDeckSize = static_cast<int>(waitingDeck.hand.size());
            for (int i = 0; i < waitingDeckSize; i++) {
                playerDeck.addCard(waitingDeck.takeCard());
            }
        }
    }

    void playCard() {
        checkDeck();
        if (!playerDeck.isDeckEmpty()) {
            currentCard = playerDeck.hand[0];
            cardID = GameDeckStruct::getCardID(playerDeck.hand[0]);
            confirmedCard = true;
        }
    }

    explicit PlayerStruct(const int& pID, const CardGraphicsStruct& cardGraphics) { //Sets the playerID's to the number given in argument.
        cardID = 52; // Last card in the texture deck.
        playerID = pID;
        cardSprite.setTexture(cardGraphics.cardTextures[cardID]);
    }

};

#endif // !CWG_MODULE_H
