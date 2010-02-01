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




#ifndef __UT_CMCCQOSHANDLER_H__
#define __UT_CMCCQOSHANDLER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mccqoseventobserver.h"

//  FORWARD DECLARATIONS
class CMccQosHandler;

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
NONSHARABLE_CLASS( UT_CMccQosHandler )
     : public CEUnitTestSuiteClass, public MMccQosEventObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccQosHandler* NewL();
        static UT_CMccQosHandler* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccQosHandler();

    private:    // Constructors and destructors

        UT_CMccQosHandler();
        void ConstructL();

    public:     // From observer interface

        void QoSParamEvent( TInt /*aError*/, TUint32 /*aEvent*/ ) {};

        void QoSConnEvent( TInt /*aError*/, TUint32 /*aEvent*/ ) {};
        
    private:    // New methods

         void SetupL();         
        
         void Teardown();
                 
        
         void UT_CMccQosHandler_EnableQosLL();
        
        
         void UT_CMccQosHandler_DisableQoSL();
        
        
         void UT_CMccQosHandler_SetSubConnectionParamsL();
        
        
         void UT_CMccQosHandler_QoSEnabledL();
         
         
         void UT_CMccQosHandler_HandleSocketHandlerDeletionL( );
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
         
        CMccQosHandler* iHandler;
        TRequestStatus iStatus; 
        RConnection iConn;
        RSocketServ iSocketServ;
        RSocket iSocket;
    };

#endif      //  __UT_CMCCQOSHANDLER_H__

// End of file
