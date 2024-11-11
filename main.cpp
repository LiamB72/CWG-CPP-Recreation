/*

/////   TO DO LIST:     /////

///// END OF TO DO LIST /////

Changelog:

07-11-2024 :
- Added post turn card exchanging function named awardTurn with winner and loser parameters.
- Added a fancy card struct :
        + easy of use with 2 variables : value and suit.
        + easy of call : call the struct and just say its value and suit with having to do the { ... }.
        + fancy getName function that let other classes and structs not have to repeat said function.
        + custom behavior for the == operator of comparaison, as much as for the < operator for the map ordering.

- Changed the cardValue to global variable CARD_VALUES.
- Changed the cardColours to global variable CARD_SUITS.
- Added tons of documentation to help for future me or other readers.
- Changed the vector<int> to the card structure.
- Removed two occurence of 'printDeck' function in player and gameDeck classes.
- Added printDeck to the deck class.
- Learned about the const keyword for function, let us and the computer know it only reads and does not change any value.
- Removed now useless extra parameter of gameDeck class as getCardName is no longer useful.
- Removed getCardName function from gameDeck class.

11-11-2024:
- Added clarification on the magic value '52'
- Tackled the issue of the "draw" situation upon having two of the same value of both player's current card.
- Updated the "awardRound" function accordingly to previous line.
- Change the parameter of the awardRound.
- Added text to tell which player won the previous round.
- Finished the previous TO-DO List :
    - Implement the players as the card decks holders. ✅
    - Implement the decks. ✅
    - Display the decks in the two other tabs to their names. ✅
    - Implement clicking on a card (when in player's tab) applying said card to the player's card displayed in the center of the window. ✅
    - Implement a ready button for both tabs -> collapses the player's deck and displays a text saying which did they chose. ✅
    - Based on the upper to do, implement the game logic. ✅
    - Implement that once both player pressed ready, display text between the two displayed cards telling who won. ✅
    - Learn about putting things in the heap to save so place. ✅
    

///// END OF CHANGELOG /////

*/

#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include "data/headers/customFunctions.h"
#include <iostream>

#include <time.h>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>

using namespace std;

const vector<string> CARD_VALUES = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace" };
const vector<string> CARD_SUITS = { "Clubs", "Spade", "Heart", "Diamonds" };

const int TARGET_FPS = 144;
const auto FRAME_DURATION = chrono::milliseconds(1000 / TARGET_FPS);


/* 
   ################################ ################################ ################################
   ################################                                  ################################
   ################################            STRUCTURES            ################################
   ################################                                  ################################
   ################################ ################################ ################################
*/

struct CardStruct {
    int value;
    int suit;

    CardStruct(int v = -1, int s = -1) : value(v), suit(s) {}

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

CardStruct DEFAULT_CARD(-1, -1);
int DEFAULT_CARD_ID = 52;

/*
 *  Definition:
 *      The deckStruct is a struct that handles the entirety of the different action
 *      permitted upon a 'deck' of cards.
 *      It holds the main functions, often used to manipulate the insides of a deck.
 */
struct deckStruct {
    vector<CardStruct> hand;

    CardStruct takeCard(CardStruct& card = DEFAULT_CARD) {
        if (!isDeckEmpty()) {

            // If the card isn't specified then take the first to come
            // PS: THIS IS SUPPOSED TO BE THE ACTUAL WAY TO PLAY
            //     YOU DON'T NORMALLY CHOOSE WHICH ON YOU ARE GOING TO PLAY!
            //     BUT RIGHT NOW IT IS FOR TESTING PURPOSE SO BEAR IN MIND THAT!!!
            if (card == DEFAULT_CARD) {
                card = hand[0];
                hand.erase(hand.begin());
                return card;
            }
            else {

                // Searching through the hand for the vector (card)
                for (int i = 0; i < hand.size(); i++) {
                    if (card == hand[i]) {
                        hand.erase(hand.begin() + i);
                        return card;
                    }
                }

                printf("The card specified was not found!\n");
            }
        }
        else {
            printf("No more cards!\n");
        }
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
        return hand.size() != 0 ? false : true;;
    }

