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




#ifndef __UT_MNATFWSOCKETMEDIACONNWRAPPERIF_H__
#define __UT_MNATFWSOCKETMEDIACONNWRAPPERIF_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "natfwsocketmediaconnwrapper.h"
#include "natfwmediawrapperobserver.h"

//  FORWARD DECLARATIONS
class MNATFWSocketMediaConnWrapper;

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
NONSHARABLE_CLASS( UT_MNATFWSocketMediaConnWrapper )
     : public CEUnitTestSuiteClass,
       public MNATFWMediaWrapperObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_MNATFWSocketMediaConnWrapper* NewL();
        static UT_MNATFWSocketMediaConnWrapper* NewLC();
        /**
         * Destructor
         */
        ~UT_MNATFWSocketMediaConnWrapper();

    private:    // Constructors and destructors

        UT_MNATFWSocketMediaConnWrapper();
        void ConstructL();

    public:     
        
        // From observer class MNATFWMediaWrapperObserver
        void ConnectingCompleted( const TInt aError );
        
        void SendingCompleted( const TInt aError );
        
        TInt ReceivedFrom( const TUint aStreamId, /*const TUint aLength*/TDes8& aReceiveBuffer );
        
        void GetNewFreePort( TUint& aPort );

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_MNATFWSocketMediaConnWrapperIF_OutgoingAddrL();
        
        
         void UT_MNATFWSocketMediaConnWrapperIF_SetIncomingAddrLL();
        
        
         void UT_MNATFWSocketMediaConnWrapperIF_StreamIdL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CNATFWSocketMediaConnWrapper *iMediaConnWrapper;
        MNATFWMediaWrapper *iMediaWrapperIF;
        MNATFWSocketMediaConnWrapper *iSocketMediaConnWrapperIF;
        RSocketServ iSocketSrv;

    };

#endif      //  __UT_MNATFWSOCKETMEDIACONNWRAPPERIF_H__

// End of file
