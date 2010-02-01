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




#include "mcertpsink.h"
#include "mcemediastream.h"
#include "mcesession.h"
#include "mcecomrtpsink.h"
#include "mceitcsender.h"
#include "mcertpobserver.h"
#include "mcemanager.h"
#include "mceevents.h"
#include "mceserial.h"
#include "mceclilogs.h"


#define _FLAT_DATA static_cast<CMceComRtpSink*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceRtpSink::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRtpSink* CMceRtpSink::NewL()
    {
    CMceRtpSink* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceRtpSink::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRtpSink* CMceRtpSink::NewLC()
    {
    CMceRtpSink* self = new (ELeave) CMceRtpSink();
    CleanupStack::PushL( self );
    self->ConstructL( EFalse, NULL );
    return self;    
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRtpSink* CMceRtpSink::NewL( TBool aSuppressRTCP,
    HBufC8* aIdentity )
    {    
    CMceRtpSink* self = new (ELeave) CMceRtpSink();
    CleanupStack::PushL( self );
    self->ConstructL( aSuppressRTCP, aIdentity );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::~CMceRtpSink
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRtpSink::~CMceRtpSink()
    {
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::EnableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSink::EnableL() 
    {
    MCECLI_DEBUG("CMceRtpSink::EnableL, Entry");
    
    CMceMediaSink::DoEnableL();

    MCECLI_DEBUG("CMceRtpSink::EnableL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::DisableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSink::DisableL() 
    {
    MCECLI_DEBUG("CMceRtpSink::DisableL, Entry");
    
    CMceMediaSink::DoDisableL();
    
    MCECLI_DEBUG("CMceRtpSink::DisableL, Exit");

    }

// -----------------------------------------------------------------------------
// CMceRtpSink::SendSRL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSink::SendSRL()
    {
    MCECLI_DEBUG("CMceRtpSink::SendSRL, Entry");
    
    __ASSERT_ALWAYS( MCE_ENDPOINT_ITC_ALLOWED( *this ), 
                     User::Leave( KErrNotReady ) );
    
	DoITCSendL( EMceItcSendSR );

    MCECLI_DEBUG("CMceRtpSink::SendSRL, Exit");
	
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::LastPacket
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMceRtpSink::LastPacket() const
    {    
    return FLAT_DATA( iLastPacket );    
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::Ssrc
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMceRtpSink::Ssrc() const
    {    
    return FLAT_DATA( iSSRC );    
    }
    
// -----------------------------------------------------------------------------
// CMceRtpSink::UpdateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSink::UpdateL( TBool aSuppressRTCP, HBufC8* aIdentity )
    {
    MCECLI_DEBUG("CMceRtpSink::UpdateL, Entry");
        
    FLAT_DATA( iSuppressRTCP ) = aSuppressRTCP;
    MCE_DELETE( FLAT_DATA( iIdentity ) );
    FLAT_DATA( iIdentity ) = aIdentity ? aIdentity->AllocL() : NULL;

    MCECLI_DEBUG("CMceRtpSink::UpdateL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::CMceRtpSink
// -----------------------------------------------------------------------------
//
CMceRtpSink::CMceRtpSink()
   : CMceMediaSink()
    {
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::ConstructL
// -----------------------------------------------------------------------------
//
void CMceRtpSink::ConstructL( TBool aSuppressRTCP, HBufC8* aIdentity )
    {
    CMceComRtpSink* sink = CMceComRtpSink::NewLC();
    CMceMediaSink::ConstructL( sink );
    CleanupStack::Pop( sink );
    
    FLAT_DATA( iSuppressRTCP ) = aSuppressRTCP;
    FLAT_DATA( iIdentity ) = aIdentity ? aIdentity->AllocL() : NULL;
    }



// -----------------------------------------------------------------------------
// CMceRtpSink::DoITCSendL
// -----------------------------------------------------------------------------
//
void CMceRtpSink::DoITCSendL( TUint8 aITCFunction )
	{
    __ASSERT_ALWAYS( iStream, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iStream->Session(), User::Leave( KErrNotReady ) );
    CMceSession* session = iStream->Session();
    
    if ( session->State() != CMceSession::EIdle )
        {
    	TMceIds ids;
    	session->PrepareForITC( ids );
    	ids.iMediaID   = iStream->Id();
    	ids.iSinkID    = Id();
    	ids.iState     = IsEnabled();
    	
        session->ITCSender().SendL( ids, static_cast<TMceItcFunctions>( aITCFunction ) );        
        }	
	}

// -----------------------------------------------------------------------------
// CMceRtpSink::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceRtpSink::EventReceivedL( TMceEvent& aEvent )
    {
    TInt status = CMceMediaSink::EventReceivedL( aEvent );
    if ( status != KMceEventNotConsumed )
        {
        return status;
        }
                
    if ( aEvent.Id().IsSinkId() )
        {
        status = HandleEvent( aEvent );
        }
    else
        {
        status = KMceEventNotConsumed;
        }
    
    return status;
    
    }


// -----------------------------------------------------------------------------
// CMceRtpSink::HandleEvent
// -----------------------------------------------------------------------------
//
TInt CMceRtpSink::HandleEvent( TMceEvent& aEvent )
    {
    MCECLI_DEBUG("CMceRtpSink::HandleEvent, Entry");
    
    TInt status = KMceEventNotConsumed;
    
    MMceRtpObserver* observer = iStream->Session()->Manager().RtpObserver();
    switch ( aEvent.Action() )
        {
        case EMceItcSRReceived:
            {
            MCECLI_DEBUG("CMceRtpSink::HandleEvent, SR received");
            if ( observer )
                {
                observer->SRReceived( *( iStream->Session() ), *iStream );
                }
            status = KMceEventConsumed;
            break;
            }
        case EMceItcRRReceived:
            {
            MCECLI_DEBUG("CMceRtpSink::HandleEvent, RR received");
            if ( observer )
                {
                observer->RRReceived( *( iStream->Session() ), *iStream );
                }
            status = KMceEventConsumed;
            break;
            }
        default:
            {
            //NOP
            MCECLI_DEBUG("CMceRtpSink::HandleEvent, not consumed");
            break;
            }
        
        }

    MCECLI_DEBUG_DVALUE("CMceRtpSink::HandleEvent, Exit. status", status );
    
    return status;
    
    }
