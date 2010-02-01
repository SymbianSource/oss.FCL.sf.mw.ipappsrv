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


#ifndef __UT_NSPUTIL_H__
#define __UT_NSPUTIL_H__

class CNSPController;
class TNSPStorage;

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

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
NONSHARABLE_CLASS( UT_NSPUtil )
     : public CEUnitTestSuiteClass
    {
public: // Constructors and destructors

    static UT_NSPUtil* NewL();
    static UT_NSPUtil* NewLC();
    ~UT_NSPUtil();

private: // Constructors and destructors

    UT_NSPUtil();
    void ConstructL();

private: // New methods

    // Setup & Teardown
    void SetupL();
    void Teardown();
    
    // Method tests
    void UT_UpdateConnectionFieldL();
    void UT_UpdateOriginFieldL();
    void UT_RejectLL();
    void UT_IsRejectL();
    void UT_IsUnspecifiedL();

private: // Data

	TNSPStorage* iStorage;
    CNSPController* iController; // own
    EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_NSPUTIL_H__

// End of file
