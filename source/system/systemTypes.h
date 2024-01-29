#pragma once

//https://www.psdevwiki.com/ps4/SELF_File_Format
struct SHeaderSelf
{
	char m_magic[4];		// Magic 4F 15 3D 1D
	char m_unknown[4];		// Always 00 01 01 12
	char m_category;		// Category 1 on SELF, 4 on PUP Entry (probably SPP). See PS3/PS Vita Category
	char m_programType;		// Program Type First Half denotes version (anything between 0, oldest and F, newest), second Half denotes true type, 4 for Games, 5 for sce_module modules, 6 for Video Apps like Netflix, 8 for System/EX Apps/Executables, 9 for System/EX modules/dlls

};