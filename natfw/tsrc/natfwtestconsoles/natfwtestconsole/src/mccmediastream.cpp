/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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




#include "mccmediastream.h"

#include <mmcccodecinformation.h>
#include <mmccinterface.h>
#include "mccsession.h"
#include "mcclink.h"


const TInt KAudioPrefVoipAudioUplink = 0x05220001;
const TInt KAudioPrefVoipAudioDownlink = 0x05210001;

// ---------------------------------------------------------------------------
// CMccMediaStream::ConstructL
// ---------------------------------------------------------------------------
// 
void CMccMediaStream::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CMccMediaStream::NewL
// ---------------------------------------------------------------------------
// 
CMccMediaStream* CMccMediaStream::NewL( CMccLink* aLink, CMccSession* aSession,
    TStreamDirection aStreamDirection )
    {
    CMccMediaStream* self = new( ELeave ) CMccMediaStream(
        aLink, aSession, aStreamDirection );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// CMccMediaStream::CMccMediaStream
// ---------------------------------------------------------------------------
// 
CMccMediaStream::CMccMediaStream( CMccLink* aLink, CMccSession* aSession,
    TStreamDirection aStreamDirection ) :
    iLink( aLink ), iSession( aSession ), iStreamDirection( aStreamDirection ),
    iState( EStreamReady )
    {
    }

// ---------------------------------------------------------------------------
// CMccMediaStream::~CMccMediaStream
// ---------------------------------------------------------------------------
//     
CMccMediaStream::~CMccMediaStream()
    {
    if ( ERtcpStreamStarted == iState )
        {
        iSession->MccInterface()->StopStream( iSession->MccSessionId(),
            iLink->LinkId(), iStreamId );
        }
    if ( iStreamId )
        {
        iSession->MccInterface()->DeleteStream( iSession->MccSessionId(),
            iLink->LinkId(), iStreamId );
        }
    }

// ---------------------------------------------------------------------------
// CMccMediaStream::CreateStreamL
// ---------------------------------------------------------------------------
//     
void CMccMediaStream::CreateStreamL( TInt32& aStreamId, TRequestStatus& aStatus )
    {
    TUint32 dataSinkId;
    TUint32 dataSourceId;

    RPointerArray<CMccCodecInformation> codecArray;
    
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL(
        codecArray );
    User::LeaveIfError( iSession->MccInterface()->GetCapabilities( codecArray ) );
    CleanupStack::Pop( &codecArray );
    
    if (  EUpStream == iStreamDirection )
        {
        User::LeaveIfError( iSession->MccInterface()->AddDataSink( KMccRtpSinkUid,
            KNullDesC8, dataSinkId ) );
        User::LeaveIfError( iSession->MccInterface()->AddDataSource( KUidMmfAudioInput,
            KNullDesC8, dataSourceId ) );
        codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioUplink );
        
        User::LeaveIfError( iSession->MccInterface()->CreateStream(
            iSession->MccSessionId(), iLink->LinkId(), iStreamId,
            KMccAudioUplinkStream, *codecArray[0] ) );
        }
    else
        {
        User::LeaveIfError( iSession->MccInterface()->AddDataSink( KUidMmfAudioOutput,
            KNullDesC8, dataSinkId ) );
        User::LeaveIfError( iSession->MccInterface()->AddDataSource( KMccRtpSourceUid,
            KNullDesC8, dataSourceId ) );
        codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioDownlink );
        
        User::LeaveIfError( iSession->MccInterface()->CreateStream(
            iSession->MccSessionId(), iLink->LinkId(), iStreamId, KMccAudioDownlinkStream,
            *codecArray[0] ) );
        }
    iState = EStreamCreated;
     
    User::LeaveIfError( iSession->MccInterface()->PrepareStream(
        iSession->MccSessionId(), iLink->LinkId(), iStreamId ) );
    
    aStreamId = iStreamId;
    aStatus = KRequestPending;
    }

// ---------------------------------------------------------------------------
// CMccMediaStream::PrepareStreamL
// ---------------------------------------------------------------------------
//     
void CMccMediaStream::PrepareStreamL( TRequestStatus& aStatus )
    {
    User::LeaveIfError( iSession->MccInterface()->PrepareStream(
        iSession->MccSessionId(), iLink->LinkId(), iStreamId ) );
    
    aStatus = KRequestPending;
    }

// ---------------------------------------------------------------------------
// CMccMediaStream::StartStreamL
// ---------------------------------------------------------------------------
//     
void CMccMediaStream::StartStreamL( TRequestStatus& aStatus )
    {
    User::LeaveIfError( iSession->MccInterface()->StartStream(
        iSession->MccSessionId(), iLink->LinkId(), iStreamId ) );
        
    aStatus = KRequestPending;
    }
    
// ---------------------------------------------------------------------------
// CMccMediaStream::StreamId
// ---------------------------------------------------------------------------
// 
TUint32 CMccMediaStream::StreamId()
    {
    return iStreamId;
    }

// ---------------------------------------------------------------------------
// CMccMediaStream::State
// ---------------------------------------------------------------------------
// 
TMccStreamState CMccMediaStream::State()
    {
    return iState;
    }

// ---------------------------------------------------------------------------
// CMccMediaStream::SetState
// ---------------------------------------------------------------------------
// 
void CMccMediaStream::SetState( TMccStreamState aState )
    {
    iState = aState;
    }
    
// ---------------------------------------------------------------------------
// CMccMediaStream::Delete
// ---------------------------------------------------------------------------
//     
TInt CMccMediaStream::Delete()
    {
    iState = EStreamReady;
    return iSession->MccInterface()->DeleteStream( iSession->MccSessionId(),
        iLink->LinkId(), iStreamId );
    }
