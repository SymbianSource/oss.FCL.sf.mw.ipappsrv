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




#ifndef __UT_CMCESECUREDESSTREAM_H__
#define __UT_CMCESECUREDESSTREAM_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "SdpCodecStringPool.h"
#include <SdpCodecStringConstants.h>
//  INTERNAL INCLUDES
#include "mcemediaobserver.h"
#include "mmccsecureinterface_stub.h"
#include "mcemediasdpcodec.h"

//  FORWARD DECLARATIONS
class CMceMediaManager;
class CMceMediaSdpCodec;
class CMccControllerStub;
class CSdpMediaField;
class CSdpDocument;
class CMceSecureMediaSession;


#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
 class UT_CMceSecureDesStream 
     : public CEUnitTestSuiteClass, public MMceMediaGeneralObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceSecureDesStream* NewL();
        static UT_CMceSecureDesStream* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceSecureDesStream();

    private:    // Constructors and destructors

        UT_CMceSecureDesStream();
        void ConstructL();

    public:     // From observer interface
		
		void MediaError( TInt /*aError*/ ) {}
        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMceSecureDesStream_NewLL();
        
        
         void UT_CMceSecureDesStream_EncodeSecureSdpLL();
        
        
         void UT_CMceSecureDesStream_DecodeSecureSdpLL();
         
         void UT_CMceSecureDesStream_DecodeSecureSdpAnswerLL();
        
        
         void UT_CMceSecureDesStream_RemvoeSecureSdpLL();
        
        
         void UT_CMceSecureDesStream_SetCryptoContextL();
        
        
         void UT_CMceSecureDesStream_RemoveCryptoContextL();
        
        
         void UT_CMceSecureDesStream_MediaStreamL(  );
        
        
         void UT_CMceSecureDesStream_MediaFieldL();
        
        
         void UT_CMceSecureDesStream_ValidateOfferByAnswerLL();
         
         void UT_CMceSecureDesStream_ValidateAnswerByOfferLL();
         
         void UT_CMceSecureDesStream_FormMKILL(  );
         
         void UT_CMceSecureDesStream_GenerateCryptoSuiteLineL();
         
         void UT_CMceSecureDesStream_GenerateCryptoLineL();
         
         void UT_CMceSecureDesStream_DecodeMKIValueL();        
         void UT_CMceSecureDesStream_SetMediaProfileL();
         void UT_CMceSecureDesStream_CopyStreamCryptoL();        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        RStringF iAudio;
		
		RStringF iVideo;
		
		RStringF iProtocol;
		
		CMceComSession* iSession;
		
		CMceMediaManager* iManager;
		
		 // Not owned
		CMceMediaSdpCodec* iAudioSdpCodec;
		
		 // Not owned
		CMceMediaSdpCodec* iVideoSdpCodec;
		
		CMccControllerStub* iMcc; 
		CSdpDocument* iOffer;
		CMccSecureInterface* iSecureInterface;
		CMccInterface* iInterface;
		CSdpMediaField* iMedia;
		CMceSecureMediaSession* iSecureSession;
    	CMceComCodec* iCodec;
   		TInt iDirection;
   		
   		CMceComMediaStream* iStream1;
   		CMceComMediaStream* iStream2;
   		
   		CMceSecureDesStream* iSecureStream;
    };

#endif      //  __UT_CMCESECUREDESSTREAM_H__

// End of file
