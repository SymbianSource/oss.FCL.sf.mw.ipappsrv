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




#include "mceilbccodec.h"
#include "mcecomilbccodec.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmcccodecinformation.h>
#include "mcesrvstream.h"
#include "mcesrvsource.h"

#endif//MCE_COMMON_SERVER_SIDE

// Frame Size
const TUint KMceiLBCMode20FrameSize = 20;
const TUint KMceiLBCMode30FrameSize = 30;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceComiLBCCodec::NewL
// -----------------------------------------------------------------------------
//
CMceComiLBCCodec* CMceComiLBCCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComiLBCCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceComiLBCCodec::NewLC
// -----------------------------------------------------------------------------
//
CMceComiLBCCodec* CMceComiLBCCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComiLBCCodec* self = new (ELeave) CMceComiLBCCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComiLBCCodec::~CMceComiLBCCodec
// -----------------------------------------------------------------------------
//
CMceComiLBCCodec::~CMceComiLBCCodec()
    {
    }
    

// -----------------------------------------------------------------------------
// CMceComiLBCCodec::CMceComiLBCCodec
// -----------------------------------------------------------------------------
//
CMceComiLBCCodec::CMceComiLBCCodec()
 : CMceComAudioCodec()
    {
    iFourCC = 0;
    iEnableVAD = EFalse;
    iSamplingFreq  = KMceiLBCSamplingFreq;
    iBitrate  = KMceiLBCLevel30Bitrate;   
    iPTime = KMceiLBCDefaultPtime30;
    iMaxPTime = KMceiLBCDefaultMaxPtime;
    iAllowedBitrates = KMceAllowediLBCBitrate20| 
					   KMceAllowediLBCBitrate30;
    iPayloadType = KMceDefaultiLBCPayloadType;
    iCodecMode = KiLBCodecMode;					   
    iFrameSize = KMceiLBCMode30FrameSize;
    }


// -----------------------------------------------------------------------------
// CMceComiLBCCodec::ConstructL
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* CMceComiLBCCodec::CloneL()
    {
    CMceComiLBCCodec* copy = new (ELeave) CMceComiLBCCodec();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    
    }


// -----------------------------------------------------------------------------
// CMceComiLBCCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComiLBCCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec::ConstructL( aSdpName );
    }


// -----------------------------------------------------------------------------
// CMceComiLBCCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComiLBCCodec::ConstructL( CMceComiLBCCodec& aCodec )
    {
    CMceComAudioCodec::ConstructL( aCodec );
    }



// -----------------------------------------------------------------------------
// CMceComiLBCCodec::SetBitrate
// -----------------------------------------------------------------------------
//
TInt CMceComiLBCCodec::SetBitrate( TUint aBitrate )
    {
	TBool isValid = ETrue;
	
    switch ( aBitrate )
    	{
    	case KMceiLBCLevel20Bitrate:
    	case KMceiLBCLevel30Bitrate:
    	    {
    	    if ( aBitrate > GetMaxBitRate() )
    	        {
        		isValid = EFalse;
    	        }
    	    break;
    	    }
    	default:
    		isValid = EFalse;
    		break;
    	}
    	
    if ( isValid )
    	{
        return CMceComCodec::SetBitrate( aBitrate );
    	}
    else
    	{
    	return KErrNotSupported;
    	}
    
    }



// -----------------------------------------------------------------------------
// CMceComiLBCCodec::SetAllowedBitrates 
// -----------------------------------------------------------------------------
//

TInt CMceComiLBCCodec::SetAllowedBitrates( TUint aBitrates )
    {
      TInt error = KErrNone;
    
      if ( !aBitrates || 
        ( aBitrates >> 8 ) > 0 ) 
        {
        error = KErrArgument;
        }
    else
        {
        CMceComCodec::SetAllowedBitrates( KMceAllowediLBCBitrateAll );
        iAllowedBitrates &= aBitrates;
        TUint maxBitrate = GetMaxBitRate();
        if ( iBitrate > maxBitrate )
            {
            error = CMceComCodec::SetBitrate( maxBitrate );
            }
        }
    
    return error;
    
    }


