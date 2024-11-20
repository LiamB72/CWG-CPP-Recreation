#include "../data/headers/cwg-module.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;


CardStruct DEFAULT_CARD(-1, -1);
map<CardStruct, string> IMAGES = { { {0, 0}, "01" }, { {1, 0}, "02" }, { {2, 0}, "03" }, { {3,  0}, "04" }, { {4,  0}, "05" }, { {5,  0}, "06" }, { {6, 0}, "07" }, { {7, 0}, "08" }, { {8, 0}, "09" }, { {9, 0}, "10" }, { {10, 0}, "11" }, { {11, 0}, "12" }, { {12, 0}, "00" }, { {0, 1}, "14" }, { {1, 1}, "15" }, { {2, 1}, "16" }, { {3,  1}, "17" }, { {4,  1}, "18" }, { {5,  1}, "19" }, { {6, 1}, "20" }, { {7, 1}, "21" }, { {8, 1}, "22" }, { {9, 1}, "23" }, { {10, 1}, "24" }, { {11, 1}, "25" }, { {12, 1}, "13" }, { {0, 2}, "27" }, { {1, 2}, "28" }, { {2, 2}, "29" }, { {3,  2}, "30" }, { {4,  2}, "31" }, { {5,  2}, "32" }, { {6, 2}, "33" }, { {7, 2}, "34" }, { {8, 2}, "35" }, { {9, 2}, "36" }, { {10, 2}, "37" }, { {11, 2}, "38" }, { {12, 2}, "26" }, { {0, 3}, "40" }, { {1, 3}, "41" }, { {2, 3}, "42" }, { {3,  3}, "43" }, { {4,  3}, "44" }, { {5,  3}, "45" }, { {6, 3}, "46" }, { {7, 3}, "47" }, { {8, 3}, "48" }, { {9, 3}, "49" }, { {10, 3}, "50" }, { {11, 3}, "51" }, { {12, 3}, "39" } };

void playerTab(PlayerStruct& player, const CardGraphicsStruct& cardGraphics, int& newCardID, bool& showPlayerActionMenu) {
    ImGui::SeparatorText("Card");
    ImGui::DragInt(": New Card ID", &newCardID, 1, 0, DEFAULT_CARD_ID, "%02d");
    ImGui::SameLine();

    if (ImGui::Button("Apply New Card")) {
        if (applyNewCard(player, newCardID)) cout << "The transfer was unsuccessful.\n";
    }
    const CardStruct temp = GameDeckStruct::getCardByID(newCardID);
    // New Card Text
    const string tempStrNCard = "New Card Vector\t: {" + to_string(temp.value) + ", " + to_string(temp.suit) + "}";
    // Current Card Text
    const string tempStrCCard = "Current Card Vector: {" + to_string(player.currentCard.value) + ", " + to_string(player.currentCard.suit) + "}";
    ImGui::Text(tempStrNCard.c_str());
    ImGui::Text(tempStrCCard.c_str());

    ImGui::SeparatorText("Decks");
    ImGui::Checkbox("Show Player's Action Menu", &showPlayerActionMenu);
    if (ImGui::TreeNode("Deck")) {
        displayPlayerDeck(player, cardGraphics, player.playerDeck, true);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Waiting Deck")) {
        displayPlayerDeck(player, cardGraphics, player.waitingDeck, false);
        ImGui::TreePop();
    }

    ImGui::SeparatorText("Values");
    if (player.cardID != DEFAULT_CARD_ID) {
        const string currentCardName = "Current Card Name\t\t\t\t  : " + player.currentCard.getName();
        ImGui::Text(currentCardName.c_str());
    }
    ImGui::Text("Amount of cards in deck\t\t\t: %d cards", player.playerDeck.getHandSize());
    ImGui::Text("Amount of cards in waiting deck\t: %d cards", player.waitingDeck.getHandSize());
    ImGui::Text("Amount of victories\t\t\t\t: %d", player.victoryCount);
}


void displayPlayerDeck(PlayerStruct& player, const CardGraphicsStruct& cardGraphics, const DeckStruct& deck, const bool changeCard) {
    for (int i = 0; i < deck.getHandSize(); i++) {
        if (i % 3 != 0) ImGui::SameLine();

        const int cardID = GameDeckStruct::getCardID(deck.hand[i]);
        string cardName = deck.hand[i].getName();
        const string cardIDStr = to_string(cardID);

        if (ImGui::ImageButton(cardGraphics.getCardSpriteAt(cardID))) {
            if (changeCard) {
                applyNewCard(player, cardID);
            }
        }
        /*if (!changeCard) {
            string tooltipStr = "Card: " + player.playerDeck.hand[i].getName() + "\nID: " + zeroPaddingInt(i) + "\nVector: {" + to_string(player.playerDeck.hand[i].value) + ", " + to_string(player.playerDeck.hand[i].suit) + "}";
            const char* toolTip = tooltipStr.c_str();

            ImGui::SetItemTooltip(toolTip);
        }*/
    }
}