    int getHandSize() const {
        return (int) hand.size();
    }

    void printCard(const CardStruct& card) {
        printf("{%d, %d}", card.value, card.suit);
    }

    void printDeck() {
        for (int i = 0; i < hand.size(); i++) {
            cout << hand[i].getName() << ", ";
        }
    }

};

/* 
   ################################ ################################ ################################
   ################################                                  ################################
   ################################             CLASSES              ################################
   ################################                                  ################################
   ################################ ################################ ################################
*/

/*
 *  Definition:
 *      The GameDeckClass is a class that is used as the main frame of value 
 *      storing and card evaluation and manipulation.
 */
class GameDeckClass {
public:
    deckStruct deck;
    map<CardStruct, string> images = {
        { {0, 0}, "01" }, { {1, 0}, "02" }, { {2, 0}, "03" }, { {3,  0}, "04" }, { {4,  0}, "05" }, { {5,  0}, "06" }, { {6, 0}, "07" }, { {7, 0}, "08" }, { {8, 0}, "09" }, { {9, 0}, "10" }, { {10, 0}, "11" }, { {11, 0}, "12" }, { {12, 0}, "00" },
        { {0, 1}, "14" }, { {1, 1}, "15" }, { {2, 1}, "16" }, { {3,  1}, "17" }, { {4,  1}, "18" }, { {5,  1}, "19" }, { {6, 1}, "20" }, { {7, 1}, "21" }, { {8, 1}, "22" }, { {9, 1}, "23" }, { {10, 1}, "24" }, { {11, 1}, "25" }, { {12, 1}, "13" },
        { {0, 2}, "27" }, { {1, 2}, "28" }, { {2, 2}, "29" }, { {3,  2}, "30" }, { {4,  2}, "31" }, { {5,  2}, "32" }, { {6, 2}, "33" }, { {7, 2}, "34" }, { {8, 2}, "35" }, { {9, 2}, "36" }, { {10, 2}, "37" }, { {11, 2}, "38" }, { {12, 2}, "26" },
        { {0, 3}, "40" }, { {1, 3}, "41" }, { {2, 3}, "42" }, { {3,  3}, "43" }, { {4,  3}, "44" }, { {5,  3}, "45" }, { {6, 3}, "46" }, { {7, 3}, "47" }, { {8, 3}, "48" }, { {9, 3}, "49" }, { {10, 3}, "50" }, { {11, 3}, "51" }, { {12, 3}, "39" }
    };

    void shuffleDeck() {
        // Uses a random int generator to shuffle the cards
        random_device rd;
        default_random_engine rng(rd());
        for (int i = 0; i < images.size(); i++) {
            shuffle(deck.hand.begin(), deck.hand.end(), rng);
        }
    }

    string getCardID(const CardStruct& playerCard){
        return images.at(playerCard);
    }

    CardStruct getCardByID(int cardID) {
        CardStruct cardFound;
        for (const auto& pair : images) {
            if (pair.second == to_string(cardID)) {
                cardFound.suit = pair.first.suit;
                cardFound.value = pair.first.value;
                break;
            }
        }
        return cardFound;
    }

    CardStruct getCardByName(string cardName) {
        // Example input format: "Ace of Spade"
        CardStruct result;

        // Find the position of " of " to split the string
        size_t pos = cardName.find(" of ");
        if (pos == string::npos) {
            return result;
        }

        // Extract the value and color parts
        string valueStr = cardName.substr(0, pos);
        string colorStr = cardName.substr(pos + 4);

        // Find value in cardValue vector
        auto valueIt = find(CARD_VALUES.begin(), CARD_VALUES.end(), valueStr);
        if (valueIt == CARD_VALUES.end()) {
            return result;
        }
        int valueIndex = (int) distance(CARD_VALUES.begin(), valueIt);

        // Find colour in cardColour vector
        auto colourIt = find(CARD_SUITS.begin(), CARD_SUITS.end(), colorStr);
        if (colourIt == CARD_SUITS.end()) {
            return result;
        }
        int suitIndex = (int) distance(CARD_SUITS.begin(), colourIt);

        // Construct the result vector {valueIndex, suitIndex}
        result.value = valueIndex;
        result.suit = suitIndex;

        return result;
    }

