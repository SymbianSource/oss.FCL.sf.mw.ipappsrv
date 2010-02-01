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




#ifndef __UT_CSTUNRELAYBINDING_H__
#define __UT_CSTUNRELAYBINDING_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <es_sock.h>
#include <CommDbConnPref.h>
#include <e32base.h>
#include "natfwstunclient.h"
#include "natfwstunclientobserver.h"
#include "natfwbindingbase.h" 
#include "natfwunsafallocateresponse.h"
#include "natfwunsafmessage.h"
#include "cncmconnectionmultiplexer.h"
#include "mncmconnectionmultiplexerobserver.h"


//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CSTUNRelayBinding;
//class TSTUNBindingEvent;
//class MSTUNClientObserver;

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
NONSHARABLE_CLASS( UT_CSTUNRelayBinding )
     : public CEUnitTestSuiteClass,
     public MSTUNClientObserver,
     public MNcmConnectionMultiplexerObserver
    {
    enum TMsgType
    {
    EAllocateResp       = 0,
    EAllocateErr        = 1,
    ESetActiveDestResp  = 2,
    ESetActiveDestErr   = 3,
    EConnectResp        = 4,
    EConnectErr         = 5
    };
    public:
        friend class CNATFWUNSAFAllocateResponse;
        friend class CNATFWUNSAFMessage;
        friend class CNATFWUNSAFRelayAddressAttribute;
        
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CSTUNRelayBinding* NewL();
        static UT_CSTUNRelayBinding* NewLC();
        /**
         * Destructor
         */
        ~UT_CSTUNRelayBinding();

    private:    // Constructors and destructors

        UT_CSTUNRelayBinding();
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
         
         void CreateBinding();
         
         HBufC8* GenerateResponseL( TMsgType aType );
         
         HBufC8* GenerateDataIndicationL( TDesC8& aResponse );
        
         void UT_CSTUNRelayBinding_NewLL();
                
         void UT_CSTUNRelayBinding_AllocateRequestLL();
         
         void UT_CSTUNRelayBinding_AllocateRequestL_AllocL();
         
         void UT_CSTUNRelayBinding_AllocateRequestErrLL();
                
         void UT_CSTUNRelayBinding_CancelRequestL();
                
         void UT_CSTUNRelayBinding_PublicAddrL();
                
         void UT_CSTUNRelayBinding_HandleDataLL();
        
         void UT_CSTUNRelayBinding_FetchCandidateLL();
                
         void UT_CSTUNRelayBinding_SetICESpecificAttributesL();
                
         void UT_CSTUNRelayBinding_TestHandleDataUsageLL();
                
         void UT_CSTUNRelayBinding_STUNClientL();
                
         void UT_CSTUNRelayBinding_SetActiveDestinationRequestLL();
         
         void UT_CSTUNRelayBinding_SetActiveDestinationRequestL_AllocL();
         
         void UT_CSTUNRelayBinding_SetActiveDestinationRequestErrLL();
                
         void UT_CSTUNRelayBinding_SendIndicationLL();
                
         void UT_CSTUNRelayBinding_ConnectRequestLL();
         
         void UT_CSTUNRelayBinding_ConnectRequestL_AllocL();
         
         void UT_CSTUNRelayBinding_ConnectRequestErrLL();
                
         void UT_CSTUNRelayBinding_ImplementationL();
                
         void UT_CSTUNRelayBinding_Implementation_1L();
         
         void UT_CSTUNRelayBinding_TestGettersL();
        
        
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        RSocketServ iSocketServ;
        RConnection iConnection;
        CDeltaTimer* iDeltatimer;
        CSTUNClient* iStunclient;
        TUint iStreamId;
        TUint iSubstreamId;
        HBufC8* iServeraddress;
        CSTUNRelayBinding* iBinding;
        HBufC8* iResponse;
        HBufC8* iRespDataIndication;
        CNcmConnectionMultiplexer* iMux;
    };

#endif      //  __UT_CSTUNRELAYBINDING_H__

// End of file