// -----------------------------------------------------------------------------
// CMceComiLBCCodec::SetCodecMode
// -----------------------------------------------------------------------------
//
TInt CMceComiLBCCodec::SetCodecMode( TUint aCodecMode )
    {
    if ( KiLBCodecMode == aCodecMode )
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
// CMceComiLBCCodec::SetPayloadType
// -----------------------------------------------------------------------------
//
TInt CMceComiLBCCodec::SetPayloadType( TUint8 aPayloadType )
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
// CMceComiLBCCodec::GetMaxBitRate  
// -----------------------------------------------------------------------------
//
TUint CMceComiLBCCodec::GetMaxBitRate()
    {
    if( iAllowedBitrates & KMceAllowediLBCBitrate20 )
        {
        return KMceiLBCLevel20Bitrate;
        }
    else if( iAllowedBitrates & KMceAllowediLBCBitrate30 )
        {
        return KMceiLBCLevel30Bitrate;
        }
    else
        {
        return 0;    
        }
    }

// -----------------------------------------------------------------------------
// CMceComiLBCCodec::SetPTime
// Just basic validity checks can be done here. Valid configuration cannot be 
// assured at method level. Client is responsible for setting ptime, maxptime 
// and bitrate so that they are aligned with each other.
// -----------------------------------------------------------------------------
//
TInt CMceComiLBCCodec::SetPTime( TUint aPTime )
    {
    if ( ( aPTime % KMceiLBCMode20FrameSize ) == 0 ||
         ( aPTime % KMceiLBCMode30FrameSize ) == 0 )
        {
        return CMceComAudioCodec::SetPTime( aPTime );
        }
    else
        {
        return KErrNotSupported;
        }
    }


// -----------------------------------------------------------------------------
// CMceComiLBCCodec::SetMaxPTime
// -----------------------------------------------------------------------------
//
TInt CMceComiLBCCodec::SetMaxPTime( TUint aMaxPTime )
    {
    if ( ( aMaxPTime % KMceiLBCMode20FrameSize ) == 0 ||
         ( aMaxPTime % KMceiLBCMode30FrameSize ) == 0 )
        {
        return CMceComAudioCodec::SetMaxPTime( aMaxPTime );
        }
    else
        {
        return KErrNotSupported;
        }
    }


#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComiLBCCodec::DoMccEncodeL
// -----------------------------------------------------------------------------
//
void CMceComiLBCCodec::DoMccEncodeL( CMccCodecInformation& aMccCodec,
                                     CMceSrvStream& aStream )
    {
    CMceComAudioCodec::DoMccEncodeL( aMccCodec, aStream );
    
    // iLBC codec may ignore ptime setting if it fights against codec mode,
	// update MCE ptime to be one which is really used
	iPTime = aMccCodec.PTime();	
    }

// -----------------------------------------------------------------------------
// CMceComiLBCCodec::DoMccValidateL
// iLBC requires that both direction streams are in same mode.
// Role answerer: mcccodec represents the received offer and codec the answer.
// Role offerer: codec represents the sent offer and mcccodec the answer.
// Do not allow bitrate upgrade when answering.
// Ignore bitrate upgrade when offering.
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceComiLBCCodec::DoMccValidateL( CMccCodecInformation& aMccCodec,
                                                CMceSrvStream& aStream,
                                                TMceNegotiationRole aRole )
    {
    CMceComAudioCodec::DoMccValidateL( aMccCodec, aStream, aRole );
     
    TBool notValid = ( aRole == EMceRoleAnswerer && aMccCodec.Bitrate() < iBitrate ); 
	User::LeaveIfError( notValid ? KErrNotSupported : KErrNone );

    CMceComCodec* bidirectional = NULL;
    
    CMceComCodec& codec = aStream.Codec();
    if ( codec.Stream()->BoundStream() )
        {
        CMceComCodec* boundCodec = 
            codec.Stream()->BoundStreamL().FindCodecL( codec );
        
        bidirectional = 
            ( boundCodec && boundCodec->iBitrate != aMccCodec.Bitrate() ) ? boundCodec : NULL;
            
        if ( bidirectional )
            {
            if ( aRole == EMceRoleOfferer )
                {
                if ( bidirectional->iBitrate < aMccCodec.Bitrate() )
                    {
                    // Ignoring bitrate upgrade
                    __ASSERT_ALWAYS( bidirectional->iFmtpAttr, User::Leave( KErrNotReady ) );
                    SetFmtpAttributeL( *bidirectional->iFmtpAttr );
                    aMccCodec.SetFmtpAttrL( *bidirectional->iFmtpAttr );
                    }
                else
                    {
                    // Bitrate downgrade, NOP
                    }
                }
            else
                {
                __ASSERT_ALWAYS( iFmtpAttr, User::Leave( KErrNotReady ) );
                bidirectional->SetFmtpAttributeL( *iFmtpAttr );
                }
            }
        }
        
    return bidirectional;    
    }

// -----------------------------------------------------------------------------
// CMceComiLBCCodec::DoMccAdjustL
// -----------------------------------------------------------------------------
//
void CMceComiLBCCodec::DoMccAdjustL( CMccCodecInformation& aMccCodec,
                                     CMceSrvStream& aStream )
    {
    CMceComAudioCodec::DoMccAdjustL( aMccCodec, aStream );
		
    // iLBC codec may ignore ptime setting if it fights against codec mode,
	// update MCE ptime to be one which is really used
	iPTime = aMccCodec.PTime();	
    }
    
// -----------------------------------------------------------------------------
// CMceComiLBCCodec::DoSetDefaultFmtpAttributeL
// -----------------------------------------------------------------------------
//
void CMceComiLBCCodec::DoSetDefaultFmtpAttributeL()
    {
    SetFmtpAttributeL( KNullDesC8 );   
    }
    
#endif
        
// End of File

