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




#ifndef __UT_CNATFWUNSAFICMPRECV_H__
#define __UT_CNATFWUNSAFICMPRECV_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <e32base.h>
#include <in_sock.h>

//  INTERNAL INCLUDES
#include "mnatfwunsaficmperrorobserver.h"

//  FORWARD DECLARATIONS
class CIcmpV4Receiver;
class CIcmpV6Receiver;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFIcmpRecv )
     : public CEUnitTestSuiteClass,
       public MIcmpErrorObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFIcmpRecv* NewL();
        static UT_CNATFWUNSAFIcmpRecv* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFIcmpRecv();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFIcmpRecv();
        void ConstructL();

    private:    // New methods

        void SetupL();
        void Teardown();

        void IcmpMessageUnreachTestL();
        void IcmpUnreachMessageNoUDPTestL();
        void OtherIcmpMessageTestL(); //redirect message
        void IcmpV6UnreachMessageTestL();
        void IcmpV6UnreachMessageNoUDPTestL();
        void OtherIcmpV6MessageTestL();
        void ProtocolNotSupported();
        void LiveTestL();

    private:    // Helpers

        void SendUDPL();
        void CreateICMPError(TChar aType, TChar aCode, TChar aProtocol);
        void CreateICMPv6Error(TChar aType, TChar aCode, TChar aProtocol);
        void CreateIPv4CompatibleICMPv6Error(TChar aType, TChar aCode,
                                             TChar aProtocol);
        void IcmpError(const TInetAddr& aAddress);
        TBool Received() { return iReceived; }

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;

        CIcmpV4Receiver* iReceiver;
        CIcmpV6Receiver* iReceiverV6;
        RSocketServ iServer;
        RConnection iConnection;
        RSocket iUdpSocket;
        TBool iReceived;
        TInetAddr iReceivedAddr;
        TBool iIsSchedulerBlocked;
    };

#endif      // __UT_CNATFWUNSAFICMPRECV_H__

// End of File
