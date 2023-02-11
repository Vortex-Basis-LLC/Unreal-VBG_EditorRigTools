#include "VBG_IKRigBPLibrary.h"

#include "IKRigDefinition.h"
#include "Framework/Notifications/NotificationManager.h"
#include "RigEditor/IKRigController.h"
#include "Solvers/IKRig_PBIKSolver.h"

#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"



void AddRetargetChainIfNotPresent(UIKRigController* IKRigController, FName ChainName, FName StartBone, FName EndBone, FName IKGoalName = NAME_None)
{
	const FBoneChain* ExistingChain = IKRigController->GetRetargetChainByName(ChainName);
	if (!ExistingChain)
	{
		UE_LOG(LogTemp, Display, TEXT("Adding chain: %s"), *ChainName.ToString());

		FBoneChain NewChain = FBoneChain(ChainName, StartBone, EndBone);
		NewChain.IKGoalName = IKGoalName;
		IKRigController->AddRetargetChain(NewChain);
	}
}

void AddOrConnectGoalToSolver(UIKRigController* IKRigController, int SolverIndex, FName GoalName, FName BoneName)
{
	UIKRigEffectorGoal* Goal = IKRigController->GetGoal(GoalName);
	if (!Goal)
	{
		UE_LOG(LogTemp, Display, TEXT("Adding goal: %s"), *GoalName.ToString());
		Goal = IKRigController->AddNewGoal(GoalName, BoneName);
	}
	
	IKRigController->ConnectGoalToSolver(*Goal, SolverIndex);
}


