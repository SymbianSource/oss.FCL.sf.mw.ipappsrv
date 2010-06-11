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
* Description:    General MCC definitions.
*
*/




#ifndef MMCCINTERFACEDEF_H
#define MMCCINTERFACEDEF_H

// INCLUDES
#include <e32def.h>
#include <ecam.h>
#include <mmf/common/mmfbase.h>
#include <mmf/common/mmffourcc.h>
#include <mmccctrlobserver.h>
#include <mmccevents.h>
#include <mmcccodecinformation.h>


const TUid KMccInterfaceUid     = { 0x102073F7 };
const TUid KMccFileSinkUid      = { 0x102747D7 };
const TUid KMccFileSourceUid    = { 0x102747D5 };
const TUid KMccRtpSourceUid     = { 0x1020740B };
const TUid KMccRtpSinkUid       = { 0x1020740C };
const TUid KMccVideoSourceUid   = { 0x102747D6 };
const TUid KMccVideoSinkUid     = { 0x102747D8 };
const TUid KMccAnySourceUid     = { 0x10283394 };
const TUid KMccAnySinkUid       = { 0x10283395 };


const TUint32 KMccAudioToPrivate = 3;
const TUint32 KMccAudioToPublic = 4;
class TMMFPrioritySettings;

const TUint32 KMccNullId = 0xffffffff;

// Mcc link types

const TInt KMccLinkGeneral = 100;     
const TInt KMccLinkLocal = 101;    
const TInt KMccLinkGeneralVideo = 102;

// Mcc stream types
const TInt KMccAudioUplinkStream = 200;
const TInt KMccAudioDownlinkStream = 201;
const TInt KMccAudioLocalStream = 202;
const TInt KMccVideoUplinkStream = 203;
const TInt KMccVideoDownlinkStream = 204;
const TInt KMccVideoLocalStream = 205;
const TInt KMccAudioDownlinkStandbyStream = 206;
const TInt KMccDtmfStream = 207;

const TInt KMccMaxBundleSize = 40;     

// ENUMERATIONS

enum TMccDevice
    {
    EMccAudioPlay = 300,
    EMccAudioRecord
    };

enum TMccVideoRotation
	{
	EMccNone, 
	EMccClockwise90Degree, 
	EMccClockwise180Degree,
	EMccClockwise270Degree
	};
    
/**
* Specifies the media quality report
*/
enum TMediaQualityReport
    {
    EMccJitterReport = 0,
    EMccPacketLossReport,
    EMccQualityReportAll
    };

/**
* Specifies the media quality report interval type
*/
enum TMediaReportInterval
    {
    EMccQualityTimeBased = 0,
    EMccQualityPacketBased
    };

class TMccVideoSinkSetting
{
public:
    TMccVideoSinkSetting() : iLocation(0,0), iSize(0,0), iVideoPayloadType(0), 
        iAudioPayloadType(0), iIAPId(0), iDeviceIndex(0), iWindowOrdinalPosition(0),
        iWindowOrdinalPriority(0), iRotation(EMccNone), iVideoFrameRate(0.0), iEnabled(ETrue)
        {
        }
        
    TPoint iLocation;
    TSize iSize;
    TUint iVideoPayloadType;
    TUint iAudioPayloadType;
    TUint iIAPId;
    TUint iDeviceIndex;
    TMMFPrioritySettings iPriority;
	TInt iWindowOrdinalPosition;
	TInt iWindowOrdinalPriority;
	TMccVideoRotation iRotation;	
	TReal iVideoFrameRate;
	TBool iEnabled;
};

typedef TPckgBuf<TMccVideoSinkSetting> TMccVideoSinkSettingBuf;    

class TMccVideoSourceSetting
{
public:
    TBuf8<KMaxConfigKeyLen> iConfigKey;
    TSize iSize;
    TUint iVideoPayloadType;
    TUint iAudioPayloadType;
    TUint iDeviceIndex;
    TMMFPrioritySettings iPriority;
};

typedef TPckgBuf<TMccVideoSourceSetting> TMccVideoSourceSettingBuf;    

class TMccVideoInfo
{
public:
    TInt iNumOfCameras;
    TCameraInfo iInfo;
};

typedef TPckgBuf<TMccVideoInfo> TMccVideoInfoBuf;    

class TMccVideoInfo2 : public TMccVideoInfo
{
public:
    TInt iCameraIndex;
};

typedef TPckgBuf<TMccVideoInfo2> TMccVideoInfoBuf2;    


class TMccFileSourceSetting
{
public:

    TFileName iFileName;
    TTimeIntervalMicroSeconds iDuration;
    TTimeIntervalMicroSeconds iPosition;   
    TReal iVideoFrameRate;
    TUint32 iVideoBitRate;
    TSize iVideoFrameSize;
    TUint32 iAudioBitRate;
    
	// Audio fourCC value; KMMFFourCCCodeNULL if audio not present
    TFourCC iAudioFourCC;
    
