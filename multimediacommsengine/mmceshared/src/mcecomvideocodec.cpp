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




#include "mcecomvideocodec.h"

#include "mceserial.h"
#include "mceevents.h"

#include "mcecomrtpsource.h"
#include "mcertpsource.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmcccodecinformation.h>
#include "mcesrvstream.h"
#include "mcesrvsource.h"


#endif//MCE_COMMON_SERVER_SIDE

// ============================ MEMBER FUNCTIONS ===============================

    
// -----------------------------------------------------------------------------
// CMceComVideoCodec::NewL
// -----------------------------------------------------------------------------
//
CMceComVideoCodec* CMceComVideoCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComVideoCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceComVideoCodec::NewLC
// -----------------------------------------------------------------------------
//
CMceComVideoCodec* CMceComVideoCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComVideoCodec* self = new (ELeave) CMceComVideoCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComVideoCodec::~CMceComAMRCodec
// -----------------------------------------------------------------------------
//
CMceComVideoCodec::~CMceComVideoCodec()
    {
    delete iConfigKey;
    }
    
// -----------------------------------------------------------------------------
// CMceComVideoCodec::CMceComVideoCodec
// -----------------------------------------------------------------------------
//
CMceComVideoCodec::CMceComVideoCodec()
    :CMceComCodec(),
     iFrameRate( 0 ),
     iMaxBitRate( 0 ),
     iAllowedResolutions( 0 ),
     iResolutionWidth( 0 ),
     iResolutionHeight( 0 ),
     iAllowedFrameRates( 0 ),
     iClockRate( 0 ),
     iReceiveFrameRate( 0 ),
     iEncodingDecodingDevice( KNullUid )
    {
    }


// -----------------------------------------------------------------------------
// CMceComVideoCodec::ConstructL
// -----------------------------------------------------------------------------
//
CMceComVideoCodec* CMceComVideoCodec::CloneL()
    {
    CMceComVideoCodec* copy = new (ELeave) CMceComVideoCodec();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    
    }


// -----------------------------------------------------------------------------
// CMceComVideoCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComVideoCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComCodec::ConstructL( aSdpName );
    }


// -----------------------------------------------------------------------------
// CMceComVideoCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComVideoCodec::ConstructL( CMceComVideoCodec& aCodec )
    {
    CMceComCodec::ConstructL( aCodec );
    
    iFrameRate = aCodec.iFrameRate;
    iMaxBitRate = aCodec.iMaxBitRate;
    iAllowedResolutions = aCodec.iAllowedResolutions;
    iResolutionWidth = aCodec.iResolutionWidth;
    iResolutionHeight = aCodec.iResolutionHeight;
    iAllowedFrameRates = aCodec.iAllowedFrameRates;
    iClockRate = aCodec.iClockRate;
    iReceiveFrameRate = aCodec.iReceiveFrameRate;
    
    SetConfigKeyL( aCodec.iConfigKey );
    }


// -----------------------------------------------------------------------------
// CMceComVideoCodec::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComVideoCodec::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComCodec::InternalizeFlatL( aReadStream );

    iFrameRate = aReadStream.ReadReal64L();
    iMaxBitRate = aReadStream.ReadUint32L();
    iAllowedResolutions = aReadStream.ReadUint32L();
    iResolutionWidth = aReadStream.ReadInt32L();
    iResolutionHeight = aReadStream.ReadInt32L();
    iAllowedFrameRates = aReadStream.ReadUint32L();
    iClockRate = aReadStream.ReadUint32L();
    iReceiveFrameRate = aReadStream.ReadReal64L();
    
    MceSerial::DecodeL( iConfigKey, aReadStream );
    
    iEncodingDecodingDevice.iUid = aReadStream.ReadUint32L();
    }
    
// -----------------------------------------------------------------------------
// CMceComVideoCodec::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComVideoCodec::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComCodec::ExternalizeFlatL( aWriteStream );
    
    aWriteStream.WriteReal64L( iFrameRate );
    aWriteStream.WriteUint32L( iMaxBitRate );
    aWriteStream.WriteUint32L( iAllowedResolutions );
    aWriteStream.WriteInt32L( iResolutionWidth );
    aWriteStream.WriteInt32L( iResolutionHeight );
    aWriteStream.WriteUint32L( iAllowedFrameRates );
    aWriteStream.WriteUint32L( iClockRate );
    aWriteStream.WriteReal64L( iReceiveFrameRate );
    
    MceSerial::EncodeL( iConfigKey, aWriteStream );
    
    aWriteStream.WriteUint32L( iEncodingDecodingDevice.iUid );
    }




