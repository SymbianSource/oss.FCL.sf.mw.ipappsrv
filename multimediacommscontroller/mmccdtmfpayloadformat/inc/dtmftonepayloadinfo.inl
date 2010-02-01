/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Inline methods implementations
*
*/




// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::TDTMFTonePayloadInfo
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
inline TDTMFTonePayloadInfo::TDTMFTonePayloadInfo()
    :
    iTone(0),
    iModulation(0),
    iTBit(EFalse),
    iVolume(0),
    iDuration(0),
    iLowFrequency(0),
    iHighFrequency(0),
    iToneOnPeriod( TTimeIntervalMicroSeconds( TInt64( 0 ) ) ),
    iToneOffPeriod( TTimeIntervalMicroSeconds( TInt64( 0 ) ) ),
    iPauseLength( TTimeIntervalMicroSeconds( TInt64( 0 ) ) )
    {
    }
    
// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::Tone
// ---------------------------------------------------------------------------
//
inline TChar TDTMFTonePayloadInfo::Tone() const
    {
    return iTone;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::SetTone
// ---------------------------------------------------------------------------
//        
inline void TDTMFTonePayloadInfo::SetTone( const TChar& aTone )
    {
    iTone = aTone;
    }
            
// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::Modulation
// ---------------------------------------------------------------------------
//
inline TUint TDTMFTonePayloadInfo::Modulation() const
    {
    return iModulation;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::SetModulation
// ---------------------------------------------------------------------------
//
inline void TDTMFTonePayloadInfo::SetModulation( TUint aModulation )
    {
    iModulation = aModulation;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::TBit
// ---------------------------------------------------------------------------
//
inline TBool TDTMFTonePayloadInfo::TBit() const
    {
    return iTBit;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::SetTBit
// ---------------------------------------------------------------------------
//
inline void TDTMFTonePayloadInfo::SetTBit( TBool aTBit )
    {
    iTBit = aTBit;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::Volume
// ---------------------------------------------------------------------------
//
inline TUint TDTMFTonePayloadInfo::Volume() const
    {
    return iVolume;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::SetVolume
// ---------------------------------------------------------------------------
//
inline void TDTMFTonePayloadInfo::SetVolume( TUint aVolume )
    {
    iVolume = aVolume;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::Duration
// ---------------------------------------------------------------------------
//
inline TUint TDTMFTonePayloadInfo::Duration() const
    {
    return iDuration;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::SetDuration
// ---------------------------------------------------------------------------
//
inline void TDTMFTonePayloadInfo::SetDuration( TUint aDuration )
    {
    iDuration = aDuration;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::LowFrequncy
// ---------------------------------------------------------------------------
//
inline TUint TDTMFTonePayloadInfo::LowFrequency() const
    {
    return iLowFrequency;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::SetDuration
// ---------------------------------------------------------------------------
//
inline void TDTMFTonePayloadInfo::SetLowFrequency( TUint aLowFrequency )
    {
    iLowFrequency = aLowFrequency;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::HighFrequncy
// ---------------------------------------------------------------------------
//
inline TUint TDTMFTonePayloadInfo::HighFrequency() const
    {
    return iHighFrequency;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::SetDuration
// ---------------------------------------------------------------------------
//
inline void TDTMFTonePayloadInfo::SetHighFrequency( TUint aHighFrequency )
    {
    iHighFrequency = aHighFrequency;
    }
    
// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::ToneOnPeriod
// ---------------------------------------------------------------------------
//
inline TTimeIntervalMicroSeconds& TDTMFTonePayloadInfo::ToneOnPeriod()
    {
    // PC_LINT #1536, reference needed for easier usage of DevSound interface
    return iToneOnPeriod;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::SetToneOnPeriod
// ---------------------------------------------------------------------------
//
inline void TDTMFTonePayloadInfo::SetToneOnPeriod(
    const TTimeIntervalMicroSeconds& aToneOnPeriod )
    {
    iToneOnPeriod = aToneOnPeriod;
    }
    
// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::ToneOffPeriod
// ---------------------------------------------------------------------------
//
inline TTimeIntervalMicroSeconds& TDTMFTonePayloadInfo::ToneOffPeriod()
    {
    // PC_LINT #1536, reference needed for easier usage of DevSound interface
    return iToneOffPeriod;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::SetToneOffPeriod
// ---------------------------------------------------------------------------
//
inline void TDTMFTonePayloadInfo::SetToneOffPeriod(
    const TTimeIntervalMicroSeconds& aToneOffPeriod )
    {
    iToneOffPeriod = aToneOffPeriod;
    } 

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::PauseLength
// ---------------------------------------------------------------------------
//
inline TTimeIntervalMicroSeconds& TDTMFTonePayloadInfo::PauseLength( )
    {
    // PC_LINT #1536, reference needed for easier usage of DevSound interface
    return iPauseLength;
    }

// ---------------------------------------------------------------------------
// TDTMFTonePayloadInfo::SetPauseLength
// ---------------------------------------------------------------------------
//
inline void TDTMFTonePayloadInfo::SetPauseLength(
    const TTimeIntervalMicroSeconds& aPauseLength )
    {
    iPauseLength = aPauseLength;
    }

// End of File
