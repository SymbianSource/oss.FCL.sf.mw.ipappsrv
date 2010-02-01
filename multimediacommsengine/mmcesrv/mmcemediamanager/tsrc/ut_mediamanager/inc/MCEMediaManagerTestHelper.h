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




#ifndef __MCEMEDIAMANAGERTESTHELPER_H__
#define __MCEMEDIAMANAGERTESTHELPER_H__

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <e32const.h>

//  INTERNAL INCLUDES
#include "mcesrv.h"
#include "mcemediaobserver.h"
#include "mceevents.h"

#include "mcecomsession.h"
#include "mcecomaudiostream.h"
#include "mcecomaudiocodec.h"
#include "mcecomamrcodec.h"
#include "mcecomg711codec.h"
#include "mcecommicsource.h"
#include "mcecomspeakersink.h"
#include "mcecomrtpsource.h"
#include "mcecomrtpsink.h"
#include "mcecomvideostream.h"
#include "mcecomvideocodec.h"
#include "mcecomcamerasource.h"
#include "mcecomdisplaysink.h"


#include "mceaudiostream.h"
#include "mcemicsource.h"
#include "mcespeakersink.h"
#include "mcertpsource.h"
#include "mcemicsource.h"
#include "mcertpsink.h"
#include "mceamrcodec.h"
#include "mceg711codec.h"
#include "mceh263codec.h"

#include "mcevideostream.h"
#include "mcecamerasource.h"
#include "mcedisplaysink.h"

#include "mcemediadefs.h"
#include "mcesdpsession.h"

//  FORWARD DECLARATIONS



