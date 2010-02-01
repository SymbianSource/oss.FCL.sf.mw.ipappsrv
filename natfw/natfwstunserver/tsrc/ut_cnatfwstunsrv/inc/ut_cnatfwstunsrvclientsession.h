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




#ifndef __UT_CNATFWSTUNSRVCLIENTSESSION_H__
#define __UT_CNATFWSTUNSRVCLIENTSESSION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "mncmconnectionmultiplexerobserver.h"

//  INTERNAL INCLUDES
#include "natfwstunsrvobserver.h"

//  FORWARD DECLARATIONS
class CNATFWSTUNSrvClientSession;
class CNcmConnectionMultiplexer;

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
NONSHARABLE_CLASS( ut_cnatfwstunsrvclientsession )
     : public CEUnitTestSuiteClass, public MNATFWStunSrvObserver,
       public MNcmConnectionMultiplexerObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cnatfwstunsrvclientsession* NewL();
        static ut_cnatfwstunsrvclientsession* NewLC();
        /**
         * Destructor
         */
        ~ut_cnatfwstunsrvclientsession();

    private:    // Constructors and destructors

        ut_cnatfwstunsrvclientsession();
        void ConstructL();

    public:     // From observer interface
    
        void STUNRequestReceivedL( const TInetAddr& aLocalAddr,
            const TInetAddr& aFromAddr, const TInetAddr& aPeerAddr,
            TUint aPriority, TBool aRemoteFavored );
            
        void RoleChangeNeeded( TNATFWIceRole /*aDesiredRole*/ ){ };    

        //MNATFWConnectionObserver
        void Notify( TUint /*aSessionId*/, TUint /*aStreamId*/,
            TNotifyType /*aType*/, TInt /*aError*/ ) {};
            
        //MNATFWConnectionMultiplexerObserver                   
        void IcmpError( TUint /*aSessionId*/, TUint /*aStreamId*/, const TInetAddr& /*aAddress*/ ) {};            
            
    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CNATFWSTUNSrvClientSession_AddAuthenticationParamsL();
        
        
         void UT_CNATFWSTUNSrvClientSession_RemoveAuthenticationParamsL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        CNcmConnectionMultiplexer* iConnMux;
        CNATFWSTUNSrvClientSession* iClientSession;

    };

#endif      //  __UT_CNATFWSTUNSRVCLIENTSESSION_H__

// End of file
