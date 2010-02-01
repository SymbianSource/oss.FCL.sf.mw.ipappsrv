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




#ifndef __UT_CMccH263OLDPAYLOADENCODER_H__
#define __UT_CMccH263OLDPAYLOADENCODER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccH263OldPayloadEncoder;
class CBufferContainer;

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
NONSHARABLE_CLASS( UT_CMccH263OldPayloadEncoder )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccH263OldPayloadEncoder* NewL();
        static UT_CMccH263OldPayloadEncoder* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccH263OldPayloadEncoder();

    private:    // Constructors and destructors

        UT_CMccH263OldPayloadEncoder();
        void ConstructL();

 	public:
    	static void ResetAndDestroy( TAny* aAny );
    	
    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccH263OldPayloadEncoder_NewLL();
        
        
         void UT_CMccH263OldPayloadEncoder_EncodeLL();
        
        
         void UT_CMccH263OldPayloadEncoder_SetPayloadTypeL();
        
         void UT_CMccH263OldPayloadEncoder_WriteHeaderData();
         
         void UT_CMccH263OldPayloadEncoder_GetHeaderInfoL();

         
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccH263OldPayloadEncoder* iEncoder;
        RPointerArray<CBufferContainer> iBufContainer;

    };

#endif      //  __UT_CMccH263OLDPAYLOADENCODER_H__

// End of file
