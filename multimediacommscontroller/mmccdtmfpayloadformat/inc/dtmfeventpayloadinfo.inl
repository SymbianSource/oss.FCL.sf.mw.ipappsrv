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
// TDTMFEventPayloadInfo::TDTMFEventPayloadInfo
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
inline TDTMFEventPayloadInfo::TDTMFEventPayloadInfo()
    :
    iEvent(0),
    iEndBit(EFalse),
    iVolume(0),
    iDuration(0),
    iTimeStamp(0)
    {
    }

// ---------------------------------------------------------------------------
// TDTMFEventPayloadInfo::Event
// Return event.
// ---------------------------------------------------------------------------
//
inline TChar TDTMFEventPayloadInfo::Event() const
    {
    return iEvent;
    }

// ---------------------------------------------------------------------------
// TDTMFEventPayloadInfo::SetEvent
// Set event.
// ---------------------------------------------------------------------------
//
inline void TDTMFEventPayloadInfo::SetEvent( const TChar& aEvent )
    {
    iEvent = aEvent;
    }

// ---------------------------------------------------------------------------
// TDTMFEventPayloadInfo::EndBit
// Return end bit.
// ---------------------------------------------------------------------------
//
inline TBool TDTMFEventPayloadInfo::EndBit() const
    {
    return iEndBit;
    }

// ---------------------------------------------------------------------------
// TDTMFEventPayloadInfo::SetEndBit
// Set end bit.
// ---------------------------------------------------------------------------
//
inline void TDTMFEventPayloadInfo::SetEndBit( TBool aEndBit )
    {
    iEndBit = aEndBit;
    }

// ---------------------------------------------------------------------------
// TDTMFEventPayloadInfo::Volume
// Return volume.
// ---------------------------------------------------------------------------
//
inline TUint TDTMFEventPayloadInfo::Volume() const
    {
    return iVolume;
    }

// ---------------------------------------------------------------------------
// TDTMFEventPayloadInfo::SetVolume
// Set volume.
// ---------------------------------------------------------------------------
//
inline void TDTMFEventPayloadInfo::SetVolume( TUint aVolume )
    {
    iVolume = aVolume;
    }

// ---------------------------------------------------------------------------
// TDTMFEventPayloadInfo::Duration
// Return duration.
// ---------------------------------------------------------------------------
//
inline TUint TDTMFEventPayloadInfo::Duration() const
    {
    return iDuration;
    }

// ---------------------------------------------------------------------------
// TDTMFEventPayloadInfo::SetDuration
// Set duration.
// ---------------------------------------------------------------------------
//
inline void TDTMFEventPayloadInfo::SetDuration( TUint aDuration )
    {
    iDuration = aDuration;
    }

// ---------------------------------------------------------------------------
// TDTMFEventPayloadInfo::TimeStamp
// Return time stamp.
// ---------------------------------------------------------------------------
//  
inline TTimeIntervalMicroSeconds32 TDTMFEventPayloadInfo::TimeStamp() const
    {
    return iTimeStamp;
    }

// ---------------------------------------------------------------------------
// TDTMFEventPayloadInfo::SetTimeStamp
// Set time stamp.
// ---------------------------------------------------------------------------
//
inline void TDTMFEventPayloadInfo::SetTimeStamp( 
    const TTimeIntervalMicroSeconds32& aTimeStamp )
    {
    iTimeStamp = aTimeStamp;
    }
       
// End of File