void UVBG_IKRigBPLibrary::SetupUE4StyleIKRig(UIKRigDefinition* IKRigDef)
{
	if (!IKRigDef)
	{
		UE_LOG(LogTemp, Error, TEXT("IKRigDef is not defined."));
		return;
	}

	// Confirm that user actually wants to update the rig.
	FText ConfirmTitle = FText::FromString(TEXT("IK Rig Setup"));
	EAppReturnType::Type ConfirmResponse = FMessageDialog::Open(EAppMsgType::YesNo, FText::FromString(TEXT("Attempt to auto-setup UE4-Style IK Rig? This will overwrite many settings of the rig. If IK Rig editor is open, you must close and re-open it after performing this operation.")), &ConfirmTitle);
	if (ConfirmResponse != EAppReturnType::Type::Yes)
	{
		return;
	}
	
	UIKRigController* IKRigController = UIKRigController::GetIKRigController(IKRigDef);
	if (IKRigController)
	{
		// Setup the IK solver if there are no solvers currently on the rig.
		if (IKRigController->GetNumSolvers() == 0)
		{
			UE_LOG(LogTemp, Display, TEXT("Setting up Full Body IK solver for IK Rig."));
			
			// We'll add a default Full Body IK solver.
			int SolverIndex = IKRigController->AddSolver(UIKRigPBIKSolver::StaticClass());
			UIKRigPBIKSolver* Solver = Cast<UIKRigPBIKSolver>(IKRigController->GetSolver(0));
			Solver->SetRootBone(TEXT("pelvis"));
			Solver->RootBehavior = EPBIKRootBehavior::PinToInput;

			IKRigController->SetRetargetRoot(TEXT("pelvis"));

			// Setup goals for the solver.
			AddOrConnectGoalToSolver(IKRigController, SolverIndex, TEXT("hand_l_Goal"), TEXT("hand_l"));
			AddOrConnectGoalToSolver(IKRigController, SolverIndex, TEXT("hand_r_Goal"), TEXT("hand_r"));
			AddOrConnectGoalToSolver(IKRigController, SolverIndex, TEXT("foot_l_Goal"), TEXT("foot_l"));
			AddOrConnectGoalToSolver(IKRigController, SolverIndex, TEXT("foot_r_Goal"), TEXT("foot_r"));

			// Setup the default Full Body IK bone settings.
			IKRigController->AddBoneSetting("pelvis", SolverIndex);
			IKRigController->AddBoneSetting("spine_01", SolverIndex);
			IKRigController->AddBoneSetting("spine_02", SolverIndex);
			IKRigController->AddBoneSetting("spine_03", SolverIndex);
			
			IKRigController->AddBoneSetting("clavicle_l", SolverIndex);
			IKRigController->AddBoneSetting("lowerarm_l", SolverIndex);
			
			IKRigController->AddBoneSetting("clavicle_r", SolverIndex);
			IKRigController->AddBoneSetting("lowerarm_r", SolverIndex);

			IKRigController->AddBoneSetting("thigh_r", SolverIndex);
			IKRigController->AddBoneSetting("calf_r", SolverIndex);
			IKRigController->AddBoneSetting("thigh_l", SolverIndex);
			IKRigController->AddBoneSetting("calf_l", SolverIndex);
			
			UIKRig_PBIKBoneSettings* PelvisBoneSetting = Cast<UIKRig_PBIKBoneSettings>(Solver->GetBoneSetting("pelvis"));
			if (PelvisBoneSetting)
			{
				PelvisBoneSetting->RotationStiffness = 1.0;
			}
			UIKRig_PBIKBoneSettings* Spine01BoneSetting = Cast<UIKRig_PBIKBoneSettings>(Solver->GetBoneSetting("spine_01"));
			if (Spine01BoneSetting)
			{
				Spine01BoneSetting->RotationStiffness = 0.784;
			}
			UIKRig_PBIKBoneSettings* Spine02BoneSetting = Cast<UIKRig_PBIKBoneSettings>(Solver->GetBoneSetting("spine_02"));
			if (Spine02BoneSetting)
			{
				Spine02BoneSetting->RotationStiffness = 0.928;
			}
			UIKRig_PBIKBoneSettings* Spine03BoneSetting = Cast<UIKRig_PBIKBoneSettings>(Solver->GetBoneSetting("spine_03"));
			if (Spine03BoneSetting)
			{
				Spine03BoneSetting->RotationStiffness = 0.936;
			}

			UIKRig_PBIKBoneSettings* ClavicleLBoneSetting = Cast<UIKRig_PBIKBoneSettings>(Solver->GetBoneSetting("clavicle_l"));
			if (ClavicleLBoneSetting)
			{
				ClavicleLBoneSetting->RotationStiffness = 1;
			}
			UIKRig_PBIKBoneSettings* ClavicleRBoneSetting = Cast<UIKRig_PBIKBoneSettings>(Solver->GetBoneSetting("clavicle_r"));
			if (ClavicleRBoneSetting)
			{
				ClavicleRBoneSetting->RotationStiffness = 1;
			}

			UIKRig_PBIKBoneSettings* LowerArmLBoneSetting = Cast<UIKRig_PBIKBoneSettings>(Solver->GetBoneSetting("lowerarm_l"));
			if (LowerArmLBoneSetting)
			{
				LowerArmLBoneSetting->bUsePreferredAngles = true;
				LowerArmLBoneSetting->PreferredAngles = FVector(0,0,90);
			}
			UIKRig_PBIKBoneSettings* LowerArmRBoneSetting = Cast<UIKRig_PBIKBoneSettings>(Solver->GetBoneSetting("lowerarm_r"));
			if (LowerArmRBoneSetting)
			{
				LowerArmRBoneSetting->bUsePreferredAngles = true;
				LowerArmRBoneSetting->PreferredAngles = FVector(0,0,90);
			}

			UIKRig_PBIKBoneSettings* ThighLBoneSetting = Cast<UIKRig_PBIKBoneSettings>(Solver->GetBoneSetting("thigh_l"));
			if (ThighLBoneSetting)
			{
				ThighLBoneSetting->bUsePreferredAngles = true;
				ThighLBoneSetting->PreferredAngles = FVector(0,0,-90);
			}
			UIKRig_PBIKBoneSettings* ThighRBoneSetting = Cast<UIKRig_PBIKBoneSettings>(Solver->GetBoneSetting("thigh_r"));
			if (ThighRBoneSetting)
			{
				ThighRBoneSetting->bUsePreferredAngles = true;
				ThighRBoneSetting->PreferredAngles = FVector(0,0,-90);
			}

			UIKRig_PBIKBoneSettings* CalfLBoneSetting = Cast<UIKRig_PBIKBoneSettings>(Solver->GetBoneSetting("calf_l"));
			if (CalfLBoneSetting)
			{
				CalfLBoneSetting->bUsePreferredAngles = true;
				CalfLBoneSetting->PreferredAngles = FVector(0,0,90);
			}
			UIKRig_PBIKBoneSettings* CalfRBoneSetting = Cast<UIKRig_PBIKBoneSettings>(Solver->GetBoneSetting("calf_r"));
			if (CalfRBoneSetting)
			{
				CalfRBoneSetting->bUsePreferredAngles = true;
				CalfRBoneSetting->PreferredAngles = FVector(0,0,90);
			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("A solver already exists on this model. Delete all solvers if you want this plugin to auto-create one."));
		}

		// Add all default chains.
		// TODO: Check for bone existence first.
		// TODO: Consider allowing for alternate bone names and variable length of chains for spine, etc.
		AddRetargetChainIfNotPresent(IKRigController, TEXT("Spine"), TEXT("spine_01"), TEXT("spine_03"));
		
		AddRetargetChainIfNotPresent(IKRigController, TEXT("LeftArm"), TEXT("upperarm_l"), TEXT("hand_l"), TEXT("hand_l_Goal"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("LeftPinky"), TEXT("pinky_01_l"), TEXT("pinky_03_l"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("LeftRing"), TEXT("ring_01_l"), TEXT("ring_03_l"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("LeftThumb"), TEXT("thumb_01_l"), TEXT("thumb_03_l"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("LeftMiddle"), TEXT("middle_01_l"), TEXT("middle_03_l"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("LeftIndex"), TEXT("index_01_l"), TEXT("index_03_l"));

		AddRetargetChainIfNotPresent(IKRigController, TEXT("RightArm"), TEXT("upperarm_r"), TEXT("hand_r"), TEXT("hand_r_Goal"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("RightPinky"), TEXT("pinky_01_r"), TEXT("pinky_03_r"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("RightRing"), TEXT("ring_01_r"), TEXT("ring_03_r"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("RightThumb"), TEXT("thumb_01_r"), TEXT("thumb_03_r"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("RightMiddle"), TEXT("middle_01_r"), TEXT("middle_03_r"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("RightIndex"), TEXT("index_01_r"), TEXT("index_03_r"));

		AddRetargetChainIfNotPresent(IKRigController, TEXT("RightLeg"), TEXT("thigh_r"), TEXT("ball_r"), TEXT("foot_r_Goal"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("LeftLeg"), TEXT("thigh_l"), TEXT("ball_l"), TEXT("foot_l_Goal"));
		
		AddRetargetChainIfNotPresent(IKRigController, TEXT("Head"), TEXT("neck_01"), TEXT("head"));

		AddRetargetChainIfNotPresent(IKRigController, TEXT("Root"), TEXT("root"), TEXT("root"));

		AddRetargetChainIfNotPresent(IKRigController, TEXT("LeftClavicle"), TEXT("clavicle_l"), TEXT("clavicle_l"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("RightClavicle"), TEXT("clavicle_r"), TEXT("clavicle_r"));

		AddRetargetChainIfNotPresent(IKRigController, TEXT("LeftFootIK"), TEXT("ik_foot_l"), TEXT("ik_foot_l"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("RightFootIK"), TEXT("ik_foot_r"), TEXT("ik_foot_r"));

		AddRetargetChainIfNotPresent(IKRigController, TEXT("LeftHandIK"), TEXT("ik_hand_l"), TEXT("ik_hand_l"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("RightHandIK"), TEXT("ik_hand_r"), TEXT("ik_hand_r"));

		AddRetargetChainIfNotPresent(IKRigController, TEXT("HandGunIK"), TEXT("ik_hand_gun"), TEXT("ik_hand_gun"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("FootRootIK"), TEXT("ik_foot_root"), TEXT("ik_foot_root"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("HandRootIK"), TEXT("ik_hand_root"), TEXT("ik_hand_root"));

		AddRetargetChainIfNotPresent(IKRigController, TEXT("LeftLowerArmTwist01"), TEXT("lowerarm_twist_01_l"), TEXT("lowerarm_twist_01_l"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("LeftUpperArmTwist01"), TEXT("upperarm_twist_01_l"), TEXT("upperarm_twist_01_l"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("RightLowerArmTwist01"), TEXT("lowerarm_twist_01_r"), TEXT("lowerarm_twist_01_r"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("RightUpperArmTwist01"), TEXT("upperarm_twist_01_r"), TEXT("upperarm_twist_01_r"));

		AddRetargetChainIfNotPresent(IKRigController, TEXT("RightThighTwist01"), TEXT("thigh_twist_01_r"), TEXT("thigh_twist_01_r"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("RightCalfTwist01"), TEXT("calf_twist_01_r"), TEXT("calf_twist_01_r"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("LeftThighTwist01"), TEXT("thigh_twist_01_l"), TEXT("thigh_twist_01_l"));
		AddRetargetChainIfNotPresent(IKRigController, TEXT("LeftCalfTwist01"), TEXT("calf_twist_01_l"), TEXT("calf_twist_01_l"));
	}
	
	FNotificationInfo NotifyInfo(FText::FromString(TEXT("IK Rig updated. Please close and re-open if already open.")));
	NotifyInfo.bUseLargeFont = true;
	NotifyInfo.FadeOutDuration = 6.f;
	FSlateNotificationManager::Get().AddNotification(NotifyInfo);

	FText CompleteTitle = FText::FromString(TEXT("IK Rig Setup Complete"));
	FMessageDialog::Open(EAppMsgType::YesNo, FText::FromString(TEXT("IK Rig setup complete. See Output Log for more details.")), &CompleteTitle);
}
