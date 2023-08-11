#include "PuzzleGameEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

#include "MoveComponentVisualizer.h"
#include "PuzzleGame/MoveComponent.h"

IMPLEMENT_GAME_MODULE(FPuzzleGameEditorModule, PuzzleGameEditor);

 void FPuzzleGameEditorModule::StartupModule()
 {
     // Check if editor is valid
     if (GUnrealEd)
     {
         // Registerin the move visualizer
         TSharedPtr<FMoveComponentVisualizer> MoveVisualizer = MakeShareable(new FMoveComponentVisualizer);
         if (MoveVisualizer.IsValid())
         {
             GUnrealEd->RegisterComponentVisualizer(UMoveComponent::StaticClass()->GetFName(), MoveVisualizer);
             MoveVisualizer->OnRegister();
         }
     }
 }

 void FPuzzleGameEditorModule::ShutdownModule()
 {
     // Check if editor is valid
     if (GUnrealEd)
     {
         GUnrealEd->UnregisterComponentVisualizer(UMoveComponent::StaticClass()->GetFName());
     }
 }