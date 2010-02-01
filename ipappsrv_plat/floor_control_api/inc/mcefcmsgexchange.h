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




#ifndef CMCEFCMSGEXCHANGE_H
#define CMCEFCMSGEXCHANGE_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MDesC8Array;
class CDesC8Array;
class MMceFcMsgObserver;
class CMceSession;
class CMceManager;
class CMceItcSender;

// CLASS DECLARATION

/**
* The class of the MCE Floor Control.
*
* It allows sending of floor control messages into
* network.
*
* Class MMceFcMsgObserver is used for receiving messages
* from network.
*
*  @lib mceclient.lib
*/
class CMceFcMsgExchange : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @param aManager Reference to manager.
        * @param aType Type identifier of messages, like 'TBCP'.
        */
        IMPORT_C static CMceFcMsgExchange* NewL(
				CMceManager& aManager,
				const TDesC8& aType );
        
        /**
        * Destructor.
        */
        virtual ~CMceFcMsgExchange();

    public: // New functions
        
        /**
        * Initializes floor control for the session.
        * @param aSession Reference to session.
        * @param aPort Port to be used.
        * @param aObserver Observer of the client.
        */
		IMPORT_C void InitializeL(
		             CMceSession& aSession,
		             TInt aPort,
		             MMceFcMsgObserver& aObserver );
        
        /**
        * Sends floor control message to recipient.
        * @param aSession Reference to session.
        * @param aMessage Floor control message to be sent. Ownership is
		*	     transferred.
        */
        IMPORT_C void SendL( CMceSession& aSession, HBufC8* aMessage );

		/**
		* Gets attribute lines of the media.
        * @param aSession Reference to session.
		* @return array of media attribute lines, owneship is transferred.
		*/
		IMPORT_C MDesC8Array* MediaAttributeLinesL( CMceSession& aSession );

        /**
		* Updates SDP media attribute lines. In order to get complete update,
		* whole session must be updated causing sending of re-invite.
        * @param aSession Reference to session.
        * @param aMediaAttributeLines Media specific SDP attribute lines.
		*		 Ownership is transferred.
        */
		IMPORT_C void UpdateL(
					 CMceSession& aSession,
					 CDesC8Array* aMediaAttributeLines );
    private:

        /**
        * C++ default constructor.
        * @param aManager Reference to manager.
        */
        CMceFcMsgExchange( CMceManager& aManager );

        /**
        * 2nd phase constructor.
        * @param aType Type identifier of messages, like 'TBCP'.
        */
        void ConstructL( const TDesC8& aType );
        
    private:    // Data
    
    	CMceManager& iManager;
    	
    	HBufC8* iType;
    	
    	CMceItcSender* iSender;
    };

#endif        
            
// End of File
