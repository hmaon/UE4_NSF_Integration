// Fill out your copyright notice in the Description page of Project Settings.


#include "NSFSynthComponent.h"

bool UNSFSynthComponent::Init(int32& SampleRate)
{
	NumChannels = 1;

	//// Initialize the DSP objects
	//Osc.Init(SampleRate);
	//Osc.SetFrequency(440.0f);
	//Osc.Start();

	ensure(NSF.LoadFile("i:\\wtf\\emu\\nsf\\tetris.nsf"));
	NSFPlayer.SetConfig(&NSFPlayerConfig);
	NSFPlayer.Load(&NSF);
	NSFPlayer.SetPlayFreq(48000.0);
	NSFPlayer.SetChannels(1);
	NSFPlayer.SetSong(1);

	return true;
}

int32 UNSFSynthComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
{
	//// Perform DSP operations here
	//for (int32 Sample = 0; Sample < NumSamples; ++Sample)
	//{
	//	OutAudio[Sample] = Osc.Generate();
	//}

	tempBuffer.resize(NumSamples);

	NSFPlayer.Render((INT16*)tempBuffer.data(), NumSamples);

	for (int i = 0; i < NumSamples; i++)
	{
		OutAudio[i] = ((float)tempBuffer[i]) / 0x7FFF;
	}

	return NumSamples;
}

void UNSFSynthComponent::SetFrequency(const float InFrequencyHz)
{
	// Use this protected base class method to push a lambda function which will safely execute in the audio render thread.
	//SynthCommand([this, InFrequencyHz]()
	//{
	//	Osc.SetFrequency(InFrequencyHz);
	//	Osc.Update();
	//});
}