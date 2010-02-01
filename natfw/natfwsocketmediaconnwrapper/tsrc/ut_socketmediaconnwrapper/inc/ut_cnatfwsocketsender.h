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




#ifndef __UT_CNATFWSOCKETSENDER_H__
#define __UT_CNATFWSOCKETSENDER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "natfwsocketsenderobserver.h"

//  FORWARD DECLARATIONS
class CNATFWSocketSender;

#include <e32def.h>
#include <es_sock.h>
#include <in_sock.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( ut_cnatfwsocketsender )
     : public CEUnitTestSuiteClass, public MNATFWSocketSenderObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cnatfwsocketsender* NewL();
        static ut_cnatfwsocketsender* NewLC();
        /**
         * Destructor
         */
        ~ut_cnatfwsocketsender();

    private:    // Constructors and destructors

        ut_cnatfwsocketsender();
        void ConstructL();

    public:     // From observer interface

        // MNATFWSocketSenderObserver
        void SendingCompleted( TInt /*aError*/ ){};

    private:    // New methods

         void SetupL();
         
         void Setup2L();
        
         void Teardown();        
        
         void ut_cnatfwsocketsender_SetRemoteAddressL();
         
         void ut_cnatfwsocketsender_RemoveFromQueueL();        
        
         void ut_cnatfwsocketsender_SendLL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        CNATFWSocketSender* iSender;
        RSocketServ iSocketSrv;
        RSocket iSocket;
        TInetAddr iAddr;         

    };

#endif      //  __UT_CNATFWSOCKETSENDER_H__

// End of file
