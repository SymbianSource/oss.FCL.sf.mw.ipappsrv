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




#ifndef __UT_CSTUNBINDINGGETADDRESS_H__
#define __UT_CSTUNBINDINGGETADDRESS_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/EUnitDecorators.h>
#include <es_sock.h>


//  INTERNAL INCLUDES
#include "natfwstunclient.h"
#include "cncmconnectionmultiplexer.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "natfwstunclientobserver.h"
#include "mstunbindingobserver.h"
#include "cstunbindinggetaddress.h"

//  FORWARD DECLARATIONS
class CSTUNBinding;
class CSTUNBindingImplementation;


//  CLASS DEFINITION
/**
 * TODO Auto-generated test suite
 *
 */
NONSHARABLE_CLASS( ut_cstunbindinggetaddress )
     : public CEUnitTestSuiteClass, public MSTUNClientObserver, 
       public MNcmConnectionMultiplexerObserver,
       public MSTUNBindingObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cstunbindinggetaddress* NewL();
        static ut_cstunbindinggetaddress* NewLC();
        /**
         * Destructor
         */
        ~ut_cstunbindinggetaddress();

    private:    // Constructors and destructors

        ut_cstunbindinggetaddress();
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
        
         void T_CSTUNBindingGetAddress__CSTUNBindingGetAddressL();
        
         void T_CSTUNBindingGetAddress_PublicAddressReceivedLL();
        
         void T_CSTUNBindingGetAddress_PublicAddressReceivedL2L();
        
         void T_CSTUNBindingGetAddress_TransactionErrorL();
        
         void T_CSTUNBindingGetAddress_SendRequestLL();
        
         void T_CSTUNBindingGetAddress_SetNeighbourStatesL();
        

    private:    // Data
		
        CSTUNBindingGetAddress* iCSTUNBindingGetAddress;
        EUNIT_DECLARE_TEST_TABLE; 

        CSTUNBinding* iBinding;
        CSTUNBindingImplementation* iImpl;
        RSocketServ iSocketServ;
        RConnection iConnection;
        CDeltaTimer* iDeltatimer;
        CSTUNClient* iStunclient;
        TUint iStreamId;
        TUint iSubstreamId;
        CNcmConnectionMultiplexer* iMux;
        RSocket iSocket;
        TRequestStatus iStatus;
    };

#endif      //  __UT_CSTUNBINDINGGETADDRESS_H__

// End of file
