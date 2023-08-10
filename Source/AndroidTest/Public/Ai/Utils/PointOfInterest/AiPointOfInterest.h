// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AiPointOfInterest.generated.h"

class AVillagerAiController;

UCLASS(BlueprintType, Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class ANDROIDTEST_API UAiPointOfInterestInstance : public UObject
{
	GENERATED_BODY()
	
public:
	UAiPointOfInterestInstance();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnArrived")
		float OnArrivedTimeout = 0.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnArrived")
		bool bOnArrivedChangeTransform = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnArrived",
		meta = (EditCondition = "bOnArrivedChangeTransform"))
		FTransform OnArrivedTransform;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnArrived")
		UAnimMontage* OnArrivedAnimation;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnArrived")
		bool bOnArrivedUseCallback = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "Options|OnArrived",
		meta = (EditCondition = "bOnArrivedUseCallback"))
		class UPoiCallback* OnArrivedCallback;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnComplete")
		bool bOnCompleteUseCallback = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "Options|OnComplete",
		meta = (EditCondition = "bOnCompleteUseCallback"))
		class UPoiCallback* OnCompleteCallback;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options")
		TSubclassOf<class AAiPointOfInterest> POI_ToSpawnClass; 
};

UCLASS(BlueprintType, Abstract)
class AAiPointOfInterest : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Init(UAiPointOfInterestInstance* Instance, AVillagerAiController* AiController);
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		bool IsComplete() const;
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		bool IsArrived() const;

	UFUNCTION(BlueprintPure)
		UAiPointOfInterestInstance* GetInstance() const;
	template<class T>
		T* GetInstance() const;
	
	UFUNCTION(BlueprintPure)
		AVillagerAiController* GetAiController() const;
	template<class T>
		T* GetAiController() const;

protected:
	virtual void OnArrived();
	virtual void OnComplete();
	
private:
	UAiPointOfInterestInstance* InstanceOfSelf = nullptr;
	AVillagerAiController* VillagerAiController = nullptr;
};

template <class T>
T* AAiPointOfInterest::GetInstance() const
{
	if(!IsValid(GetInstance()))
		return nullptr;
	const auto ToRet = Cast<T>(GetInstance());
	check(ToRet);
	return ToRet;
}

template <class T>
T* AAiPointOfInterest::GetAiController() const
{
	if(!IsValid(GetAiController()))
		return nullptr;
	const auto ToRet = Cast<T>(GetAiController());
	check(ToRet);
	return ToRet;
}

UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew, Abstract)
class UPoiCallback : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Execute(AAiPointOfInterest* PointOfInterest);
};
