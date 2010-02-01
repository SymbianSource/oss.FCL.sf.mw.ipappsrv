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




#ifndef __UT_CNATFWUNSAFUDPSENDER_H__
#define __UT_CNATFWUNSAFUDPSENDER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <es_sock.h>
#include <in_sock.h>

//  INTERNAL INCLUDES
#include "mnatfwunsafudpsenderobserver.h"

//  FORWARD DECLARATIONS
class CNATFWUNSAFUdpSender;
class CNATFWUNSAFMessage;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFUdpSender )
     : public CEUnitTestSuiteClass,
       public MNATFWUNSAFUdpSenderObserver
    {

    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFUdpSender* NewL();
        static UT_CNATFWUNSAFUdpSender* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFUdpSender();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFUdpSender();
        void ConstructL();

    public:     // From observer interface

        void UNSAFUdpMessageSentL();

        void UNSAFUdpMessageFailure( TInt aError );

    private:    // New methods

        void SetupL();
        void Teardown();
        void SendTestL();
        void SendFailedTestL();
        void CancelSendTestL();
        void QueueTestL();


    private: // Helpers

        void CompleteSendL( TInt aStatus );


    private: // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFUdpSender* iSender;

        RSocketServ iServer;
        RConnection iConn;
        RSocket iSocket;

        CNATFWUNSAFMessage* iMsg;

        TInetAddr iAddr;

        TBool iIsMsgSent;
        TInt iLastError;

        };

#endif      //  __UT_CNATFWUNSAUDPSENDER_H__

// End of file
