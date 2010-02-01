/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UT_CNCMICMPRECEIVER_H__
#define __UT_CNCMICMPRECEIVER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <in_sock.h>


//  INTERNAL INCLUDES
#include "cncmicmpv4receiver.h"
#include "cncmicmpv6receiver.h"
#include "mncmicmpobserver.h"

//  FORWARD DECLARATIONS


//  CLASS DEFINITION
/**
 * TODO Auto-generated test suite
 *
 */
NONSHARABLE_CLASS( UT_CNcmIcmpReceiver )
	: public CEUnitTestSuiteClass, public MNcmIcmpObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNcmIcmpReceiver* NewL();
        static UT_CNcmIcmpReceiver* NewLC();
        /**
         * Destructor
         */
        ~UT_CNcmIcmpReceiver();
        
        /**
         * From MNcmIcmpObserver
         *
         * Notifies that an ICMP error is occurred using
         * the specified remote address. Gets a local address and
         * a remote address to be used in the creation of the ICMP message.
         *
         * @since   S60 v3.2
         * @param   aAddress        Remote address on which the error occurred
         * @param   aLocalAddress   Reference to local address
         * @param   aRemoteAddress  Reference to remote address
         * @return  void
         */
        void IcmpError( const TInetAddr& aAddress,
            TInetAddr& aLocalAddress, TInetAddr& aRemoteAddress );

    private:    // Constructors and destructors

        UT_CNcmIcmpReceiver();
        void ConstructL();
        
        void CreateICMPError( TChar aType, TChar aCode, TChar aProtocol );

        void CreateIPv4CompatibleICMPv6Error( TChar aType, TChar aCode, TChar aProtocol );
	
	    void CreateICMPv6Error( TChar aType, TChar aCode, TChar aProtocol );

    private:    // New methods

        void SetupL();

        void Teardown();
        
        void UT_IcmpUnreachMessageNoUDPTestL();
         
        void UT_OtherIcmpMessageTestL();

        void UT_IcmpV6UnreachMessageTestL();
       
        void UT_IcmpV6UnreachMessageNoUDPTestL();
       
        void UT_OtherIcmpV6MessageTestL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        		
        CNcmIcmpV4Receiver* iCNcmIcmpV4Receiver;
        CNcmIcmpV6Receiver* iCNcmIcmpV6Receiver;

        RSocketServ iTestSocketSrv;
        RConnection iTestConnection;
        TInetAddr iTestAddress;
        
        TInetAddr iReceivedAddr;
        TBool iReceived;
    };

#endif      //  __UT_CNCMICMPRECEIVER_H__

// End of file
