/*

/////   TO DO LIST:     /////

// Make an AI that can play against the player.
// Make a TO-DO list to know how to do it.

///// END OF TO DO LIST /////

Changelog:

07-11-2024:
- Added post-turn card exchanging function named awardTurn with winner and loser parameters.
- Added a fancy card struct:
        + easy of use with 2 variables: value and suit.
        + easy of call: call the struct and just say its value and suit with having to do the { ... }.
        + fancy getName function that lets other classes and structs doesn't have to repeat said function.
        + custom behavior for the == operator of comparison, as much as for the < operator for the map ordering.

- Changed the cardValue to global variable CARD_VALUES.
- Changed the cardColours to global variable CARD_SUITS.
- Added tons of documentation to help for future me or other readers.
- Changed the vector<int> to the card structure.
- Removed two occurrences of 'printDeck' function in player and gameDeck classes.
- Added printDeck to the deck class.
- Learn about the const keyword for function, let us and the computer know it only reads and does not change any value.
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
    - Implement clicking on a card (when in player's tab) applying said card to the player's card displayed in the centre of the window. ✅
    - Implement a ready button for both tabs -> collapses the player's deck and displays a text saying which they chose. ✅
    - Based on the upper to do, implement the game logic. ✅
    - Implement that once both players pressed ready, display text between the two displayed cards telling who won. ✅
    - Learn about putting things in the heap to save such a place. ✅

- Additional changes:
    - Correctness in putting keywords such as 'const' in the right place.
    - Changing a for loop to one with an auto iterator.
    - Learn about static functions in classes. (very useful -> makes so that you don't really have to initialize the class but still can use the function itself.)
    - Used the proper way to cast types in C++. (a.k.a. using static_cast<type> thus an 'explicit cast', rather than (type) which is an 'implicit cast' from C.)
    - Removed useless import (time.h which is apparently deprecated???).
    - Learned about explicit constructors.
    - Probably will learn about the malloc function to use rather than the new keyword in the future. (But rn I'm too lazy :p)
    - Now using the constexpr keyword to define constants. Even though I still don't clearly understand what it does.

13-11-2024 to 19-11-2024 +:
- Big overhaul of the files structure.
- Finally using a header file with a module file to contain most of the structures and functions.
- Change every class into structs.
- Changed the way to hard limit the fps to avoid unlimited resource consumption
- Removed the 'hardcoded' editableslider. Replaced with a dragInt one, and works about the same.
- New system, in the debug menu, to change cards.
- Talking about debug menus. Overhaul them too.
    + Changed the ui to a drop-down menu to access the three main debug menus.
    + Can see the average fps in the dropdown menu
    _ Removed the previous ui
    + Made a simplified version and separated the different utils into different menus.
    + Made a player menu, being able to manipulate almost every value each player has.
    + Made a sprite menu, being able to see every sprite in the game, and clickable to get their info.
    + Made a Decks menu to visualize every deck in one place.

- Finally added the text to tell who won.
- Made a safeguard to tell you whenever you didn't select a card or if you did.
- Stuck the two player menus to each of the sides with the menu's name being the player's ID.
- Implemented the game logic to be more prone to work.
- Added two helper functions for debugging purposes. -> i.e: playerTab (debug menu), applyCard (debug menu)
- Changed the global variables to the header file and used macros to define them.
- Changed to a newer version of Dear ImGui.
- Included the demo file of Dear ImGui (why didn't I do that before??)

20-11-2024:
- Added an end screen for when one of the two players have an empty deck
- Added a button to automatically play a round in the player's debug menu.
- Added multiple safeguards to avoid playing with empty decks (looking at you debug menu >:\ ).
- So finally, the game has an end.
- Tried to give a custom font to the Dear ImGui menu, without success.
- Changed the architecture of the data file (will probably rename said file to "cwg" in the future).

///// END OF CHANGELOG /////

*/

#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include <windows.h>
#include "data/headers/cwg-module.h"

using namespace std;


