#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include <stdio.h>

#include <sstream>
#include <iomanip>
#include <string>

using namespace std;


struct EditableSliderState {
    bool editMode = false;
};

static int EditableSliderInt(const char* label, int* value, int min, int max, EditableSliderState& state);
static int EditableSliderFloat(const char* label, float* value, float min, float max, EditableSliderState& state);
static string zeroPaddingInt(int num);


static int EditableSliderInt(const char* label, int* value, int min, int max, EditableSliderState& state) {

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

static int EditableSliderFloat(const char* label, float* value, float min, float max, EditableSliderState& state) {

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
}

static string zeroPaddingInt(int num) {
    stringstream ss;
    ss << setfill('0') << setw(2) << num;
    return ss.str();
}