// Copyright Hubber Studios 2023. All Rights Reserved.

#include "SaveFileEditorStyle.h"
#include "Fonts/SlateFontInfo.h"
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IPluginManager.h"
#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FSaveFileEditorStyle::StyleInstance = nullptr;

void FSaveFileEditorStyle::Initialize() {
    if (!StyleInstance.IsValid()) {
        StyleInstance = Create();
        FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
    }
}

void FSaveFileEditorStyle::Shutdown() {
    FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
    ensure(StyleInstance.IsUnique());
    StyleInstance.Reset();
}

FName FSaveFileEditorStyle::GetStyleSetName() {
    static FName StyleSetName(TEXT("SaveFileEditorStyle"));
    return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef<FSlateStyleSet> FSaveFileEditorStyle::Create() {
    TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("SaveFileEditorStyle"));
    Style->SetContentRoot(IPluginManager::Get().FindPlugin("SaveFileEditor")->GetBaseDir() / TEXT("Resources"));

    FSlateFontInfo NormalBigFont = FAppStyle::Get().GetFontStyle("NormalFont");
    NormalBigFont.Size = NormalBigFont.Size * 1.3;

    Style->Set("SaveFileEditor.NormalFont", FAppStyle::Get().GetFontStyle("NormalFont"));
    Style->Set("SaveFileEditor.NormalBigFont", NormalBigFont);
    Style->Set("SaveFileEditor.BoldFont", FAppStyle::Get().GetFontStyle("BoldFont"));
    Style->Set("SaveFileEditor.NormalFontItalic", FAppStyle::Get().GetFontStyle("NormalFontItalic"));

    return Style;
}

void FSaveFileEditorStyle::ReloadTextures() {
    if (FSlateApplication::IsInitialized()) {
        FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
    }
}

const ISlateStyle& FSaveFileEditorStyle::Get() {
    return *StyleInstance;
}
