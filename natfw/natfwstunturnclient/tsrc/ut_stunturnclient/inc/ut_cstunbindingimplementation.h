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




#ifndef __UT_CSTUNBINDINGIMPLEMENTATION_H__
#define __UT_CSTUNBINDINGIMPLEMENTATION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "natfwstunclient.h"
#include "cncmconnectionmultiplexer.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "natfwstunclientobserver.h"
#include "mstunbindingobserver.h"

//  FORWARD DECLARATIONS
class CSTUNBinding;
class CSTUNBindingImplementation;

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
NONSHARABLE_CLASS( ut_cstunbindingimplementation )
     : public CEUnitTestSuiteClass, public MSTUNClientObserver, 
       public MNcmConnectionMultiplexerObserver,
       public MSTUNBindingObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cstunbindingimplementation* NewL();
        static ut_cstunbindingimplementation* NewLC();
        /**
         * Destructor
         */
        ~ut_cstunbindingimplementation();

    private:    // Constructors and destructors

        ut_cstunbindingimplementation();
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
        
         void UT_CSTUNBindingImplementation_NewLL();
        
        
         void UT_CSTUNBindingImplementation_NewL_1L();
        
        
         void UT_CSTUNBindingImplementation_TimerExpiredLL();
        
        
         void UT_CSTUNBindingImplementation_LeaveFromTimerExpiredL();
        
        
         void UT_CSTUNBindingImplementation_PublicAddressObtainedLL();
        
        
         void UT_CSTUNBindingImplementation_PublicAddressObtainedL_1L();
        
        
         void UT_CSTUNBindingImplementation_TransactionErrorL();
        
        
         void UT_CSTUNBindingImplementation_TransactionEventOccurredLL();
        
        
         void UT_CSTUNBindingImplementation_SendRequestLL();
        
        
         void UT_CSTUNBindingImplementation_SendRequestL_1L();
        
        
         void UT_CSTUNBindingImplementation_CancelRequestL();
        
        
         void UT_CSTUNBindingImplementation_SocketL();
        
        
         void UT_CSTUNBindingImplementation_AddressResolvedL();
        
        
         void UT_CSTUNBindingImplementation_PublicAddrL();
         
         
         void UT_CSTUNBindingImplementation_AlternateServerAddrL();
        
         
         void UT_CSTUNBindingImplementation_HandleDataLL();
        
        
         void UT_CSTUNBindingImplementation_STUNClientL();
        
        
         void UT_CSTUNBindingImplementation_GetServerAddressLL();
        
        
         void UT_CSTUNBindingImplementation_GetSharedSecretLL();
         
         
         void UT_CSTUNBindingImplementation_UsernameL();
        
        
         void UT_CSTUNBindingImplementation_DetachClientL();
        
        
         void UT_CSTUNBindingImplementation_StoreAddressLL();
        
        
         void UT_CSTUNBindingImplementation_ChangeStateL();
        
        
         void UT_CSTUNBindingImplementation_TerminateL();
        
        
         void UT_CSTUNBindingImplementation_IsWaitingSharedSecretL();
        
        
         void UT_CSTUNBindingImplementation_SharedSecretObtainedLL();
        
        
         void UT_CSTUNBindingImplementation_CreateBindingRequestLL();
        
        
         void UT_CSTUNBindingImplementation_SendBindingRequestLL();
        
        
         void UT_CSTUNBindingImplementation_HandleTransactionErrorL();
        
         
         void UT_CSTUNBindingImplementation_HandleTransactionError2L();
         
        
         void UT_CSTUNBindingImplementation_IcmpErrorL();
        
        
         void UT_CSTUNBindingImplementation_WaitBeforeRetryingL();
        
        
         void UT_CSTUNBindingImplementation_SetICESpecificAttributesL();
         
         
         //void UT_CSTUNBindingImplementation_ExecuteImmediateRetransmitLL();
        
         
         void UT_CSTUNBindingImplementation_ValidateMsgTypeL();
         
                    
         void UT_CSTUNBindingImplementation_StreamIdL();
         
         
         void UT_CSTUNBindingImplementation_ConnectionIdL();
         
         
         void UT_CSTUNBindingImplementation_RealmFromResponseL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        CSTUNBinding* iBinding;
        CSTUNBindingImplementation* iImpl;
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

#endif      //  __UT_CSTUNBINDINGIMPLEMENTATION_H__

// End of file