int main()
{
    float windowSize[2] = {1336.0, 780.0};
    sf::RenderWindow window(sf::VideoMode(
        static_cast<int> (windowSize[0]),
        static_cast<int> (windowSize[1])
        ),
        "Card War",
        sf::Style::Titlebar | sf::Style::Close);

    ImGui::SFML::Init(window);
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    window.setFramerateLimit(144);

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


    // Initialization of basic values;
    auto* cardGraphics = new CardGraphicsStruct(); // Very big class that requires it to be in the heap
    /*auto* cardGraphics = static_cast<CardClass *>(malloc(sizeof(CardClass)));*/

    GameDeckStruct gameDeck;
    DeckStruct battleDeck;
    int battleTime = 0;

    // Player initialization
    PlayerStruct P1(1, *cardGraphics);
    sf::Sprite* p1Card = &P1.cardSprite;
    int p1NewCardID = DEFAULT_CARD_ID;

    PlayerStruct P2(2, *cardGraphics);
    sf::Sprite* p2Card = &P2.cardSprite;
    int p2NewCardID = DEFAULT_CARD_ID;

    bool debugWindow = false;
    bool dropDownMenu = false;
    bool playerDebugMenu = false;
    bool spriteDebugMenu = false;
    bool decksDebugMenu = false;

    // Deck Manipulation
    gameDeck.shuffleDeck();
    const int gameDeckSize = gameDeck.deck.getHandSize();

    for (int i = 0; i < gameDeckSize/2; i++) {
        P1.playerDeck.addCard(gameDeck.deck.takeCard());
    }

    for (int i = 0; i < gameDeckSize/2; i++) {
        P2.playerDeck.addCard(gameDeck.deck.takeCard());
    }

    // Graphics management
    p1Card->setPosition(
        static_cast<float>(windowSize[0] / 2 - cardGraphics->cardTextures[0].getSize().x / 2.0), // X position
        static_cast<float>(windowSize[1] / 3 - cardGraphics->cardTextures[0].getSize().y / 2.0) // Y position
    );
    p2Card->setPosition(
        static_cast<float>(windowSize[0] / 2 - cardGraphics->cardTextures[0].getSize().x / 2.0), // X position
        static_cast<float>(windowSize[1] * 2.075 / 3 - cardGraphics->cardTextures[0].getSize().y / 2.0) // Y position
    );

    sf::Text previousRoundWinner;

    sf::Font sfFont;
    sfFont.loadFromFile("data/font/MapleMono.ttf");
    /*ImFont* font = io.Fonts->AddFontFromFileTTF("data\\font\\MapleMono.ttf", 12);
    ImGui::PushFont(font);*/

    previousRoundWinner.setFont(sfFont);                    // Set the loaded font
    previousRoundWinner.setString("No Winner Yet.");        // Set the text content
    previousRoundWinner.setCharacterSize(30);               // Set the character size
    previousRoundWinner.setFillColor(sf::Color::White);     // Set the colour

    sf::FloatRect textBounds = previousRoundWinner.getLocalBounds();
    previousRoundWinner.setOrigin(textBounds.width / 2, textBounds.height / 2);
    previousRoundWinner.setPosition(windowSize[0] / 2, windowSize[1] / 2);


    // Other values
    sf::Clock deltaClock;
    bool wasF6Pressed = false;
    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F6)) {
            if (!wasF6Pressed) { // Only toggle when F6 transitions from not pressed to press
                debugWindow = !debugWindow;
                wasF6Pressed = true; // Mark that F6 is currently pressed
            }
        }
        else {
            wasF6Pressed = false; // Reset when F6 is released
        }
        
        // ImGui Logic Here.
        ImGuiWindowFlags windowFlagsPlayers = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
        constexpr float pWinSizeX = 535;
        constexpr float pWinSizeY = 500;

        if (P1.showActionMenu) {
            ImGui::SetNextWindowSize(ImVec2(pWinSizeX, pWinSizeY));
            ImGui::SetNextWindowPos(ImVec2(0,
                static_cast<float>(windowSize[1] / 3 - cardGraphics->cardTextures[0].getSize().y / 2.0)));

            ImGui::Begin("Player 1", nullptr, windowFlagsPlayers);
            if (!P1.confirmedCard) {
                if (ImGui::TreeNode("Deck:")) {
                    displayPlayerDeck(P1, *cardGraphics, P1.playerDeck);
                    ImGui::TreePop();
                }
                if (ImGui::Button("Confirm Chosen Card?")) {
                    if (P1.cardID != DEFAULT_CARD_ID) {
                        P1.confirmedCard = true;
                    }
                }
            }
            if (P1.showReadyText)
                ImGui::Text("You have not selected a card yet!");
            ImGui::End();
        }

        if (P2.showActionMenu) {
            ImGui::SetNextWindowSize(ImVec2(pWinSizeX, pWinSizeY));
            ImGui::SetNextWindowPos(ImVec2(windowSize[0] - pWinSizeX,
                static_cast<float>(windowSize[1] / 3.0 - cardGraphics->cardTextures[0].getSize().y / 2.0)));

            ImGui::Begin("Player 2", nullptr, windowFlagsPlayers);
            if (!P2.confirmedCard) {
                if (ImGui::TreeNode("Deck:")) {
                    displayPlayerDeck(P2, *cardGraphics, P2.playerDeck);
                    ImGui::TreePop();
                }

                if (ImGui::Button("Confirm Chosen Card?")) {
                    if (P2.cardID != 52) {
                        P2.confirmedCard = true;
                    }
                }
                if (P2.showReadyText)
                    ImGui::Text("You have not selected a card yet!");
            }
            ImGui::End();
        }
        
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
        
        if (debugWindow) {
            ImGui::SetNextWindowSize(ImVec2(windowSize[0], 35));
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowBgAlpha(100);

            ImGui::Begin("Debug Window", nullptr, windowFlags);

            ImGui::PushID(0);
            if (!dropDownMenu)
                ImGui::PushStyleColor(ImGuiCol_Button, static_cast<ImVec4>(ImColor::HSV(0.0f, 0.0f, 0.06f)));
            else
                ImGui::PushStyleColor(ImGuiCol_Button, static_cast<ImVec4>(ImColor::HSV(0.588f, 0.68f, 0.43f)));

            if (ImGui::Button("Debug")) {
                dropDownMenu = !dropDownMenu;
            }
            ImGui::PopStyleColor(1);
            ImGui::PopID();
            ImGui::End();

            // Dropdown little menu code
            if (dropDownMenu) {
                ImGui::SetNextWindowSize(ImVec2(160, 90));
                ImGui::SetNextWindowPos(ImVec2(15, 34));
                ImGui::SetNextWindowBgAlpha(100);

                ImGui::Begin("DropDown Menu", nullptr, windowFlags);
                if (ImGui::MenuItem("Players' Values")) { playerDebugMenu = !playerDebugMenu; }
                if (ImGui::MenuItem("Sprites Grid")) { spriteDebugMenu = !spriteDebugMenu; }
                if (ImGui::MenuItem("Decks Menu")) { decksDebugMenu = !decksDebugMenu; }
                ImGui::Text("%.1f FPS", io.Framerate);

                ImGui::End();
            }

            // Players' menu to change their values.
            if (playerDebugMenu) {
                ImGui::SetNextWindowBgAlpha(100);


                ImGui::Begin("Player", &playerDebugMenu);
                
                
                // play a round automatically without a player input.
                

                if (ImGui::Button("Play a Round")) {
                    P1.playCard(); 
                    P2.playCard();
                }
                ImGui::SetItemTooltip("Plays a round without the need to select and confirm for both players");

                if (P1.playerDeck.isDeckEmpty() || P2.playerDeck.isDeckEmpty()) {
                    ImGui::SameLine();
                    ImGui::TextDisabled("One of the players have an empty deck.");
                }

                ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
                if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {
                    if (ImGui::BeginTabItem("Player 1")) {
                        playerTab(P1, *cardGraphics, p1NewCardID, P1.showActionMenu);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Player 2")) {
                        playerTab(P2, *cardGraphics, p2NewCardID, P2.showActionMenu);
                        ImGui::EndTabItem();
                    }
                }
                ImGui::EndTabBar();
                ImGui::End();
            }

            // Sprites window
            if (spriteDebugMenu) {
                ImGui::SetNextWindowBgAlpha(100);

                ImGui::Begin("Sprite Debug Menu", &spriteDebugMenu);
                int cardOnLine = 0;
                for (int i = 0; i < 52; i++) {
                    ImGui::ImageButton(cardGraphics->getCardSpriteAt(i));
                    showTooltip(i);
                    cardOnLine++;

                    if (cardOnLine != 8) {
                        ImGui::SameLine();
                    }
                    else cardOnLine = 0;
                }
                ImGui::End();
            }

            if (decksDebugMenu) {
                ImGui::SetNextWindowBgAlpha(100);

                ImGui::Begin("Decks Menu", &decksDebugMenu);

                ImGui::SeparatorText("Battle Deck");
                if (ImGui::TreeNode(" ")) {

                    int battleCardsOnLine = 0;
                    for (auto& i : battleDeck.hand) {

                        // Get the card Index using the static function getCardID 
                        // that take a CardStruct as a parameter.
                        int cardIndex = GameDeckStruct::getCardID(i);
                        
                        // Create buttons with images
                        ImGui::ImageButton(cardGraphics->getCardSpriteAt(cardIndex));
                        showTooltip(cardIndex);
                        battleCardsOnLine++;

                        // Allow 8 cards on the same row.
                        if (battleCardsOnLine != 4) {
                            ImGui::SameLine();
                        }
                        else battleCardsOnLine = 0;
                    }
                    ImGui::TreePop();

                }

                ImGui::SeparatorText("Players Decks");
                if (ImGui::TreeNode("Player 1")) {
                    if (ImGui::TreeNode("Deck")) {
                        displayPlayerDeck(P1, *cardGraphics, P1.playerDeck, false);
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode("Waiting Deck")) {
                        displayPlayerDeck(P1, *cardGraphics, P1.waitingDeck, false);
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Player 2")) {
                    if (ImGui::TreeNode("Deck")) {
                        displayPlayerDeck(P2, *cardGraphics, P2.playerDeck, false);
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode("Waiting Deck")) {
                        displayPlayerDeck(P2, *cardGraphics, P2.waitingDeck, false);
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
                ImGui::End();
            }
        }

        // Looping Logics Here.
        window.clear(sf::Color(18, 33, 43)); // Colour background

        if (P1.cardID != DEFAULT_CARD_ID)
            P1.showReadyText = false;
        else
            P1.showReadyText = true;

        if (P2.cardID != DEFAULT_CARD_ID)
            P2.showReadyText = false;
        else
            P2.showReadyText = true;

        // >> Start game logic HERE.
        // Upon confirmation of both parties:
        if (P1.confirmedCard && P2.confirmedCard) {
            P1.checkDeck(); P2.checkDeck();
            if (P1.playerDeck.getHandSize() - 1 && P2.playerDeck.getHandSize() - 1){
                if (P1.currentCard.value > P2.currentCard.value) {
                    // Player 1 won
                    previousRoundWinner.setString("Player 1 won!");
                    if (!battleTime) awardRound(P1, P2, *cardGraphics, false, battleTime, &battleDeck);
                    else awardRound(P1, P2, *cardGraphics, true, battleTime, &battleDeck);
                }
                else if (P1.currentCard.value < P2.currentCard.value) {
                    // Player 2 won
                    previousRoundWinner.setString("Player 2 won!");
                    if (!battleTime) awardRound(P2, P1, *cardGraphics, false, battleTime, &battleDeck);
                    else awardRound(P2, P1, *cardGraphics, true, battleTime, &battleDeck);
                }
                else {
                    // Draw
                    // The order doesn't matter, it's a draw, nobody has won.
                    previousRoundWinner.setString("IT'S A DRAW!");
                    battleTime += 2;
                    awardRound(P1, P2, *cardGraphics, true, battleTime, &battleDeck);
                }

                // Lasting thing done is the reactivation of the decks and buttons.
                P1.confirmedCard = P2.confirmedCard = false;
            }
            else if (P1.playerDeck.getHandSize() - 1 == 0) {
                P2.victoryCount++;
                previousRoundWinner.setString("Player 2 won the game!");
                P1.showActionMenu = P2.showActionMenu = false;
                P1.confirmedCard = P2.confirmedCard = false;
            }
            else if (P2.playerDeck.getHandSize() - 1 == 0) {
                P1.victoryCount++;
                previousRoundWinner.setString("Player 1 won the game!");
                P1.showActionMenu = P2.showActionMenu = false;
                P1.confirmedCard = P2.confirmedCard = false;
            }
            
        }

        // Draw everything past here.
        window.draw(*p1Card);
        window.draw(*p2Card);
        window.draw(previousRoundWinner);

        ImGui::SFML::Render(window);
        window.display();
    }

    //ImGui::PopFont();
    ImGui::SFML::Shutdown();
    delete cardGraphics;
    return 0;
}
