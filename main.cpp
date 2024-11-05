/*

///// TO DO LIST: /////

- Implement the players as the card decks holders. ✅
- Implement the decks. ✅
- Display the decks in the two other tabs to their names. ✅
- Implement clicking on a card (when in player's tab) applying said card to the player's card displayed in the center of the window. ✅
- Implement a ready button for both tabs -> collapses the player's deck and displays a text saying which did they chose.
- Implement that once both player pressed ready, display text between the two displayed cards telling who won.
- Based on the upper to do, implement the game logic.
- Learn about putting things in the heap to save so place.

///// END TO DO LIST /////

*/

#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include "data/headers/customFunctions.h"
#include <iostream>

#include <time.h>
#include <algorithm>
#include <random>

using namespace std;


static void displayPlayerDeck(PlayerClass& player, GameDeckClass& gameDeck, CardClass& cardObject);
void cardComparing(PlayerClass& player1, PlayerClass& player2, GameDeckClass& gameDeck);

/*
 *  Definition:
 *      The deckStruct is a struct that handles the entirety of the different action
 *      permitted upon a 'deck' of cards.
 *      It holds the main functions, often used to manipulate the insides of a deck.
 */
struct deckStruct {
    vector<vector<int>> hand;

    vector<int> takeCard() {
        vector<int> cardTaken = { -1, -1 };
        if (!isDeckEmpty()) {
            cardTaken = hand[0];
            hand.erase(hand.begin());
        }
        else {
            printf("No more cards!\n");
        }
        return cardTaken;
    }

    void removeCard(vector<int> card) {
        for (int i = 0; i < hand.size(); i++) {
            if (hand[i] == card) {
                hand.erase(hand.begin() + i);
                break;
            }
        }
    }

    void addCard(vector<int> card) {
        hand.push_back(card);
    }

    bool isDeckEmpty() const {
        return hand.size() != 0 ? false : true;;
    }

    int getHandSize() const {
        return (int) hand.size();
    }

    void printCard(vector<int> card) {
        string output = "";
        output += "{";
        for (int i = 0; i < card.size(); ++i) {
            output += to_string(card[i]) + ", ";
        }
        output.resize(output.size() - 2);
        output += "}";
        cout << output << endl;
    }

};

/*
 *  Definition:
 *      The GameDeckClass is a class is used as the main frame of value 
 *      storing and card evaluation and manipulation.
 */
class GameDeckClass {
public:
    deckStruct deck;
    vector<string> cardValue = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace" };
    vector<string> cardColour = { "Clubs", "Spade", "Heart", "Diamonds" };
    map<vector<int>, string> images = {
        { {0, 0}, "01" }, { {1, 0}, "02" }, { {2, 0}, "03" }, { {3,  0}, "04" }, { {4,  0}, "05" }, { {5,  0}, "06" }, { {6, 0}, "07" }, { {7, 0}, "08" }, { {8, 0}, "09" }, { {9, 0}, "10" }, { {10, 0}, "11" }, { {11, 0}, "12" }, { {12, 0}, "00" },
        { {0, 1}, "14" }, { {1, 1}, "15" }, { {2, 1}, "16" }, { {3,  1}, "17" }, { {4,  1}, "18" }, { {5,  1}, "19" }, { {6, 1}, "20" }, { {7, 1}, "21" }, { {8, 1}, "22" }, { {9, 1}, "23" }, { {10, 1}, "24" }, { {11, 1}, "25" }, { {12, 1}, "13" },
        { {0, 2}, "27" }, { {1, 2}, "28" }, { {2, 2}, "29" }, { {3,  2}, "30" }, { {4,  2}, "31" }, { {5,  2}, "32" }, { {6, 2}, "33" }, { {7, 2}, "34" }, { {8, 2}, "35" }, { {9, 2}, "36" }, { {10, 2}, "37" }, { {11, 2}, "38" }, { {12, 2}, "26" },
        { {0, 3}, "40" }, { {1, 3}, "41" }, { {2, 3}, "42" }, { {3,  3}, "43" }, { {4,  3}, "44" }, { {5,  3}, "45" }, { {6, 3}, "46" }, { {7, 3}, "47" }, { {8, 3}, "48" }, { {9, 3}, "49" }, { {10, 3}, "50" }, { {11, 3}, "51" }, { {12, 3}, "39" }
    };

    string GetCardName(vector<int> card) {
        return cardValue[card[0]] + " of " + cardColour[card[1]];
    }

    void shuffleDeck() {
        // Uses a random int generator to shuffle the cards
        random_device rd;
        default_random_engine rng(rd());
        for (int i = 0; i < images.size(); i++) {
            shuffle(deck.hand.begin(), deck.hand.end(), rng);
        }
    }

    string getCardID(vector<int> playerCard){
        return images.at(playerCard);
    }

