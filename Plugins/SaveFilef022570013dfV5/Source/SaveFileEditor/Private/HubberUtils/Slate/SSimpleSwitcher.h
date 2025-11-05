// Copyright Hubber Studios 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Framework/Application/SlateApplication.h"
#include "Widgets/Colors/SComplexGradient.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"

#include "SlateUtils.h"

struct FSwitcherElement {

public:
    FSwitcherElement(FText InName, TSharedRef<SWidget> InWidget) : Name(InName), Widget(InWidget) { }

    FText Name;
    TSharedRef<SWidget> Widget;
};

class SSimpleSwitcherSelector : public SCompoundWidget {
public:
    DECLARE_DELEGATE_OneParam(FOnSelected, int32);

    SLATE_BEGIN_ARGS(SSimpleSwitcherSelector) { }
    SLATE_ARGUMENT(int32, Index)
    SLATE_ARGUMENT(FText, Label)
    SLATE_ARGUMENT(bool, Active)
    SLATE_EVENT(FOnSelected, OnSelected)
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs) {
        Index = InArgs._Index;
        Label = InArgs._Label;
        bActive = InArgs._Active;
        OnSelected = InArgs._OnSelected;

        GenericTabStyle = &FAppStyle::Get().GetWidgetStyle<FDockTabStyle>("Docking.Tab");

        CreateTab();
    }

    const FSlateBrush* GetImageBrush() const {
        // Pick the right brush based on whether the tab is active or hovered.
        if (IsHovered()) {
            return &GenericTabStyle->HoveredBrush;
        } else if (bActive) {
            return &GenericTabStyle->ForegroundBrush;
        }
        return &GenericTabStyle->NormalBrush;
    }

    EVisibility GetActiveTabIndicatorVisibility() const {
        // return IsActive() && GetVisualTabRole() != ETabRole::MajorTab ? EVisibility::HitTestInvisible : EVisibility::Collapsed;
        return bActive ? EVisibility::HitTestInvisible : EVisibility::Collapsed;
    }

    FMargin GetTabPadding() const {
        FMargin NewPadding = GenericTabStyle->TabPadding;
        NewPadding.Top *= 1.2f;
        NewPadding.Bottom *= 1.2f;
        return NewPadding;
    }

    void CreateTab() {
        static FLinearColor ActiveBorderColor = FAppStyle::Get().GetSlateColor("Docking.Tab.ActiveTabIndicatorColor").GetSpecifiedColor();
        static FLinearColor ActiveBorderColorTransparent = FLinearColor(ActiveBorderColor.R, ActiveBorderColor.G, ActiveBorderColor.B, 0.0f);
        static TArray<FLinearColor> GradientStops { ActiveBorderColorTransparent, ActiveBorderColor, ActiveBorderColorTransparent };

        ChildSlot[SNew(SBorder)
                      .BorderImage(FStyleDefaults::GetNoBrush())
                      .VAlign(VAlign_Bottom)
                      .Padding(0.0f)
                      .ForegroundColor(FSlateColor::UseStyle())
                      .OnMouseButtonDown(this, &SSimpleSwitcherSelector::OnMouseDown)
                          [SNew(SOverlay)
                              + SOverlay::Slot()
                                  [SNew(SImage)
                                          .Image(this, &SSimpleSwitcherSelector::GetImageBrush)]

                              + SOverlay::Slot()
                                    .VAlign(VAlign_Top)
                                    .HAlign(HAlign_Fill)
                                        [SNew(SComplexGradient)
                                                .Visibility(this, &SSimpleSwitcherSelector::GetActiveTabIndicatorVisibility)
                                                .DesiredSizeOverride(FVector2D(1.0f, 1.0f))
                                                .GradientColors(GradientStops)
                                                .Orientation(EOrientation::Orient_Vertical)]

                              + SOverlay::Slot()
                                    .Padding(TAttribute<FMargin>::Create(TAttribute<FMargin>::FGetter::CreateSP(this, &SSimpleSwitcherSelector::GetTabPadding)))
                                    .VAlign(VAlign_Center)
                                    .HAlign(HAlign_Fill)
                                        [SNew(SHorizontalBox)
                                                .Visibility(EVisibility::Visible)

                                            + SHorizontalBox::Slot()
                                                  .FillWidth(1.0f)
                                                  .Padding(5.0f, 5.0f)
                                                  .HAlign(HAlign_Left)
                                                  .VAlign(VAlign_Center)
                                                      [SNew(SHorizontalBox)

                                                          + SHorizontalBox::Slot()
                                                                .FillWidth(1.0f)
                                                                .Padding(0.0f, 1.0f)
                                                                .VAlign(VAlign_Center)
                                                                    [SNew(STextBlock)
                                                                            .Text(Label)
                                                                            .OverflowPolicy(ETextOverflowPolicy::Ellipsis)]]]]];
    }

    FReply OnMouseDown(const FGeometry& SenderGeometry, const FPointerEvent& MouseEvent) {
        OnSelected.ExecuteIfBound(Index);
        return FReply::Handled();
    }

    void SetActive(bool InbActive) {
        bActive = InbActive;
    }

    int32 GetIndex() { return Index; }

