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




#ifndef __UT_CNATFWUNSAFTLSTRANSPORT_H__
#define __UT_CNATFWUNSAFTLSTRANSPORT_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <in_sock.h>

//  INTERNAL INCLUDES
#include "mnatfwunsaftlstransportobserver.h"

//  FORWARD DECLARATIONS
class CNATFWUNSAFTlsTransport;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFTlsTransport )
     : public CEUnitTestSuiteClass,
       public MNATFWUNSAFTlsTransportObserver
    {

    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFTlsTransport* NewL();
        static UT_CNATFWUNSAFTlsTransport* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFTlsTransport();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFTlsTransport();
        void ConstructL();

    public:     // From observer interface

        void TlsConnectedL();

        void IncomingMessageL( CNATFWUNSAFMessage* aMessage );

        void ErrorOccured( TInt aError );

    private:    // New methods

        void SetupL();
        void Teardown();
        void ConnectTestL();
        void TcpConnectFailedTestL();
        void TlsHandshakeFailedTestL();
        void CancelTcpConnectTestL();
        void CancelTlsHandshakeTestL();
        void SendOverTlsTestL();

    private: // Helper functions

        void CompleteTcpConnectL( TInt aStatus );

        void CompleteTlsHandshakeL( TInt aStatus );

    private: // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFTlsTransport* iTransport;

        RSocketServ iServer;
        RConnection iConn;

        TBool iTlsConnected;
        TInt iLastError;
    };

#endif      //  __UT_CNATFWUNSATLSTRANSPORT_H__

// End of file
