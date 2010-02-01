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




#ifndef __UT_CSTUNBINDING_H__
#define __UT_CSTUNBINDING_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "natfwstunclient.h"
#include "cncmconnectionmultiplexer.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "natfwstunclientobserver.h"

//  FORWARD DECLARATIONS
class CSTUNBinding;

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
NONSHARABLE_CLASS( ut_cstunbinding )
     : public CEUnitTestSuiteClass, public MSTUNClientObserver, 
       public MNcmConnectionMultiplexerObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cstunbinding* NewL();
        static ut_cstunbinding* NewLC();
        /**
         * Destructor
         */
        ~ut_cstunbinding();

    private:    // Constructors and destructors

        ut_cstunbinding();
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
        void IcmpError( TUint aSessionId, TUint aStreamId, const TInetAddr& aAddress );
        
    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CSTUNBinding_NewLL();
        
        
         void UT_CSTUNBinding_NewL_1L();
        
        
         void UT_CSTUNBinding_NewLCL();
        
        
         void UT_CSTUNBinding_NewLC_1L();
        
        
         void UT_CSTUNBinding_SendRequestLL();
        
        
         void UT_CSTUNBinding_SendRequestL_1L();
        
        
         void UT_CSTUNBinding_CancelRequestL();
        
        
         void UT_CSTUNBinding_SocketL();
        
        
         void UT_CSTUNBinding_AddressResolvedL();
        
        
         void UT_CSTUNBinding_PublicAddrL();
        
        
         void UT_CSTUNBinding_HandleDataLL();
        
        
         void UT_CSTUNBinding_SetICESpecificAttributesL();
        
        
         void UT_CSTUNBinding_StreamIdL();
         
         
         void UT_CSTUNBinding_ConnectionIdL();
        
        
         //void UT_CSTUNBinding_ExecuteImmediateRetransmitLL();
        
        
         void UT_CSTUNBinding_STUNClientL();
        
        
         void UT_CSTUNBinding_AlternateServerAddrL();
         
         
         void UT_CSTUNBinding_CancelRetransmissionL();
         

         void UT_CSTUNBinding_SendBindingIndicationLL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        CSTUNBinding* iBinding;
        RSocketServ iSocketServ;
        RConnection iConnection;
        CDeltaTimer* iDeltatimer;
        CSTUNClient* iStunclient;
        TUint iStreamId;
        TUint iSubstreamId;
        HBufC8* iServeraddress;
        HBufC8* iResponse;
        HBufC8* iRespDataIndication;
        CNcmConnectionMultiplexer* iMux;
        RSocket iSocket;
        TRequestStatus iStatus; 
        TBool iCallbackCalled;
        
    public: 
            
        static CActiveSchedulerWait iActiveSchedulerWait;

    };

#endif      //  __UT_CSTUNBINDING_H__

// End of file
