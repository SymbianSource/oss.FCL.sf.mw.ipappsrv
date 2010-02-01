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


#ifndef __UT_CNSPMEDIASTREAMCONTAINER_H__
#define __UT_CNSPMEDIASTREAMCONTAINER_H__

class CNSPController;
class CNSPMediaStreamContainer;
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
NONSHARABLE_CLASS( UT_CNSPMediaStreamContainer )
     : public CEUnitTestSuiteClass
    {
public: // Constructors and destructors

    static UT_CNSPMediaStreamContainer* NewL();
    static UT_CNSPMediaStreamContainer* NewLC();
    ~UT_CNSPMediaStreamContainer();

private: // Constructors and destructors

    UT_CNSPMediaStreamContainer();
    void ConstructL();


private: // New methods

    // Setup & Teardown
    void SetupL();
    void Teardown();
    
    // Method tests
    void UT_CreateStreamsL();
    void UT_RemoveStreamsL();
    void UT_RemoveStreamL();
    void UT_GetCandidatesL();
    void UT_GetCredentialsL();
    
private: // Data
    
	TNSPStorage* iStorage; // not own
    CNSPController* iController; // own
    CNSPMediaStreamContainer* iContainer; // own
    
    EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_CNSPMEDIASTREAMCONTAINER_H__

// End of file
