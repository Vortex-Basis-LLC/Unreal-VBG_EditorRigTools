# NOTE: Currently incompatible with Unreal 5.2+
This plugin was originally written for UE 5.1.1. The class names in use changed in 5.2, so this plugin will not work as-is on newer versions, but
the general principles likely remain the same.

# VBG Editor Rig Tools

This plugin tries to configure an IK Rig based on a standard UE4 mannequin skeleton. The main target use case is speeding up 
the setup of IK Rigs for characters imported from Reallusion's Character Creator.

Original Blog Post: https://blog.vortexbasis.com/auto-setup-ik-rig-in-ue5-for-reallusion-character-creator-models

## General Usage

- Create "IK Rig" for your character's skeletal mesh.
- Right-click on the "IK Rig". Click "Scripted Asset Actions" / "IK Rig" / "Setup UE4-Style IK Rig".
  - The plugin will then attempt to setup all required bone chains, the FBIK solver, and associated goals.

## Supported Unreal Versions
- This has only been tested on UE 5.1.1.

## Notes on Retargeting

- To retarget to your character, you must create an "IK Rig Retargeter" using the source skeleton for your existing animations and then set your new IK Rig as the target.
- If you open the "IK Rig Retargeter", it seems that there might be a bug where all the available animations won't show up in the Asset Browser.
  - If this happens to you, you can still right-click on a set of selected animations in Content Browser and select "Duplicate and Retarget Animation Assets" from there.
- The default CC4 A-Pose seems to result in finger tips pointing back oddly and arms too far out when retargeting UE5-mannequin animations.
  - You can edit the default pose to turn down all finger tips by about 20 degrees and tilt the shoulders down about 10 degrees.
  - TODO: Create some sort of helper for auto-correcting the default pose.

## Installation from Source

- Under the "Plugins" folder in your project, run the following command in your OS command prompt:
```
git clone https://github.com/Vortex-Basis-LLC/Unreal-VBG_EditorRigTools.git VBG_EditorRigTools
```
- Build solution prior to opening your Unreal project or use CTRL+ALT+F11 to trigger Live Coding compile with UE open.
