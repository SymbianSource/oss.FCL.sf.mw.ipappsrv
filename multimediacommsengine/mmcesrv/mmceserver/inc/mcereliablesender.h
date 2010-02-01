/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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





#ifndef CMCERELIABLESENDER_H
#define CMCERELIABLESENDER_H

#include <e32base.h>
#include <sipservertransaction.h> 

#include "mcereliableobserver.h"
#include "mcesrv.h"


// CLASS DECLARATION 
/**
 *  MCE Reliable Sender
 *
 *  @lib 
 *  @since
 */
class CMceReliableSender : public CBase
    
	{
    public:  //Construtor and Destructor
    	
        // Two-phase Constructor
         static CMceReliableSender* NewL( MMceReliableObserver& aObserver );
	    						
		// Destructor.
		 ~CMceReliableSender( );
        		
    public:     // New Functions
    
       /**
        * Get an RSeq Value
        * @param aTransaction CSIPServerTransaction 
        * @return TUint Rseq Value
        **/
         
        TUint Rseq( CSIPServerTransaction& aTransaction );
        
       /**
        * Send Reliable Provisional Responses
        * @param aTransaction CSIPServerTransaction 
        * @param aElements Response Elements.
        **/
	   					
		void SendResponseL( CSIPServerTransaction& aTransaction, 
		                    CSIPResponseElements* aElements );
		
	   /**
        * Match PRACK agains received responses.
        * @param aPrack Server Transaction.
        * @return TBool ETrue if aPrack matched.
        **/
		
		TBool PrackReceived( CSIPServerTransaction& aPrack );
		
    private:    //New Functions

       /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
                                                    
        
       /**
        * C++ default constructor
        */
        CMceReliableSender( MMceReliableObserver& aObserver );
        
        
        void TimeOut( CSIPServerTransaction* aTransaction );
	    	    
        void CloneAndSendResponseL( );
        
        	    
	private: // New Functions
	     	
        static TInt TimerExpire( TAny* aSelf );						
	    
	   	static CSIPResponseElements* ResponseCloneL( 
	   	                                CSIPResponseElements& aElements );

    
    private: // data

       	TInt  iT1;
       	TUint iSequenceNumber;
		TInt  iTimerVal;
		TInt  iRetransTimer;
		TUint iTotaltime;
		
		CSIPServerTransaction* iServerTransaction;

		TBool iTransactionStatus;
		TBool iTransactionMatched;
		TBool iTransTimeoutFlag;
	    
					
		CSIPResponseElements* iResponseElements;

		MMceReliableObserver* iObserver;
		
		// Delta Timer Active Object 
		CDeltaTimer* iDTimer;
		
		// Create TDeltaTimerEntry object 
        TDeltaTimerEntry iDeltaEntry;
                
        //definitions for unit testing
	    MCESRV_UT_DEFINITIONS
        
        };
#endif      // CMCERELIABLESENDE_H
            
// End of File