	// Video fourCC value; KMMFFourCCCodeNULL if video not present
    TFourCC iVideoFourCC;

};

typedef TPckgBuf<TMccFileSourceSetting> TMccFileSourceSettingBuf;    

class TMccRtpSourceSetting
    {
    public:
        TUint32 iStandByTimerValue; 
    };

typedef TPckgBuf<TMccRtpSourceSetting> TMccRtpSourceSettingBuf;

/**
* Class for configuring the media quality reports
* 
* @lib N/A
* @since Series 60 3.0
*/
class TMccMediaQualityConfig
    {
public: // Constructor
    /**
    * Default constructor
    */
    inline TMccMediaQualityConfig() : iJitterLevel( 0 ), iPacketLoss( 0 ), 
        iReportType( EMccQualityReportAll ), 
        iReportIntervalType( EMccQualityTimeBased ), iReportInterval( 0 ),
        iSessionId( KMccNullId ), iEndpointId( 0 )
        {   };
    
    /**
    * Copy constructor
    */
    inline TMccMediaQualityConfig( const TMccMediaQualityConfig& aConfig ) : 
        iJitterLevel( aConfig.iJitterLevel ), iPacketLoss( aConfig.iPacketLoss ),
        iReportType( aConfig.iReportType ), 
        iReportIntervalType( aConfig.iReportIntervalType ),
        iReportInterval( aConfig.iReportInterval ), 
        iSessionId( aConfig.iSessionId ), 
        iEndpointId( aConfig.iEndpointId )
        {   };
        
public: // New functions
    /**
    * Checks the current configuration
    * @since Series 60 3.0
    * @param None
    * @return TInt System-wide error code
    */
    inline TInt CheckConfig() const
        {
        if( ( !iJitterLevel || !iPacketLoss ) && !iReportInterval )
            {
            return KErrArgument;
            }
        else if( KMccNullId == iSessionId )
            {
            return KErrArgument;
            }
        else
            {
            return KErrNone;
            }
        }; 
             
public: // Data
    // Jitterlevel to activate media quality report
    TInt iJitterLevel;
        
    // Packetloss percentage to activate media quality report
    TInt iPacketLoss;
        
    // Determines whether to report jitterlevel, packetloss or both
    TMediaQualityReport iReportType;
        
    // Determines wheter to report on a timebased interval or packet based 
    // interval
    TMediaReportInterval iReportIntervalType;
        
    // Determines the reporting interval, milliseconds or packet count
    TInt iReportInterval;
    
    // Determines the session from where to report media quality
    TUint32 iSessionId;
    
    // Endpoint
    TUint32 iEndpointId;
    };

typedef TPckgBuf<TMccMediaQualityConfig> TMccMediaQualConfBuf;

class TMccStreamBundle
{
public:
    TUint iType;
    TUint32 iStreams[ KMccMaxBundleSize ];
};

const TUint32 KMccCamZoomFactor = 1;
const TUint32 KMccCamDigitZoomFactor = 2;
const TUint32 KMccCamContrast = 3;
const TUint32 KMccCamBrightness = 4;
const TUint32 KMccCamWhiteBalance = 5;
const TUint32 KMccCamLocalVideo = 6;
const TUint32 KMccCamInfo = 7;
const TUint32 KMccCamCount = 8;
const TUint32 KMccCamExposure = 9;

const TUint32 KMccMicGain = 10;
const TUint32 KMccMicMaxGain = 11;
const TUint32 KMccMicBalance = 12;
const TUint32 KMccSpeakerVolume = 13;
const TUint32 KMccSpeakerMaxVolume = 14;
const TUint32 KMccSpeakerBalance = 15;
const TUint32 KMccSpeakerRouting = 16;

const TUint32 KMccDisplayLocalVideo = 17;

// File info retrieval, all possible info or specific values.
// Use TMccFileSourceSettingBuf as a parameter value.
//
const TUint32 KMccFileInfo = 18;
const TUint32 KMccFileName = 19;
const TUint32 KMccFileDuration = 20;
const TUint32 KMccFilePosition = 21;
const TUint32 KMccFileVideoFrameRate = 22;
const TUint32 KMccFileVideoBitRate = 23;
const TUint32 KMccFileVideoFrameSize = 24;
const TUint32 KMccFileAudioBitRate = 25;
const TUint32 KMccFileAudioFourCC = 26;
const TUint32 KMccFileVideoFourCC = 27;

// File playing fastforward/rewind.
// Use TPckgBuf<TInt> as a parameter value. Integer value defines
// forwarding/rewinding speed. E.g. value 4 causes four times
// faster playback and -2 causes two times faster rewinding.
// Returning to normal mode is achieved with 0 value.
const TUint32 KMccFileFastForward = 28;

const TUint32 KMccRtpCName = 29;

// Use TReal as a parameter value
const TUint32 KMccVideoFrameRate = 30;

// Use TBuf8<KMaxConfigKeyLen> as a parameter value
const TUint32 KMccConfigKey = 31;

#endif    // MMCCINTERFACEDEF_H
