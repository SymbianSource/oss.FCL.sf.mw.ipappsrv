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


#ifndef __UT_CNSPCONTENTPARSER_H__
#define __UT_CNSPCONTENTPARSER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CNSPContentParser;
class TNSPStorage;

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
NONSHARABLE_CLASS( UT_CNSPContentParser )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        static UT_CNSPContentParser* NewL();
        static UT_CNSPContentParser* NewLC();
        ~UT_CNSPContentParser();

    private:    // Constructors and destructors

        UT_CNSPContentParser();
        void ConstructL();


    public: // New functions
    
        static void CleanupArray( TAny* anArray );
        

    private:    // New methods

        void SetupL();        
        void Teardown();
        
        void UT_NewLL();
        void UT_IsIceSupported();
        void UT_IsMismatchL();
        void UT_IsMismatchAttribute();
        void UT_IsLiteAttribute();
        void UT_FindRTCP();
        void UT_ModifyRTCPL();
        void UT_AddLiteL();
        void UT_AddMismatchL();
        void UT_AppendNumToTDes8L();
        void UT_AppendProtocolToTDes8L();
        void UT_AppendTInetAddrToTDes8L();
        void UT_AppendTypeToTDes8L();
        void UT_EndodeCandidateLineL();
        void UT_DecodeCandidateLineL();
        void UT_EndodeRemoteCandidatesLineL();
        void UT_DecodeRemoteCandidatesLineL();
        
        void UT_LexTPtrC8ToTUintL();
        void UT_SolveTransportProtocolL();
        void UT_InputTPtr8ToTInetAddrL();
        void UT_SolveCandidateTypeL();
        void UT_LeaveIfTooLongL();
        
    private:    // Data
    
    	TNSPStorage* iStorage;
        CNSPContentParser* iContentParser;
        
        EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_CNSPCONTENTPARSER_H__

// End of file
