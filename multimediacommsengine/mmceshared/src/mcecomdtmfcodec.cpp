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




#include "mcedtmfcodec.h"
#include "mcecomdtmfcodec.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmcccodecinformation.h>
#include "mcecomaudiostream.h"
#include "mcertpsink.h"
#include "mcertpsource.h"
#include "mcemmlogs.h"

#endif //MCE_COMMON_SERVER_SIDE

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CMceComDtmfCodec::NewL
// -----------------------------------------------------------------------------
//
CMceComDtmfCodec* CMceComDtmfCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComDtmfCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceComDtmfCodec::NewLC
// -----------------------------------------------------------------------------
//
CMceComDtmfCodec* CMceComDtmfCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComDtmfCodec* self = new (ELeave) CMceComDtmfCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComDtmfCodec::~CMceComDtmfCodec
// -----------------------------------------------------------------------------
//
CMceComDtmfCodec::~CMceComDtmfCodec()
    {
    }
    

// -----------------------------------------------------------------------------
// CMceComDtmfCodec::CMceComDtmfCodec
// -----------------------------------------------------------------------------
//
CMceComDtmfCodec::CMceComDtmfCodec()
 : CMceComAudioCodec()
    {
    iFourCC = 0;
    iEnableVAD = EFalse;
	iSamplingFreq  = KMceDtmfSamplingFreq;
	iPTime = KMceDtmfDefaultPtime;
    const TUint KMceDtmfDefaultMaxPtime = 200;
	iMaxPTime = KMceDtmfDefaultMaxPtime;
	iAllowedBitrates = KMceAllowedDtmfEvents0to15;
	iPayloadType = KMceDefaultDtmfPayloadType;
    iCodecMode = EMceOutbound;
    }


// -----------------------------------------------------------------------------
// CMceComDtmfCodec::CloneL
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* CMceComDtmfCodec::CloneL()
    {
    CMceComDtmfCodec* copy = new (ELeave) CMceComDtmfCodec();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    }

// -----------------------------------------------------------------------------
// CMceComDtmfCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComDtmfCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec::ConstructL( aSdpName );
    }

// -----------------------------------------------------------------------------
// CMceComDtmfCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComDtmfCodec::ConstructL( CMceComDtmfCodec& aCodec )
    {
    CMceComAudioCodec::ConstructL( aCodec );
    }

// -----------------------------------------------------------------------------
// CMceComDtmfCodec::SetBitrate
// -----------------------------------------------------------------------------
//
TInt CMceComDtmfCodec::SetBitrate( TUint /*aBitrate*/ )
    {
	return KErrNotSupported;
    }  
   
// -----------------------------------------------------------------------------
// CMceComDtmfCodec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMceComDtmfCodec::SetAllowedBitrates( TUint aBitrates )
    {
    // Nothing else is supported at the moment
    if( !( aBitrates == KMceAllowedDtmfEvents0to15 ) )
    	{
    	return KErrArgument;
    	}
    
    return CMceComCodec::SetAllowedBitrates( aBitrates );
    }
       
