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




#include "mcecodec.h"
#include "mcesession.h"
#include "mcemanager.h"
#include "mcemediastream.h"
#include "mceclientserver.h"
#include "mcefactory.h"
#include "mceevents.h"
#include "mcestreamobserver.h"
#include "mceclientserver.h"
#include "mcecomcodec.h"


#define _FLAT_DATA static_cast<CMceComCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceCodec::~CMceCodec
// -----------------------------------------------------------------------------
//
CMceCodec::~CMceCodec()
    {
    delete iFlatData;
    }

// -----------------------------------------------------------------------------
// CMceCodec::InitializeL
// -----------------------------------------------------------------------------
//
void CMceCodec::InitializeL( CMceMediaStream& aParent )
    {    
    iStream = &aParent;
        
    if ( !Id().IsAssigned() )
        {
        FLAT_DATA( iID ) = iStream->Session()->Manager().NextMediaId();
        }
    }

// -----------------------------------------------------------------------------
// CMceCodec::SetStateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetStateL( CMceCodec::TState aState )
    {
    FLAT_DATA( iState ) = aState;
    }


// -----------------------------------------------------------------------------
// CMceCodec::SetStandByTimerL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetStandByTimerL( TUint32 /*aTimerValue*/ )
    {
    // NOT SUPPORTED YET
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CMceCodec::SetMMFPriorityL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetMMFPriorityL( TInt aPriority )
    {
    FLAT_DATA( iPriority ) = aPriority;
    }

// -----------------------------------------------------------------------------
// CMceCodec::SetMMFPriorityPreferenceL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetMMFPriorityPreferenceL( TInt aPriorityPreference )
    {
    FLAT_DATA( iPriorityPreference ) = aPriorityPreference;
    }

// -----------------------------------------------------------------------------
// CMceCodec::SetKeepAliveTimerL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetKeepAliveTimerL( TUint8 aTimerValue )
    {
    FLAT_DATA( iKeepAliveTimer ) = aTimerValue;
    }

// -----------------------------------------------------------------------------
// CMceCodec::SetKeepAlivePayloadTypeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetKeepAlivePayloadTypeL( TUint8 aKeepAlivePT )
    {
    FLAT_DATA( iKeepAlivePayloadType ) = aKeepAlivePT;
    }

// -----------------------------------------------------------------------------
// CMceCodec::SetKeepAliveDataL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetKeepAliveDataL( const TDesC8& aData )        
    {
    _FLAT_DATA->SetKeepAliveDataL( aData );    
    }


// -----------------------------------------------------------------------------
// CMceCodec::State
// -----------------------------------------------------------------------------
//
EXPORT_C CMceCodec::TState CMceCodec::State() const
    {
    return static_cast<CMceCodec::TState>( FLAT_DATA( iState ) );
    }
    
// -----------------------------------------------------------------------------
// CMceCodec::Type
// -----------------------------------------------------------------------------
//   
EXPORT_C TMceCodecType CMceCodec::Type() const
    {
    return iType;
    }

// -----------------------------------------------------------------------------
// CMceCodec::FourCC
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMceCodec::FourCC() const
    {
    return FLAT_DATA( iFourCC );
    }
       
// -----------------------------------------------------------------------------
// CMceCodec::Bitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CMceCodec::Bitrate() const
    {
    return FLAT_DATA( iBitrate );
    }

// -----------------------------------------------------------------------------
// CMceAudioCodec::AllowedBitrates
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CMceCodec::AllowedBitrates() const
    {
    return FLAT_DATA( iAllowedBitrates );
    }
    
// -----------------------------------------------------------------------------
// CMceCodec::FrameSize
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CMceCodec::FrameSize() const
    {
    return FLAT_DATA( iFrameSize );
    }
    
// -----------------------------------------------------------------------------
// CMceCodec::CodecMode
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CMceCodec::CodecMode() const
    {
    return FLAT_DATA( iCodecMode );
    }    

// -----------------------------------------------------------------------------
// CMceCodec::PayloadType
// -----------------------------------------------------------------------------
//
EXPORT_C TUint8 CMceCodec::PayloadType() const
    {
    return FLAT_DATA( iPayloadType );
    }

    
// -----------------------------------------------------------------------------
// CMceCodec::SdpName
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMceCodec::SdpName() const
    {
    return FLAT_DATA( iSdpName );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CMceCodec::MMFPriority() const
    {
    return FLAT_DATA( iPriority );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CMceCodec::MMFPriorityPreference() const
    {
    return FLAT_DATA( iPriorityPreference );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TUint8 CMceCodec::KeepAliveTimer() const
    {
    return FLAT_DATA( iKeepAliveTimer );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TUint8 CMceCodec::KeepAlivePayloadType() const
    {
    return FLAT_DATA( iKeepAlivePayloadType );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C const TDesC8& CMceCodec::KeepAliveData() const
    {
    if ( FLAT_DATA( iKeepAliveData ) )
        {
        return *FLAT_DATA( iKeepAliveData );
        }
    return KNullDesC8;
    }

// -----------------------------------------------------------------------------
// CMceCodec::Preference
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceCodec::Preference() const
    {
    return FLAT_DATA( iPreference );
    }
	    
// -----------------------------------------------------------------------------
// CMceCodec::SetPreference
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetPreferenceL( TInt aPeference )
    {
    FLAT_DATA( iPreference ) = aPeference;
    }

// -----------------------------------------------------------------------------
// CMceCodec::Id
// -----------------------------------------------------------------------------
//
TMceMediaId CMceCodec::Id() const
    {
    return FLAT_DATA( iID );
    }
     
// -----------------------------------------------------------------------------
// CMceCodec::SerializationId
// -----------------------------------------------------------------------------
//
TUint64 CMceCodec::SerializationId() const
    {
    return FLAT_DATA( SerializationId() );
    }
     
// -----------------------------------------------------------------------------
// CMceCodec::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceCodec::InternalizeFlatL( RReadStream& aReadStream )
    {
    __ASSERT_ALWAYS( iFlatData , User::Leave( KErrNotReady ) );
    _FLAT_DATA->InternalizeFlatL( aReadStream );
    }

// -----------------------------------------------------------------------------
// CMceCodec::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceCodec::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    __ASSERT_ALWAYS( iFlatData , User::Leave( KErrNotReady ) );
    _FLAT_DATA->ExternalizeFlatL( aWriteStream );
    }

// -----------------------------------------------------------------------------
// CMceCodec::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceCodec::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    InternalizeFlatL( aSerCtx.ReadStream() );
    }

// -----------------------------------------------------------------------------
// CMceCodec::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceCodec::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    ExternalizeFlatL( aSerCtx.WriteStream() );
    }

// -----------------------------------------------------------------------------
// CMceCodec::BaseFactory
// -----------------------------------------------------------------------------
//
TMceFactory CMceCodec::BaseFactory()
    {
    return TMceFactory();
    }

// -----------------------------------------------------------------------------
// CMceCodec::FlatData
// -----------------------------------------------------------------------------
//    
CMceComCodec* CMceCodec::FlatData()
    {
    return iFlatData;
    }

// -----------------------------------------------------------------------------
// CMceCodec::Attach
// -----------------------------------------------------------------------------
//
void CMceCodec::Attach( CMceMediaStream& aParent )
    {
    iStream = &aParent;
    }
    
// -----------------------------------------------------------------------------
// CMceCodec::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceCodec::EventReceivedL( TMceEvent& aEvent )
    {
    TInt status = KMceEventNotOwner;
    TBool idMatch = aEvent.Id().iCodecID == Id();
    if ( !idMatch )
        {
        return KMceEventNotOwner;
        }
        
    if ( aEvent.Id().IsCodecId() )
        {
        status = KMceEventUpdate;
        }
    else
        {
        status = KMceEventNotConsumed;
        }
    
    return status;
    }

// -----------------------------------------------------------------------------
// CMceCodec::Updated
// -----------------------------------------------------------------------------
//
void CMceCodec::Updated()
    {
    }

// -----------------------------------------------------------------------------
// CMceCodec::SetSdpNameL
// -----------------------------------------------------------------------------
//
void CMceCodec::SetSdpNameL( const TDesC8& /*aSdpName*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// CMceCodec::CMceCodec
// -----------------------------------------------------------------------------
//
CMceCodec::CMceCodec()
    {
    }

// -----------------------------------------------------------------------------
// CMceCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceCodec::ConstructL( CMceComCodec* aFlatData )
    {
    iFlatData = aFlatData;
    FLAT_DATA( iID ) = TMceMediaId();//as not assigned
    }

