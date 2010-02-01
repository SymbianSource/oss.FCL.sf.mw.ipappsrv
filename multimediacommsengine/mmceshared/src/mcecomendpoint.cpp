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




#include "mcecomendpoint.h"
#include "mceserial.h"
#include "mceevents.h"
#include "mcecommediastream.h"
#include "mcecomendpointproxy.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include "mcemediaobserver.h"

#else

#include "mce.h"
#include "mceitcsender.h"

#endif

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComEndpoint::~CMceComEndpoint
// -----------------------------------------------------------------------------
//
CMceComEndpoint::~CMceComEndpoint()
    {
    iParents.Close();
    
#ifndef MCE_COMMON_SERVER_SIDE
    delete iItcSender;
#endif
    }
    

// -----------------------------------------------------------------------------
// CMceComEndpoint::CMceComEndpoint
// -----------------------------------------------------------------------------
//
CMceComEndpoint::CMceComEndpoint( TMceEndpointCategory aCategory, TUint8 aType )
  : iCategory( aCategory ),
    iType( aType ),
    iID( TMceMediaId( (TUint32)&iID ) ),
    iIsEnabled( ETrue )
    {
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::Category
// -----------------------------------------------------------------------------
//
TMceEndpointCategory CMceComEndpoint::Category() const
    {
    return iCategory;
    }
    
// -----------------------------------------------------------------------------
// CMceComEndpoint::Type
// -----------------------------------------------------------------------------
//
TUint8 CMceComEndpoint::Type() const
    {
    return iType;
    }    

// -----------------------------------------------------------------------------
// CMceComEndpoint::IsEnabled
// -----------------------------------------------------------------------------
//
TBool CMceComEndpoint::IsEnabled( TBool /*aLocally*/ ) const
    {
    return iIsEnabled;
    }
    
    
// -----------------------------------------------------------------------------
// CMceComEndpoint::Enabled
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::Enabled( TBool aValue )
    {
    iIsEnabled = aValue;
    }

    
// -----------------------------------------------------------------------------
// CMceComEndpoint::CreateEndpointProxyL
// -----------------------------------------------------------------------------
//
MMceComEndPointProxy* CMceComEndpoint::EndpointProxy() const
    {
    return iEndpointProxy;
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::CreateEndpointProxyL
// -----------------------------------------------------------------------------
//
MMceComEndPointProxy* CMceComEndpoint::CreateEndpointProxyL() const
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::UsesEndpointProxy
// -----------------------------------------------------------------------------
//
TBool CMceComEndpoint::UsesEndpointProxy() const
    {
    return EFalse;
    }
    
// -----------------------------------------------------------------------------
// CMceComEndpoint::SetupEndpointProxyL
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::SetupEndpointProxyL( MMceComEndPointProxy* aEndpointProxy )
    {
    if ( !UsesEndpointProxy() )
        {
        return;
        }
        
    if ( !aEndpointProxy && !iEndpointProxy )
        {
        iEndpointProxy = CreateEndpointProxyL();
        iEndpointProxy->AddProxyClientL( *this );
        }
    else if ( aEndpointProxy && !iEndpointProxy )
        {
        aEndpointProxy->AddProxyClientL( *this );
        iEndpointProxy = aEndpointProxy;
        }
    else
        {
        iEndpointProxy->AddProxyClientL( *this );
        }
        
    }
    
// -----------------------------------------------------------------------------
// CMceComEndpoint::SerializationId
// -----------------------------------------------------------------------------
//
TUint64 CMceComEndpoint::SerializationId() const
    {
    return iID.Uint64();
    }

    
// -----------------------------------------------------------------------------
// CMceComEndpoint::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::InternalizeFlatL( RReadStream& aReadStream )
    {
    //iType, needs to be first value to internalize
    iType = aReadStream.ReadUint8L();
    //iCategory
    iCategory = aReadStream.ReadUint8L();
    //iID
    MceSerial::DecodeL( iID, aReadStream );
    //iIsEnabled
    iIsEnabled = static_cast<TBool>( aReadStream.ReadUint8L() );
        
    }
    
// -----------------------------------------------------------------------------
// CMceComEndpoint::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    //iType, needs to be first value to externalize
	aWriteStream.WriteUint8L( iType );
	//iCategory
    aWriteStream.WriteUint8L( iCategory ); 
    //iID
    MceSerial::EncodeL( iID, aWriteStream );
    //iIsEnabled
	aWriteStream.WriteUint8L( iIsEnabled );
    
    }


// -----------------------------------------------------------------------------
// CMceComEndpoint::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    InternalizeFlatL( aSerCtx.ReadStream() );
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    ExternalizeFlatL( aSerCtx.WriteStream() );
    }


