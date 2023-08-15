// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AiPointOfInterest.generated.h"

class ANpcAiController;

UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EPoiCompleteCauser : uint8
{
	NONE		= 0 UMETA(Hidden),
	Undefined	= 1 << 0,
	Timeout		= 1 << 1,
	Clock		= 1 << 2,
};
ENUM_CLASS_FLAGS(EPoiCompleteCauser);

UCLASS(BlueprintType, Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class ANDROIDTEST_API UAiPointOfInterestInstance : public UObject
{
	GENERATED_BODY()
	
public:
	UAiPointOfInterestInstance();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnStart")
		bool bOnStartUseCallbacks = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "Options|OnStart",
		meta = (EditCondition = "bOnStartUseCallbacks"))
		TArray<class UPoiCallback*> OnStartCallbacks;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnArrived")
		bool bOnArrivedUseTimeout = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnArrived",
		meta = (EditCondition = "bOnArrivedUseTimeout"))
		float OnArrivedTimeout = 0.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnArrived")
		bool bOnArrivedChangeTransform = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnArrived",
		meta = (EditCondition = "bOnArrivedChangeTransform"))
		FTransform OnArrivedTransform;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnArrived")
		UAnimMontage* OnArrivedAnimation;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnArrived")
		bool bOnArrivedUseCallbacks = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "Options|OnArrived",
		meta = (EditCondition = "bOnArrivedUseCallbacks"))
		TArray<class UPoiCallback*> OnArrivedCallbacks;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnComplete")
		bool bOnCompleteChangeTransform = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnComplete",
		meta = (EditCondition = "bOnArrivedChangeTransform"))
		FTransform OnCompleteTransform;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|OnComplete")
		bool bOnCompleteUseCallbacks = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "Options|OnComplete",
		meta = (EditCondition = "bOnCompleteUseCallbacks"))
		TArray<class UPoiCallback*> OnCompleteCallbacks;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options")
		TSubclassOf<class AAiPointOfInterest> POI_ToSpawnClass; 
};

UCLASS(BlueprintType, Abstract)
class AAiPointOfInterest : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		virtual void Init(UAiPointOfInterestInstance* Instance, ANpcAiController* AiController);
	UFUNCTION(BlueprintPure)
		virtual bool IsComplete() const;
	UFUNCTION(BlueprintPure)
		virtual bool IsArrived() const;

	UFUNCTION(BlueprintPure, Category = "POI")
		EPoiCompleteCauser GetCompleteCauser() const;
	UFUNCTION(BlueprintPure, Category = "POI")
		bool IsCompleteByReasons(UPARAM(meta = (Bitmask, BitmaskEnum = EPoiCompleteCauser)) uint8 Reasons) const;
protected:
	virtual void SetCompleteCauser(EPoiCompleteCauser Causer);
public:

	UFUNCTION(BlueprintPure)
		UAiPointOfInterestInstance* GetInstance() const;
	template<class T>
		T* GetInstance() const;
	
	UFUNCTION(BlueprintPure)
		ANpcAiController* GetAiController() const;
	template<class T>
		T* GetAiController() const;

protected:
	virtual void OnArrived();
	virtual void OnComplete();
	
private:
	UAiPointOfInterestInstance* InstanceOfSelf = nullptr;
	ANpcAiController* NpcAiController = nullptr;
	EPoiCompleteCauser CompleteCauser = EPoiCompleteCauser::NONE;
	bool bIsComplete = false;
	bool bIsArrived = false;
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
