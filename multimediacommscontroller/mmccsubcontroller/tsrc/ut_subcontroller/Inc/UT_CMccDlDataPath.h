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




#ifndef __UT_CMccDLDATAPATH_H__
#define __UT_CMccDLDATAPATH_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/common/mmfcontrollerframework.h>
//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccDlDataPath;
class MDataSource;
class MDataSink;
class CMccResourcePoolStub;
class CAmrPayloadFormatRead;

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
NONSHARABLE_CLASS( UT_CMccDlDataPath )
     : public CEUnitTestSuiteClass,
       public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccDlDataPath* NewL();
        static UT_CMccDlDataPath* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccDlDataPath();

    private:    // Constructors and destructors

        UT_CMccDlDataPath();
        void ConstructL();

    public: // From observer interface MAsyncEventHandler

        TInt SendEventToClient(const TMMFEvent& /*aEvent*/)
	        {
	        return KErrNone;	
	        }

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccDlDataPath_NegotiateLL();
        
        
         void UT_CMccDlDataPath_BufferFilledLL();
        
        
         void UT_CMccDlDataPath_BufferEmptiedLL();
        
        
         void UT_CMccDlDataPath_PrimeLL();
        
        
         void UT_CMccDlDataPath_PlayLL();
        
        
         void UT_CMccDlDataPath_PauseLL();
        
        
         void UT_CMccDlDataPath_StopLL();
        
        
         void UT_CMccDlDataPath_ResumeLL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CMccDlDataPath*        iDlPath;
        MDataSource*           iSourceStub;
        MDataSink*             iSinkStub;
        CAmrPayloadFormatRead* iRead;
        CMccResourcePoolStub*  iResources;

    };

#endif      //  __UT_CMccDLDATAPATH_H__

// End of file