    GameDeckClass() {
        for (int i = 0; i < CARD_SUITS.size(); i++) {
            for (int j = 0; j < CARD_VALUES.size(); j++) {
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

    sf::Sprite getCardSpriteAt(int index) const {
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
    deckStruct playerDeck;
    deckStruct waitingDeck;
    CardStruct currentCard = DEFAULT_CARD;
    int cardID;
    int playerID;
    int victoryCount = 0;
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

    void playCard(const CardStruct& card) {
        currentCard = card;
        confirmedCard = true;
    }

    PlayerClass(int pID) { //Sets the playerID's to the number given in argument.
        cardID = 52; // Last card in the texture deck.
        playerID = pID;    }

};

static void displayPlayerDeck(PlayerClass& player, GameDeckClass& gameDeck, CardClass& cardObject);
static void awardRound(PlayerClass& winner, PlayerClass& loser, CardClass& cardObject, bool draw, int* battleTime, deckStruct* battleDeck);


/* 
   ################################ ################################ ################################
   ################################                                  ################################
   ################################          MAIN FUNCTION           ################################
   ################################                                  ################################
   ################################ ################################ ################################
*/


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


    // Initialisation of basic values;
    CardClass* cardObject = new CardClass(); // Very big class that requires it to be in the heap
    GameDeckClass gameDeck;
    deckStruct battleDeck;
    int battleTime = 0;

    EditableSliderState player1Slider, player2Slider;


    // Players initialisation
    PlayerClass P1(1);
    sf::Sprite* p1Card = &P1.cardSprite;
    bool p1ShowReadyText = true;

    PlayerClass P2(2);
    sf::Sprite* p2Card = &P2.cardSprite;
    bool p2ShowReadyText = true;
    

    // Deck Manipulation
    gameDeck.shuffleDeck();
    for (int i = 0; i < (gameDeck.deck.getHandSize() / 2); i++) {
        P1.playerDeck.addCard(gameDeck.deck.takeCard());
    }
    for (int i = 0; i < gameDeck.deck.getHandSize(); i++) {
        P2.playerDeck.addCard(gameDeck.deck.takeCard());
    }


    // Graphics gestion
    p1Card->setPosition(
        (float)((windowSize[0] / 2) - cardObject->cardTextures[0].getSize().x / 2), // X position
        (float)((windowSize[1] / 3) - cardObject->cardTextures[0].getSize().y / 2) // Y position
    );
    p2Card->setPosition(
        (float)((windowSize[0] / 2) - cardObject->cardTextures[0].getSize().x / 2), // X position
        (float)(((windowSize[1] * 2) / 3) - cardObject->cardTextures[0].getSize().y / 2) // Y position
    );

    sf::Text previousRoundWinner;
    previousRoundWinner.setPosition(
        (float)(windowSize[0] * 1.25 / 3),
        (float)(windowSize[1] / 2) - 15
    );

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) { // Make sure to replace with a valid font path
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    // Create a text object
    previousRoundWinner.setFont(font);                      // Set the loaded font
    previousRoundWinner.setString("No Winner Yet.");          // Set the text content
    previousRoundWinner.setCharacterSize(30);               // Set the character size
    previousRoundWinner.setFillColor(sf::Color::White);     // Set the color


    // Other values
    sf::Clock deltaClock;
    while (window.isOpen())
    {
        auto frameStartTime = std::chrono::steady_clock::now();  // Start frame timing

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
                    P1.currentCard = gameDeck.getCardByID(P1.cardID);
                }

                if (EditableSliderInt("Player 2", &P2.cardID, 0, 52, player2Slider)) {
                    P2.cardSprite.setTexture(cardObject->cardTextures[P2.cardID]);
                    P2.currentCard = gameDeck.getCardByID(P2.cardID);
                }

                ImGui::Separator();

                // This tree nodes is organized in 4 parts, each containing every 13 cards
                // seperated per category, as Clubs, Spades, Hearts and Diamonds.
                // 
                // PS: COULD PROBABLY BE BETTER CODED!
                int i;
                if (ImGui::TreeNode("Clubs")) {
                    ImGui::NewLine();
                    for (i = 0; i < 13; i++) {
                        if (i != 12 && i % 3 != 0) ImGui::SameLine();
                        ImGui::ImageButton(cardObject->getCardSpriteAt(i));
                    }
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Spades")) {
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
                ImGui::Separator();
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
                            cout << "Player 1 as choosen: " << P1.currentCard.getName() << endl;
                        }
                    }
                }
                if (p1ShowReadyText) 
                    ImGui::Text("You have not selected a card yet!");

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
                            cout << "Player 2 as choosen: " << P2.currentCard.getName() << endl;
                        }
                        else {
                            cout << "P2 not ready\n";
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

        if (P1.cardID != 52)
            p1ShowReadyText = false;
        else
            p1ShowReadyText = true;

        if (P2.cardID != 52)
            p2ShowReadyText = false;
        else
            p2ShowReadyText = true;

        ////////////////////////////////////////////////////////////////////
        // 
        // 
        // Upon confirmation of both parties:
        // >> Start game logic HERE.
        if (P1.confirmedCard && P2.confirmedCard) {
            
            if (P1.currentCard.value > P2.currentCard.value) {
                // Player 1 won
                // Winner Text Y position: (p1Card->getPosition().y + p2Card->getPosition().y) / 2
                previousRoundWinner.setString("Player 1 won this round!");
                if (!battleTime) awardRound(P1, P2, *cardObject, false, &battleTime, &battleDeck);
                else awardRound(P1, P2, *cardObject, false, &battleTime, &battleDeck);
            }
            else if (P1.currentCard.value < P2.currentCard.value){
                // Player 2 won
                previousRoundWinner.setString("Player 2 won this round!");
                if (!battleTime) awardRound(P2, P1, *cardObject, false, &battleTime, &battleDeck);
                else awardRound(P2, P1, *cardObject, true, &battleTime, &battleDeck);
            }
            else {
                // Draw
                // The order doesn't matter, it's a draw, nobody has won.
                previousRoundWinner.setString("IT'S A DRAW!");
                battleTime += 2;
                awardRound(P1, P2, *cardObject, true, &battleTime, &battleDeck);
            }

            // Lasting thing done is the reactivation of the decks and buttons.
            P1.confirmedCard = P2.confirmedCard = false;
        }
        //
        //
        ////////////////////////////////////////////////////////////////////

        // Draw everything past here.
        window.draw(*p1Card);
        window.draw(*p2Card);
        window.draw(previousRoundWinner);

        ImGui::SFML::Render(window);
        window.display();

        auto frameEndTime = std::chrono::steady_clock::now();
        auto frameTime = frameEndTime - frameStartTime;

        // If the frame finished early, delay to cap FPS
        if (frameTime < FRAME_DURATION) {
            this_thread::sleep_for(FRAME_DURATION - frameTime);
        }
    }

