// Copyright Hubber Studios 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Framework/Application/SlateApplication.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SBoxPanel.h"

#include "HubberUtils/Slate/SlateUtils.h"
#include "HubberUtils/Tab/TabToolBase.h"

struct FHeaderColumnSettings {
public:
    FName Column;
    FText DefaultLabel;
    float FillWidth = 0.2f;
    EHorizontalAlignment HAlignHeader = EHorizontalAlignment::HAlign_Center;

    FHeaderColumnSettings(FName InColumn, FText InDefaultLabel, float InFillWidth = 0.2f, EHorizontalAlignment InHAlignHeader = EHorizontalAlignment::HAlign_Center)
        : Column(InColumn), DefaultLabel(InDefaultLabel), FillWidth(InFillWidth), HAlignHeader(InHAlignHeader) { }
};

struct FSimpleListItemBase {
    FSimpleListItemBase() {};
};

class SSimpleListItemBase : public SMultiColumnTableRow<TSharedPtr<FSimpleListItemBase>> {
public:
    void Construct(const FArguments& InArgs, TSharedPtr<FSimpleListItemBase> InItem, const TSharedRef<STableViewBase>& InOwnerTableView) {
        Item = InItem;
        SMultiColumnTableRow<TSharedPtr<FSimpleListItemBase>>::Construct(SMultiColumnTableRow<TSharedPtr<FSimpleListItemBase>>::FArguments().Padding(0), InOwnerTableView);
    }

    virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override {
        return SNullWidget::NullWidget;
    }

protected:
    TSharedPtr<FSimpleListItemBase> Item;

private:
    bool bInGame = false;
};

class SSimpleListBase : public SCompoundWidget {
    typedef SListView<TSharedPtr<FSimpleListItemBase>> SItemBaseListView;

    SLATE_BEGIN_ARGS(SSimpleListBase) { }
    SLATE_ARGUMENT(TWeakPtr<FTabToolBase>, Tool)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs) {
        TabTool = InArgs._Tool;

        Initialize();
        GenerateHeader();
        Update();
    }

    void Refresh() {
        GenerateItemsSource();
        ItemsListView->RequestListRefresh();
    }

protected:
    /* Parent tab for this list */
    TWeakPtr<FTabToolBase> TabTool;

    /* List of attributes used by the list */
    TArray<TSharedPtr<FSimpleListItemBase>> ItemsSource;

    /* List of row entry generated based on ItemsSource */
    TSharedPtr<SItemBaseListView> ItemsListView;

    virtual void Initialize() = 0;
    virtual void GenerateHeader() = 0;
    virtual void GenerateItemsSource() = 0;

    virtual TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<FSimpleListItemBase> InItem, const TSharedRef<STableViewBase>& OwnerTable) {
        return SNew(SSimpleListItemBase, InItem, OwnerTable);
    }

    virtual void Update() {
        ItemsSource.Empty();
        GenerateItemsSource();

        TSharedRef<SHeaderRow> MyHeader = SNew(SHeaderRow).CanSelectGeneratedColumn(true);

        for (auto& Colum : HeaderColums) {
            MyHeader->AddColumn(SHeaderRow::Column(Colum.Column)
                                    .DefaultLabel(Colum.DefaultLabel)
                                    .HAlignHeader(Colum.HAlignHeader)
                                    .FillWidth(Colum.FillWidth));
        }

        ChildSlot
            [SNew(SScrollBox)
                + SScrollBox::Slot()
                      .VAlign(VAlign_Top)
                      .Padding(5)
                          [SNew(SBorder)
                                  .BorderBackgroundColor(FColor(192, 192, 192, 255))
                                  .Padding(0.0f)
                                      [SAssignNew(ItemsListView, SItemBaseListView)
                                              .ItemHeight(24.f)
                                              .ListItemsSource(&ItemsSource)
                                              .OnGenerateRow(this, &SSimpleListBase::OnGenerateRow)
                                              .HeaderRow(MyHeader)]]];
    }

    TArray<FHeaderColumnSettings> HeaderColums;
};
