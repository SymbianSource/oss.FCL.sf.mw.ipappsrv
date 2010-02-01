/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/




#include "mdaaudiotoneplayer.h"

enum TMmfMdaAudioToneUtility
	{
	EBadArgument,
	EPostConditionViolation
	};

CMdaAudioToneUtility* CMdaAudioToneUtility::NewL(MMdaAudioToneObserver& aObserver, CMdaServer* aServer /*= NULL*/)
	{
	return CMdaAudioToneUtility::NewL(aObserver, aServer, EMdaPriorityNormal, EMdaPriorityPreferenceTimeAndQuality);
	}

CMdaAudioToneUtility* CMdaAudioToneUtility::NewL(MMdaAudioToneObserver& /*aObserver*/, CMdaServer* /*aServer = NULL*/,
														  TInt /*aPriority*/ /*= EMdaPriorityNormal*/,
														  TInt /*aPref*/ /*= EMdaPriorityPreferenceTimeAndQuality*/)
	{
	CMdaAudioToneUtility* self = new(ELeave) CMdaAudioToneUtility();
	return self;
	}
	
CMdaAudioToneUtility::~CMdaAudioToneUtility()
	{
	}
	
TMdaAudioToneUtilityState CMdaAudioToneUtility::State()
	{
	}	
	
TInt CMdaAudioToneUtility::MaxVolume()
	{
	return 0;
	}
	
TInt CMdaAudioToneUtility::Volume()
	{
	return 0;
	}	
	
void CMdaAudioToneUtility::SetVolume(TInt /*aVolume*/)
	{
	}	

void CMdaAudioToneUtility::SetPriority(TInt /*aPriority*/, TInt /*aPref*/)
	{
	}	

void CMdaAudioToneUtility::SetDTMFLengths(TTimeIntervalMicroSeconds32 /*aToneLength*/,
								TTimeIntervalMicroSeconds32 /*aToneOffLength*/,
								TTimeIntervalMicroSeconds32 /*aPauseLength*/)
	{
	}
	
void CMdaAudioToneUtility::SetRepeats(TInt /*aRepeatNumberOfTimes*/, const TTimeIntervalMicroSeconds& /*aTrailingSilence*/)
	{
	}
	
void CMdaAudioToneUtility::SetVolumeRamp(const TTimeIntervalMicroSeconds& /*aRampDuration*/)
	{
	}
	
TInt CMdaAudioToneUtility::FixedSequenceCount()
	{
	return 0;
	}	
	
const TDesC& CMdaAudioToneUtility::FixedSequenceName(TInt /*aSequenceNumber*/)
	{
	return KNullDesC;
	}	
	
void CMdaAudioToneUtility::PrepareToPlayTone(TInt /*aFrequency*/, const TTimeIntervalMicroSeconds& /*aDuration*/)
	{
	}	
	
void CMdaAudioToneUtility::PrepareToPlayDTMFString(const TDesC& /*aDTMF*/)
	{
	}
	
void CMdaAudioToneUtility::PrepareToPlayDesSequence(const TDesC8& /*aSequence*/)
	{
	}

void CMdaAudioToneUtility::PrepareToPlayFileSequence(const TDesC& /*aFileName*/)
	{
	}

void CMdaAudioToneUtility::PrepareToPlayFixedSequence(TInt /*aSequenceNumber*/)
	{
	}	
	
void CMdaAudioToneUtility::CancelPrepare()
	{
	}	
	
void CMdaAudioToneUtility::Play()
	{
	}	
	
void CMdaAudioToneUtility::CancelPlay()
	{
	}
		
void CMdaAudioToneUtility::SetBalanceL(TInt /*aBalance=KMMFBalanceCenter*/)
	{
	}
	
TInt CMdaAudioToneUtility::GetBalanceL()
	{
	return 0;
	}			
		
void CMdaAudioToneUtility::PrepareToPlayDualTone(TInt /*aFrequencyOne*/, 
												TInt /*aFrequencyTwo*/, 
												const TTimeIntervalMicroSeconds& /*aDuration*/)
	{
	}		
		
void CMdaAudioToneUtility::PrepareToPlayFileSequence(RFile& /*aFile*/)
	{
	}
	
TAny* CMdaAudioToneUtility::CustomInterface(TUid /*aInterfaceId*/)
	{
	return NULL;
	}

void MMMFClientUtility::ReservedVirtual1()
	{
	}
	
void MMMFClientUtility::ReservedVirtual2()
	{
	}

void MMMFClientUtility::ReservedVirtual3()
	{
	}

void MMMFClientUtility::ReservedVirtual4()
	{
	}

void MMMFClientUtility::ReservedVirtual5()
	{
	}
	
void MMMFClientUtility::ReservedVirtual6()
	{
	}

	