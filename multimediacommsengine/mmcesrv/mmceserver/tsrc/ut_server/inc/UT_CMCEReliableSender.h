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



#ifndef __UT_CMCERELIABLESENDER_H__
#define __UT_CMCERELIABLESENDER_H__


/* EXTERNAL INCLUDES */
#include <digia/eunit/ceunittestsuiteclass.h>
#include "mcereliablesender.h"
#include "mcereliableobserver.h"

/*
 * INTERNAL INCLUDES ;
 * FORWARD DECLARATIONS
 */
class   CMceReliableSender;
class	CRepository;

/* CLASS DEFINITION */


// =======================================================================================================================
// EUnitWizard generated test class.
// =======================================================================================================================
class UT_CMceReliableSender :
	public CEUnitTestSuiteClass,
	public MMceReliableObserver 
    {




// =======================================================================================================================
// Function from observer interface.
// =======================================================================================================================
//
    public: 

    void NoPrackReceived( CSIPServerTransaction& aTransaction );
    void ReliableFailed( CSIPServerTransaction& aTransaction, TInt aError );


// =======================================================================================================================
// Constructor and Destructor Definition.  
// =======================================================================================================================
//
    public: // Constructors and destructors 
	    
	    //  Two phase construction  
	    static UT_CMceReliableSender* NewL();
	    static UT_CMceReliableSender* NewLC();

	    //  Destructor  
	    ~UT_CMceReliableSender();

        
// =======================================================================================================================
// Second phase constructor.
// =======================================================================================================================
//
    private:	// Constructors and destructors 
        UT_CMceReliableSender();
	    void ConstructL();



// =======================================================================================================================
// Testing functions. 
// =======================================================================================================================
//
    private:	
	    void SetupL();   // Constructing Dunction Definition.
	    void SetupL1();
	    void Teardown(); // Destroying Function Definition.
	    void UT_CMceReliableSender_GetRseq();
	    void UT_CMceReliableSender_SendResponseL();
	    void UT_CMceReliableSender_PrackReceivedL();
	    void UT_CMceReliableSender_PrackReceivedL1();
	    void UT_CMceReliableSender_timeOut();
	    
	    EUNIT_DECLARE_TEST_TABLE;


// =======================================================================================================================
// Private Data
// =======================================================================================================================
//
    private:	/* Data */
    
        CMceReliableSender* iReliableSender;    
        TInt	iT1;
	    TUint	iSequenceNumber;
	    TUint	iTimerMaxCount;
	    TInt	iTimerCount;
	    TInt	iTimerVal;
	    TBool   iTransactionStatus;
	    TInt    iTimeOut;
	    

    };

    
#endif /* __UT_CMceRELIABLESENDER_H__ */

/* End of file */