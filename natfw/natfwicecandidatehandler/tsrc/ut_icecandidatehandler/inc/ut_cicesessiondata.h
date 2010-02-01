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




#ifndef __UT_CNATFWCANDIDATESTORAGE_H__
#define __UT_CNATFWCANDIDATESTORAGE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mnatfwpluginobserver.h"

//  FORWARD DECLARATIONS
class CIceSessionData;
class CNATFWCandidate;
class CNATFWCandidatePair;

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
NONSHARABLE_CLASS( UT_CIceSessionData )
     : public CEUnitTestSuiteClass
     
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIceSessionData* NewL();
        static UT_CIceSessionData* NewLC();
        /**
         * Destructor
         */
        ~UT_CIceSessionData();

    private:    // Constructors and destructors

        UT_CIceSessionData();
        void ConstructL();

    public:     // From observer interface
    
    private:    // New methods

         void SetupL();
         
         void Teardown();
        
         void UT_CNATFWCandidateStorage_AddPeerReflexiveCandidateLL();
         
         void UT_CNATFWCandidateStorage_FindLocalCandidate_1L();
        
         void UT_CNATFWCandidateStorage_FindLocalCandidate_2L();
         
         void UT_CNATFWCandidateStorage_FindRemoteCandidateL();
         
         void UT_CNATFWCandidateStorage_GetLocalCandidatesL();

         
         void UT_CNATFWCandidateStorage_GetLocalCandidates_1L();
        
        
         void UT_CNATFWCandidateStorage_GetLocalCandidates_2L();
        
        
         void UT_CNATFWCandidateStorage_DeleteCollectionL();
        
        
         void UT_CNATFWCandidateStorage_AddIdentificationLL();
         
         void UT_CNATFWCandidateStorage_IdentificationsL();
        
         void UT_CNATFWCandidateStorage_IdentificationL();
        
         void UT_CNATFWCandidateStorage_AddRemoteCandidateLL();
        
        
         void UT_CNATFWCandidateStorage_GetRemoteCandidatesLL();
        
        
         void UT_CNATFWCandidateStorage_AddRemoteCandidatesLL();
        
        
         void UT_CNATFWCandidateStorage_GetRemoteCandidatesL_1L();
        
        
         void UT_CNATFWCandidateStorage_AddLocalCandidateLL();
        
         
         void UT_CNATFWCandidateStorage_GetStreamCollectionIdsLL();
        
        
         void UT_CNATFWCandidateStorage_CreateCollectionL();
        
         void UT_CNATFWCandidateStorage_CollectionLL();
         
         void UT_CNATFWCandidateStorage_GetCollectionIdForComponentL();
         
         void UT_CNATFWCandidateStorage_CleanupStreamDataL();
         
         void UT_CNATFWCandidateStorage_RemoveLocalCandidatesL();
         
         void UT_CNATFWCandidateStorage_RemoveRemoteCandidatesL();
         
         void UT_CNATFWCandidateStorage_SetOperationModeLL();
         
         void UT_CNATFWCandidateStorage_OperationModeL();
         
         void UT_CNATFWCandidateStorage_TieBreakerL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CIceSessionData* iCandStorage;
    };

#endif      //  __UT_CNATFWCANDIDATESTORAGE_H__

// End of file
