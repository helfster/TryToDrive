// Copyright Hubber Studios 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/* Simple text box */
#define SLATE_TEXTBOX_ASSIGN(InText, InPtr) \
    SNew(SBox)                              \
        .HAlign(HAlign_Center)              \
        .VAlign(VAlign_Center)              \
        .Padding(FMargin(10.0f, 5.0f))      \
            [SAssignNew(InPtr, STextBlock)  \
                    .Text(InText)           \
                    .Justification(ETextJustify::Center)]

/* Simple text box */
#define SLATE_TEXTBOX(InText)          \
    SNew(SBox)                         \
        .HAlign(HAlign_Center)         \
        .VAlign(VAlign_Center)         \
        .Padding(FMargin(10.0f, 5.0f)) \
            [SNew(STextBlock)          \
                    .Text(InText)      \
                    .Justification(ETextJustify::Center)]

/* Simple checkbox disabled */
#define SLATE_CHECKBOX(InCheck)         \
    SNew(SBox)                          \
        .HAlign(HAlign_Center)          \
        .VAlign(VAlign_Center)          \
        .Padding(FMargin(10.0f, 5.0f))  \
            [SNew(SCheckBox)            \
                    .IsChecked(InCheck) \
                    .Visibility(EVisibility::HitTestInvisible)]

/* Simple checkbox disabled */
#define SLATE_CHECKBOX_ASSIGN(InCheck, InPtr) \
    SNew(SBox)                                \
        .HAlign(HAlign_Center)                \
        .VAlign(VAlign_Center)                \
        .Padding(FMargin(10.0f, 5.0f))        \
            [SAssignNew(InPtr, SCheckBox)     \
                    .IsChecked(InCheck)       \
                    .Visibility(EVisibility::HitTestInvisible)]

/* Checkbox with state change function as 2 param (func(ECheckBoxState State))*/
#define SLATE_CHECKBOX_BIND(InCheck, InStateChangedFunction) \
    SNew(SBox)                                               \
        .HAlign(HAlign_Center)                               \
        .VAlign(VAlign_Center)                               \
        .Padding(FMargin(10.0f, 5.0f))                       \
            [SNew(SCheckBox)                                 \
                    .IsChecked(InCheck)                      \
                    .OnCheckStateChanged(this, InStateChangedFunction)]