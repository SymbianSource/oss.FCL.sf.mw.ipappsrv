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




#ifndef __UT_CNATFWSTUNSRVIMPL_H__
#define __UT_CNATFWSTUNSRVIMPL_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "mncmconnectionmultiplexerobserver.h"

//  INTERNAL INCLUDES
#include "natfwstunsrvobserver.h"

//  FORWARD DECLARATIONS
class CNATFWSTUNSrvImpl;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
class MNcmConnectionMultiplexerObserver;
class CNcmConnectionMultiplexer;
class CNATFWUNSAFMessage;

/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( ut_cnatfwstunsrvimpl )
     : public CEUnitTestSuiteClass, public MNATFWStunSrvObserver,
       public MNcmConnectionMultiplexerObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cnatfwstunsrvimpl* NewL();
        static ut_cnatfwstunsrvimpl* NewLC();
        /**
         * Destructor
         */
        ~ut_cnatfwstunsrvimpl();

    private:    // Constructors and destructors

        ut_cnatfwstunsrvimpl();
        void ConstructL();

    public:     // From observer interface
    
    void STUNRequestReceivedL( const TInetAddr& aLocalAddr,
        const TInetAddr& aFromAddr, const TInetAddr& aPeerAddr,
        TUint aPriority, TBool aRemoteFavored );
        
    void RoleChangeNeeded( TNATFWIceRole /*aDesiredRole*/ ){ };    
        

    private:    // New methods

        void SetupL();

        void Teardown();
        
        HBufC8* EncodeMessageL( const CNATFWUNSAFMessage& aRequest,
                                const TBool aUseSharedSecret = ETrue,
                                const TBool aAddFingerPrint = ETrue );

        void ut_cnatfwstunsrvimpl_AddAuthenticationParamsLL();        

        void ut_cnatfwstunsrvimpl_RemoveAuthenticationParamsL();
        
        void ut_cnatfwstunsrvimpl_SetOperationModeLL();        

        void ut_cnatfwstunsrvimpl_IncomingMessageLL();

        void ut_cnatfwstunsrvimpl_IncomingMessageL2L();
        
        // private methods
        
        void ut_cnatfwstunsrvimpl_IsRoleConflictPresentL();
        
        void ut_cnatfwstunsrvimpl_SetPeerRemoteAddressL();
        
        void ut_cnatfwstunsrvimpl_DecodeMessageLL();
        
        void ut_cnatfwstunsrvimpl_EncodeMessageLL();

        void ut_cnatfwstunsrvimpl_CheckIntegrityLL();

        void ut_cnatfwstunsrvimpl_GetPasswordL();

        void ut_cnatfwstunsrvimpl_SendResponseLL();                           

        void ut_cnatfwstunsrvimpl_IsRequestValidLL();
        
        //MNATFWConnectionObserver
        void Notify( TUint /*aSessionId*/, TUint /*aStreamId*/,
        TNotifyType /*aType*/, TInt /*aError*/ ) {};
        
        //MNATFWConnectionMultiplexerObserver                   
        void IcmpError( TUint /*aSessionId*/, TUint /*aStreamId*/, const TInetAddr& /*aAddress*/ ) {};                   
        
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        CNATFWSTUNSrvImpl* iStunSrvImpl;

        CNcmConnectionMultiplexer* iConnMux;
    };

#endif      //  __UT_CNATFWSTUNSRVIMPL_H__

// End of file