// -----------------------------------------------------------------------------
// CMceComVideoCodec::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComVideoCodec::UpdateL( CMceComCodec& aCodec )
    {
    CMceComCodec::UpdateL( aCodec );

    CMceComVideoCodec& codec = static_cast<CMceComVideoCodec&>( aCodec );

    iMaxBitRate = codec.iMaxBitRate;
    iAllowedResolutions = codec.iAllowedResolutions;
    iAllowedFrameRates = codec.iAllowedFrameRates;
    
    if ( IS_RECEIVESTREAM( iStream ) )
        {
        iFrameRate = codec.iFrameRate;
        iResolutionWidth = codec.iResolutionWidth;
        iResolutionHeight = codec.iResolutionHeight;
        iClockRate = codec.iClockRate;
        }
    else
        {       
        SetConfigKeyL( codec.iConfigKey );
        }
        
    iEncodingDecodingDevice = codec.iEncodingDecodingDevice;
    }


// -----------------------------------------------------------------------------
// CMceComVideoCodec::Factory
// -----------------------------------------------------------------------------
//
TMceComVideoCodecFactory CMceComVideoCodec::Factory()
    {
    return TMceComVideoCodecFactory();
    }

// -----------------------------------------------------------------------------
// CMceComVideoCodec::SetMaxBitrate
// -----------------------------------------------------------------------------
//
TInt CMceComVideoCodec::SetMaxBitrate( TUint aMaxBitrate )
    {
    if ( aMaxBitrate )
        {
        iMaxBitRate = aMaxBitrate;
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

// -----------------------------------------------------------------------------
// CMceComVideoCodec::GetMaxBitRate
// -----------------------------------------------------------------------------
//
TUint CMceComVideoCodec::GetMaxBitRate()
    {
    return iMaxBitRate > 0 ? iMaxBitRate : iBitrate;
    }

// -----------------------------------------------------------------------------
// CMceComVideoCodec::CompareSdpIndex
// -----------------------------------------------------------------------------
//
TInt CMceComVideoCodec::CompareSdpIndex( 
    const CMceComVideoCodec& aIndex1, 
    const CMceComVideoCodec& aIndex2 )
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
// CMceComVideoCodec::SetConfigKeyL
// -----------------------------------------------------------------------------
//                                 
TBool CMceComVideoCodec::SetConfigKeyL( HBufC8* aConfigKey )
    {
    TBool updated( EFalse );
    if ( IsConfigKeyValid( aConfigKey ) )
        {
        // Set key only if none exists or new key differs from existing one
        if ( !iConfigKey || ( iConfigKey->Compare( *aConfigKey ) != 0 ) )
            { 
            HBufC8* configKey = aConfigKey->AllocL();
            delete iConfigKey;
            iConfigKey = configKey;
            updated = ETrue;
            }
        }
    return updated;
    }



// -----------------------------------------------------------------------------
// CMceComVideoCodec::IsConfigKeyValid
// -----------------------------------------------------------------------------
// 
TBool CMceComVideoCodec::IsConfigKeyValid() const
    {
    return IsConfigKeyValid( iConfigKey );
    }
    
// -----------------------------------------------------------------------------
// CMceComVideoCodec::IsConfigKeyValid
// -----------------------------------------------------------------------------
//    
TBool CMceComVideoCodec::IsConfigKeyValid( HBufC8* aConfigKey )
    {
    return ( aConfigKey && aConfigKey->Length() > 0 );
    }
    
#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComVideoCodec::operator=
// -----------------------------------------------------------------------------
//
/*lint -e1539 */

CMceComCodec& CMceComVideoCodec::operator=( CMccCodecInformation& aMccCodec )
    {

	iPayloadType = aMccCodec.PayloadType();
	iBitrate = aMccCodec.Bitrate();
	iAllowedBitrates = aMccCodec.AllowedBitrates();
	iCodecMode = aMccCodec.CodecMode();
	iFourCC = aMccCodec.FourCC();
	iFrameSize = aMccCodec.FrameSize();

    iFrameRate = aMccCodec.Framerate();
    iMaxBitRate = aMccCodec.MaxBitrate();
    iResolutionWidth = aMccCodec.FrameWidth();
    iResolutionHeight = aMccCodec.FrameHeight();
    iClockRate = aMccCodec.SamplingFreq();
    
    HBufC8* configKey = NULL;
    TRAPD( err, configKey = aMccCodec.ConfigKeyL() );
    if ( !err )
    	{
    	TRAP_IGNORE( SetConfigKeyL( configKey ) )	
    	}
    delete configKey;
    iEncodingDecodingDevice = aMccCodec.PreferredEncodingDecodingDevice();

    return *this;
    
    }


// -----------------------------------------------------------------------------
// CMceComVideoCodec::DoMccDecodeL
// -----------------------------------------------------------------------------
//
void CMceComVideoCodec::DoMccDecodeL( CMccCodecInformation& aMccCodec )
    {
    // copy values, which are not explicitely in SDP, from mcc codec
	iClockRate = aMccCodec.SamplingFreq();
    iResolutionWidth = aMccCodec.FrameWidth();
    iResolutionHeight = aMccCodec.FrameHeight();
    iMaxBitRate = aMccCodec.MaxBitrate();
    HBufC8* configKey = aMccCodec.ConfigKeyL();
    CleanupStack::PushL( configKey );
    SetConfigKeyL( configKey );
    CleanupStack::PopAndDestroy( configKey );
    iEncodingDecodingDevice = aMccCodec.PreferredEncodingDecodingDevice();	
    }

// -----------------------------------------------------------------------------
// CMceComVideoCodec::DoMccEncodeL
// -----------------------------------------------------------------------------
//
void CMceComVideoCodec::DoMccEncodeL( CMccCodecInformation& aMccCodec,
                                      CMceSrvStream& aStream )
    {
    User::LeaveIfError( aMccCodec.SetSamplingFreq( iClockRate ) );
    User::LeaveIfError( aMccCodec.SetFrameWidth( iResolutionWidth ) );
    User::LeaveIfError( aMccCodec.SetFrameHeight( iResolutionHeight ) );
	User::LeaveIfError( aMccCodec.SetMaxBitrate( iMaxBitRate ) );
    User::LeaveIfError( aMccCodec.SetAverageBitrate( iBitrate ) );
    User::LeaveIfError( aMccCodec.SetFramerate( iFrameRate ) );
    if ( IsConfigKeyValid() )
        {
        aMccCodec.SetConfigKeyL( *iConfigKey );
        }
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
    aMccCodec.SetPreferredEncodingDecodingDevice(iEncodingDecodingDevice);
    }


// -----------------------------------------------------------------------------
// CMceComVideoCodec::DoMccValidateL
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceComVideoCodec::DoMccValidateL( CMccCodecInformation& aMccCodec,
                                         CMceSrvStream& aStream,
                                         TMceNegotiationRole aRole )
    {	
    // TBD: validate frame width and height
    // Explicitly call default implementation of DoMccValidateL in base class
    // to enable the check for codecMode
    return CMceComCodec::DoMccValidateL( aMccCodec, aStream, aRole );
    }
    
// -----------------------------------------------------------------------------
// CMceComAudioCodec::DoMccRequireSignalling
// -----------------------------------------------------------------------------
//
TInt CMceComVideoCodec::DoMccRequireSignalling( 
    const CMceSrvStream& aStream,
    const CMccCodecInformation& aMccCurentCodec, 
    const CMccCodecInformation& aMccUpdateCodec ) const
    {
    TInt action = KMceNoSignalling;

    if ( aStream.StreamType() == CMceComMediaStream::EReceiveStream ||
         aStream.StreamType() == CMceComMediaStream::EReceiveOnlyStream )
        {
        if ( aMccCurentCodec.Framerate() != aMccUpdateCodec.Framerate() )
            {
            action = KMceRequiresSignalling;
            }
        }
    
    if ( aMccCurentCodec.FrameWidth() != aMccUpdateCodec.FrameWidth() ||
         aMccCurentCodec.FrameHeight() != aMccUpdateCodec.FrameHeight() )
        {
        action = KMceRequiresSignalling;
        }
        
    return action;
    
    }
    
// -----------------------------------------------------------------------------
// CMceComVideoCodec::DoMccAdjustL
// -----------------------------------------------------------------------------
//
void CMceComVideoCodec::DoMccAdjustL( CMccCodecInformation& aMccCodec,
                                      CMceSrvStream& aStream )
    {
	if( aStream.Data().iStreamType == CMceComMediaStream::ESendStream ||
        aStream.Data().iStreamType == CMceComMediaStream::ESendOnlyStream )
		{
		User::LeaveIfError( aMccCodec.SetFramerate( iFrameRate ) );
		User::LeaveIfError( aMccCodec.SetAverageBitrate( iBitrate ) );
		}
    User::LeaveIfError( aMccCodec.SetFrameWidth( iResolutionWidth ) );
    User::LeaveIfError( aMccCodec.SetFrameHeight( iResolutionHeight ) );
    }

// -----------------------------------------------------------------------------
// CMceComVideoCodec::DoSetDefaultFmtpAttributeL
// -----------------------------------------------------------------------------
//
void CMceComVideoCodec::DoSetDefaultFmtpAttributeL()
    {
    // NOP
    }
        
#endif// MCE_COMMON_SERVER_SIDE

