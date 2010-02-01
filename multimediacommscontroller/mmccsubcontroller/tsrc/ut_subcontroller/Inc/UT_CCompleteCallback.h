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




#ifndef __UT_CCOMPLETECALLBACK_H__
#define __UT_CCOMPLETECALLBACK_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/common/mmfcontrollerframework.h>

//  INTERNAL INCLUDES
#include "MccInternalDef.h"
#include "mcculdatapath.h"
#include "mccdldatapath.h"
#include "mcccompletecallback.h"
#include "Mcctesteventhandler.h"

//  FORWARD DECLARATIONS
class CCompleteCallback;
class CMccResourcePoolStub;

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
NONSHARABLE_CLASS( UT_CCompleteCallback )
     : public CEUnitTestSuiteClass,
       public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CCompleteCallback* NewL();
        static UT_CCompleteCallback* NewLC();
        /**
         * Destructor
         */
        ~UT_CCompleteCallback();

    private:    // Constructors and destructors

        UT_CCompleteCallback();
        void ConstructL();

    public:     // from MAsyncEventHandler

        TInt SendEventToClient(const TMMFEvent &aEvent);

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CCompleteCallback_CCompleteCallbackL();
        
        
         void UT_CCompleteCallback_SignalDataPathCompleteL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CMccUlDataPath* iDataPath;
        CCompleteCallback* iCallback;
        CMccResourcePoolStub* iResources;

    };

#endif      //  __UT_CCOMPLETECALLBACK_H__

// End of file
