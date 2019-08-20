// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetris.h"
#include "Modules/ModuleManager.h"

#include "Internationalization/StringTableRegistry.h"
#include "Paths.h"


void FTetris::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();
	FStringTableRegistry::Get().UnregisterStringTable("StringTableCSV");
	LOCTABLE_FROMFILE_GAME("StringTableCSV", "Namespace", "Code/StringTable.csv");
}


void FTetris::ShutdownModule()
{
	FDefaultGameModuleImpl::ShutdownModule();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FTetris, Tetris, "Tetris");