void awardRound(PlayerStruct& winner, PlayerStruct& loser, const CardGraphicsStruct& cardGraphics, const bool& draw, int& battleTime, DeckStruct* battleDeck) {
    if (!draw && !battleTime) {

        // 3 stones in 2 lines:
        // adding to the waiting deck of the winner, the card of both winner and loser.
        // Moreover, the cards from both parties are also removed from their deck.
        winner.waitingDeck.addCard(winner.playerDeck.takeCard(winner.currentCard));
        winner.waitingDeck.addCard(loser.playerDeck.takeCard(loser.currentCard));

        winner.cardSprite.setTexture(cardGraphics.cardTextures[winner.cardID]);
        loser.cardSprite.setTexture(cardGraphics.cardTextures[loser.cardID]);

        winner.currentCard = loser.currentCard = DEFAULT_CARD;
        winner.cardID = loser.cardID = DEFAULT_CARD_ID;

        winner.victoryCount++;
    }
    else if (draw && battleTime == 1) {

        winner.waitingDeck.addCard(winner.playerDeck.takeCard(winner.currentCard));
        winner.waitingDeck.addCard(loser.playerDeck.takeCard(loser.currentCard));
        int tempSize = battleDeck->getHandSize();
        for (int i = 0; i < tempSize; i++) {
            winner.waitingDeck.addCard(battleDeck->takeCard());
        }

        winner.cardSprite.setTexture(cardGraphics.cardTextures[winner.cardID]);
        loser.cardSprite.setTexture(cardGraphics.cardTextures[loser.cardID]);

        winner.currentCard = loser.currentCard = DEFAULT_CARD;
        winner.cardID = loser.cardID = DEFAULT_CARD_ID;

        winner.victoryCount++;
        battleTime--;
    }
    else if (draw) {
        if (battleTime >= 2) {
            // This prompt shenanigan is only for debug purpose
            // system("cls");
            // printf("---------------------------------------------------------\n");
            // printf("It's a draw\n");
            // printf("Your first card has been automatically played and hidden.\n");
            // printf("This turn will give the winner a total of 6 cards.\n");
            // printf("---------------------------------------------------------\n");

            /*
            // Quick explanation if you don't know how this works.
            // When a draw occurs, basically that both players have the same value for their current card,
            // then the way it is played is that both players put their current card to the middle,
            // take the first card in their hand and put it on their previous current card, but
            // they put this new card as hidden, so they don't show what it is.
            // and then they finally draw the third card which is decisive of who's the winner of that draw.
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

int applyNewCard(PlayerStruct& dest, const int& cardID) {
    if (cardID == DEFAULT_CARD_ID) return 1;
    const CardStruct newCard = GameDeckStruct::getCardByID(cardID);

    dest.currentCard = newCard;
    dest.cardID = cardID;
    return 0;
}

int EditableSliderInt(const char* label, int* value, const int min, const int max, EditableSliderState& state) {

    if (!state.editMode) {
        ImGui::SliderInt(label, value, min, max);
        if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0)) {
            state.editMode = true;
            ImGui::SetKeyboardFocusHere(-1); // Focus on input
        }
    }

    if (state.editMode) {
        ImGui::InputInt(label, value, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);
        if (ImGui::IsItemDeactivated()) {
            state.editMode = false; // Stop editing when losing focus
        }
    }
    return 1;
}

int EditableSliderFloat(const char* label, float* value, const float min, const float max, EditableSliderState& state) {

    if (!state.editMode) {
        ImGui::SliderFloat(label, value, min, max);
        if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0)) {
            state.editMode = true;
            ImGui::SetKeyboardFocusHere(-1); // Focus on input
        }
    }

    if (state.editMode) {
        ImGui::InputFloat(label, value, 0.0f, 0.0f, ".2f", ImGuiInputTextFlags_EnterReturnsTrue);
        if (ImGui::IsItemDeactivated()) {
            state.editMode = false; // Stop editing when losing focus
        }
    }
    return 1;
}

string zeroPaddingInt(const int num){
    stringstream ss;
    ss << setfill('0') << setw(2) << num;
    return ss.str();
}

void showTooltip(const int cardIndex) {
    CardStruct cardTemp = GameDeckStruct::getCardByID(cardIndex);
    string tooltipStr = "Card: " + cardTemp.getName() + "\nID: " + zeroPaddingInt(cardIndex) + "\nVector: {" + to_string(cardTemp.value) + ", " + to_string(cardTemp.suit) + "}";
    const char* toolTip = tooltipStr.c_str();

    ImGui::SetItemTooltip(toolTip);
}

void setVictoryScreen(PlayerStruct& winner, PlayerStruct& notWinner, sf::Text& previousWinner, bool actionMenu0, bool actionMenu1) {
    winner.victoryCount++;
    previousWinner.setString("Player 1 won the game!");
    actionMenu0 = actionMenu1 = false;
}