protected:
    const FDockTabStyle* GenericTabStyle;

private:
    int32 Index;
    FText Label;
    FOnSelected OnSelected;

    bool bActive = false;
};

class SSimpleSwitcher : public SCompoundWidget {
    SLATE_BEGIN_ARGS(SSimpleSwitcher) { }
    SLATE_ARGUMENT(TArray<FSwitcherElement>, Tabs)
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs) {
        Tabs = InArgs._Tabs;
        ;
        BackgroundHeaderBrush = FAppStyle::Get().GetWidgetStyle<FDockTabStyle>("Docking.Tab").NormalBrush;
        BackgroundHeaderBrush.TintColor = FAppStyle::Get().GetSlateColor("Colors.Foldout");
        BackgroundHeaderBrush.DrawAs = ESlateBrushDrawType::Type::Image;

        ChildSlot
            [SNew(SConstraintCanvas)
                + SConstraintCanvas::Slot()
                      .Anchors(FAnchors(0, 0, 1, 1))
                      .Offset(FMargin(0))
                          [SNew(SVerticalBox)
                              + SVerticalBox::Slot()
                                    .AutoHeight()
                                        [SNew(SOverlay)
                                            + SOverlay::Slot()
                                                [SNew(SBorder)
                                                        .BorderImage(&BackgroundHeaderBrush)
                                                        .Padding(0)] //.BorderBackgroundColor(FAppStyle::Get().GetSlateColor("Colors.Foldout"))

                                            + SOverlay::Slot()
                                                [SAssignNew(TabHeader, SHorizontalBox)]]
                              + SVerticalBox::Slot()
                                    .FillHeight(1.0f)
                                        [SAssignNew(Switcher, SWidgetSwitcher)]]];

        int32 Idx = 0;
        for (auto& Tab : Tabs) {
            TSharedPtr<SSimpleSwitcherSelector> SwitcherSelectorPtr;
            TabHeader->AddSlot().AutoWidth()
                [SAssignNew(SwitcherSelectorPtr, SSimpleSwitcherSelector)
                        .Index(Idx)
                        .Active(ActiveIdx == Idx)
                        .Label(Tab.Name)
                        .OnSelected(this, &SSimpleSwitcher::TabSelected)];
            Switcher->AddSlot()[Tab.Widget];
            TabButtons.Add(SwitcherSelectorPtr);
            Idx++;
            // TabHeader->AddSlot()[SLATE_TEXTBOX(Tab.Name)];
        }
    }

    void TabSelected(int32 NewIdx) {
        if (NewIdx == ActiveIdx) {
            return;
        }
        for (auto Button : TabButtons) {
            Button->SetActive(NewIdx == Button->GetIndex());
        }
        Switcher->SetActiveWidgetIndex(NewIdx);

        ActiveIdx = NewIdx;
    }

private:
    TSharedPtr<SHorizontalBox> TabHeader;
    TSharedPtr<SWidgetSwitcher> Switcher;

    int32 ActiveIdx = 0;
    TArray<FSwitcherElement> Tabs;
    TArray<TSharedPtr<SSimpleSwitcherSelector>> TabButtons;

    FSlateBrush BackgroundHeaderBrush;
};
