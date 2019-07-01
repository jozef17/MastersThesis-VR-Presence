// Fill out your copyright notice in the Description page of Project Settings.

#include "DPGameModeBase.h"
#include "DPCharacter.h"

ADPGameModeBase::ADPGameModeBase()
{
	DefaultPawnClass = ADPCharacter::StaticClass();

}