// -----------------------------------------------------------------------------
// CMceComEndpoint::BaseFactory
// -----------------------------------------------------------------------------
//
TMceComFactory CMceComEndpoint::BaseFactory()
    {
    return TMceComFactory();
    }

        
// -----------------------------------------------------------------------------
// CMceComEndpoint::Id
// -----------------------------------------------------------------------------
//
TMceMediaId CMceComEndpoint::Id() const
    {
    return iID;
    }


// -----------------------------------------------------------------------------
// CMceComEndpoint::ReferenceCount
// -----------------------------------------------------------------------------
//
TInt& CMceComEndpoint::ReferenceCount()
    {
    return iReferenceCount;
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::InitializeL
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::InitializeL( CMceComMediaStream& aParent )
    {
    StreamAddedL( aParent );
    iStream = iParents[0];
    
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::StreamAddedL
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::StreamAddedL( CMceComMediaStream& aParent )
    {
    
    if ( iParents.Find( &aParent ) == KErrNotFound )
        {
        iParents.AppendL( &aParent );
        }
    
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::UnInitialize
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::UnInitialize( CMceComMediaStream& aParent )
    {
    TInt index = iParents.Find( &aParent );
    if ( index >= 0 )
        {
        iParents.Remove( index );
        }
    iStream = NULL;
    
    if ( iParents.Count() > 0 )
        {
        iStream = iParents[0];
        }
    
    }


// -----------------------------------------------------------------------------
// CMceComEndpoint::MediaStream
// -----------------------------------------------------------------------------
//
CMceComMediaStream* CMceComEndpoint::MediaStream() const
    {
    return iStream;
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::operator==
// -----------------------------------------------------------------------------
//
TBool CMceComEndpoint::operator==( const CMceComEndpoint& aEndpoint ) const
    {
    return Id() == aEndpoint.Id();
    }


// -----------------------------------------------------------------------------
// CMceComEndpoint::Zero
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::Zero()
    {
    iStream = NULL;
    iReferenceCount = 0;
    iParents = RPointerArray<CMceComMediaStream>( KMceArrayGranularity );
    }


// -----------------------------------------------------------------------------
// CMceComEndpoint::BaseUpdateL
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::BaseUpdateL( CMceComEndpoint& aEndpoint )
    {
    iCategory = aEndpoint.iCategory;
    iType = aEndpoint.iType;
    iID = aEndpoint.iID;
    iIsEnabled = aEndpoint.iIsEnabled;
    //if updating data only (orphan endpoint)
    iReferenceCount = aEndpoint.iReferenceCount > 0 ? 
                                aEndpoint.iReferenceCount : iReferenceCount;
    
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::Reusable
// -----------------------------------------------------------------------------
//
TBool CMceComEndpoint::Reusable( const CMceComMediaStream& /*aParent*/ ) const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::UseRTCP
// -----------------------------------------------------------------------------
//
TBool CMceComEndpoint::UseRTCP()
    {
    return ETrue;
    }
    
#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComEndpoint::InitParamL
// -----------------------------------------------------------------------------
//
const TDesC8& CMceComEndpoint::InitParamL( const CMceComCodec& /*aCodec*/ )
    {
    return KNullDesC8;
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::InitializedL
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::InitializedL()
    {
    //NOP
    }
    
// -----------------------------------------------------------------------------
// CMceComEndpoint::EnableL
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::EnableL( const CMceComCodec& /*aCodec*/ )
    {
    //NOP
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::DisableL
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::DisableL()
    {
    //NOP
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::PrepareL
// -----------------------------------------------------------------------------
//    
void CMceComEndpoint::PrepareL()
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::EventReceived
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::EventReceived( TMceMccComEvent& aEvent )
    {
    
    CMceComSession* session = MediaStream()->Session();
    
    switch( aEvent.iEvent )
        {
        case KMccStreamPrepared:
            {
            TInt err = DoPrepared();
            if ( err != KErrNone )
                {
                aEvent.iError = err;
                session->MediaObserver().MediaError( aEvent );
                }
            break;
            }
        case KMccStreamPaused:
        case KMccStreamResumed:
            {
            aEvent.iItcEvent = EMceItcStateChanged;
            aEvent.iItcData = static_cast<TUint32>( iIsEnabled );
            session->MediaObserver().EventReceived( aEvent );
            break;
            }
        case KMccInactivityEvent:
            {
            aEvent.iItcEvent = EMceItcInactivityTimeout;
            aEvent.iItcData = MediaStream()->iState;
            session->MediaObserver().EventReceived( aEvent );
            break;
            }
        case KMccStreamError:
            {
            aEvent.iItcEvent = EMceItcInProgress;
            
            aEvent.iItcData = 
                MCE_ENCODE_DOUBLE_STATE( MediaStream()->iState, iIsEnabled );

            session->MediaObserver().MediaError( aEvent );
            break;
            }
        case KMccRtcpReceived:
        case KMccDtmfReceived:
        case KMccDtmfControl:
            {
            aEvent.iItcEvent = EMceItcNotAssigned;
            switch( aEvent.iEventSubType )
                {
                case KRtcpSrPacket:
                    {
                    aEvent.iItcEvent = EMceItcSRReceived;
                    break;
                    }
                case KRtcpRrPacket:
                    {
                    aEvent.iItcEvent = EMceItcRRReceived;
                    break;
                    }
                case KRtcpPacketUndefined:
                    {
                    aEvent.iItcEvent = EMceItcFCMsgReceived;
                    break;
                    }
                case KMccDtmfManualStart:
                case KMccDtmfSequenceStart:
                    {
                    aEvent.iItcEvent = EMceItcDtmfSendStarted;
                    break;
                    }
                case KMccDtmfManualStop:
                case KMccDtmfSequenceStop:
                case KMccDtmfStopInDtmfString:
                case KMccDtmfSendingComplete:
                    {
                    aEvent.iItcEvent = EMceItcDtmfSendCompleted;
                    break;
                    }
                case KMccDtmfManualAbort:
                case KMccDtmfSequenceAbort:
                    {
                    aEvent.iItcEvent = EMceItcDtmfSendError;
                    break;
                    }
                default:
                    {
                    //NOP
                    break;
                    }
                }
            if ( aEvent.iItcEvent != EMceItcNotAssigned )
                {
                aEvent.iItcData = MediaStream()->iState;
                session->MediaObserver().EventReceived( aEvent );
                }
            break;
            }
        case KMccStreamIdle:
        case KMccStreamPlaying:
        case KMccStreamStarted:
        case KMccStreamBuffering:
        case KMccStreamStopped:
        case KMccStreamClosed:
        case KMccResourceNotAvailable:
        case KMccActivityEvent:
        case KMccResourceReserved:
        case KMccResourceReleased:
            {
            aEvent.iItcEvent = EMceItcInProgress;
            aEvent.iItcData = 
                MCE_ENCODE_DOUBLE_STATE( MediaStream()->iState, iIsEnabled );    
        
            session->MediaObserver().EventReceived( aEvent );
            break;
            }
        default:
            {
            //NOP
            break;
            }
                
        }
        
    }
    

// -----------------------------------------------------------------------------
// CMceComEndpoint::Merge
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::Merge( CMceComEndpoint& aMergeWith )
    {
    if ( !MediaStream()->DowngradedEndpoints() )
        {
        iIsEnabled = aMergeWith.iIsEnabled;
        }
    }
    
// -----------------------------------------------------------------------------
// CMceComEndpoint::SetState
// -----------------------------------------------------------------------------
//
void CMceComEndpoint::SetState( TMceMccComEvent& aEvent )
    {

    switch( aEvent.iEvent )
        {
        case KMccStreamResumed:
        case KMccStreamStarted:
            {
            iIsEnabled = ETrue;
            break;
            }
        case KMccStreamStopped:
        case KMccStreamPaused:
        case KMccStreamError:
            {
            iIsEnabled = EFalse;
            break;
            }
        case KMccResourceNotAvailable:
            {
            if ( aEvent.iSource || aEvent.iSink )
                {
                iIsEnabled = EFalse;
                }
            break;
            }
        default:
            {
            //NOP
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMceComEndpoint::DoPrepared
// -----------------------------------------------------------------------------
//
TInt CMceComEndpoint::DoPrepared()
    {
    return KErrNone;
    }
    
#else // MCE CLIENT SIDE

// -----------------------------------------------------------------------------
// CMceComEndpoint::SenderL
// -----------------------------------------------------------------------------
//
CMceItcSender& CMceComEndpoint::SenderL( RMce& aItcProvider )
    {
    if ( !iItcSender )
        {
        iItcSender = CMceItcSender::NewL( aItcProvider );
        }
    return *iItcSender;
    }
#endif//MCE_COMMON_SERVER_SIDE
