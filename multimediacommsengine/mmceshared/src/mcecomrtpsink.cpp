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
#include "mcecomrtpsink.h"
#include "mceevents.h"
#include "mceserial.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmccinterface.h>
#include "mcepreconditions.h"
#include "mcesdpsession.h"
#include "mcemediamanager.h"


#endif//MCE_COMMON_SERVER_SIDE

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComRtpSink::NewL
// -----------------------------------------------------------------------------
//
CMceComRtpSink* CMceComRtpSink::NewL()
    {
    CMceComRtpSink* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComRtpSink::NewLC
// -----------------------------------------------------------------------------
//
CMceComRtpSink* CMceComRtpSink::NewLC()
    {
    CMceComRtpSink* self = new (ELeave) CMceComRtpSink();
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComRtpSink::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComRtpSink::ConstructL( CMceComRtpSink& aSink )
    {
    Mem::Copy( this, &aSink, sizeof(CMceComMediaSink) );
    Zero();
    iSuppressRTCP = aSink.iSuppressRTCP;
    iSSRC = aSink.iSSRC;
    iLastPacket = aSink.iLastPacket;
    iIdentity = aSink.iIdentity ? aSink.iIdentity->AllocL() : NULL;
    
    }

// -----------------------------------------------------------------------------
// CMceComRtpSink::~CMceComRtpSink
// -----------------------------------------------------------------------------
//
CMceComRtpSink::~CMceComRtpSink()
    {
    delete iIdentity;
    }


// -----------------------------------------------------------------------------
// CMceComRtpSink::CMceComRtpSink
// -----------------------------------------------------------------------------
//
CMceComRtpSink::CMceComRtpSink()
   : CMceComMediaSink( KMceRTPSink ),
     iSuppressRTCP( EFalse ),
     iSSRC( 0 ),
     iLastPacket( 0 ),
     iIdentity( NULL )
    {
    }


// -----------------------------------------------------------------------------
// CMceComRtpSink::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSink* CMceComRtpSink::CloneL()
    {
    CMceComRtpSink* copy = new (ELeave) CMceComRtpSink();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy ); 
    return copy;
        
    }


// -----------------------------------------------------------------------------
// CMceComRtpSink::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComRtpSink::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaSink::InternalizeFlatL( aReadStream );
    
    iSuppressRTCP = static_cast<TBool>( aReadStream.ReadUint8L() );
    iSSRC = aReadStream.ReadUint32L();
    iLastPacket = aReadStream.ReadUint32L();
    MceSerial::DecodeL( iIdentity, aReadStream ); 
    }



// -----------------------------------------------------------------------------
// CMceComRtpSink::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComRtpSink::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaSink::ExternalizeFlatL( aWriteStream );
    
    aWriteStream.WriteUint8L( iSuppressRTCP );
    aWriteStream.WriteUint32L( iSSRC );
    aWriteStream.WriteUint32L( iLastPacket );
    MceSerial::EncodeL( iIdentity, aWriteStream ); 
    }



// -----------------------------------------------------------------------------
// CMceComRtpSink::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComRtpSink::UpdateL( CMceComMediaSink& aUpdate )
    {
    CMceComMediaSink::UpdateL( aUpdate );
    CMceComRtpSink& update = static_cast<CMceComRtpSink&>( aUpdate );
    
    iSuppressRTCP = update.iSuppressRTCP;
    iSSRC = update.iSSRC;
    iLastPacket = update.iLastPacket;
    
    delete iIdentity;
    iIdentity = NULL;
    iIdentity = update.iIdentity ? update.iIdentity->AllocL() : NULL;
    }


// -----------------------------------------------------------------------------
// CMceComRtpSink::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceComRtpSink::EventReceivedL( TMceComEvent& aEvent )
    {
    TInt status = CMceComMediaSink::EventReceivedL( aEvent );
    if ( status != KMceEventNotConsumed )
        {
        return status;
        }
                
    if ( aEvent.Id().IsSinkId() )
        {
        status = aEvent.Handler().HandleL( *this, aEvent );
        }
    else
        {
        status = KMceEventNotConsumed;
        }
    
    return status;
    
    }

// -----------------------------------------------------------------------------
// CMceComRtpSink::UseRTCP
// -----------------------------------------------------------------------------
//
TBool CMceComRtpSink::UseRTCP()
    {
    return !iSuppressRTCP;
    }
    
    
// -----------------------------------------------------------------------------
// CMceComRtpSink::SetSSRC
// -----------------------------------------------------------------------------
//
void CMceComRtpSink::SetSSRC( TUint32 aSSRC )
    {
    iSSRC = aSSRC;
    }
    

#ifdef MCE_COMMON_SERVER_SIDE


// -----------------------------------------------------------------------------
// CMceComRtpSink::IsEnabled
// -----------------------------------------------------------------------------
//
TBool CMceComRtpSink::IsEnabled( TBool aLocally ) const
    {
    TBool isEnabled = CMceComEndpoint::IsEnabled();
    
    if ( !aLocally )
        {
        if ( MediaStream() && MediaStream()->Preconditions().Count() )
            {
            for ( int i =0; i < MediaStream()->Preconditions().Count(); i++ )
            	{
            	
            	isEnabled = isEnabled && 
                        !MediaStream()->Preconditions()[ i ]->RemoteReservationNeeded();
            	}
            }
        }
        
    return isEnabled;
    }


// -----------------------------------------------------------------------------
// CMceComRtpSink::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComRtpSink::MccType()
    {
    return KUidMccRtpSink;
    }

// -----------------------------------------------------------------------------
// CMceComRtpSink::PrepareL
// -----------------------------------------------------------------------------
//
void CMceComRtpSink::PrepareL()
    {
    if ( MediaStream() && MediaStream()->Session() && 
         iIdentity && 
         iIdentity->Length() > 0 )
        {
        MediaStream()->Session()->SdpSession().Manager().SetCNameL( *this, *iIdentity );
        }
    }
    
#endif//MCE_COMMON_SERVER_SIDE

