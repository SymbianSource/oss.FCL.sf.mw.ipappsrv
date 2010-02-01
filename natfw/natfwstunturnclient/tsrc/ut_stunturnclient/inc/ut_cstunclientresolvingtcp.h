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




#ifndef __UT_CSTUNCLIENTRESOLVINGTCP_H__
#define __UT_CSTUNCLIENTRESOLVINGTCP_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/EUnitDecorators.h>
#include <es_sock.h>

//  INTERNAL INCLUDES
#include "cstunclientresolvingtcp.h"
#include "cncmconnectionmultiplexer.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "natfwstunclientobserver.h"

//  FORWARD DECLARATIONS
class CSTUNBinding;
class CSTUNClientResolvingTLS;
class CSTUNClientResolvingUDP;

//  CLASS DEFINITION
/**
 * TODO Auto-generated test suite
 *
 */
NONSHARABLE_CLASS( ut_cstunclientresolvingtcp )
	: public CEUnitTestSuiteClass, public MSTUNClientObserver, 
    public MNcmConnectionMultiplexerObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cstunclientresolvingtcp* NewL();
        static ut_cstunclientresolvingtcp* NewLC();
        /**
         * Destructor
         */
        ~ut_cstunclientresolvingtcp();

    private:    // Constructors and destructors

        ut_cstunclientresolvingtcp();
        void ConstructL();
        
    public:     // From mstunclientobserver

        void STUNClientInitCompleted( const CSTUNClient& aClient,
                                  TInt aCompletionCode );   


        void STUNBindingEventOccurredL( TSTUNBindingEvent aEvent, 
                                    const CBinding& aBinding );


        void STUNBindingErrorOccurred( const CBinding& aBinding, 
                                   TInt aError );

    public:     // From MNcmConnectionMultiplexerObserver

        void Notify( TUint aSessionId, TUint aStreamId, TNotifyType aType, TInt aError );    

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void T_CSTUNClientResolvingTCP__CSTUNClientResolvingTCPL();
        
         void T_CSTUNClientResolvingTCP_ResolvingCompletedLL();
        
         void T_CSTUNClientResolvingTCP_ResolvingFailedL();
        

    private:    // Data
		
        CSTUNClientResolvingTCP* iCSTUNClientResolvingTCP;
        EUNIT_DECLARE_TEST_TABLE;
        
        CSTUNClientImplementation* iCSTUNClientImplementation;
        RSocketServ iSocketServ;
        RConnection iConnection;
        CDeltaTimer* iDeltatimer;
        CSTUNClient* iStunclient;
        TUint iStreamId;
        TUint iSubstreamId;
        CNcmConnectionMultiplexer* iMux;
        CSTUNClientResolvingTLS* iResolvingTLS;
        CSTUNClientResolvingUDP* iResolvingUDP;
        TRequestStatus iStatus;

    };

#endif      //  __UT_CSTUNCLIENTRESOLVINGTCP_H__

// End of file
