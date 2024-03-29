// Fill out your copyright notice in the Description page of Project Settings.


#include "NSFSynthComponent.h"

bool UNSFSynthComponent::Init(int32& SampleRate)
{
	NumChannels = 1;
	sampleRate = SampleRate;

	//// Initialize the DSP objects
	//Osc.Init(SampleRate);
	//Osc.SetFrequency(440.0f);
	//Osc.Start();

	//NSF.SetDefaults(0, 0, 0);

	return true;
}

int32 UNSFSynthComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
{
	//// Perform DSP operations here
	//for (int32 Sample = 0; Sample < NumSamples; ++Sample)
	//{
	//	OutAudio[Sample] = Osc.Generate();
	//}

	if (!LoadedSuccessfully)
	{
		return 0;
	}

	tempBuffer.resize(NumSamples);

	UINT32 ret = this->NSFPlayer.Render((INT16*)tempBuffer.data(), NumSamples);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 0.3f, FColor::Yellow, *FString::Printf(TEXT("ret == %d"), ret));

	for (int i = 0; i < NumSamples; i++)
	{
		OutAudio[i] = ((float)this->tempBuffer[i]) / 0x7FFF;
	}

	return NumSamples;
}

void UNSFSynthComponent::OnStart()
{
	FString FullPath = FPaths::GameContentDir() + PathToMusic;
	auto path = TCHAR_TO_ANSI(*FullPath);
	LoadedSuccessfully = NSF.LoadFile(path);

	if (!LoadedSuccessfully)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, *FString::Printf(TEXT("Cannot load %s"), *FullPath));
		return;
	}
	

	//NSFPlayerConfig.SetValue
	NSFPlayer.SetConfig(&NSFPlayerConfig);
	NSFPlayer.Load(&NSF);
	NSFPlayer.SetPlayFreq((double)sampleRate);
	NSFPlayer.SetChannels(NumChannels);
	NSFPlayer.SetSong(1);
	NSFPlayer.Reset();
}

//void UNSFSynthComponent::SetFrequency(const float InFrequencyHz)
//{
//	// Use this protected base class method to push a lambda function which will safely execute in the audio render thread.
//	//SynthCommand([this, InFrequencyHz]()
//	//{
//	//	Osc.SetFrequency(InFrequencyHz);
//	//	Osc.Update();
//	//});
//}