    vector<int> getCardVector(string cardName) {
        // Example input format: "Ace of Spade"
        vector<int> result;

        // Find the position of " of " to split the string
        size_t pos = cardName.find(" of ");
        if (pos == string::npos) {
            return result;
        }

        // Extract the value and color parts
        string valueStr = cardName.substr(0, pos);
        string colorStr = cardName.substr(pos + 4);

        // Find value in cardValue vector
        auto valueIt = find(cardValue.begin(), cardValue.end(), valueStr);
        if (valueIt == cardValue.end()) {
            return result;
        }
        int valueIndex = (int) distance(cardValue.begin(), valueIt);

        // Find colour in cardColour vector
        auto colourIt = find(cardColour.begin(), cardColour.end(), colorStr);
        if (colourIt == cardColour.end()) {
            return result;
        }
        int colourIndex = (int) distance(cardColour.begin(), colourIt);

        // Construct the result vector {valueIndex, suitIndex}
        result.push_back(valueIndex);
        result.push_back(colourIndex);

        return result;
    }

    void printDeck() {
        for (int i = 0; i < deck.hand.size(); i++) {
            cout << GetCardName(deck.hand[i]) << endl;
        }
    }

    GameDeckClass() {
        for (int i = 0; i < cardColour.size(); i++) {
            for (int j = 0; j < cardValue.size(); j++) {
                deck.addCard({ j, i });
            }
        }
    }
};

/*
 *  Definition:
 *      The CardClass is a class destined to be used as a placeholder for
 *      sprite and texture manipulation onto the screen.
 *      Its purpose is purely graphical and hold no technical functionality
 *      in relation to the deck of cards.
 */
class CardClass {
public:
    static const int NUM_CARDS = 53;

    sf::Texture cardTextures[NUM_CARDS];
    sf::Sprite cardSprites[NUM_CARDS];

    sf::Sprite getCardSpriteAt(int index) {
        if (index < NUM_CARDS) {
            return cardSprites[index];
        }

        return cardSprites[NUM_CARDS - 1];
    }

    CardClass() {
        for (int i = 0; i < NUM_CARDS; i++) {
            string cardLoaded = "./data/textures/cards/" + zeroPaddingInt(i) + ".png";
            cardTextures[i].loadFromFile(cardLoaded);
            cardSprites[i].setTexture(cardTextures[i]);
        }
    }
};

/*
 *  Definition:
 *      The PlayerClass is a class that has purpose in quantifying the different elements
 *      required in order to play a game of war.
 *      It also holds useful functions allowing the player to easily interfer with its deck.
 */
class PlayerClass {
public:
    GameDeckClass placeholder;
    deckStruct playerDeck;
    deckStruct waitingDeck;
    vector<int> currentCard = { -1, -1 };
    int cardID;
    int playerID;
    bool confirmedCard = false;
    sf::Sprite cardSprite;

    void checkDeck() {
        if (playerDeck.hand.size() < 6) {
            for (int i = 0; i < waitingDeck.hand.size(); i++) {
                waitingDeck.addCard(waitingDeck.hand[i]);
                waitingDeck.hand.erase(waitingDeck.hand.begin());
            }
            if (waitingDeck.hand.size() != 0) { waitingDeck.hand.clear(); }
        }
    }

    void printDeck() {
        for (int i = 0; i < playerDeck.hand.size(); i++) {
            cout << placeholder.GetCardName(playerDeck.hand[i]) << ", ";
        }
    }

    void playCard(string card) {
        currentCard = placeholder.getCardVector(card);
    }

    PlayerClass(int pID, GameDeckClass& cardGameDeck) { //Sets the playerID's to the number given in argument.
        cardID = 52; // Last card in the texture deck.
        playerID = pID;
        placeholder = cardGameDeck;
    }

};


/*
 *  Definition:
 *      The main function. While obligated in order to run the code, here lies 
 *      the entirety of the window's logic, variable storing, game logic and many more things.
 */
