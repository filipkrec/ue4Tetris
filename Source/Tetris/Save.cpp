// Fill out your copyright notice in the Description page of Project Settings.


#include "Save.h"

USave::USave()
{
}

FString USave::GetScoresString()
{
	HighScores.ValueSort([](int A, int B) {
		return A > B; // sort keys in reverse
	});
	FString toReturn;

	int i = 1;
	for (auto It = HighScores.CreateConstIterator(); It && i < 11; ++It && ++i)
	{
		toReturn += FString::FromInt(i) + ".   " + It->Key + "   " + FString::FromInt(It->Value) + "\n";
	}

	return toReturn;
}