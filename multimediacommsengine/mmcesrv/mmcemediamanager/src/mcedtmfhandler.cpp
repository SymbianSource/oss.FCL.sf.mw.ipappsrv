/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#include "mcedtmfhandler.h"
#include "mcemmlogs.h"
#include "mceevents.h"
#include "mcesrvstream.h"
#include "mcesrvsource.h"
#include "mcecomsession.h"
#include "mcecommediasource.h"
#include "mcesrvstreamiterator.h"
#include <mmccinterface.h>

const TInt KMceDtmfToneMaxLen = 2;
MCEIDS_DEFINE_SPARE_INDEX_1( KMceDtmfToneIndex );
MCEIDS_DEFINE_SPARE_INDEX_2( KMceDtmfToneDurationIndex );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceDtmfHandler::NewL
// -----------------------------------------------------------------------------
//
CMceDtmfHandler* CMceDtmfHandler::NewL( 
    CMceSrvStream& aStream, 
    CMccInterface& aMccInterface )
    {
    CMceDtmfHandler* self = 
        new ( ELeave ) CMceDtmfHandler( aStream, aMccInterface );       
    return self;
    }

// -----------------------------------------------------------------------------
// CMceDtmfHandler::DtmfL
// -----------------------------------------------------------------------------
//
void CMceDtmfHandler::DtmfL( 
    CMceComMediaSource& aSource,
    TMceComEvent& aEvent,
    const TDesC8& aSequence )
    {
    MCEMM_DEBUG( "CMceDtmfHandler::DtmfL, Entry" )
     
    // Store source and stream ids, since multiple sources can use the same
    // dtmf stream.Received dtmf events will be routed to last one who has
    // used the stream.

    TMceSrvStreamIterator streams( aSource.MediaStream()->Session()->MccStreams(), 
                                   *aSource.MediaStream() );
    CMceSrvStream* stream = NULL;
    __ASSERT_ALWAYS( streams.Next( stream ), User::Leave( KErrNotSupported ) );
    iCurrentUserStreamId = stream->Id();
    iCurrentUserSourceId = stream->Source().Id();
    
    MCEMM_DEBUG_DVALUE( "CMceDtmfHandler::DtmfL, current streamId", 
                        iCurrentUserStreamId )
    MCEMM_DEBUG_DVALUE( "CMceDtmfHandler::DtmfL, current sourceId", 
                        iCurrentUserSourceId )
    
    TMceIds& ids = aEvent.Id();
    
    switch( aEvent.Action() )
        {
        case EMceItcIsDtmfActive:
            {
            ids.iSpare1 = IsDtmfActive();
            break;
            }
        case EMceItcStartDtmfTone:
            {
            TBuf8<KMceDtmfToneMaxLen> tone;
            tone.Append( static_cast<TChar>( ids.Get( KMceDtmfToneIndex ) ) );
            SendDtmfStringL( KMccDtmfSigStartTone, tone );
            SetDtmfActive( ETrue );
            break;
            }
        case EMceItcStopDtmfTone:
            {
            StopDtmfL();
            break;
            }
        case EMceItcSendDtmfTone:
            {
            TUint32 duration = ids.Get( KMceDtmfToneDurationIndex );
            TMccDtmfEventType eventType = 
                ( duration > 0 ) ? KMccDtmfSigStartTone : KMccDtmfSigSendString;
            TBuf8<KMceDtmfToneMaxLen> tone;
            tone.Append( static_cast<TChar>( ids.Get( KMceDtmfToneIndex ) ) );
            SendDtmfStringL( eventType, 
                             tone, 
                             duration );
            break;
            }
        case EMceItcSendDtmfToneSequence:
            {
            __ASSERT_ALWAYS( aSequence.Length() > 0, User::Leave( KErrArgument ) );
            SendDtmfStringL( KMccDtmfSigSendString, aSequence );
            break;
            }
        case EMceItcCancelSendDtmfToneSequence:
            {
            // Will need checking if some other actions need to be taken here.
            SendDtmfStringL( KMccDtmfSigCancelSending );
            break;
            }
        default:
            {
            break;
            }
        }
    
    MCEMM_DEBUG( "CMceDtmfHandler::DtmfL, Exit" )
    }

// -----------------------------------------------------------------------------
// CMceDtmfHandler::DtmfReceivedL
// -----------------------------------------------------------------------------
//    
void CMceDtmfHandler::DtmfReceivedL( 
    const TMccEvent& aMccEvent, 
    TMceMccComEvent& aEvent )
    {
    const TMccDtmfEventData& dtmfEvent = 
                    (*reinterpret_cast<const TMccDtmfEventDataPackage*>( 
                        &aMccEvent.iEventData ))();
    
    if ( dtmfEvent.iDtmfEventType == KMccDtmfSequenceStop )
        {
        // This event can be ignored, it just means that one tone of the
        // seuence was completed, KMccDtmfSendingComplete will tell that
        // whole sequence was completed (client will receive as many started
        // events as there is tones but only one completed event).
        aEvent.iEvent = KMccEventNone;
        }
    else
        {
        aEvent.iEventSubType = dtmfEvent.iDtmfEventType;
        aEvent.iStreamId = iCurrentUserStreamId;
        aEvent.iEndpointId = iCurrentUserSourceId;
        
        if ( aEvent.iEventSubType == KMccDtmfManualAbort ||
             aEvent.iEventSubType == KMccDtmfSequenceAbort )
            {
            // Error occured, no need to send stop dtmf event
            SetDtmfActive( EFalse );
            Cancel();
            }
               
        MCEMM_DEBUG_DVALUE( "CMceDtmfHandler::DtmfReceivedL, subtype:", aEvent.iEventSubType )
        }
    }

