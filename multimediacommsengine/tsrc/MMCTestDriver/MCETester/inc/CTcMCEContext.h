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
* Description:    See class definition below.
*
*/



#ifndef __CTCSIPCONTEXT_H__
#define __CTCSIPCONTEXT_H__

// INCLUDES
#include "CTcContextBase.h"
#include "CTcIAPManager.h"

#include "CTcMCEProfileContainer.h"
#include "CTcMCEReceived.h"
#include <MCEManager.h>
#include <MCEFCMsgExchange.h>
#include <MCETransactionDataContainer.h>

#include "SIPObserver.h"

// FORWARD DECLARATIONS

class CTcMCEReceiveQueue;
class CTcMCEMultimediaAPIObserver;
class CTcMCEEventAPIObserver;
class CTcMCEFloorControlObserver;

// CLASS DEFINITION
/**
 * CTcMCEContext implements the test context (MTcTestContext) for MCE testing.
 */
class CTcMCEContext
	: public CTcContextBase, 
	  public MSIPObserver
	{
	public:	// Constructors and destructor

		/**
		 * Static constructor. Leaves pointer to cleanup stack.
		 *
		 * @param aCodec Reference to CTRL codec object,
		 *				 initialized to a valid request.
		 * @param aIAPId IAPId as selected by user from TC settings.
    
		 * @return An initialized instance of this class.
		 */
		static CTcMCEContext* NewLC( CTcCTRLCodec& aCodec,
									 TInt aIAPId );

		/// Destructor
		~CTcMCEContext();
    
					
	public: // from MSIPObserver
	
		void IncomingRequest(TUint32 aIapId,
					         CSIPServerTransaction* aTransaction);

		void TimedOut(CSIPServerTransaction& aTransaction);
					
	private:

		/**
		 * Constructor.
		 *
		 * @param aCodec Reference to CTRL codec object,
		 *				 initialized to a valid request.
		 */
		CTcMCEContext( CTcCTRLCodec& aCodec );

		/// Default constructor. Not implemented.
		CTcMCEContext();

		/**
		 * 2nd phase constructor
		 *
		 * @param aIAPId IAPId as selected by user from TC settings.
         * @param aIsMandatory  used for determining if the SIP stack 
         *        is automatically started. If default value (ETrue) is used, 
         *        then SIP stack is started automatically.
		 */
		void ConstructL( TInt aIAPId );
		

	protected: // From CTcContextBase

		TTcCommandBase* CreateCommandL( TTcIdentifier& aId,
										MTcTestContext& aContext );

    
	public: // New methods

		/// @return Reference to MCEManager.
		inline CMceManager& MCEManager()
				{ return *iMCEManager; };

		/// @return Reference to the SIP stack.
		inline CSIP& SIP()
				{ return *iSip; };
				
        /// @return Reference to the default SIP profile.
        inline CTcMCEProfileContainer& Profile()
                { return *iSipProfile; };
             
        /// @return Reference to the floor controller
        inline CMceFcMsgExchange& FC()
                { return *iFC; };
                
        /// @return Reference to the floor control observer
        inline CTcMCEFloorControlObserver& FCObserver()
        		{ return *iFCObserver; };

		/// @return Reference to the multimedia observer
		inline CTcMCEMultimediaAPIObserver& MultimediaObserver()
			{
			return *iMMObserver;
			}

		/// @return Reference to the event observer
		inline CTcMCEEventAPIObserver& EventObserver()
			{
			return *iEventObserver;
			}

    public: //New methods, received queue handling
		/**
		 * Returns the first item off the session receive queue (FIFO). 
		 * If no items are present, the function will wait for aTimeout seconds
		 * and then try fetching again.
		 *
		 * @param aTimeOut Time in seconds waited for items to arrive.
		 */
		CTcMCEReceived& ReceivedSessionItemL( TInt aTimeout );

		CTcMCEReceived& ReceivedUpdatedSessionItemL( TInt aTimeout );
		/**
		 * Returns the first item off the RTP receive queue (FIFO). 
		 * If no items are present, the function will wait for aTimeout seconds
		 * and then try fetching again.
		 *
		 * @param aTimeOut Time in seconds waited for items to arrive.
		 */
		CTcMCEReceived& ReceivedRTPItemL( TInt aTimeout );	

		/**
		 * Returns the first item off the Stream receive queue (FIFO). 
		 * If no items are present, the function will wait for aTimeout seconds
		 * and then try fetching again.
		 *
		 * @param aTimeOut Time in seconds waited for items to arrive.
		 */
		CTcMCEReceived& ReceivedStreamItemL( TInt aTimeout );

		/**
		 * Returns the first item off the Event receive queue (FIFO). 
		 * If no items are present, the function will wait for aTimeout seconds
		 * and then try fetching again.
		 *
		 * @param aTimeOut Time in seconds waited for items to arrive.
		 */
		CTcMCEReceived& ReceivedEventItemL( TInt aTimeout );

		/**
		 * Returns the first item off the Floor control receive queue (FIFO). 
		 * If no items are present, the function will wait for aTimeout seconds
		 * and then try fetching again.
		 *
		 * @param aTimeOut Time in seconds waited for items to arrive.
		 */
		CTcMCEReceived& ReceivedFCItemL( TInt aTimeout );
			

	private: // New methods


	private: // data

		// Floor controller, owned
		CMceFcMsgExchange* iFC;	
	
		// MCE Manager, owned
		CMceManager* iMCEManager;
		
		// MultimediaObserver, owned
		CTcMCEMultimediaAPIObserver* iMMObserver;
		
		// Floor control observer, owned
		CTcMCEFloorControlObserver* iFCObserver;
		
		// Event API observer, owned
		CTcMCEEventAPIObserver* iEventObserver;		
		
		/// SIP stack. Owned.
		CSIP* iSip;

		/// Default SIP profile. Owned.
		CTcMCEProfileContainer* iSipProfile;

		TMceTransactionDataContainer iDataContainer;

		TInt iIAPId;
	};

#endif // __CTCSIPCONTEXT_H__
