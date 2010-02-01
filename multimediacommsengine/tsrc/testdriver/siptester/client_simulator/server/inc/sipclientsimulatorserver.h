/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The server's main class
*
*/

#ifndef CSIPCLIENTSIMULATORSERVER_H
#define CSIPCLIENTSIMULATORSERVER_H

//  INCLUDES
#include "sipclientsimulatorcli.h"
#include <e32base.h>

//  FORWARD DECLARATIONS
class CSipClientSimulator;
class CSdpMediaField;
class CSIPContentTypeHeader;

//  CLASS DEFINITION
/**
* CSipClientSimulatorServer is the main server class responsible for starting
* and running the SIP Client Simulator server.
*/
class CSipClientSimulatorServer : public CServer2
    {
    public: // Constructors and destructor

		/**
		* Constructor, leaves pointer to cleanup-stack
		* @return An initialised instance of this class.
		*/
		static CSipClientSimulatorServer* NewLC();

		/**
		* Destructor.
		*/
		~CSipClientSimulatorServer();

	public: // New functions

		/**
		* Server entry point method (Process entry point).
		* Starts the server up. Creates a cleanupstack and activescheduler.
		*/
		static TInt Main();

		/// Called by a session when it is created. Increments iSessionCount
		void SessionAdded();

		/// Called by a session when it is destroyed. Decrements iSessionCount.
		/// Stops the ActiveScheduler if there are no more sessions.
		void SessionRemoved();

		/// Called by ThreadMain, starts and initialises the server.
		/// This function will block for the lifetime of the server.
		static void InitServerL();

        void CreateClientSimulatorL( const TUid& aClientUid );
        
        void DestroyClientSimulator();

        void SetClientMatchType(
            RSipClientSimulator::TMatchType aMatchType,
            const TUid& aClientUid );
            
        void SetClientSupportedContentTypesL(
    		    RPointerArray<CSIPContentTypeHeader> aContentTypeHeaders,
    			const TUid& aClientUid );
    			
    	RPointerArray<CSIPContentTypeHeader>& 
    								ClientSupportedContentTypesL();
    			
    	void SetClientSupportedSdpMediasL(
    		    RPointerArray<CSdpMediaField> aSdpMediaFields,
    			const TUid& aClientUid );
    			
    	RPointerArray<CSdpMediaField>& ClientSupportedSdpMediasL();
    
        
        RSipClientSimulator::TMatchType ClientMatchType() const;
        
        const TUid& ClientUid() const;

    private: // Constructors

        CSipClientSimulatorServer();
        void ConstructL();

	private: // From CActive

		TInt RunError( TInt aError );

	private: // From CServer

	    CSession2* NewSessionL( const TVersion& aVersion,
	    						const RMessage2& aMessage ) const;

    private: // New functions
    
        static void PanicServer( TInt aPanic );

	private: // Data

		/// Number of active sessions.
		TInt iSessionCount;
        RSipClientSimulator::TMatchType iMatchType;
        TUid iClientUid;
		CSipClientSimulator* iSimulator;
		RPointerArray<CSIPContentTypeHeader> iContentTypeHeaders;
		RPointerArray<CSdpMediaField> iSdpMediaField;
    };

#endif // CSIPCLIENTSIMULATORSERVER_H
