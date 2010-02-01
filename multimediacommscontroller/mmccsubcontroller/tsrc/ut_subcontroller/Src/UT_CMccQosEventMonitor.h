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




#ifndef __UT_CMCCQOSEVENTMONITOR_H__
#define __UT_CMCCQOSEVENTMONITOR_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <es_sock.h>

//  INTERNAL INCLUDES
#include "mccqoseventobserver.h"

//  FORWARD DECLARATIONS
class CMccQosEventMonitor;

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
NONSHARABLE_CLASS( UT_CMccQosEventMonitor )
     : public CEUnitTestSuiteClass, public MMccQosEventObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccQosEventMonitor* NewL();
        static UT_CMccQosEventMonitor* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccQosEventMonitor();

    private:    // Constructors and destructors

        UT_CMccQosEventMonitor();
        void ConstructL();

    public:     // From observer interface

        void QoSParamEvent( TInt /*aError*/, TUint32 /*aEvent*/ ) {};

        void QoSConnEvent( TInt /*aError*/, TUint32 /*aEvent*/ ) {};        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccQosEventMonitor_NewLL();
         
         void UT_CMccQosEventMonitor_StartListenLL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
         
        CMccQosEventMonitor* iMonitor;
        TRequestStatus iStatus; 
        RConnection iConn;
        RSocketServ iSocketServ;
        RSocket iSocket;
        RSubConnection iSubConnection;
                
    };

#endif      //  __UT_CMCCQOSEVENTMONITOR_H__

// End of file
