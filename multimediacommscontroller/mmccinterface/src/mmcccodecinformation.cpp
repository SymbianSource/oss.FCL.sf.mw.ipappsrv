/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    MCC CodecInformation
*
*/




// INCLUDE FILES
#include <AudioPreference.h>
#include <delimitedpathsegment8.h>

#include "mmcccodecinformation.h"
#include "mccinternalcodecs.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

const TUint KDefaultSamplingFreq = 8000;
const TUint KDefaultBufferLength = 10;
const TUint KDefaultBufferThreshold = 5;
const TUint KDefaultPreference = EMdaPriorityPreferenceNone;
const TUint KDefaultPriority = EMdaPriorityNormal;
const TInt KDefaultModeChangePeriod = 1;
const TUint KMccSecsToMicroSecs = 1000000;


// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMccCodecInformation::CMccCodecInformation
// default constructor
// -----------------------------------------------------------------------------
//
CMccCodecInformation::CMccCodecInformation() :
    iType( KUidMediaTypeAudio ),
    iSamplingFreq( KDefaultSamplingFreq ),
    iMaxPTime( KMaxPtime ),
    iFmtpAttr( NULL ),
    iBitrateMask( 0 ),
    iRedCount( 0 ),
    iRedundancyPT( KMccPayloadTypeMax ),
    iJitterBufThreshold( KDefaultBufferThreshold ),
    iJitterBufBufferLength( KDefaultBufferLength ),
    iModeChangePeriod( KDefaultModeChangePeriod ),
    iNeighbor( EFalse ),
    iComfortNoiseGenerationPt( KPayloadTypeUndefined ),
    iPriority( KDefaultPriority ),
    iPriorityPreference( KDefaultPreference ),
    iAverageBitrate( 0 ),
    iMaxBitrate( 0 ),
    iFramerate( 0 ),
    iVideoWidth( 0 ),
    iVideoHeight( 0 ),
    iPayloadFormatEncoder( 0 ),
    iPayloadFormatDecoder( 0 ),
    iEncodingDecodingDevice( KNullUid )
    {
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::~CMccCodecInformation
// Destructor
// -----------------------------------------------------------------------------
//
CMccCodecInformation::~CMccCodecInformation()
    {
    delete iFmtpAttr;
    delete iConfigKey;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::SetValues
// -----------------------------------------------------------------------------
//
void CMccCodecInformation::SetValues( TMccCodecInfo codecInfo )
    {
    iType = codecInfo.iType;
    iSdpName = codecInfo.iSdpName;
    iSamplingFreq = codecInfo.iSamplingFreq;
    iBitrateMask = codecInfo.iBitrateMask;
    iJitterBufInactivityTimeOut = codecInfo.iJitterBufInactivityTimeOut;
    iJitterBufThreshold = codecInfo.iJitterBufThreshold;
    iJitterBufBufferLength = codecInfo.iJitterBufBufferLength;
    iModeChangePeriod = codecInfo.iModeChangePeriod;
    iNeighbor = codecInfo.iNeighbor;
    iComfortNoiseGenerationPt = codecInfo.iComfortNoiseGenerationPt;
    iPriority = codecInfo.iPriority.iPriority;
    iPriorityPreference = codecInfo.iPriority.iPref;
    iAverageBitrate = codecInfo.iAverageBitrate;
    iMaxBitrate = codecInfo.iMaxBitrate;
    iFramerate = codecInfo.iFramerate;
    iVideoWidth = codecInfo.iVideoWidth;
    iVideoHeight = codecInfo.iVideoHeight;
    iPayloadFormatEncoder = codecInfo.iPayloadFormatEncoder;
    iPayloadFormatDecoder = codecInfo.iPayloadFormatDecoder;
    
    iFourCC = codecInfo.iFourCC.FourCC();
    iAlgoUsed = codecInfo.iAlgoUsed;
    iRedCount = codecInfo.iRedundancyCount;
    iRedundancyPT = codecInfo.iRedundantPayload;
    SetBitrate( codecInfo.iBitrate ); // Some codecs have special behavior
    iCodecMode = codecInfo.iCodecMode;
    iEnableVAD = codecInfo.iEnableDTX;
    iMaxPTime = codecInfo.iMaxPtime;
    iPTime = codecInfo.iPtime;
    iPayloadType = codecInfo.iPayloadType;
    iFrameSize = codecInfo.iFrameSize;
    iHwFrameTime = codecInfo.iHwFrameTime;
    
    if( (codecInfo.iKeepaliveInterval / KMccSecsToMicroSecs) > KMaxTUint8 )
    	{
    	iKeepAliveTimer = KMaxTUint8;
    	iKeepAliveTimerMicroSecs = KMaxTUint8 * KMccSecsToMicroSecs;	
    	}
    else
    	{
    	iKeepAliveTimer = codecInfo.iKeepaliveInterval / KMccSecsToMicroSecs;
    	iKeepAliveTimerMicroSecs = codecInfo.iKeepaliveInterval;	
    	}
    iKeepAlivePT = codecInfo.iKeepalivePT;
    iKeepAliveData = codecInfo.iKeepaliveData;
    
    iMaxRed = codecInfo.iMaxRed;
    
    if ( codecInfo.iConfigKey != KNullDesC8 )
        {
        TRAP_IGNORE( SetConfigKeyL( codecInfo.iConfigKey ) )
        }
    iEncodingDecodingDevice = codecInfo.iEncodingDecodingDevice;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecInformation::GetValues
// -----------------------------------------------------------------------------
//
void CMccCodecInformation::GetValues( TMccCodecInfo& codecInfo ) const
    {
    codecInfo.iType = iType;
    codecInfo.iSdpName = iSdpName;
    codecInfo.iSamplingFreq = iSamplingFreq;
    codecInfo.iBitrateMask = iBitrateMask;
    codecInfo.iJitterBufInactivityTimeOut = iJitterBufInactivityTimeOut;
    codecInfo.iJitterBufThreshold = iJitterBufThreshold;
    codecInfo.iJitterBufBufferLength = iJitterBufBufferLength;
    codecInfo.iPriority.iPriority = iPriority;
    codecInfo.iPriority.iPref = TMdaPriorityPreference( iPriorityPreference );
    codecInfo.iAverageBitrate = iAverageBitrate;
    codecInfo.iMaxBitrate = iMaxBitrate;
    codecInfo.iFramerate = iFramerate;
    codecInfo.iComfortNoiseGenerationPt = iComfortNoiseGenerationPt;
    codecInfo.iVideoWidth = iVideoWidth;
    codecInfo.iVideoHeight = iVideoHeight;
    codecInfo.iPayloadFormatEncoder = iPayloadFormatEncoder;
    codecInfo.iPayloadFormatDecoder = iPayloadFormatDecoder;

    codecInfo.iFourCC = TFourCC( iFourCC );
    codecInfo.iAlgoUsed = iAlgoUsed;
    codecInfo.iRedundancyCount = iRedCount;
    codecInfo.iRedundantPayload = iRedundancyPT;
    codecInfo.iBitrate = iBitrate;
    codecInfo.iCodecMode = iCodecMode;
    codecInfo.iEnableDTX = iEnableVAD;
    codecInfo.iMaxPtime = iMaxPTime;
    codecInfo.iPtime = iPTime;
    codecInfo.iPayloadType = iPayloadType;
    codecInfo.iFrameSize = iFrameSize;
    codecInfo.iHwFrameTime = iHwFrameTime;
    codecInfo.iRedundantPayload = iRedundancyPT;
    codecInfo.iNeighbor = iNeighbor;
    codecInfo.iModeChangePeriod = iModeChangePeriod;
    codecInfo.iKeepaliveInterval = iKeepAliveTimerMicroSecs;
    codecInfo.iKeepalivePT = iKeepAlivePT;
    codecInfo.iKeepaliveData = iKeepAliveData;
    codecInfo.iMaxRed = iMaxRed;
    
    if ( iConfigKey && iConfigKey->Length() <= KMaxConfigKeyLen )
        {
        codecInfo.iConfigKey.Copy( *iConfigKey );
        }
        
    codecInfo.iEncodingDecodingDevice = iEncodingDecodingDevice;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecInformation::RequireSignalling
// -----------------------------------------------------------------------------
//
TBool CMccCodecInformation::RequireSignalling(
    const CMccCodecInformation& /*aCandidate*/ ) const
    {
    return EFalse;    
    }  

// -----------------------------------------------------------------------------
// CMccCodecInformation::Type
// -----------------------------------------------------------------------------
//
TUid CMccCodecInformation::Type() const
    {
    return iType;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::VAD
// -----------------------------------------------------------------------------
//
TBool CMccCodecInformation::VAD() const
    {
    return iEnableVAD;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::Bitrate
// -----------------------------------------------------------------------------
//
TUint CMccCodecInformation::Bitrate() const
   {
   return iBitrate;
   }

// -----------------------------------------------------------------------------
// CMccCodecInformation::SamplingFreq
// Gets the sampling frequency. 
// -----------------------------------------------------------------------------
//
TUint32 CMccCodecInformation::SamplingFreq() const
    {
    return iSamplingFreq;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::SdpName
// Gets the codec name.
// -----------------------------------------------------------------------------
//
const TDesC8& CMccCodecInformation::SdpName() const
    {
    return iSdpName;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::CodecMode
// Gets the codec mode e.g. uLaw/aLaw.
// -----------------------------------------------------------------------------
//
TCodecMode CMccCodecInformation::CodecMode() const
   {
   return iCodecMode;
   }

// -----------------------------------------------------------------------------
// CMccCodecInformation::PayloadType
// Gets the payloadtype.
// -----------------------------------------------------------------------------
//
TUint8 CMccCodecInformation::PayloadType() const
    {
    return iPayloadType;
    }
        
// -----------------------------------------------------------------------------
// CMccCodecInformation::PTime
// Gets the packet time.
// -----------------------------------------------------------------------------
//
TInt CMccCodecInformation::PTime() const
    {
    return iPTime;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecInformation::MaxPTime
// Gets the max packet time.
// -----------------------------------------------------------------------------
//
TInt CMccCodecInformation::MaxPTime() const
    {
    return iMaxPTime;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::GetFmtpL
// Gets the fmtp attribute
// -----------------------------------------------------------------------------
//
TDesC8& CMccCodecInformation::GetFmtpL()
    {
    if( !iFmtpAttr )
        {
        iFmtpAttr = HBufC8::NewL(0);
        }
    
    return *iFmtpAttr;
    }


// -----------------------------------------------------------------------------
// CMccCodecInformation::CreateFmtpAttrListL
// If fmtp does not exist, then create it from internal values
// -----------------------------------------------------------------------------
//   
void CMccCodecInformation::CreateFmtpAttrListL()
    {
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::CreateFmtpAttrListL
// If fmtp does not exist, then create it from internal values and 
// the bitrate array
// -----------------------------------------------------------------------------
//        
void CMccCodecInformation::CreateFmtpAttrListL( const RArray<TUint>&  /*aBitrates*/ )
    {
    }

  
// -----------------------------------------------------------------------------
// CMccCodecInformation::SetFmtpAttrL
// Gets the fmtp attribute
// -----------------------------------------------------------------------------
//
void CMccCodecInformation::SetFmtpAttrL( const TDesC8& aFmtp, TBool parseFmtp )
    {
    if ( iFmtpAttr )
        {
        delete iFmtpAttr;
        iFmtpAttr = NULL;
        }

    iFmtpAttr = HBufC8::NewL( aFmtp.Length() );
    iFmtpAttr->Des().Copy( aFmtp );
           
    //Check if the fmtp string should be parsed
    if( parseFmtp )
        {        
        ParseFmtpAttrL( *iFmtpAttr );    
        }
    }
   
// -----------------------------------------------------------------------------
// CMccCodecInformation::ParseFmtpAttrL
// Parse the fmtp string, but doesn't update the iFmtpAttr
// -----------------------------------------------------------------------------
//
TBool CMccCodecInformation::ParseFmtpAttrL( const TDesC8& /*aFmtp*/ )
    {
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CMccCodecInformation::SetAllowedBitrates
// Gets Codec FourCC
// -----------------------------------------------------------------------------
//
TInt CMccCodecInformation::SetAllowedBitrates( TUint /*aBitrateMask*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::AllowedBitrates
// Gets Codec FourCC
// -----------------------------------------------------------------------------
//
TUint CMccCodecInformation::AllowedBitrates() const
    {
    return iBitrateMask;
    }
 
// -----------------------------------------------------------------------------
// CMccCodecInformation::FourCC
// Gets Codec FourCC
// -----------------------------------------------------------------------------
//
TUint32 CMccCodecInformation::FourCC( ) const
    {
    return iFourCC;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecInformation::FrameTime
// Gets Frametime (ms).
// -----------------------------------------------------------------------------
//
TInt CMccCodecInformation::FrameTime() const
    {
    return iHwFrameTime;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecInformation::FrameSize
// Gets FrameSize.
// -----------------------------------------------------------------------------
//
TInt CMccCodecInformation::FrameSize() const
    {
    return iFrameSize;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecInformation::SetAlgo
// Sets Algorithm.
// -----------------------------------------------------------------------------
//
TInt CMccCodecInformation::SetAlgo( TAlgoUsed aAlgo )
    {
    iAlgoUsed = aAlgo;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::Algo
// Gets used algorithm.
// -----------------------------------------------------------------------------
//
TAlgoUsed CMccCodecInformation::Algo( ) const
    {
    return iAlgoUsed;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecInformation::SetRedCount
// Sets Redundancy Count.
// -----------------------------------------------------------------------------
//
TInt CMccCodecInformation::SetRedCount( TUint aRedCount )
    {
    if ( KMaxGenRedCount < aRedCount )
        {
        return KErrArgument;
        }
    
    TAlgoUsed algo = aRedCount > 0 ? EGenRedUsed : ENoAlgoUsed;
    SetAlgo( algo );

    iRedCount = aRedCount;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::RedCount
// Gets Redundancy Count.
// -----------------------------------------------------------------------------
//
TUint CMccCodecInformation::RedCount( ) const
    {
    return iRedCount;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::SetRedundancyPT
// Sets Redundancy PayloadType.
// -----------------------------------------------------------------------------
//
TInt CMccCodecInformation::SetRedundancyPT( TUint8 aRedPT )
    {
    iRedundancyPT = aRedPT;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::RedundancyPT
// Gets Redundancy PayloadType.
// -----------------------------------------------------------------------------
//
TUint8 CMccCodecInformation::RedundancyPT( ) const
    {
    return iRedundancyPT;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::SetJitterBufInactivityTimeOut
// Set JitterBuffer Timeout time
// -----------------------------------------------------------------------------
//
TInt CMccCodecInformation::SetJitterBufInactivityTimeOut( TUint aJitterBufInactivityTimeOut )
    {
    iJitterBufInactivityTimeOut = aJitterBufInactivityTimeOut;
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::JitterBufInactivityTimeOut
// Get JitterBuffer timeout time
// -----------------------------------------------------------------------------
//
TUint CMccCodecInformation::JitterBufInactivityTimeOut() const
    {
    return iJitterBufInactivityTimeOut;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::SetJitterBufThreshold
// Set JitterBuffer treshold
// -----------------------------------------------------------------------------
//
TInt CMccCodecInformation::SetJitterBufThreshold( TUint aJitterBufThreshold )
    {
    iJitterBufThreshold = aJitterBufThreshold;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::JitterBufThreshold
// Get JitterBuffer threshold
// -----------------------------------------------------------------------------
//
TUint CMccCodecInformation::JitterBufThreshold() const
    {
    return iJitterBufThreshold;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::SetJitterBufBufferLength
// Set JitterBuffer Buffer length
// -----------------------------------------------------------------------------
//
TInt CMccCodecInformation::SetJitterBufBufferLength( TUint aJitterBufBufferLength )
    {
    if( aJitterBufBufferLength == 0 || aJitterBufBufferLength < iJitterBufThreshold )
        {
        return KErrUnderflow;
        }
    
    else
        {
        iJitterBufBufferLength = aJitterBufBufferLength;
        return KErrNone;
        }
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::JitterBufBufferLength
// Get JitterBuffer Buffer length
// -----------------------------------------------------------------------------
//
TUint CMccCodecInformation::JitterBufBufferLength() const
    {
    return iJitterBufBufferLength;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::SetModeChangePeriod
// Default implementation
// -----------------------------------------------------------------------------
//
TInt CMccCodecInformation::SetModeChangePeriod( const TInt /*aPeriod*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::ModeChangePeriod
// Default implementation
// -----------------------------------------------------------------------------
//
TInt CMccCodecInformation::ModeChangePeriod( ) const
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::SetModeChangeNeighbor
// Default implementation
// -----------------------------------------------------------------------------
//
TInt CMccCodecInformation::SetModeChangeNeighbor( const TBool /*aNeighbor*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::ModeChangeNeighbor
// Default implementation
// -----------------------------------------------------------------------------
//
TBool CMccCodecInformation::ModeChangeNeighbor( ) const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::SetCrc
// Default implementation
// -----------------------------------------------------------------------------
//  
TInt CMccCodecInformation::SetCrc( const TBool /*aCrc*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::GetCrc
// Default implementation
// -----------------------------------------------------------------------------
// 
TBool CMccCodecInformation::GetCrc( ) const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::SetRobustSorting
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::SetRobustSorting( const TBool /*aRobust*/)
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::GetRobustSorting
// Default implementation
// -----------------------------------------------------------------------------
// 
TBool CMccCodecInformation::GetRobustSorting( ) const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::SetInterleaving
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::SetInterleaving( const TInt /*aFrameBlockCount*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:GetInterleaving
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::GetInterleaving( ) const
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::SetChannels
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::SetChannels( TInt /*aNumOfChannels*/ )
    {
    return KErrNotSupported;
    };

// -----------------------------------------------------------------------------
// CMccCodecInformation:SetMaxRed
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::SetMaxRed( TInt /*aMaxRed*/ )
    {
    return KErrNotSupported;
    }    

// -----------------------------------------------------------------------------
// CMccCodecInformation:SetConfigKeyL
// -----------------------------------------------------------------------------
// 
void CMccCodecInformation::SetConfigKeyL( const TDesC8& /*aConfigKey*/ )
    {
    User::Leave( KErrNotSupported );
    }
    
// -----------------------------------------------------------------------------
// CMccCodecInformation:SetPreferredEncodingDecodingDevice
// -----------------------------------------------------------------------------
// 
void CMccCodecInformation::SetPreferredEncodingDecodingDevice( 
											TUid /*aEncodingDecodingDevice*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// CMccCodecInformation:GetChannels
// Return array of allowed modes
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::GetChannels( TInt& /*aNumOfChannels*/ ) const
    {
    return KErrNotSupported;
    };

// -----------------------------------------------------------------------------
// CMccCodecInformation:SetComfortNoiseGeneration
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::SetComfortNoiseGeneration( TUint8 /*aComfortNoisePT*/ )
    {
    return KErrNotSupported;
    };
 
        
// -----------------------------------------------------------------------------
// CMccCodecInformation:ComfortNoiseGeneration
// Default implementation
// -----------------------------------------------------------------------------
// 
TUint8 CMccCodecInformation::ComfortNoiseGeneration() const
    {
    return iComfortNoiseGenerationPt;
    };


// -----------------------------------------------------------------------------
// CMccCodecInformation:MaxRed
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::MaxRed() const
    {
    return iMaxRed;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:RedPayloadsL
// -----------------------------------------------------------------------------
//
void CMccCodecInformation::RedPayloadsL( RArray<TUint>& /*aRedPayloads*/ ) const
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation::ConfigKeyL
// -----------------------------------------------------------------------------
//
HBufC8* CMccCodecInformation::ConfigKeyL() const
    {
    HBufC8* key = NULL;
    if ( iConfigKey )
        {
        key = iConfigKey->AllocL();
        }
    return key;
    }
    
 // -----------------------------------------------------------------------------
// CMccCodecInformation::PreferredEncodingDecodingDevice
// -----------------------------------------------------------------------------
//
TUid CMccCodecInformation::PreferredEncodingDecodingDevice() const
    {
    return iEncodingDecodingDevice;
    }   
    
// -----------------------------------------------------------------------------
// CMccCodecInformation:GetPriority
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::Priority( ) const
    {
    return iPriority;        
    }
    
// -----------------------------------------------------------------------------
// CMccCodecInformation:SetPriority
// Default implementation
// -----------------------------------------------------------------------------
// 
void CMccCodecInformation::SetPriority( TInt aPriority )
    {
    iPriority = aPriority;        
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:GetPriorityPreference
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::PriorityPreference( ) const
    {
    return iPriorityPreference;        
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:SetPriorityPreference
// Default implementation
// -----------------------------------------------------------------------------
// 
void CMccCodecInformation::SetPriorityPreference(TInt aPreference )
    {
    iPriorityPreference = aPreference;    
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:SetMaxBitrate
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::SetMaxBitrate( TUint aMaxBitrate )
    {
    iMaxBitrate = aMaxBitrate;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:SetAverageBitrate
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::SetAverageBitrate( TUint aAverageBitrate )
    {
    iAverageBitrate = aAverageBitrate;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:SetFramerate
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::SetFramerate( TReal aFramerate )
    {
    iFramerate = aFramerate;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:SetFrameHeight
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::SetFrameHeight( TUint aFrameHeight )
    {
    iVideoHeight = aFrameHeight;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:SetFrameWidth
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::SetFrameWidth( TUint aFrameWidth )
    {
    iVideoWidth = aFrameWidth;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:MaxBitrate
// Default implementation
// -----------------------------------------------------------------------------
// 
TUint CMccCodecInformation::MaxBitrate() const
    {    
    return iMaxBitrate;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:AverageBitrate
// Default implementation
// -----------------------------------------------------------------------------
// 
TUint CMccCodecInformation::AverageBitrate() const
    {
    return iAverageBitrate;    
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:Framerate
// Default implementation
// -----------------------------------------------------------------------------
// 
TReal CMccCodecInformation::Framerate() const
    {
    return iFramerate;    
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:FrameHeight
// Default implementation
// -----------------------------------------------------------------------------
// 
TUint CMccCodecInformation::FrameHeight() const
    {
    return iVideoHeight;    
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:FrameWidth
// Default implementation
// -----------------------------------------------------------------------------
// 
TUint CMccCodecInformation::FrameWidth() const
    {
    return iVideoWidth;        
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:SetKeepAliveTimer
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::SetKeepAliveTimer( TUint8 aTimerValue )
    {
    iKeepAliveTimer = aTimerValue;
    iKeepAliveTimerMicroSecs = aTimerValue * KMccSecsToMicroSecs;
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:SetKeepAlivePT
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::SetKeepAlivePT( TUint8 aKeepAlivePT )
    {
    iKeepAlivePT = aKeepAlivePT;
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:SetKeepAliveData
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::SetKeepAliveData( const TDesC8& aData )
    {
	if ( aData.Length() > KMaxKeepAliveDataLen )
	    {
	    return KErrArgument;
	    }
	
    iKeepAliveData = aData;
    return KErrNone;        
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:KeepAliveTimer
// Default implementation
// -----------------------------------------------------------------------------
// 
TUint8 CMccCodecInformation::KeepAliveTimer() const
    {
    return iKeepAliveTimer;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:KeepAlivePT
// Default implementation
// -----------------------------------------------------------------------------
// 
TUint8 CMccCodecInformation::KeepAlivePT() const
    {
    return iKeepAlivePT;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:KeepAliveData
// Default implementation
// -----------------------------------------------------------------------------
// 
const TDesC8& CMccCodecInformation::KeepAliveData() const
    {
    return iKeepAliveData;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:SetKeepAliveTimerMicroSecs
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecInformation::SetKeepAliveTimerMicroSecs( 
									TUint32 aTimerValueMicroSecs )
    {
    if( (aTimerValueMicroSecs / KMccSecsToMicroSecs) > KMaxTUint8 )
    	{
    	iKeepAliveTimer = KMaxTUint8;
    	iKeepAliveTimerMicroSecs = KMaxTUint8 * KMccSecsToMicroSecs;	
    	}
    else
    	{
    	iKeepAliveTimer = aTimerValueMicroSecs / KMccSecsToMicroSecs;	
    	iKeepAliveTimerMicroSecs = aTimerValueMicroSecs;
    	}
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CMccCodecInformation:KeepAliveTimerMicroSecs
// Default implementation
// -----------------------------------------------------------------------------
// 
TUint32 CMccCodecInformation::KeepAliveTimerMicroSecs() const
    {
    return iKeepAliveTimerMicroSecs;
    }
