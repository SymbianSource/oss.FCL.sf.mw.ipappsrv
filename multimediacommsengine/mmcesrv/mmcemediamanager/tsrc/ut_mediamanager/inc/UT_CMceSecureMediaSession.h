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




#ifndef __UT_CMCESECUREMEDIASESSION_H__
#define __UT_CMCESECUREMEDIASESSION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

#include "SdpCodecStringPool.h"
#include <SdpCodecStringConstants.h>
#include "mcemediaobserver.h"
#include "mmccsecureinterface_stub.h"
//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS

class CMceMediaManager;
class CMceMediaSdpCodec;
class CMceComMediaStream;
class CMccControllerStub;
class CSdpMediaField;
class CSdpDocument;
//  FORWARD DECLARATIONS
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
NONSHARABLE_CLASS( UT_CMceSecureMediaSession )
     : public CEUnitTestSuiteClass , public MMceMediaGeneralObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceSecureMediaSession* NewL();
        static UT_CMceSecureMediaSession* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceSecureMediaSession();

    private:    // Constructors and destructors

        UT_CMceSecureMediaSession();
        void ConstructL();

    public:     // From observer interface

        void MediaError( TInt /*aError*/ ) {}

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMceSecureMediaSession_NewLL();
        
        
         void UT_CMceSecureMediaSession_NewLCL();
        
        
         void UT_CMceSecureMediaSession_EncodeSecureDesSdpOfferLL();
        
        
         void UT_CMceSecureMediaSession_DecodeSecureDesSdpAnswerLL();
        
        
         void UT_CMceSecureMediaSession_DecodeSecureDesSdpOfferLL();
        
        
         void UT_CMceSecureMediaSession_EncodeSecureDesSdpAnswerLL();
        
        
         void UT_CMceSecureMediaSession_DecodeSecureDesSdpUpdateLL();
        
        
         void UT_CMceSecureMediaSession_CleanSecureCryptoInfoLL();
        
        
         void UT_CMceSecureMediaSession_BindCryptoL();
        
        
         void UT_CMceSecureMediaSession_ContextNeedUpdatedL();
         
         void UT_CMceSecureMediaSession_CopyStreamsLL( );

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
		TInt iDirection;
		
		CMceComMediaStream* iStream1;
   		CMceComMediaStream* iStream2;

    };

#endif      //  __UT_CMCESECUREMEDIASESSION_H__

// End of file
