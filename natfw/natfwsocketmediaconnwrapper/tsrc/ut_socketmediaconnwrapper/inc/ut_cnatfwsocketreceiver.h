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




#ifndef __UT_CNATFWSOCKETRECEIVER_H__
#define __UT_CNATFWSOCKETRECEIVER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "natfwsocketreceiverobserver.h"

//  FORWARD DECLARATIONS
class CNATFWSocketReceiver;

#include <e32def.h>
#include <es_sock.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( ut_cnatfwsocketreceiver )
     : public CEUnitTestSuiteClass, public MNATFWSocketReceiverObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cnatfwsocketreceiver* NewL();
        static ut_cnatfwsocketreceiver* NewLC();
        /**
         * Destructor
         */
        ~ut_cnatfwsocketreceiver();

    private:    // Constructors and destructors

        ut_cnatfwsocketreceiver();
        void ConstructL();

    public:     // From observer interface

        // MNATFWSocketReceiverObserver
        void DeliverBuffer( TDes8& /*aStreamPortion*/ ){};

    private:    // New methods

         void SetupL();
         
         void Teardown();        
        
         void ut_cnatfwsocketreceiver_StartListeningL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CNATFWSocketReceiver* iReceiver;
        RSocket iSocket;
        RSocketServ iServ;
    };

#endif      //  __UT_CNATFWSOCKETRECEIVER_H__

// End of file
