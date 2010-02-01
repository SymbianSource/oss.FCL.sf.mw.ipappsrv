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




#include "mcertpsource.h"
#include "mcesession.h"
#include "mcemanager.h"
#include "mcemediastream.h"
#include "mcertpobserver.h"
#include "mcecomrtpsource.h"
#include "mceitcsender.h"
#include "mceevents.h"
#include "mceserial.h"
#include "mceclilogs.h"


#define KMCETIMERDISABLED 0
#define _FLAT_DATA static_cast<CMceComRtpSource*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceRtpSource::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRtpSource* CMceRtpSource::NewL(
					   TUint aBufferLength,
					   TUint aBufferTreshold,
					   TUint32 aInactivityTimer,
					   HBufC8* aIdentity )
    {
    CMceRtpSource* self = NewLC( aBufferLength, aBufferTreshold,
    	 aInactivityTimer, aIdentity );
    CleanupStack::Pop( self );
    return self;   
    }

// -----------------------------------------------------------------------------
// CMceRtpSource::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRtpSource* CMceRtpSource::NewLC(
					   TUint aBufferLength,
					   TUint aBufferTreshold,
					   TUint32 aInactivityTimer,
					   HBufC8* aIdentity )
    {
    CMceRtpSource* self = new (ELeave) CMceRtpSource();
    CleanupStack::PushL( self );
    self->ConstructL( aBufferLength, aBufferTreshold, 
                      aInactivityTimer, NULL, aIdentity );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceRtpSource::~CMceRtpSource
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRtpSource::~CMceRtpSource()
    {
    }

// -----------------------------------------------------------------------------
// CMceRtpSource::EnableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSource::EnableL() 
    {
    MCECLI_DEBUG("CMceRtpSource::EnableL, Entry");
    
    CMceMediaSource::DoEnableL();

    MCECLI_DEBUG("CMceRtpSource::EnableL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceRtpSource::DisableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSource::DisableL() 
    {
    MCECLI_DEBUG("CMceRtpSource::DisableL, Entry");
    
    CMceMediaSource::DoDisableL();
    
    MCECLI_DEBUG("CMceRtpSource::DisableL, Exit");
    
    }


// -----------------------------------------------------------------------------
// CMceRtpSource::EnableInactivityTimerL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSource::EnableInactivityTimerL( TUint32 aInactivityTimer ) 
    {
    MCECLI_DEBUG("CMceRtpSource::EnableInactivityTimerL, Entry");
    MCECLI_DEBUG_DVALUE("timer", aInactivityTimer );
    
    __ASSERT_ALWAYS( MCE_ENDPOINT_ITC_ALLOWED( *this ), 
        User::Leave( KErrNotReady ) );
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        CMceSession* session = iStream->Session();

        TMceIds ids;
        session->PrepareForITC( ids );
        ids.iMediaID = iStream->Id();
        ids.iSourceID  = Id();
        
        TMceItcArgTUint32 inactivity( aInactivityTimer );
        session->ITCSender().WriteL( ids, EMceItcEnableInactivityTimer, inactivity );
        }
    MCECLI_DEBUG("CMceRtpSource::EnableInactivityTimerL, Exit");
        
    }

// -----------------------------------------------------------------------------
// CMceRtpSource::DisableInactivityTimer
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSource::DisableInactivityTimerL() 
    {
    MCECLI_DEBUG("CMceRtpSource::DisableInactivityTimerL, Entry");
    
    FLAT_DATA( iInactivityTimer ) = KMCETIMERDISABLED;

    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        DoITCSendL( EMceItcDisableInactivityTimer );    
        }
    MCECLI_DEBUG("CMceRtpSource::DisableInactivityTimerL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceRtpSource::UpdateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSource::UpdateL ( TUint aBufferLength,
								TUint aBufferTreshold,
								TUint32 aInactivityTimer,
								HBufC8* aIdentity )
	{
    MCECLI_DEBUG("CMceRtpSource::UpdateL, Entry");
    MCECLI_DEBUG_DVALUE("buffer length", aBufferLength );
    MCECLI_DEBUG_DVALUE("buffer treshold", aBufferTreshold );
    MCECLI_DEBUG_DVALUE("inactivity timer", aInactivityTimer );
	
    FLAT_DATA( iInactivityTimer ) = aInactivityTimer;
    FLAT_DATA( iBufferLength ) = aBufferLength;
    FLAT_DATA( iBufferTreshold ) = aBufferTreshold;
    MCE_DELETE( FLAT_DATA( iTalkBurstIndicator ) );
    FLAT_DATA( iTalkBurstIndicator ) = 0;
    MCE_DELETE( FLAT_DATA( iIdentity ) );
    FLAT_DATA( iIdentity ) = aIdentity ? aIdentity->AllocL() : NULL;

    MCECLI_DEBUG("CMceRtpSource::UpdateL, Exit");
	}

// -----------------------------------------------------------------------------
// CMceRtpSource::SendRRL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSource::SendRRL() 
    {
    MCECLI_DEBUG("CMceRtpSource::SendRRL, Entry");
    
    __ASSERT_ALWAYS( MCE_ENDPOINT_ITC_ALLOWED( *this ), 
                     User::Leave( KErrNotReady ) );
    
    DoITCSendL( EMceItcSendRR );
    
    MCECLI_DEBUG("CMceRtpSource::SendRRL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceRtpSource::Ssrc
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMceRtpSource::Ssrc() const
    {
    return 0;
    }
    
// -----------------------------------------------------------------------------
// CMceRtpSource::Ssrcs
// -----------------------------------------------------------------------------
//
EXPORT_C const RArray<TUint>& CMceRtpSource::Ssrcs() const			
    {
    return FLAT_DATA( iNotSupported );    
    }

// -----------------------------------------------------------------------------
// CMceRtpSource::EnableSsrcL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSource::EnableSsrcL( TUint /*aSsrc*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMceRtpSource::DisableSsrcL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSource::DisableSsrcL( TUint /*aSsrc*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMceRtpSource::IsSsrcEnabledL
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMceRtpSource::IsSsrcEnabledL( TUint /*aSsrc*/ )
    {
    User::Leave( KErrNotSupported );
    return EFalse;
    }
	
// -----------------------------------------------------------------------------
// CMceRtpSource::CMceRtpSource
// -----------------------------------------------------------------------------
//
CMceRtpSource::CMceRtpSource()
   : CMceMediaSource()
    {
    }


// -----------------------------------------------------------------------------
// CMceRtpSource::ConstructL
// -----------------------------------------------------------------------------
//
void CMceRtpSource::ConstructL( TUint aBufferLength,
                                TUint aBufferTreshold,
                                TUint32 aInactivityTimer,
                                HBufC8* aTalkBurstIndicator,
                                HBufC8* aIdentity )
    {
    CMceComRtpSource* source = CMceComRtpSource::NewLC();
    CMceMediaSource::ConstructL( source );
    CleanupStack::Pop( source );
    
    FLAT_DATA( iInactivityTimer ) = aInactivityTimer;
    FLAT_DATA( iBufferLength ) = aBufferLength;
    FLAT_DATA( iBufferTreshold ) = aBufferTreshold;
    FLAT_DATA( iTalkBurstIndicator ) = aTalkBurstIndicator;
    FLAT_DATA( iIdentity ) = aIdentity ? aIdentity->AllocL() : NULL;
    }



// -----------------------------------------------------------------------------
// CMceRtpSource::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceRtpSource::EventReceivedL( TMceEvent& aEvent )
    {
    TInt status = CMceMediaSource::EventReceivedL( aEvent );
    if ( status != KMceEventNotConsumed )
        {
        return status;
        }
                
    if ( aEvent.Id().IsSourceId() )
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
// CMceRtpSource::HandleEvent
// -----------------------------------------------------------------------------
//
TInt CMceRtpSource::HandleEvent( TMceEvent& aEvent )
    {
    MCECLI_DEBUG("CMceRtpSource::HandleEvent, Entry");
    
    TInt status = KMceEventNotConsumed;
    
    MMceRtpObserver* observer = iStream->Session()->Manager().RtpObserver();
    switch ( aEvent.Action() )
        {
        case EMceItcSRReceived:
            {
            MCECLI_DEBUG("CMceRtpSource::HandleEvent, SR received");
            if ( observer )
                {
                observer->SRReceived( *( iStream->Session() ), *iStream );
                }
            status = KMceEventConsumed;
            break;
            }
        case EMceItcRRReceived:
            {
            MCECLI_DEBUG("CMceRtpSource::HandleEvent, RR received");
            if ( observer )
                {
                observer->RRReceived( *( iStream->Session() ), *iStream );
                }
            status = KMceEventConsumed;
            break;
            }
        case EMceItcInactivityTimeout:
            {
            MCECLI_DEBUG("CMceRtpSource::HandleEvent, inactivity timeout received");
            if ( observer )
                {
                observer->InactivityTimeout( *iStream, *this );
                }
            status = KMceEventConsumed;
            break;
            }
        default:
            {
            //NOP
            MCECLI_DEBUG("CMceRtpSource::HandleEvent, not consumed");
            break;
            }
        
        }
    
    MCECLI_DEBUG_DVALUE("CMceRtpSource::HandleEvent, Exit. status", status );
    return status;
    
    }
    
// -----------------------------------------------------------------------------
// CMceRtpSource::DoITCSendL
// -----------------------------------------------------------------------------
//
void CMceRtpSource::DoITCSendL( TUint8 aITCFunction )
	{
    __ASSERT_ALWAYS( iStream, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iStream->Session(), User::Leave( KErrNotReady ) );
    CMceSession* session = iStream->Session();
    
    if ( session->State() != CMceSession::EIdle )
        {
    	TMceIds ids;
    	session->PrepareForITC( ids );
    	ids.iMediaID   = iStream->Id();
	    ids.iSourceID  = Id();
    	ids.iState     = IsEnabled();
    	
        session->ITCSender().SendL( ids, static_cast<TMceItcFunctions>( aITCFunction ) );        
        }	
	}

