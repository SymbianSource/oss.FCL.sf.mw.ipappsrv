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




#include "mcecomaudiocodec.h"
#include "mceamrcodec.h"
#include "mceg711codec.h"   
#include "mceg729codec.h"    
#include  "mceilbccodec.h"  

#include "mceserial.h"
#include "mceevents.h"

#include "mcecomrtpsource.h"
#include "mcertpsource.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmcccodecinformation.h>
#include "mcesrvstream.h"
#include "mcesrvsource.h"
#include "mcemmlogs.h"

#endif//MCE_COMMON_SERVER_SIDE

// CONSTANTS
const TUint KMceSamplingFreq8000 = 8000;

// ============================ MEMBER FUNCTIONS ===============================

    
// -----------------------------------------------------------------------------
// CMceComAudioCodec::NewL
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* CMceComAudioCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceComAudioCodec::NewLC
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* CMceComAudioCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec* self = new (ELeave) CMceComAudioCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComAudioCodec::~CMceComAudioCodec
// -----------------------------------------------------------------------------
//
CMceComAudioCodec::~CMceComAudioCodec()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceComAudioCodec::CMceComAudioCodec
// -----------------------------------------------------------------------------
//
CMceComAudioCodec::CMceComAudioCodec()
  : CMceComCodec(),
    iEnableVAD( EFalse ),
    iSamplingFreq( 0 ),
    iPTime( 0 ),
    iMaxPTime( 0 ),
    iRedPayloadType( KMcePayloadTypeUndefined ),
    iRedCount( 0 ),
    iComfortNoiseEnabled( EFalse )
    {
    }


// -----------------------------------------------------------------------------
// CMceComAudioCodec::ConstructL
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* CMceComAudioCodec::CloneL()
    {
    CMceComAudioCodec* copy = new (ELeave) CMceComAudioCodec();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    
    }


// -----------------------------------------------------------------------------
// CMceComAudioCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComAudioCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComCodec::ConstructL( aSdpName );
    }

// -----------------------------------------------------------------------------
// CMceComAudioCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComAudioCodec::ConstructL( CMceComAudioCodec& aCodec )
    {
    CMceComCodec::ConstructL( aCodec );
    
    iEnableVAD = aCodec.iEnableVAD;
	iSamplingFreq = aCodec.iSamplingFreq;	
	iPTime = aCodec.iPTime;
	iMaxPTime = aCodec.iMaxPTime;
	
	iRedPayloadType = aCodec.iRedPayloadType;
    iRedCount = aCodec.iRedCount;
    iComfortNoiseEnabled = aCodec.iComfortNoiseEnabled;
    }



// -----------------------------------------------------------------------------
// CMceComAudioCodec::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComAudioCodec::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComCodec::InternalizeFlatL( aReadStream );
    
    iEnableVAD = static_cast<TBool>( aReadStream.ReadUint8L() );
	iSamplingFreq = aReadStream.ReadUint16L();
	iPTime = aReadStream.ReadUint16L();
	iMaxPTime = aReadStream.ReadUint16L();
	
	iRedPayloadType = aReadStream.ReadUint8L();
	iRedCount = aReadStream.ReadUint16L();
	iComfortNoiseEnabled = static_cast<TBool>( aReadStream.ReadUint8L() );
    }
    
// -----------------------------------------------------------------------------
// CMceComAudioCodec::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComAudioCodec::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComCodec::ExternalizeFlatL( aWriteStream );
    
    aWriteStream.WriteUint8L( iEnableVAD );
    aWriteStream.WriteUint16L( iSamplingFreq );
    aWriteStream.WriteUint16L( iPTime );
    aWriteStream.WriteUint16L( iMaxPTime ); 
    
    aWriteStream.WriteUint8L( iRedPayloadType );
    aWriteStream.WriteUint16L( iRedCount ); 
    aWriteStream.WriteUint8L( iComfortNoiseEnabled );
    }



// -----------------------------------------------------------------------------
// CMceComAudioCodec::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComAudioCodec::UpdateL( CMceComCodec& aUpdate )
    {
    CMceComCodec::UpdateL( aUpdate );

    CMceComAudioCodec& update = static_cast<CMceComAudioCodec&>( aUpdate );

    iEnableVAD = update.iEnableVAD;
    iSamplingFreq = update.iSamplingFreq;
    
    if ( IS_RECEIVESTREAM( iStream ) )
        {
        iPTime = update.iPTime;
        iMaxPTime = update.iMaxPTime;
        }
        
    iRedPayloadType = update.iRedPayloadType;
    iRedCount = update.iRedCount;
    iComfortNoiseEnabled = update.iComfortNoiseEnabled;
    }