    ImGui::SFML::Shutdown();
    delete(cardObject);
    return 0;
}

/* 
   ################################ ################################ ################################
   ################################                                  ################################
   ################################         HELPER FUNCTIONS         ################################
   ################################                                  ################################
   ################################ ################################ ################################
*/


/*
 *  Description:
 *      Takes in a player, the gameDeck and the graphic card classes.
 *      It uses those classes to display a player's deck in its entierty.
 * 
 *      Moreover, when one card is clicked, its information is printed 
 *      onto the command line.
 *
 *      Returns nothing.
 */
static void displayPlayerDeck(PlayerClass& player, GameDeckClass& gameDeck, CardClass& cardObject) {
    for (int i = 0; i < player.playerDeck.getHandSize(); i++) {
        if (i % 3 != 0) ImGui::SameLine();

        string cardIDStr = gameDeck.getCardID(player.playerDeck.hand[i]);
        int cardID = stoi(cardIDStr);
        string cardName = player.playerDeck.hand[i].getName();

        if (ImGui::ImageButton(cardObject.getCardSpriteAt(cardID))) {
            /*player.cardSprite.setTexture(cardObject.cardTextures[cardID]);*/
            player.currentCard = gameDeck.getCardByName(cardName);
            player.cardID = cardID;

            cout << "P" << player.playerID << " clicked card: " << cardName << " (ID: " << zeroPaddingInt(cardID) << ")" << endl;
            cout << "P" << player.playerID << " current card: [" << player.currentCard.value << ", " << player.currentCard.suit << "] (ID: " << zeroPaddingInt(cardID) << ")" << endl;
        }
    }
}


/*
 *  Description:
 *      Takes in two players, one supposed round winner and the other, the round loser.
 *      This function acts on both player's values. It removes the current card of 
 *      both players to put them inside of the winner's waiting deck.
 *      
 *      Moreover, the winner's victory count is increased by one.
 *      
 *      Returns nothing.
 */
static void awardRound(PlayerClass& winner, PlayerClass& loser, CardClass& cardObject, bool draw, int* battleTime, deckStruct* battleDeck) {
    if (!draw && !*battleTime) {
        // The term 'loser' is not degrading as in "booo you loser!". 
        // it's more for technicality and understanding of the roles
        printf("Player %d won\n", winner.playerID);

        // 3 stones in 2 lines:
        // adding to the waiting deck of the winner, the card of both winner and loser
        // moreover, the cards from both parties are also removed from their deck.
        winner.waitingDeck.addCard(winner.playerDeck.takeCard(winner.currentCard));
        winner.waitingDeck.addCard(loser.playerDeck.takeCard(loser.currentCard));

        winner.cardSprite.setTexture(cardObject.cardTextures[winner.cardID]);
        loser.cardSprite.setTexture(cardObject.cardTextures[loser.cardID]);

        winner.currentCard = loser.currentCard = DEFAULT_CARD;
        winner.cardID = loser.cardID = DEFAULT_CARD_ID;

        winner.victoryCount++;
    }
    else if (draw && *battleTime == 1) {
        printf("Player %d won\n", winner.playerID);
        winner.waitingDeck.addCard(winner.playerDeck.takeCard(winner.currentCard));
        winner.waitingDeck.addCard(loser.playerDeck.takeCard(loser.currentCard));
        for (int i = 0; i < battleDeck->getHandSize(); i++) {
            winner.waitingDeck.addCard(battleDeck->takeCard());
        }

        winner.cardSprite.setTexture(cardObject.cardTextures[winner.cardID]);
        loser.cardSprite.setTexture(cardObject.cardTextures[loser.cardID]);

        winner.currentCard = loser.currentCard = DEFAULT_CARD;
        winner.cardID = loser.cardID = DEFAULT_CARD_ID;

        winner.victoryCount++;
        battleTime--;
    }
    else if (draw) {
        if (*battleTime >= 2){
            // This prompt shenanegan is only for debug purpose
            system("cls");
            printf("---------------------------------------------------------\n");
            printf("It's a draw\n");
            printf("Your first card has been automatically played and hidden.\n");
            printf("This turn will give the winner a total of 6 cards.\n");
            printf("---------------------------------------------------------\n");

            /*
            // Quick explaination if you don't know how this works.
            // When a draw occurs, basically that both player have the same value for their current card,
            // then the way it is played is that both player put their current card to the middle,
            // take the first card in their hand and put it on their previous current card, but
            // they put this new card as hidden, so they dont show what it is.
            // and then they finally draw the third card which is decisif of who's the winner of that draw.
            */


            battleDeck->addCard(winner.playerDeck.takeCard(winner.currentCard));
            battleDeck->addCard(winner.playerDeck.takeCard()); // So that's the second card that is hidden

            battleDeck->addCard(loser.playerDeck.takeCard(loser.currentCard));
            battleDeck->addCard(loser.playerDeck.takeCard()); // So that's the second card that is hidden

            winner.currentCard = loser.currentCard = DEFAULT_CARD;
            winner.cardID = loser.cardID = DEFAULT_CARD_ID;
            battleTime--;
        }
    }
}