// -----------------------------------------------------------------------------
// CMceDtmfHandler::RunL
// -----------------------------------------------------------------------------
//
void CMceDtmfHandler::RunL()
    {
    MCEMM_DEBUG( "CMceDtmfHandler::RunL, tone expired" )
    StopDtmfL();
    }

// -----------------------------------------------------------------------------
// CMceDtmfHandler::DoCancel
// -----------------------------------------------------------------------------
//    
void CMceDtmfHandler::DoCancel()
    {
    MCEMM_DEBUG( "CMceDtmfHandler::DoCancel, stopping dtmf sending" )
    iTimer.Cancel();
    if ( IsDtmfActive() )
        {
        SetDtmfActive( EFalse );
        TRAP_IGNORE( SendDtmfStringL( KMccDtmfSigStopTone ) )
        }
    }

// -----------------------------------------------------------------------------
// CMceDtmfHandler::RunError
// -----------------------------------------------------------------------------
//     
TInt CMceDtmfHandler::RunError( TInt aError )
    {
    MCEMM_DEBUG_DVALUE( "CMceDtmfHandler::RunError, error:", aError )
    if ( aError != KErrNoMemory )
        {
        aError = KErrNone;
        }
    return aError;
    }

// -----------------------------------------------------------------------------
// CMceDtmfHandler::SendDtmfStringL
// -----------------------------------------------------------------------------
//
void CMceDtmfHandler::SendDtmfStringL( 
    TMccDtmfEventType aDtmfEventType, 
    const TDesC8& aDtmfString,
    TUint32 aDurationInMicroSecs )
    {
    MCEMM_DEBUG_DVALUE( "CMceDtmfHandler::SendDtmfString, type:", aDtmfEventType )
    MCEMM_DEBUG_SVALUE( "CMceDtmfHandler::SendDtmfString, string:", aDtmfString )
    
    __ASSERT_ALWAYS( aDtmfString.Length() <= KMccMaxDtmfStringLength,
                     User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( !IsDtmfActive(), User::Leave( KErrInUse ) );
    
    TMccEvent* event = new ( ELeave ) TMccEvent;
    CleanupStack::PushL( event );
    
    event->iEventCategory = KMccEventCategoryDtmf;
    event->iEventType = KMccDtmfControl;
    event->iSessionId = iStream.SessionId();
    event->iLinkId = iStream.LinkId();
    event->iStreamId = iStream.Id();
    
    {       
    TMccDtmfEventData eventData;
    eventData.iDtmfEventType = aDtmfEventType;
    eventData.iDtmfString.Copy( aDtmfString );
    TMccDtmfEventDataPackage eventDataBuf( eventData );
    event->iEventData.Copy( eventDataBuf );
    }
    
    iMccInterface.SendMediaSignalL( *event );
    
    CleanupStack::PopAndDestroy( event );
    
    if ( aDurationInMicroSecs > 0 )
        {
        MCEMM_DEBUG( "CMceDtmfHandler::SendDtmfString, activate stop timer" )
        iTimer.After( iStatus, TTimeIntervalMicroSeconds32( aDurationInMicroSecs ) );
        SetActive();
        SetDtmfActive( ETrue );  
        }
    
    MCEMM_DEBUG( "CMceDtmfHandler::SendDtmfString, Exit" )
    }

// -----------------------------------------------------------------------------
// CMceDtmfHandler::StopDtmfL
// -----------------------------------------------------------------------------
//
void CMceDtmfHandler::StopDtmfL()
    {
    MCEMM_DEBUG( "CMceDtmfHandler::StopDtmfL" )
    
    Cancel();
    
    if ( IsDtmfActive() )
        {
        MCEMM_DEBUG( "CMceDtmfHandler::StopDtmfL, stopping dtmf sending" )
        
        SetDtmfActive( EFalse );
        SendDtmfStringL( KMccDtmfSigStopTone );
        } 
    }

// -----------------------------------------------------------------------------
// CMceDtmfHandler::SetDtmfActive
// -----------------------------------------------------------------------------
//
void CMceDtmfHandler::SetDtmfActive( TBool aIsActive )
    {
    iSendingDtmf = aIsActive;
    }
    
// -----------------------------------------------------------------------------
// CMceDtmfHandler::IsDtmfActive
// -----------------------------------------------------------------------------
//
TBool CMceDtmfHandler::IsDtmfActive() const
    {
    return iSendingDtmf;
    }
    
// -----------------------------------------------------------------------------
// CMceDtmfHandler::~CMceDtmfHandler
// -----------------------------------------------------------------------------
//
CMceDtmfHandler::~CMceDtmfHandler()
    {
    MCEMM_DEBUG( "CMceDtmfHandler::~CMceDtmfHandler, Entry" )
    TRAP_IGNORE( StopDtmfL() );
    iTimer.Close();
    MCEMM_DEBUG( "CMceDtmfHandler::~CMceDtmfHandler, Exit" )
    }

// -----------------------------------------------------------------------------
// CMceDtmfHandler::CMceDtmfHandler
// -----------------------------------------------------------------------------
//
CMceDtmfHandler::CMceDtmfHandler( 
    CMceSrvStream& aStream, 
    CMccInterface& aMccInterface ) : 
    CActive( EPriorityStandard ),
    iStream( aStream ),
    iMccInterface( aMccInterface )
    {
    CActiveScheduler::Add( this );
    
    iTimer.CreateLocal();
    }
    
// End of file

