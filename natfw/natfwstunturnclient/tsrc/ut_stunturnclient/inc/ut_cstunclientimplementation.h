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




#ifndef __UT_CSTUNCLIENTIMPLEMENTATION_H__
#define __UT_CSTUNCLIENTIMPLEMENTATION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/EUnitDecorators.h>


//  INTERNAL INCLUDES
#include "cstunclientimplementation.h"
#include "cncmconnectionmultiplexer.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "natfwstunclientobserver.h"

//  FORWARD DECLARATIONS
class CSTUNBinding;

//  CLASS DEFINITION
/**
 * TODO Auto-generated test suite
 *
 */
NONSHARABLE_CLASS( ut_cstunclientimplementation )
	: public CEUnitTestSuiteClass, public MSTUNClientObserver,
      public MNcmConnectionMultiplexerObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cstunclientimplementation* NewL();
        static ut_cstunclientimplementation* NewLC();
        /**
         * Destructor
         */
        ~ut_cstunclientimplementation();
        
    public:     // From mstunclientobserver

    void STUNClientInitCompleted( const CSTUNClient& aClient,
                                  TInt aCompletionCode );   


    void STUNBindingEventOccurredL( TSTUNBindingEvent aEvent, 
                                    const CBinding& aBinding );


    void STUNBindingErrorOccurred( const CBinding& aBinding, 
                                   TInt aError );

public:     // From MNcmConnectionMultiplexerObserver

    void Notify( TUint aSessionId, TUint aStreamId, TNotifyType aType, TInt aError );

    private:    // Constructors and destructors

        ut_cstunclientimplementation();
        void ConstructL();

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void T_CSTUNClientImplementation__CSTUNClientImplementationL();
        
         void T_CSTUNClientImplementation_TimerExpiredLL();
        
         void T_CSTUNClientImplementation_LeaveFromTimerExpiredL();
        
         void T_CSTUNClientImplementation_CompletedLL();
        
         void T_CSTUNClientImplementation_ErrorOccuredL();
        
         void T_CSTUNClientImplementation_IcmpErrorL();
        
         void T_CSTUNClientImplementation_SharedSecretObtainedLL();
        
         void T_CSTUNClientImplementation_SharedSecretErrorLL();
        
         void T_CSTUNClientImplementation_STUNClientL();
        
         void T_CSTUNClientImplementation_TimerProviderL();
        
         void T_CSTUNClientImplementation_RetransmitIntervalL();
        
         void T_CSTUNClientImplementation_TransportProtocolL();
        
         void T_CSTUNClientImplementation_AddressResolvedLL();
        
         void T_CSTUNClientImplementation_ObtainSharedSecretLL();
        
         void T_CSTUNClientImplementation_SharedSecretRejectedLL();
        
         void T_CSTUNClientImplementation_ObtainServerAddressL();
        
         void T_CSTUNClientImplementation_ObtainTransactionIDLL();
        
         void T_CSTUNClientImplementation_AttachBindingLL();
        
         void T_CSTUNClientImplementation_DetachBindingL();
        
         void T_CSTUNClientImplementation_BindingErrorLL();
        
         void T_CSTUNClientImplementation_BindingEventOccurredL();
        
         void T_CSTUNClientImplementation_RenewSharedSecretLL();
        
         void T_CSTUNClientImplementation_UsernameForIndicationL();
        
         void T_CSTUNClientImplementation_PasswordForIndicationL();
        
         void T_CSTUNClientImplementation_IsInitializedL();
        
         void T_CSTUNClientImplementation_STUNServerAddrLL();
        
         void T_CSTUNClientImplementation_SetCredentialsLL();
        
         void T_CSTUNClientImplementation_SharedSecretObtainedL();
        
         void T_CSTUNClientImplementation_HasPresetCredentialsL();
        
         void T_CSTUNClientImplementation_ChangeStateL();
        
         void T_CSTUNClientImplementation_TerminateL();
        
         void T_CSTUNClientImplementation_TlsResolvedLL();
        
         void T_CSTUNClientImplementation_TcpResolvedLL();
        
         void T_CSTUNClientImplementation_FreeResolverResourcesL();
        
         void T_CSTUNClientImplementation_GetSharedSecretFromServerLL();
        
         void T_CSTUNClientImplementation_PassSharedSecretToBindingLL();
        
         void T_CSTUNClientImplementation_InformWaitingBindingsLL();
        
         void T_CSTUNClientImplementation_HandleSharedSecretErrorLL();
        
         void T_CSTUNClientImplementation_DoesSharedSecretMatchL();
        
         void T_CSTUNClientImplementation_PassInitCompletedLL();
        
         void T_CSTUNClientImplementation_PassCredentialsRejectedLL();
        
         void T_CSTUNClientImplementation_StartSharedSecretTimerL();
        
         void T_CSTUNClientImplementation_MultiplexerInstanceL();
        
         void T_CSTUNClientImplementation___DbgTestInvariantL();
         
         void T_CSTUNClientImplementation_RemoveAddressL();
        

    private:    // Data
		
        CSTUNClientImplementation* iCSTUNClientImplementation;
        EUNIT_DECLARE_TEST_TABLE;
        
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
        CSTUNBinding* iBinding;
        CSTUNClientRenewSharedSecret* iRenewSharedSecret;
        CSTUNClientReady* iReady;
        CSTUNClientGetSharedSecret *iGetSharedSecret;
        CSTUNClientResolvingUDP* iResolvingUDP;
    };

#endif      //  __UT_CSTUNCLIENTIMPLEMENTATION_H__

// End of file