int main()
{
    int windowSize[2] = {1336, 780};
    sf::RenderWindow window(sf::VideoMode(windowSize[0], windowSize[1]), "Window Title", sf::Style::Titlebar | sf::Style::Close);
    ImGui::SFML::Init(window);

    CardClass* cardObject = new CardClass(); // Very big class that requires it to be in the heap
    GameDeckClass gameDeck;

    EditableSliderState player1Slider, player2Slider;

    PlayerClass P1(1, gameDeck);
    sf::Sprite* p1Card = &P1.cardSprite;

    PlayerClass P2(2, gameDeck);
    sf::Sprite* p2Card = &P2.cardSprite;

    gameDeck.shuffleDeck();
    for (int i = 0; i < (gameDeck.deck.getHandSize() / 2); i++) {
        P1.playerDeck.addCard(gameDeck.deck.takeCard());
    }
    for (int i = 0; i < gameDeck.deck.getHandSize(); i++) {
        P2.playerDeck.addCard(gameDeck.deck.takeCard());
    }

    p1Card->setPosition(
        (float)((windowSize[0] / 2) - cardObject->cardTextures[0].getSize().x / 2), // X position
        (float)((windowSize[1] / 3) - cardObject->cardTextures[0].getSize().y / 2) // Y position
    );
    p2Card->setPosition(
        (float)((windowSize[0] / 2) - cardObject->cardTextures[0].getSize().x / 2), // X position
        (float)(((windowSize[1] * 2) / 3) - cardObject->cardTextures[0].getSize().y / 2) // Y position
    );

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        // ImGui Window's Flags
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

        ImGui::SetNextWindowSize(ImVec2(400, 560), ImGuiCond_FirstUseEver);

        // ImGui Logic Here.
        ImGui::Begin("Window title", nullptr, windowFlags);

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {   
            // Debugging tab
            if (ImGui::BeginTabItem("Debugging"))
            {   

                // Sliders to swiftly change the player's current cards.
                if (EditableSliderInt("Player 1", &P1.cardID, 0, 52, player1Slider)) {
                    P1.cardSprite.setTexture(cardObject->cardTextures[P1.cardID]);
                }
                if (EditableSliderInt("Player 2", &P2.cardID, 0, 52, player2Slider)) {
                    P2.cardSprite.setTexture(cardObject->cardTextures[P2.cardID]);
                }

                ImGui::Separator();

                // This tree nodes is organized in 4 parts, each containing every 13 cards
                // seperated per category, as Spades, Clubs, Hearts and Diamonds.
                // 
                // PS: COULD PROBABLY BE BETTER CODED!
                int i;
                if (ImGui::TreeNode("Cards Sprites")) {
                    if (ImGui::TreeNode("Spades")){
                        ImGui::NewLine();
                        for (i = 0; i < 13; i++) {
                            if (i != 12 && i % 3 != 0) ImGui::SameLine();
                            ImGui::ImageButton(cardObject->getCardSpriteAt(i));
                        }
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode("Clubs")) {
                        for (i = 13; i < 26; i++) {
                            ImGui::ImageButton(cardObject->getCardSpriteAt(i));
                            if (i != 24 && i % 3 != 0) ImGui::SameLine();
                        }
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode("Hearts")) {
                        for (i = 26; i < 39; i++) {
                            ImGui::ImageButton(cardObject->getCardSpriteAt(i));
                            if (i != 38 && i % 3 != 0) ImGui::SameLine();
                        }
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode("Diamonds")) {
                        for (i = 39; i < 52; i++) {
                            ImGui::ImageButton(cardObject->getCardSpriteAt(i));
                            if (i != 52 && i % 3 != 0) ImGui::SameLine();
                        }
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Player 1")) {
                if (!P1.confirmedCard) {
                    if (ImGui::TreeNode("Deck:")) {
                        displayPlayerDeck(P1, gameDeck, *cardObject);
                        ImGui::TreePop();
                    }
                    if (ImGui::Button("Confirm Choosen Card?")) {
                        if (P1.cardID != 52) {
                            P1.confirmedCard = true;
                            cout << "Player 1 as choosen: " << gameDeck.GetCardName(P1.currentCard) << endl;
                        }
                    }
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Player 2")) {
                if (!P2.confirmedCard) {
                    if (ImGui::TreeNode("Deck:")) {
                        displayPlayerDeck(P2, gameDeck, *cardObject);
                        ImGui::TreePop();
                    }

                    if (ImGui::Button("Confirm Choosen Card?")) {
                        if (P2.cardID != 52) {
                            P2.confirmedCard = true;
                            cout << gameDeck.GetCardName(P2.currentCard) << endl;
                        }
                    }
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();

        // Looping Logics Here.
        window.clear(sf::Color(18, 33, 43)); // Color background

        ////////////////////////////////////////////////////////////////////



        // Upon confirmation of both parties:
        // >> Start game logic HERE.
        if (P1.confirmedCard && P2.confirmedCard) {
            

            // Lasting thing done is the reactivation of the decks and buttons.
            P1.confirmedCard = P2.confirmedCard = false;
        }



        ////////////////////////////////////////////////////////////////////

        // Draw everything past here.
        window.draw(*p1Card);
        window.draw(*p2Card);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    delete(cardObject);
    return 0;
}

static void displayPlayerDeck(PlayerClass& player, GameDeckClass& gameDeck, CardClass& cardObject) {
    for (int i = 0; i < player.playerDeck.getHandSize(); i++) {
        if (i % 3 != 0) ImGui::SameLine();

        string cardIDStr = gameDeck.getCardID(player.playerDeck.hand[i]);
        int cardID = stoi(cardIDStr);
        string cardName = gameDeck.GetCardName(player.playerDeck.hand[i]);

        if (ImGui::ImageButton(cardObject.getCardSpriteAt(cardID))) {
            player.cardSprite.setTexture(cardObject.cardTextures[cardID]);
            player.currentCard = gameDeck.getCardVector(cardName);
            player.cardID = cardID;

            cout << "P" << player.playerID << " clicked card: " << cardName << " (ID: " << zeroPaddingInt(cardID) << ")" << endl;
            printf("P%d current card: %d, %d (ID:)\n", player.playerID, player.currentCard[0], player.currentCard[1]);
        }
    }
}