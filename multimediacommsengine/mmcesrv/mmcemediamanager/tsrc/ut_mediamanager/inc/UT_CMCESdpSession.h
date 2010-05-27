/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UT_CMCESDPSESSION_H__
#define __UT_CMCESDPSESSION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "SdpCodecStringPool.h"
#include "mcemediaobserver.h"


//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMceSdpSession;
class CMceComSession;
class CMceMediaManager;
class CMceMediaSdpCodec;
class CMceComVideoStream;
class CMceComVideoCodec;
class CMccControllerStub;
class CSdpMediaField;
class CMceServerStub;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceSdpSession
     : public CEUnitTestSuiteClass, public MMceMediaGeneralObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceSdpSession* NewL();
        static UT_CMceSdpSession* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceSdpSession();

    private:    // Constructors and destructors

        UT_CMceSdpSession();
        void ConstructL();

	public: 	// From observer interface
		
		void MediaError( TInt /*aError*/ ) {}

    private:    // New methods

         void SetupL();
        
         void Teardown();   
        
         void UT_CMceSdpSession_MatchingStreamsToMLinesL();

         void UT_CMceSdpSession_DecodePullModeUpdateMediaLineL();
        
         void UT_CMceSdpSession_CreateOfferLL_OK_1();
         
         void UT_CMceSdpSession_CreateOfferLL_NOK_1();    
         
         void UT_CMceSdpSession_CreateOfferLL_REFRESH_OK_1();
        
         void UT_CMceSdpSession_DecodeAnswerLL_OK_1();
        
         void UT_CMceSdpSession_DecodeAnswerLL_OK_2();
        
         void UT_CMceSdpSession_DecodeAnswerLL_NOK_1();

         void UT_CMceSdpSession_DecodeOfferLL_CreateAnswer_OK_1();
        
         void UT_CMceSdpSession_DecodeOfferLL_CreateAnswer_NOK_1();
         
         
         void UT_CMceSdpSession_CreateOfferLL_Video_OK_1();
         
         void UT_CMceSdpSession_DecodeAnswerLL_Video_OK_1();
         
         void UT_CMceSdpSession_DecodeAnswerLL_Video_OK_2();
         
         void UT_CMceSdpSession_DecodeAnswerLL_Video_NOK_1();
         
         void UT_CMceSdpSession_DecodeOfferLL_CreateAnswer_Video_OK_1();
         
         void UT_CMceSdpSession_DecodeOfferLL_CreateAnswer_Video_NOK_1();
         
        
         void UT_CMceSdpSession_EncodeSessionParamsL();
         
         void UT_CMceSdpSession_EncodeConnectionFieldL();
		 
         void UT_CMceSdpSession_DecodeOfferL_1L();

         void UT_CMceSdpSession_DecodeOfferL_2L();
         
         void UT_CMceSdpSession_DecodeOfferL_3L( );
          
         void UT_CMceSdpSession_DecodeOfferL_4L( );

        void UT_CMceSdpSession_ForkLL();

        void UT_CMceSdpSession_ConsumesL();

        void UT_CMceSdpSession_ContextSwitchL();

        void UT_CMceSdpSession_AttachContextL();

        void UT_CMceSdpSession_ContextSwitchRequestedL();
 
        void UT_CMceSdpSession_EncodeClientAttributesLL();
         
        void UT_CMceSdpSession_DecodeClientAttributesLL();
        
        void UT_CMceSdpSession_StoreRemoteOriginL();
        
        void UT_CMceSdpSession_StoreRemoteMediaFieldsL();
        
        void UT_CMceSdpSession_CompareMediaLinesL();
        
        void UT_CMceSdpSession_SetRemoteIpAddressL();

     
    private: 
    
        CMceComSession* CreateSessionWithDowlinkL();
    
        CMceComSession* CreateSessionWithVideoDowlinkL();    
        
        void AddVideoUplinkL( CMceComSession& aSession );
        
        void AddVideoCodecL( CMceComVideoStream& aStream, TUint aPayloadType, TSize aResolution );

        void EncodeDirectionAttrL( CSdpMediaField& aMediaLine, TInt aDirection );


    private:    // Data

		EUNIT_DECLARE_TEST_TABLE;
		RStringF iAudio;
		
		RStringF iVideo;
		
		RStringF iProtocol;
		
		CMceComSession* iAudioOutSession;
		CMceComSession* iVideoOutSession;
		CMceComSession* iInSession;
		CMceSdpSession* iSdpSession;
		CMceMediaManager* iManager;
		CMceServerStub* iServer;
		 // Not owned
		CMceMediaSdpCodec* iAudioSdpCodec;
		
		 // Not owned
		CMceMediaSdpCodec* iVideoSdpCodec;
		
		CMccControllerStub* iMcc;
    };

#endif      //  __UT_CMCESDPSESSION_H__

// End of file
