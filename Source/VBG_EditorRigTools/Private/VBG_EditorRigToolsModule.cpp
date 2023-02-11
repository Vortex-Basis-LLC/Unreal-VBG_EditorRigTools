#include "VBG_EditorRigToolsModule.h"

#define LOCTEXT_NAMESPACE "VBG_EditorRigToolsModule"

void FVBG_EditorRigToolsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FVBG_EditorRigToolsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVBG_EditorRigToolsModule, VBG_EditorRigToolsEditorMode)