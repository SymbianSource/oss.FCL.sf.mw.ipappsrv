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




#ifndef __UT_CSTUNRELAYBINDINGIMPLEMENTATION_H__
#define __UT_CSTUNRELAYBINDINGIMPLEMENTATION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "natfwstunclient.h"
#include "cncmconnectionmultiplexer.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "natfwstunclientobserver.h"
#include "mstunbindingobserver.h"

//  FORWARD DECLARATIONS
class CSTUNRelayBinding;
class CSTUNRelayBindingImplementation;

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
NONSHARABLE_CLASS( ut_cstunrelaybindingimplementation )
     : public CEUnitTestSuiteClass, public MSTUNClientObserver, 
       public MNcmConnectionMultiplexerObserver,
       public MSTUNBindingObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cstunrelaybindingimplementation* NewL();
        static ut_cstunrelaybindingimplementation* NewLC();
        /**
         * Destructor
         */
        ~ut_cstunrelaybindingimplementation();

    private:    // Constructors and destructors

        ut_cstunrelaybindingimplementation();
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
        
    public: // From MSTUNBindingObserver

        const CSTUNClient& STUNClient() const;

        CDeltaTimer& TimerProvider();
        
        TInt RetransmitInterval() const;
        
        TTransportProtocol TransportProtocol() const;

        void AddressResolvedL( const CBinding& aBinding ) const;

        void ObtainSharedSecretL( CBinding& aBinding );

        TBool SharedSecretRejectedL( const CBinding& aBinding,
                                     const TDesC8& aUsername,
                                        const TDesC8& aPassword );

        TBool ObtainServerAddress( TInetAddr& aAddress );
        
        void ObtainTransactionIDL( TNATFWUNSAFTransactionID& aTransactionID );

        void AttachBindingL( const CBinding& aBinding );

        void DetachBinding( const CBinding& aBinding );
        
        void BindingErrorL( const CBinding& aBinding, TInt aError, TBool aIsFatal );
        
        void BindingEventOccurred( const CBinding& aBinding, 
                                   TSTUNCallbackInfo::TFunction aEvent );
                                   
        const TDesC8& UsernameForIndication();

        const TDesC8& PasswordForIndication();
        
        
    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CSTUNRelayBindingImplementation_NewLL();
        
        
         void UT_CSTUNRelayBindingImplementation_NewL_1L();
        
        
         void UT_CSTUNRelayBindingImplementation_TimerExpiredLL();
        
        
         void UT_CSTUNRelayBindingImplementation_LeaveFromTimerExpiredL();
        
        
         void UT_CSTUNRelayBindingImplementation_PublicAddressObtainedLL();
        
        
         void UT_CSTUNRelayBindingImplementation_PublicAddressObtainedL_1L();
        
        
         void UT_CSTUNRelayBindingImplementation_TransactionErrorL();
        

         void UT_CSTUNRelayBindingImplementation_TransactionEventOccurredLL();
        
        
         void UT_CSTUNRelayBindingImplementation_AllocateRequestLL();
        
        
         void UT_CSTUNRelayBindingImplementation_CancelRequestL();
        
        
         void UT_CSTUNRelayBindingImplementation_SocketL();
        
        
         void UT_CSTUNRelayBindingImplementation_AddressResolvedL();
        
        
         void UT_CSTUNRelayBindingImplementation_PublicAddrL();
        
        
         void UT_CSTUNRelayBindingImplementation_AlternateServerAddrL();
        
         
         void UT_CSTUNRelayBindingImplementation_HandleDataLL();
        
         
         void UT_CSTUNRelayBindingImplementation_HandleDataL2L();
         
         
         void UT_CSTUNRelayBindingImplementation_HandleDataL3L();  
         
         
         void UT_CSTUNRelayBindingImplementation_HandleDataL4L();
         
         
         void UT_CSTUNRelayBindingImplementation_STUNClientL();
        
        
         void UT_CSTUNRelayBindingImplementation_GetServerAddressLL();
        
        
         void UT_CSTUNRelayBindingImplementation_GetSharedSecretLL();
        
        
         void UT_CSTUNRelayBindingImplementation_DetachClientL();
         
         
         void UT_CSTUNRelayBindingImplementation_StoreAddressLL();
        
        
         void UT_CSTUNRelayBindingImplementation_StoreAddressL_1L();
        
        
         void UT_CSTUNRelayBindingImplementation_ChangeStateL();
        
        
         void UT_CSTUNRelayBindingImplementation_TerminateL();
        
        
         void UT_CSTUNRelayBindingImplementation_IsWaitingSharedSecretL();
        
        
         void UT_CSTUNRelayBindingImplementation_SharedSecretObtainedLL();
        
        
         void UT_CSTUNRelayBindingImplementation_CreateBindingRequestLL();
        
        
         void UT_CSTUNRelayBindingImplementation_SendBindingRequestLL();
        
        
         void UT_CSTUNRelayBindingImplementation_HandleTransactionErrorL();
        
         
         void UT_CSTUNRelayBindingImplementation_HandleTransactionError_AllocL();
         
         
         void UT_CSTUNRelayBindingImplementation_IcmpErrorL();
        
        
         void UT_CSTUNRelayBindingImplementation_WaitBeforeRetryingL();
        
        
         void UT_CSTUNRelayBindingImplementation_SetICESpecificAttributesL();
        
        
         void UT_CSTUNRelayBindingImplementation_FetchCandidateLL();
         
         
         void UT_CSTUNRelayBindingImplementation_ValidateMsgTypeL();
        
        
         void UT_CSTUNRelayBindingImplementation_SetActiveDestinationRequestLL();
        
        
         void UT_CSTUNRelayBindingImplementation_SendIndicationLL();
        
        
         void UT_CSTUNRelayBindingImplementation_ConnectRequestLL();
        
        
         void UT_CSTUNRelayBindingImplementation_StreamIdL();
         
         
         void UT_CSTUNRelayBindingImplementation_ConnectionIdL();
         
         
         void UT_CSTUNRelayBindingImplementation_RealmFromResponseL();
        
        
         //void UT_CSTUNRelayBindingImplementation_ExecuteImmediateRetransmitLL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        CSTUNRelayBinding* iBinding;
        CSTUNRelayBindingImplementation* iImpl;
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
        RPointerArray<CBinding> iBindings;
        CDeltaTimer* iDeltatimer2;
    };

#endif      //  __UT_CSTUNRELAYBINDINGIMPLEMENTATION_H__

// End of file
