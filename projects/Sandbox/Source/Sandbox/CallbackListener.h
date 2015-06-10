// Copyright Audiokinetic 2015

#pragma once

#include "GameFramework/Actor.h"
#include "CallbackListener.generated.h"

//USTRUCT()
//struct FMidiAutomations
//{
//    GENERATED_USTRUCT_BODY()
//
//    UPROPERTY(BlueprintReadOnly)
//    float Automation1;
//    UPROPERTY(BlueprintReadOnly)
//    float Automation2;
//    UPROPERTY(BlueprintReadOnly)
//    float Automation3;
//    UPROPERTY(BlueprintReadOnly)
//    float Automation4;
//    UPROPERTY(BlueprintReadOnly)
//    float Automation5;
//    UPROPERTY(BlueprintReadOnly)
//    float Automation6;
//
//    FMidiAutomations() {}
//};

UCLASS()
class SANDBOX_API ACallbackListener : public AActor
{
	GENERATED_BODY()
	
public:	
    
    static bool IsDestroyed;

	// Sets default values for this actor's properties
	ACallbackListener();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void EndPlay( EEndPlayReason::Type const EndPlayReason ) override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION(BlueprintImplementableEvent, Category = "GlobalEvents")
    virtual void OnMidiNote( int32 Channel, int32 Pitch, float Velocity, bool IsNoteOn );
    UFUNCTION(BlueprintImplementableEvent, Category = "GlobalEvents")
    virtual void OnMidiAutomation( int32 channel, int32 id, float value );
    UFUNCTION(BlueprintImplementableEvent, Category = "GlobalEvents")
    virtual void OnMarker( const FString& label );

    void SetMidiAutomation( int32 id, int32 index, float value );

    UFUNCTION(BlueprintCallable, Category = "MIDI")
    float GetMidiAutomation( int32 id, int32 index );


private:

    float MidiAutomations[16][6];

    unsigned char MapAutomationIndex( unsigned char index ) const;

};
