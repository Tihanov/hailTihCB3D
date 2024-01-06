// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TitleInterface.generated.h"

UINTERFACE(MinimalAPI)
class UTitleInterface : public UInterface
{
	GENERATED_BODY()
};

class ANDROIDTEST_API ITitleInterface
{
	GENERATED_BODY()

public:

	virtual FText GetTitle() const { return FText::FromString(TEXT("IMPLEMENT THIS FUNCTION")); }
};
