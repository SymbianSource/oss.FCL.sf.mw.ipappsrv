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




#ifndef __UT_CMCCSECUREINTERFACE_H__
#define __UT_CMCCSECUREINTERFACE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmccnetworksettings.h>
#include "mmccctrlobserver.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccSecureInterface;
class CMccInterface;
class TMccNetSettings;
class CMccInterfaceStub;

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
NONSHARABLE_CLASS( UT_CMccSecureInterface )
     : public CEUnitTestSuiteClass,
       public MMccCtrlObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccSecureInterface* NewL();
        static UT_CMccSecureInterface* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccSecureInterface();

    private:    // Constructors and destructors

        UT_CMccSecureInterface();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccSecureInterface_NewLL();
        
        
         void UT_CMccSecureInterface_CreateContextL();
        
        
         void UT_CMccSecureInterface_UpdateContextL();
        
        
         void UT_CMccSecureInterface_RemoveContextL();
        
        
         void UT_CMccSecureInterface_BindL();
        
    public: //MMccCtrlObserver, pure virtual functions 
        /**
        * Callback function to receive media signalling
        * @param aSignal Received media signal
        * @return void
        */
        void MccEventReceived( const TMccEvent& aEvent );
        
        /**
        * Callback function to receive stream started event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        */
        void MccMediaStarted( TUint32 aSessionId,
                                    TUint32 aLinkId,     
                                    TUint32 aStreamId,
                                    TUint32 aSinkSourceId );
        
        /**
        * Callback function to receive stream stopped event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        */
        void MccMediaStopped( TUint32 aSessionId,
                                    TUint32 aLinkId,
                                    TUint32 aStreamId,
                                    TUint32 aSinkSourceId );

        /**
        * Callback function to receive stream paused event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        */
        void MccMediaPaused( TUint32 aSessionId,
                                   TUint32 aLinkId,
                                    TUint32 aStreamId,
                                    TUint32 aSinkSourceId );

        /**
        * Callback function to receive stream resumed event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        */
        void MccMediaResumed( TUint32 aSessionId,
                                    TUint32 aLinkId,
                                    TUint32 aStreamId,
                                    TUint32 aSinkSourceId );

        /**
        * Callback function to receive stream prepared event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        */
        void MccMediaPrepared( TUint32 aSessionId,
                                    TUint32 aLinkId,
                                    TUint32 aStreamId,
                                    TUint32 aSinkSourceId );

        /**
        * Callback function to receive stream inactivity event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        */
        void MccMediaInactive( TUint32 aSessionId,
                                    TUint32 aLinkId,
                                    TUint32 aStreamId,
                                    TUint32 aSinkSourceId );
        
        /**
        * Callback function to receive stream activity event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        */
        void MccMediaActive( TUint32 aSessionId,
                                    TUint32 aLinkId,
                                    TUint32 aStreamId,
                                    TUint32 aSinkSourceId );

        /**
        * Callback function to receive error event.
        * @param aError Error code
        * @return None
        */
        void MccCtrlError( TInt aError );

        /**
        * Callback function to receive error event.
        * @param aError Error code
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        */
        void MccCtrlError( TInt aError,
                                   TUint32 aSessionId,
                                   TUint32 aLinkId,
                                   TUint32 aStreamId,
                                   TUint32 aSinkSourceId );

         

        void WaitForEvent( TUint32 aSessionId, TUint32 aLinkId,
                    TUint32 aStreamId, TMccEventType aEventType );


    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccInterface* iInterface;
        CMccSecureInterface* iSecureIntf;
        
        TUint32 iSessionId;
        TUint32 iUpLinkId;
        TUint32 iDownLinkId;
        
        TUint32 iLinkId;
        
        // DL
        TUint32 iRtpSourceId;
        TUint32 iAudioSinkId;
        TUint32 iDlStreamId;
        
        // UL
        TUint32 iRtpSinkId;
        TUint32 iMicSourceId;
        TUint32 iUlStreamId;
        
        TMccNetSettings iNetsettings;
        
    };

#endif      //  __UT_CMCCSECUREINTERFACE_H__

// End of file