// -----------------------------------------------------------------------------
// CMceComDtmfCodec::SetCodecMode
// -----------------------------------------------------------------------------
//
TInt CMceComDtmfCodec::SetCodecMode( TUint aCodecMode )
    {
    if ( EMceOutbound == aCodecMode || EMceInbound == aCodecMode )
        {
        iCodecMode = aCodecMode;
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }


// -----------------------------------------------------------------------------
// CMceComDtmfCodec::SetPayloadType
// -----------------------------------------------------------------------------
//
TInt CMceComDtmfCodec::SetPayloadType( TUint8 aPayloadType )
    {
    if ( KMinDynPayloadType <= aPayloadType )
        {
        return CMceComCodec::SetPayloadType( aPayloadType );
        }
    else
        {
        return KErrNotSupported;
        }
    }


// -----------------------------------------------------------------------------
// CMceComDtmfCodec::GetMaxBitRate
// -----------------------------------------------------------------------------
//
TUint CMceComDtmfCodec::GetMaxBitRate()
	{
    return 0;
    }   

// -----------------------------------------------------------------------------
// CMceComDtmfCodec::SetEnabled
// -----------------------------------------------------------------------------
//
void CMceComDtmfCodec::SetEnabled( TBool /*aIsEnabled*/ )
    {
    // Dtmf codec cannot be disabled
    }

// -----------------------------------------------------------------------------
// CMceComDtmfCodec::SendSupported
// -----------------------------------------------------------------------------
//
TBool CMceComDtmfCodec::SendSupported()
    {
    return EFalse;
    }

#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComDtmfCodec::DoMccValidateL
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceComDtmfCodec::DoMccValidateL( CMccCodecInformation& aMccCodec,
                                                CMceSrvStream& /*aStream*/,
                                                TMceNegotiationRole /*aRole*/ )
    {
    //if answerer mcc codec represents the received offer and codec the answer
    //if offerer codec represents the sent offer and mcc codec the answer
    
    TBool notValid = ( iAllowedBitrates && aMccCodec.AllowedBitrates() == 0 );
	
	User::LeaveIfError( notValid ? KErrNotSupported : KErrNone );
	
	return NULL;
    }

// -----------------------------------------------------------------------------
// CMceComDtmfCodec::DoDecodeAudioL
// -----------------------------------------------------------------------------
//

void CMceComDtmfCodec::DoDecodeAudioL( 
    TInt aCodecIndex,
    RPointerArray<CMceSrvStream>& aStreams,
    CMceComAudioStream& aAudio,
    CMceMediaManager& aManager )
    {
    // Special handling for dtmf codecs, need to create dummy source
    // for dtmf uplink and dummy sink for dtmf downlink
    //
    if ( IS_RECEIVESTREAM( &aAudio ) )
        {
        if ( aAudio.Source()->Type() == KMceRTPSource )
            {
            CMceComMediaSink& emptySink = aAudio.EmptySinkL();
            CMceSrvStream* srvStream = 
                CMceSrvStream::NewL( aManager, 
                                     aAudio, 
                                     *aAudio.Source(),
                                     emptySink,
                                     *this );
            CleanupStack::PushL( srvStream );
            MCEMM_DEBUG_STREAM( "CMceComDtmfCodec::DoDecodeAudioL(): decoded dtmf downlink", 
                                *srvStream );
            aStreams.AppendL( srvStream );
            CleanupStack::Pop( srvStream );
            }
        }
    else
        {
        CMceComMediaSink* rtpSink = NULL;
        for( TInt sinkNdx = 0; sinkNdx < aAudio.Sinks().Count() && !rtpSink; sinkNdx++ )
            {
            if ( aAudio.Sinks()[ sinkNdx ]->Type() == KMceRTPSink )
                {
                rtpSink = aAudio.Sinks()[ sinkNdx ];
                }
            }   
        if ( rtpSink )
            {
            CMceComMediaSource& emptySource = aAudio.EmptySourceL();
            CMceSrvStream* srvStream = 
                CMceSrvStream::NewL( aManager, 
                                     aAudio, 
                                     emptySource,
                                     *rtpSink,
                                     *this );
            CleanupStack::PushL( srvStream );
            MCEMM_DEBUG_STREAM( "CMceComDtmfCodec::DoDecodeAudioL(): decoded dtmf uplink", 
                                *srvStream );
            aStreams.AppendL( srvStream );
            CleanupStack::Pop( srvStream );
            }
        }
    
    // As dtmf cannot be used for streaming, select some other codec
    // for sending.    

    const TInt codecCount = aAudio.CodecCount();

    for ( TInt i = 0 ; i < codecCount ; ++i )
        {
        UpdateSendCodec( aCodecIndex, aAudio, *aAudio.CodecL( i ) );
        }
    }
    
#endif
    
// End of File
    
    
