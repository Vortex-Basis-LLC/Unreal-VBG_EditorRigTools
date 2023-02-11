#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "VBG_IKRigBPLibrary.generated.h"



class UIKRigDefinition;

/**
 * 
 */
UCLASS()
class VBG_EDITORRIGTOOLS_API UVBG_IKRigBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Setup UE4-Style IK Rig", Keywords = "rig"), Category = "VBG|IK Rig")
	static void SetupUE4StyleIKRig(UIKRigDefinition* IKRigDef);	
};
