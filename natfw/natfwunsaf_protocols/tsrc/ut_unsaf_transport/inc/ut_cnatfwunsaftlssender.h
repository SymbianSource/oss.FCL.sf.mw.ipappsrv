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




#ifndef __UT_CNATFWUNSAFTLSSENDER_H__
#define __UT_CNATFWUNSAFTLSSENDER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <in_sock.h>

//  INTERNAL INCLUDES
#include "mnatfwunsaftlstransportobserver.h"

//  FORWARD DECLARATIONS
class CNATFWUNSAFTlsSender;
class CNATFWUNSAFMessage;
class CSecureSocket;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFTlsSender )
     : public CEUnitTestSuiteClass,
       public MNATFWUNSAFTlsTransportObserver
    {

    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFTlsSender* NewL();
        static UT_CNATFWUNSAFTlsSender* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFTlsSender();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFTlsSender();
        void ConstructL();

    public:     // From observer interface

        void TlsConnectedL();

        void IncomingMessageL( CNATFWUNSAFMessage* aMessage );

        void ErrorOccured( TInt aError );

    private:    // New methods

        void SetupL();
        void Teardown();
        void SendTestL();
        void SendFailedTestL();
        void CancelSendTestL();
        void QueueTestL();
        void RunErrorTestL();

    private: // Helpers

        void CompleteSendL( TInt aStatus );

    private: // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFTlsSender* iSender;

        RSocketServ iServer;
        RConnection iConn;
        RSocket iSocket;
        CSecureSocket* iSecureSocket;

        CNATFWUNSAFMessage* iMsg;

        TInt iLastError;
    };

#endif      //  __UT_CNATFWUNSATLSSENDER_H__

// End of file
