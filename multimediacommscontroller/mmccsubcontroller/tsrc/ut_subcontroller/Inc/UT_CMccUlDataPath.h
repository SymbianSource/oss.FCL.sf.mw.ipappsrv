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




#ifndef __UT_CMccULDATAPATH_H__
#define __UT_CMccULDATAPATH_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/common/mmfcontrollerframework.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccUlDataPath;
class MDataSource;
class MDataSink;
class CMccResourcePoolStub;
class CAmrPayloadFormatWrite;

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
NONSHARABLE_CLASS( UT_CMccUlDataPath )
     : public CEUnitTestSuiteClass,
       public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccUlDataPath* NewL();
        static UT_CMccUlDataPath* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccUlDataPath();

    private:    // Constructors and destructors

        UT_CMccUlDataPath();
        void ConstructL();

    public:     // From observer interface

        TInt SendEventToClient(const TMMFEvent& /*aEvent*/)
	        {
	        return KErrNone;	
	        }

    private:    // New methods

         void SetupL();
        
         void Teardown();
           
         void UT_CMccUlDataPath_NegotiateLL();
        
        
         void UT_CMccUlDataPath_BufferEmptiedLL();
        
        
         void UT_CMccUlDataPath_BufferFilledLL();
        
        
         void UT_CMccUlDataPath_PrimeLL();
        
        
         void UT_CMccUlDataPath_PlayLL();
        
        
         void UT_CMccUlDataPath_PauseLL();
        
        
         void UT_CMccUlDataPath_StopLL();
        
        
         void UT_CMccUlDataPath_ResumeLL();
         
         void UT_CMccUlDataPath_RunLL();
        
         void UT_CMccUlDataPath_EmptySinkBuffer();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CMccUlDataPath* iUlPath;
        MDataSource* iSourceStub;
        MDataSink*   iSinkStub;
        CAmrPayloadFormatWrite* iWrite;
        CMccResourcePoolStub* iResources;

    };

#endif      //  __UT_CMccULDATAPATH_H__

// End of file
