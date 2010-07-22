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
* Description:    
*
*/




#include "mcedisplaysink.h"
#include "mcecomdisplaysink.h"
#include "mceevents.h"
#include "mcecomcodec.h"
#include "mcecommediastream.h"
#include "mcecomsession.h"
#include "mcecomavsink.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmccinterface.h>
#include "mcesdpsession.h"
#include "mcemediamanager.h"
#include "mcevideostream.h"
#include "mcecomvideocodec.h"

#endif//MCE_COMMON_SERVER_SIDE

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CMceComDisplaySink::NewL
// -----------------------------------------------------------------------------
//
CMceComDisplaySink* CMceComDisplaySink::NewL()
    {
    CMceComDisplaySink* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComDisplaySink::NewLC
// -----------------------------------------------------------------------------
//
CMceComDisplaySink* CMceComDisplaySink::NewLC()
    {
    CMceComDisplaySink* self = new (ELeave) CMceComDisplaySink();
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComDisplaySink::~CMceComDisplaySink
// -----------------------------------------------------------------------------
//
CMceComDisplaySink::~CMceComDisplaySink()
    {
    
    delete iSettings;
    if ( iEndpointProxy )
        {
        CMceComAvSink* endpointProxy = static_cast<CMceComAvSink*>( iEndpointProxy );
        endpointProxy->RemoveProxyClient( *this );
        if ( endpointProxy->ReferenceCount() == 0 )
            {
            delete endpointProxy;
            }
        iEndpointProxy = NULL;
        }
    
    }


// -----------------------------------------------------------------------------
// CMceComDisplaySink::CMceComDisplaySink
// -----------------------------------------------------------------------------
//
CMceComDisplaySink::CMceComDisplaySink()
  : CMceComMediaSink( KMceDisplaySink )
    {
    // Temporarily some default values
    // TRect (0,83,352,378);
    iDisplayIndex = 0;
    iDisplayRectTlX = 0;
    iDisplayRectTlY = 83;
    iDisplayRectBrX = 352;
    iDisplayRectBrY = 378;
    iPriority = 0;
    iIsViewFinder = EFalse;
    iRotation = CMceDisplaySink::ENone;
    iDisplaysAvailable = 0;
    iInitialInfoRetrieved = EFalse;
    
    }

// -----------------------------------------------------------------------------
// CMceComDisplaySink::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSink* CMceComDisplaySink::CloneL()
    {
    CMceComDisplaySink* copy = new (ELeave) CMceComDisplaySink();
    Mem::Copy( copy, this, sizeof(CMceComDisplaySink) );
    copy->Zero();
    copy->iSettings = 0;
    return copy;
        
    }

// -----------------------------------------------------------------------------
// CMceComDisplaySink::UsesEndpointProxy()
// -----------------------------------------------------------------------------
//
TBool CMceComDisplaySink::UsesEndpointProxy() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMceComDisplaySink::CreateEndpointProxyL()
// -----------------------------------------------------------------------------
//
MMceComEndPointProxy* CMceComDisplaySink::CreateEndpointProxyL() const
    {
    return CMceComAvSink::NewL();
    }



// -----------------------------------------------------------------------------
// CMceComDisplaySink::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComDisplaySink::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaSink::InternalizeFlatL( aReadStream );
    
    iDisplayIndex = aReadStream.ReadInt32L();
    iDisplayRectTlX = aReadStream.ReadInt32L();
    iDisplayRectTlY = aReadStream.ReadInt32L();
    iDisplayRectBrX = aReadStream.ReadInt32L();
    iDisplayRectBrY = aReadStream.ReadInt32L();
    iPriority = aReadStream.ReadUint32L();
    iIsViewFinder = aReadStream.ReadInt32L();
    iRotation = static_cast<CMceDisplaySink::TRotation> ( aReadStream.ReadInt32L() );
    
    iDisplaysAvailable = aReadStream.ReadInt32L();
    iInitialInfoRetrieved = static_cast<TBool>( aReadStream.ReadUint8L() );

    }



// -----------------------------------------------------------------------------
// CMceComDisplaySink::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComDisplaySink::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaSink::ExternalizeFlatL( aWriteStream );
    
    aWriteStream.WriteInt32L( iDisplayIndex );
    aWriteStream.WriteInt32L( iDisplayRectTlX );
    aWriteStream.WriteInt32L( iDisplayRectTlY );
    aWriteStream.WriteInt32L( iDisplayRectBrX );
    aWriteStream.WriteInt32L( iDisplayRectBrY );
    aWriteStream.WriteUint32L( iPriority );
    aWriteStream.WriteInt32L( iIsViewFinder );
    aWriteStream.WriteInt32L( iRotation );
    aWriteStream.WriteInt32L( iDisplaysAvailable );
    aWriteStream.WriteUint8L( iInitialInfoRetrieved );
    
    }

  
    