_LIT8( KMceMMTestSdpOfferAudioIMS5PrecsSendrecv, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
m=audio 5078 RTP/AVP 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");

_LIT8( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv, "v=0\r\n\
o=usera 63329854699050000 63329854699050000 IN IP4 10.20.180.74\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.74\r\n\
t=0 0\r\n\
m=audio 5000 RTP/AVP 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote sendrecv\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos mandatory remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");

_LIT8( KMceMMTestSdpAnswerAudioIMS5PrecsRecvonly, "v=0\r\n\
o=usera 63329854699050000 63329854699050000 IN IP4 10.20.180.74\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.74\r\n\
t=0 0\r\n\
m=audio 5000 RTP/AVP 96\r\n\
a=recvonly\r\n\
a=curr:qos local recvonly\r\n\
a=curr:qos remote recvonly\r\n\
a=des:qos mandatory local recvonly\r\n\
a=des:qos mandatory remote recvonly\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");


_LIT8( KMceMMTestSdpOfferVideoIMS5PrecsSendrecv, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
m=video 5078 RTP/AVP 97\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:97 H263-2000/90000\r\n\
a=fmtp:97 profile=0;level=10\r\n");

_LIT8( KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv, "v=0\r\n\
o=usera 1234567890 1234567890 IN IP4 10.20.180.74\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.74\r\n\
t=0 0\r\n\
m=video 5000 RTP/AVP 97\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos mandatory remote sendrecv\r\n\
a=rtpmap:97 H263-2000/90000\r\n\
a=fmtp:97 profile=0;level=10\r\n");

_LIT8( KMceMMTestSdpAnswer2VideosIMS5PrecsSendrecv, "v=0\r\n\
o=usera 1234567890 1234567890 IN IP4 10.20.180.74\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.74\r\n\
t=0 0\r\n\
m=video 5000 RTP/AVP 97\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos mandatory remote sendrecv\r\n\
a=rtpmap:97 H263-2000/90000\r\n\
a=fmtp:97 profile=0;level=10\r\n\
m=video 5002 RTP/AVP 107\r\n\
a=sendonly\r\n\
a=curr:qos local sendonly\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendonly\r\n\
a=des:qos mandatory remote sendonly\r\n\
a=rtpmap:107 H263-2000/90000\r\n\
a=fmtp:107 profile=0;level=10\r\n");

_LIT8( KMceMMTestSdpAnswerAudio2VideosIMS5PrecsSendrecv, "v=0\r\n\
o=usera 1234567890 1234567890 IN IP4 10.20.180.74\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.74\r\n\
t=0 0\r\n\
m=audio 5078 RTP/AVP 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote sendrecv\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n\
m=video 5000 RTP/AVP 97\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos mandatory remote sendrecv\r\n\
a=rtpmap:97 H263-2000/90000\r\n\
a=fmtp:97 profile=0;level=10\r\n\
m=video 5002 RTP/AVP 107\r\n\
a=sendonly\r\n\
a=curr:qos local sendonly\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendonly\r\n\
a=des:qos mandatory remote sendonly\r\n\
a=rtpmap:107 H263-2000/90000\r\n\
a=fmtp:107 profile=0;level=10\r\n");


_LIT8( KMceMMTestSdpOfferAudioVideoIMS5PrecsSendrecv, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
m=audio 5078 RTP/AVP 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote sendrecv\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n\
m=video 5080 RTP/AVP 97\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:97 H263-2000/90000\r\n\
a=fmtp:97 profile=0;level=10\r\n");

_LIT8( KMceMMTestSdpAnswerAudioVideoIMS5PrecsSendrecv, "v=0\r\n\
o=usera 1234567890 1234567890 IN IP4 10.20.180.74\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.74\r\n\
t=0 0\r\n\
m=audio 5000 RTP/AVP 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote sendrecv\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos mandatory remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n\
m=video 5080 RTP/AVP 97\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos mandatory remote sendrecv\r\n\
a=rtpmap:97 H263-2000/90000\r\n\
a=fmtp:97 profile=0;level=10\r\n");

_LIT8( KMceMMTestSdpUnKnownQoSParameter, "v=0\r\n\
o=usera 1234567890 1234567890 IN IP4 10.20.180.74\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.74\r\n\
t=0 0\r\n\
m=audio 5000 RTP/AVP 100\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qosX local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qosX mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:100 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:100 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");


_LIT8( KMceMMTestSdpUnKnownSecParameter, "v=0\r\n\
o=usera 1234567890 1234567890 IN IP4 10.20.180.74\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.74\r\n\
t=0 0\r\n\
m=audio 5000 RTP/AVP 100\r\n\
a=sendrecv\r\n\
a=curr:secX local sendrecv\r\n\
a=des:secX mandatory local sendrecv\r\n\
a=rtpmap:100 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:100 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");



const TInt KH263FormatModeNew( 20 );
const TInt KH263DefaultAllowedBitrate( 1 );


const TUint KMceMMTestAudioLocalPort = 5006;
const TUint KMceMMTestVideoLocalPort = 5008;
const TUint KMceMMTestAudioPayloadType = 96;
const TUint KMceMMTestVideoPayloadType = 97;
const TUint KMceMMTestVideoPayloadTypeExtra = 107;
const TUint KMceMMTestAudioSamplingFreq = 8000;
const TUint KMceMMTestVideoClockrate = 90000;
const TAmrCodecMode KMceMMTestAudioCodecMode = EMceBandwidthEfficient;
const TUint KMceMMTestVideoCodecMode = KH263FormatModeNew;
const TUint KMceMMTestAudioAllowedBitrates = KMceAllowedAmrNbBitrateAll;
const TUint KMceMMTestVideoAllowedBitrates = KH263DefaultAllowedBitrate;
const TUint KMceMMTestAudioBitrate = KMceAmrNbBitrate795;
const TUint KMceMMTestVideoBitrate = 64000;

const TUint KMceMMTestAudioPTime = 60;
const TUint KMceMMTestAudioMaxPTime = 120;
const TUint KMceMMTestDefaultBufferLength = 10;
const TUint KMceMMTestDefaultBufferThreshold = 5;


#define MCE_MCC_EVENT_POSTCONDITION_L if ( iServer && iServer->iEvent == EMceMediaError )\
User::Leave( KErrNoMemory );

#define MCE_MMTEST_RTP_MAP_L( rtpmap, medialine )\
    if ( medialine )\
        {\
        RPointerArray< CSdpFmtAttributeField >& formatLines =  \
                   medialine->FormatAttributeFields();\
        for ( TInt i = 0; i < formatLines.Count(); i++ )\
        	{\
        	CSdpFmtAttributeField* formatLine = formatLines[ i ];\
            if ( formatLine->Attribute() == \
                        SDP_STRINGL( SdpCodecStringConstants::EAttributeRtpmap ) )\
                {\
                rtpmap = formatLine;\
                break;\
                }\
        	}\
        }


#define MCE_LEAVE_HANDLING( func, oomMappedError ) {\
TRAPD( mceUnitTestLeaveError, func );\
if ( mceUnitTestLeaveError == KErrNoMemory || mceUnitTestLeaveError == oomMappedError )\
    {\
    User::Leave( KErrNoMemory );\
    }\
}

class CSdpDocument;
class CMceServerStub;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class MceMediaManagerTestHelper
    {
    public: //static API
    

     static CMceComSession* CreateInSessionL();
    
     static CMceComSession* CreateAudioOutSessionL( CMceComSession* aSession = NULL,
                                                    const TDesC8& aFmtp = KNullDesC8,
                                                    TBuf8<KMceMaxSdpNameLength> aSdpName = KMceSDPNameAMR() );
     static CMceComSession* CreateVideoOutSessionL( CMceComSession* aSession = NULL, TBool aUseAvc = EFalse );
     
     static CMceComSession* CreateVideoOutSessionFileSourceL( CMceComSession* aSession = NULL, TBool aUseAvc = EFalse );
     static CMceComSession* CreateMultimediaOutSessionL( TBool aUseAvc = EFalse );
     static CMceComSession* CreateMultimediaBundledOutSessionL();
    
     static CSdpDocument* EstablishOutSessionL( 
                                        CMceComSession& aSession,
                                        CMceMediaManager& aManager,
                                        const TDesC8& aAnswer,
                                        CMceServerStub* aServer = NULL,
                                        CSdpDocument* aUpdatedSdp = NULL );

     static CSdpDocument* EstablishOutMultimediaSessionL( 
                                        CMceComSession& aSession,
                                        CMceMediaManager& aManager,
                                        const TDesC8& aAnswer,
                                        CMceServerStub* aServer = NULL );
                                        

    static CMceComMediaStream* CreateVideoDownLinkL( TInt aPayload = KMceMMTestVideoPayloadType, 
                                                     TBool aUseAvc = EFalse );
    static CMceComMediaStream* CreateLocalVideoStreamL( CMceComMediaSource* aCameraSource );
    static CMceComSession* EncodeDecodeSessionL( CMceComSession* aSession );
    static void LinkCreated( CMceMediaManager& aManager, TUint32 aLinkId, 
                                              TUint32 aSessionId = 0,
                                              TUint aPort = KMaxTUint32 , 
                                              TUint32 aInetAddr = 0 );

    static HBufC8* MceMediaManagerTestHelper::ToTextLC( CSdpDocument& aDocument );
    
    };



const TMceMediaEventCode KMMTestNoValue( (TMceMediaEventCode) KMaxTInt16 );


class CMceServerStub : public CBase, 
                       public MMceMediaSessionObserver, 
                       public MMceMediaGeneralObserver
                            
    {
    
public:


    inline CMceServerStub()
        {
        Reset();
        }
        
    inline ~CMceServerStub()
        {
        Reset();
        }
    
    inline void Reset()
        {
        iEvent = KMMTestNoValue;
        iSession = NULL;
        iStream = NULL;
        iSink = NULL;
        iSource = NULL;
        iError = KMMTestNoValue;
        iItcEvent = EMceItcNotAssigned;
        iItcData = KMceNotAssigned;
        iItcDataDesC = NULL;
        if ( iStockedSession )
            {
            CMceComSession::Delete( iStockedSession, iStockedSession->SdpSession().Manager() );
            }
        iStockedSession = NULL;
        }
    
    inline void Updated( CMceComSession& aSession )
        {
        iEvent = EMceMediaUpdated;
    	iSession = &aSession;
        }

    inline void Reserved( CMceComSession& aSession )
        {
        iEvent = EMceMediaReserved;
    	iSession = &aSession;
        }
    inline void SessionClosed( CMceComSession& aSession )
    	{
        iEvent = EMceMediaSessionStopped;
    	iSession = &aSession;
    	}
    	
    inline void EventReceived( TMceMccComEvent& aEvent )
    	{
        iEvent = EMceMediaEventReceived;
        iStream = aEvent.iStream;
        iSink = aEvent.iSink;
        iSource = aEvent.iSource;
        iItcEvent = aEvent.iItcEvent;
        iItcData = aEvent.iItcData;
        iItcDataDesC = aEvent.iItcDataDesC;
    	}
    	
    inline void MediaError( TMceMccComEvent& aEvent )
    	{
        iEvent = EMceMediaError;
    	iError = aEvent.iError;
        iStream = aEvent.iStream;
        iSink = aEvent.iSink;
        iSource = aEvent.iSource;
        iItcEvent = aEvent.iItcEvent;
        iItcData = aEvent.iItcData;
        iItcDataDesC = aEvent.iItcDataDesC;
    	}
    inline void MediaError( TInt aError )
    	{
        iEvent = EMceMediaError;
    	iError = aError;
    	iSession = NULL;
    	}
    inline void SessionCreatedL ( CMceComSession* aSession )
        {
        iEvent = (TMceMediaEventCode)KMceEventCodeNotAssigned;
        if ( iStockedSession )
            {
            iStockedSession->SdpSession().AttachContext( NULL );
            CMceComSession::Delete( iStockedSession, aSession->SdpSession().Manager() );
            }
        CMceComSession::Delete( iStockedSession, aSession->SdpSession().Manager() );
        iStockedSession = aSession;
        }
    	
    	
public:

    TMceMediaEventCode iEvent;
    CMceComSession* iSession;
    CMceComMediaStream* iStream;
    CMceComEndpoint* iSink;
    CMceComEndpoint* iSource;
    CMceComRtpSource* iRtpSource;
    TInt iError;
    TMceItcCallBack iItcEvent;
    TUint32 iItcData;
    const TDesC8* iItcDataDesC;
    CMceComSession* iStockedSession;
    
    };
    
    
                            
                            

#endif      //  __MCEMEDIAMANAGERTESTHELPER_H__

// End of file