// -----------------------------------------------------------------------------
// CMceComAudioCodec::Factory
// -----------------------------------------------------------------------------
//
TMceComAudioCodecFactory CMceComAudioCodec::Factory()
    {
    return TMceComAudioCodecFactory();
    }


// -----------------------------------------------------------------------------
// CMceComAudioCodec::CompareSdpIndex
// -----------------------------------------------------------------------------
//
TInt CMceComAudioCodec::CompareSdpIndex( 
    const CMceComAudioCodec& aIndex1, const CMceComAudioCodec& aIndex2 )
    {
    // NOTE: if zero (equals) is returned from here, order is strangely anyway
    // changed. Returning positive value if indexes are equal is for avoiding
    // this quirk.

    if ( aIndex1.iCodecSdpIndex >= aIndex2.iCodecSdpIndex )
        {
        return (1);
        }
    else if ( aIndex1.iCodecSdpIndex < aIndex2.iCodecSdpIndex )
        {
        return (-1);
        }
    else
        {
        return (0);
        }
    }


// -----------------------------------------------------------------------------
// CMceComAudioCodec::SetSamplingFreq
// -----------------------------------------------------------------------------
//
TInt CMceComAudioCodec::SetSamplingFreq( TUint aSamplingFreq )
    {
    if ( KMceSamplingFreq8000 == aSamplingFreq )
        {
        iSamplingFreq = aSamplingFreq;
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }


// -----------------------------------------------------------------------------
// CMceComAudioCodec::SetPTime
// -----------------------------------------------------------------------------
//
TInt CMceComAudioCodec::SetPTime( TUint aPTime )
    {
    if ( ( 0 < aPTime ) && ( aPTime <= iMaxPTime ) )
        {
        iPTime = aPTime;
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }


// -----------------------------------------------------------------------------
// CMceComAudioCodec::SetMaxPTime
// -----------------------------------------------------------------------------
//
TInt CMceComAudioCodec::SetMaxPTime( TUint aMaxPTime )
    {
    if ( ( 0 < aMaxPTime ) && ( iPTime <= aMaxPTime ) )
        {
        iMaxPTime = aMaxPTime;
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }

#ifdef MCE_COMMON_SERVER_SIDE


// -----------------------------------------------------------------------------
// CMceComAudioCodec::operator=
// -----------------------------------------------------------------------------
//
/*lint -e1539 */
CMceComCodec& CMceComAudioCodec::operator=( CMccCodecInformation& aMccCodec )
    {

	iPayloadType = aMccCodec.PayloadType();
	iBitrate = aMccCodec.Bitrate();
	iAllowedBitrates = aMccCodec.AllowedBitrates();
	iCodecMode = aMccCodec.CodecMode();
	iFourCC = aMccCodec.FourCC();
	iFrameSize = aMccCodec.FrameSize();

    iEnableVAD = aMccCodec.VAD();
	iSamplingFreq = aMccCodec.SamplingFreq();
	iPTime = aMccCodec.PTime();
	iMaxPTime = aMccCodec.MaxPTime();
	
	iRedPayloadType = aMccCodec.RedundancyPT();
	iRedCount = aMccCodec.RedCount();
	iComfortNoiseEnabled = 
	    ( aMccCodec.ComfortNoiseGeneration() != KPayloadTypeUndefined );
    return *this;
    
    }
    
// -----------------------------------------------------------------------------
// CMceComAudioCodec::DoMccDecodeL
// -----------------------------------------------------------------------------
//
void CMceComAudioCodec::DoMccDecodeL( CMccCodecInformation& aMccCodec )
    {
    //copy values, which are not explicitely in SDP, from mcc codec
	iSamplingFreq = aMccCodec.SamplingFreq();
	
	iBitrate = aMccCodec.Bitrate();
	iRedPayloadType = aMccCodec.RedundancyPT();
	iRedCount = aMccCodec.RedCount();
	iComfortNoiseEnabled = aMccCodec.ComfortNoiseGeneration();
	//copy the value of VAD which are received from MO side
	iEnableVAD = aMccCodec.VAD();
    }

// -----------------------------------------------------------------------------
// CMceComAudioCodec::DoMccEncodeL
// -----------------------------------------------------------------------------
//
void CMceComAudioCodec::DoMccEncodeL( CMccCodecInformation& aMccCodec,
                                      CMceSrvStream& aStream )
    {
    User::LeaveIfError( aMccCodec.SetSamplingFreq( iSamplingFreq ) );
    
    // Packet times are negotiated at session or media line level but all 
    // codecs in a group may not support all packet times. In this case 
    // codec's defaults are taken into use so that session negotiation is not 
    // unnecessary failed.
    if ( KErrNone != aMccCodec.SetMaxPTime( iMaxPTime ) )
        {
	    iMaxPTime = aMccCodec.MaxPTime();
	    }
    
    if ( KErrNone != aMccCodec.SetPTime( iPTime ) )
        {
        iPTime = aMccCodec.PTime();
        }
    
    User::LeaveIfError( aMccCodec.EnableVAD( iEnableVAD ) );
    User::LeaveIfError( aMccCodec.SetBitrate( iBitrate ) );
    
    if ( aStream.Source().Data().iType == KMceRTPSource )
        {
        CMceComRtpSource& rtpSource = 
		    static_cast<CMceComRtpSource&> ( aStream.Source().Data() );
		
		User::LeaveIfError( 
    	    aMccCodec.SetJitterBufInactivityTimeOut( rtpSource.iInactivityTimer ) );
    	User::LeaveIfError( 
    	    aMccCodec.SetJitterBufThreshold( rtpSource.iBufferTreshold ) );
    	User::LeaveIfError( 
    	    aMccCodec.SetJitterBufBufferLength( rtpSource.iBufferLength ) );
        }
    
    DoMccEncodeComfortNoiseL( aMccCodec, aStream );
    DoMccEncodeRedundancyL( aMccCodec, aStream );
    }


// -----------------------------------------------------------------------------
// CMceComAudioCodec::DoMccValidateL
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceComAudioCodec::DoMccValidateL( CMccCodecInformation& aMccCodec,
                                                 CMceSrvStream& aStream,
                                                 TMceNegotiationRole aRole )
    {
    //if answerer mcc codec represents the received offer and codec the answer
    //if offerer codec represents the sent offer and mcc codec the answer
    TBool notValid = aRole == EMceRoleAnswerer ?
                  ( iAllowedBitrates && aMccCodec.AllowedBitrates() == 0 ||
	                iAllowedBitrates > aMccCodec.AllowedBitrates() ) :
                  ( iAllowedBitrates && aMccCodec.AllowedBitrates() == 0 ||
	                iAllowedBitrates < aMccCodec.AllowedBitrates() );
	
	User::LeaveIfError( notValid ? KErrNotSupported : KErrNone );
	//Explicitly call default implementation of DoMccValidateL in base class
	// to enable the check for codecMode
	return CMceComCodec::DoMccValidateL( aMccCodec, aStream, aRole);  
    }
    
// -----------------------------------------------------------------------------
// CMceComAudioCodec::DoMccRequireSignalling
// -----------------------------------------------------------------------------
//
TInt CMceComAudioCodec::DoMccRequireSignalling( 
    const CMceSrvStream& aStream,
    const CMccCodecInformation& aMccCurentCodec, 
    const CMccCodecInformation& aMccUpdateCodec ) const
    {
    TInt action = KMceNoSignalling;

    if ( aStream.StreamType() == CMceComMediaStream::EReceiveStream ||
         aStream.StreamType() == CMceComMediaStream::EReceiveOnlyStream )
        {
        if ( aMccCurentCodec.PTime() != aMccUpdateCodec.PTime() ||
             aMccCurentCodec.MaxPTime() != aMccUpdateCodec.MaxPTime() )
            {
            action = KMceRequiresSignalling;
            }
        }
        
    return action;
    
    }
    
// -----------------------------------------------------------------------------
// CMceComAudioCodec::DoMccAdjustL
// -----------------------------------------------------------------------------
//
void CMceComAudioCodec::DoMccAdjustL( CMccCodecInformation& aMccCodec,
                                      CMceSrvStream& aStream )
    {
	if( aStream.Data().iStreamType == CMceComMediaStream::ESendStream ||
        aStream.Data().iStreamType == CMceComMediaStream::ESendOnlyStream )
		{
	    // Do nothing if iPTime or iMaxPTime is greater than codecs supports.
	    // Then codecs default values is used.
		aMccCodec.SetPTime( iPTime );
		aMccCodec.SetMaxPTime( iMaxPTime );
		User::LeaveIfError( aMccCodec.SetBitrate( iBitrate ) );
		
		DoMccEncodeComfortNoiseL( aMccCodec, aStream );
		}
    }

// -----------------------------------------------------------------------------
// CMceComAudioCodec::DoSetDefaultFmtpAttributeL
// -----------------------------------------------------------------------------
//
void CMceComAudioCodec::DoSetDefaultFmtpAttributeL()
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// CMceComAudioCodec::DoDecodeAudioL
// -----------------------------------------------------------------------------
//

void CMceComAudioCodec::DoDecodeAudioL( 
    TInt aCodecIndex,
    RPointerArray<CMceSrvStream>& aStreams,
    CMceComAudioStream& aAudio,
    CMceMediaManager& aManager )
    {
    if ( aCodecIndex == 0 || IS_RECEIVESTREAM( &aAudio ) )
        {
        SetEnabled( ETrue );
        }

    for( TInt sinkNdx = 0; sinkNdx < aAudio.Sinks().Count(); sinkNdx++ )
        {
        CMceSrvStream* srvStream = 
            CMceSrvStream::NewL( aManager, 
                                 aAudio, 
                                 *aAudio.Source(),
                                 *aAudio.Sinks()[ sinkNdx ],
                                 *this );
        CleanupStack::PushL( srvStream );
        MCEMM_DEBUG_STREAM( "CMceComAudioCodec::DoDecodeAudioL(): decoded audio", 
                            *srvStream );
        aStreams.AppendL( srvStream );
        CleanupStack::Pop( srvStream );
        } 
    }

// -----------------------------------------------------------------------------
// CMceComAudioCodec::DoMccEncodeComfortNoiseL
// -----------------------------------------------------------------------------
//
void CMceComAudioCodec::DoMccEncodeComfortNoiseL( 
    CMccCodecInformation& aMccCodec,
    CMceSrvStream& aStream )
    {
    // If CN codec is not anymore present in session, disable comfort noise
    // generation from this codec.
    CMceComAudioCodec* searchCodec = Factory().CreateCodecLC( KMceSDPNameCn() );
    
    CMceComCodec* cnCodec = aStream.Data().FindCodecL( *searchCodec );
    
    iComfortNoiseEnabled = cnCodec ? cnCodec->iIsNegotiated : EFalse;
    
    // KPayloadTypeUndefined is used if cn is disabled   
    TUint8 comforNoisePt = 
        iComfortNoiseEnabled ? cnCodec->iPayloadType : KPayloadTypeUndefined;
    
    // If cn is disabled, handle removing the codec here
    if ( !iComfortNoiseEnabled )
    	{
    	aStream.Data().RemoveCodecL( cnCodec );
    	if ( aStream.Data().BoundStream() )
            {
            CMceComCodec* boundCnCodec = 
            	aStream.Data().BoundStreamL().FindCodecL( *searchCodec );
            	
           	aStream.Data().BoundStreamL().RemoveCodecL( boundCnCodec );
            }
			
		// update keep alive payload type and data
	    aMccCodec.SetKeepAlivePT( 120 );
        aMccCodec.SetKeepAliveData( KNullDesC8 );
    	}
    	
    CleanupStack::PopAndDestroy( searchCodec );
    
    // All codecs do not necessarily support cn, ignore errors
    aMccCodec.SetComfortNoiseGeneration( comforNoisePt );
    }

// -----------------------------------------------------------------------------
// CMceComAudioCodec::DoMccEncodeRedundancyL
// -----------------------------------------------------------------------------
//    
void CMceComAudioCodec::DoMccEncodeRedundancyL( 
    CMccCodecInformation& aMccCodec,
    CMceSrvStream& aStream )
    {
    // If red codec is not anymore present in session, disable redundancy
    // from this codec.
    CMceComAudioCodec* searchCodec = Factory().CreateCodecLC( KMceSDPNameRed() );
    
    CMceComCodec* dtmfCodec = aStream.Data().FindCodecL( *searchCodec );
    if ( !dtmfCodec || !dtmfCodec->iIsNegotiated )
        {
        iRedPayloadType = KMcePayloadTypeUndefined;
        iRedCount = 0;
        }
        
    CleanupStack::PopAndDestroy( searchCodec );
    
    User::LeaveIfError( aMccCodec.SetRedundancyPT( iRedPayloadType ) );
	User::LeaveIfError( aMccCodec.SetRedCount( iRedCount ) );
    }

// -----------------------------------------------------------------------------
// CMceComAudioCodec::UpdateSendCodec
// -----------------------------------------------------------------------------
//
void CMceComAudioCodec::UpdateSendCodec( 
    TInt& aCodecIndex, 
    CMceComAudioStream& aAudio, 
    CMceComAudioCodec& aCodec )
    { 
    if ( aCodecIndex == 0 && 
        IS_SENDSTREAM( &aAudio ) && 
        aCodec.SendSupported() &&
        aCodec.iIsNegotiated )
        {
        // Changing send codec
        aCodec.SetEnabled( ETrue );
        aCodecIndex = KErrNotFound;
        }
    }
                                                  
#endif// MCE_COMMON_SERVER_SIDE