// -----------------------------------------------------------------------------
// CMceComDisplaySink::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceComDisplaySink::EventReceivedL( TMceComEvent& aEvent )
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
// CMceComDisplaySink::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComDisplaySink::UpdateL( CMceComMediaSink& aUpdate )
    {
    CMceComMediaSink::UpdateL( aUpdate );
    CMceComDisplaySink& update = static_cast<CMceComDisplaySink&>( aUpdate );
    
    iDisplayIndex = update.iDisplayIndex;
    iDisplayRectTlX = update.iDisplayRectTlX;
    iDisplayRectTlY = update.iDisplayRectTlY;
    iDisplayRectBrX = update.iDisplayRectBrX;
    iDisplayRectBrY = update.iDisplayRectBrY;
    iPriority = update.iPriority;
    iIsViewFinder = update.iIsViewFinder;
    iDisplaysAvailable = update.iDisplaysAvailable;
    iInitialInfoRetrieved = update.iDisplaysAvailable;
    
    }
    

#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComDisplaySink::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComDisplaySink::MccType()
    {
    TUid id = KUidMccVideoSink;
    
    if ( EndpointProxy() )
        {
        CMceComAvSink* endpointProxy = static_cast<CMceComAvSink*>( EndpointProxy() );
        id = endpointProxy->MccType();
        }
    return id;
        
    }


// -----------------------------------------------------------------------------
// CMceComDisplaySink::EnableL
// -----------------------------------------------------------------------------
//
void CMceComDisplaySink::EnableL( const CMceComCodec& aCodec )
    {
    __ASSERT_ALWAYS( iIsViewFinder, User::Leave( KErrArgument ) );
    CMceMediaManager& manager = MediaStream()->Session()->SdpSession().Manager();
    
    // Update iSettings
    DoInitParamL( aCodec );
    
    manager.EnableViewFinderL( *this, *iSettings );		
    
    }

// -----------------------------------------------------------------------------
// CMceComMediaSource::DisableL
// -----------------------------------------------------------------------------
//
void CMceComDisplaySink::DisableL()
    {
    __ASSERT_ALWAYS( iIsViewFinder, User::Leave( KErrArgument ) );
    CMceMediaManager& manager = MediaStream()->Session()->SdpSession().Manager();
    
    manager.DisableViewFinderL( *this );
    
    }


// -----------------------------------------------------------------------------
// CMceComDisplaySink::InitParamL
// -----------------------------------------------------------------------------
//
const TDesC8& CMceComDisplaySink::InitParamL( const CMceComCodec& aCodec )
    {
    delete iSettings;
    iSettings = NULL;

    if ( EndpointProxy() )
        {
        CMceComAvSink* endpointProxy = static_cast<CMceComAvSink*>( EndpointProxy() );
        return endpointProxy->InitParamL( aCodec );
        }
    else
        {
        DoInitParamL( aCodec );
        }
       
    return *iSettings;
    
    }

// -----------------------------------------------------------------------------
// CMceComDisplaySink::PrepareL
// -----------------------------------------------------------------------------
//    
void CMceComDisplaySink::PrepareL()
    {
    if ( EndpointProxy() )
        {
        CMceComAvSink* endpointProxy = static_cast<CMceComAvSink*>( EndpointProxy() );
        endpointProxy->PrepareL();
        }
    }

