// Copyright Audiokinetic 2015

#include "Sandbox.h"
#include "CallbackListener.h"

#include "../../Engine/Source/Runtime/AkAudio/Classes/AkComponent.h"
#include "../../Engine/Source/Runtime/AkAudio/Public/AkAudioDevice.h"

bool ACallbackListener::IsDestroyed;

static void AkComponentCallback( AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo )
{
    if ( ACallbackListener::IsDestroyed )
        return;

	UAkComponent * pObj = ( (UAkComponent *) in_pCallbackInfo->pCookie );
    ACallbackListener* pOwner = (ACallbackListener* )pObj->GetOwner();

    //GEngine->AddOnScreenDebugMessage( -1, 15.0f, FColor::Red, "Callback reached" );

    switch ( in_eType )
    {
        case AkCallbackType::AK_Marker:
        {
            AkMarkerCallbackInfo* marker = (AkMarkerCallbackInfo*)in_pCallbackInfo;
            //pOwner->OnMarker.ExecuteIfBound( marker->strLabel );

            pOwner->OnMarker( marker->strLabel );

            //GEngine->AddOnScreenDebugMessage( -1, 15.0f, FColor::Red, "Callback reached" );

            break;
        }

        case AkCallbackType::AK_MidiEvent:
        {
            AkMidiEventCallbackInfo* midiCallback = (AkMidiEventCallbackInfo*)in_pCallbackInfo;
            AkMidiEvent* midi = &midiCallback->midiEvent;
            
            // Fill out automation data
            if (midi->IsCcEvent())
            {
                unsigned char index = midi->Gen.byParam1;
                unsigned char map = 0;

                switch ( index )
                {
                    case  1: map = 0; break;
                    case  2: map = 1; break;
                    case  4: map = 2; break;
                    case  5: map = 3; break;
                    case 91: map = 4; break;
                    case 93: map = 5; break;

                    default: map = 0; break;
                }

                //pOwner->OnMidiAutomation.ExecuteIfBound( midi->byChan, map, (float)midi->Gen.byParam2 / 127.0f );
                //pOwner->OnMidiAutomation( midi->byChan, map, (float)midi->Gen.byParam2 / 127.0f );
                pOwner->SetMidiAutomation( midi->byChan, map, (float)midi->Gen.byParam2 / 127.0f );
            }
            // Fill out note data
            else if (midi->IsNoteEvent())
            {
                //pOwner->OnMidiNote.ExecuteIfBound( midi->byChan, midi->NoteOnOff.byNote, (float)midi->NoteOnOff.byVelocity / 127.0f, midi->IsNoteOn() );
                pOwner->OnMidiNote( midi->byChan, midi->NoteOnOff.byNote, (float)midi->NoteOnOff.byVelocity / 127.0f, midi->IsNoteOn() );
            }
            break;
        }
    }
}

// Sets default values
ACallbackListener::ACallbackListener()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACallbackListener::BeginPlay()
{
	Super::BeginPlay();
	
    ACallbackListener::IsDestroyed = false;

    ((UAkComponent*)RootComponent)->Callback = AkComponentCallback;
}

void ACallbackListener::EndPlay( EEndPlayReason::Type const EndPlayReason )
{
    ACallbackListener::IsDestroyed = true;
}

// Called every frame
void ACallbackListener::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ACallbackListener::SetMidiAutomation( int32 id, int32 index, float value )
{
    MidiAutomations[id][index] = value;
}

float ACallbackListener::GetMidiAutomation( int32 id, int32 index )
{
    return MidiAutomations[id][index];
}

unsigned char ACallbackListener::MapAutomationIndex( unsigned char index ) const
{
    unsigned char map = 0;

    switch ( index )
    {
        case  1: map = 0; break;
        case  2: map = 1; break;
        case  4: map = 2; break;
        case  5: map = 3; break;
        case 91: map = 4; break;
        case 93: map = 5; break;

        default: map = 0; break;
    }

    return map;
}