// -----------------------------------------------------------------------------
// CMceComDisplaySink::DoInitParamL
// -----------------------------------------------------------------------------
//
const TDesC8& CMceComDisplaySink::DoInitParamL( const CMceComCodec& aCodec )
    {
    delete iSettings;
    iSettings = NULL;

    TMccVideoSinkSetting displaySetting;
    TRect displayRect = DisplayRect();
    
    CMceComDisplaySink::ToPointAndSize( displayRect, 
                                        displaySetting.iLocation, 
                                        displaySetting.iSize );
    displaySetting.iDeviceIndex = 0;
    TMMFPrioritySettings priority;
    priority.iPriority = aCodec.iPriority;
    priority.iPref = TMdaPriorityPreference(aCodec.iPriorityPreference);
    displaySetting.iPriority = priority;
    
    displaySetting.iVideoPayloadType = aCodec.iPayloadType;
    displaySetting.iIAPId = MediaStream()->Session()->iIapId;
    displaySetting.iWindowOrdinalPosition = 1;
    displaySetting.iWindowOrdinalPriority = iPriority;
    displaySetting.iRotation = EMccNone;
    displaySetting.iEnabled = ETrue;
    
    if ( aCodec.Stream() && aCodec.Stream()->iType == KMceVideo )
        {
        displaySetting.iVideoFrameRate = 
            reinterpret_cast<const CMceComVideoCodec&>( aCodec ).iReceiveFrameRate;
        }
    else
        {
        displaySetting.iVideoFrameRate = 0;
        }

    TMccVideoSinkSettingBuf settings( displaySetting );
    
    iSettings = settings.AllocL();
    
    return *iSettings;

    }
    
// -----------------------------------------------------------------------------
// CMceComDisplaySink::DisplayRect
// -----------------------------------------------------------------------------
//
TRect CMceComDisplaySink::DisplayRect()
    {
    return TRect( iDisplayRectTlX,
                  iDisplayRectTlY,
                  iDisplayRectBrX,
                  iDisplayRectBrY );
    }

// -----------------------------------------------------------------------------
// CMceComDisplaySink::SetDisplayRect
// -----------------------------------------------------------------------------
//
void CMceComDisplaySink::SetDisplayRect( const TRect& aRect )
    {
    iDisplayRectTlX = aRect.iTl.iX;
    iDisplayRectTlY = aRect.iTl.iY;
    iDisplayRectBrX = aRect.iBr.iX;
    iDisplayRectBrY = aRect.iBr.iY;
    }

// -----------------------------------------------------------------------------
// CMceComDisplaySink::ToRect
// -----------------------------------------------------------------------------
//
void CMceComDisplaySink::ToRect( const TPoint& aPoint, const TSize& aSize, TRect& aRect )
    {
    aRect = TRect( aPoint, aSize );
    }


// -----------------------------------------------------------------------------
// CMceComDisplaySink::ToPointAndSize
// -----------------------------------------------------------------------------
//
void CMceComDisplaySink::ToPointAndSize( const TRect& aRect, TPoint& aPoint, TSize& aSize )
    {
    aPoint = TPoint( aRect.iTl.iX, aRect.iTl.iY );
    aSize = TSize( aRect.iBr.iX - aRect.iTl.iX,
                   aRect.iBr.iY - aRect.iTl.iY );
    
    }
    

// -----------------------------------------------------------------------------
// CMceComDisplaySink::SetViewFinder
// -----------------------------------------------------------------------------
//
void CMceComDisplaySink::SetViewFinder( TBool aIsViewFinder )
    {
    iIsViewFinder = aIsViewFinder;
    }

// -----------------------------------------------------------------------------
// CMceComDisplaySink::MccParameterName
// -----------------------------------------------------------------------------
//    
TUint32 CMceComDisplaySink::MccParameterName() const
    {
    return iIsViewFinder ? KMccCamLocalVideo : KMccDisplayLocalVideo;
    }

#endif//MCE_COMMON_SERVER_SIDE

