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
* Description:  See below
*
*/

#ifndef RSIPCLIENTSIMULATOR_H
#define RSIPCLIENTSIMULATOR_H

//	INCLUDES
#include <e32base.h>

class CSdpMediaField;
class CSIPContentTypeHeader;
class RReadStream;
class RWriteStream;
//	CLASS DEFINITION
/**
 * RSipClientSimulator implements the client-side endpoint of a 
 * SIP Client Simulator client-server session.
 */
class RSipClientSimulator : public RSessionBase
	{
	public: // Enumerations
	
        /** Simulated client match types */
        enum TMatchType
            {
			/** No match */
			ESipNoClientMatch = 0,            
            /** Accept-Contact-header match */
			ESipAcceptContactHeaderMatch,
			/** Event-header match */
			ESipEventHeaderMatch,
			/** Match by the whole request */
			ESipRequestMatch
            };	    
	
	public: // Constructors

		/// Default constructor
		IMPORT_C RSipClientSimulator();

	public: // from RHandleBase

		/// Closes the connection to the server.
		/// Stops watching for this client.
		IMPORT_C void Close();

	public:	// New methods

		/**
		* Establishes a connection to the SIP Client Simulator server.
		* The server is automatically started 
		* if it was not already up and running.
		*/
		IMPORT_C TInt Connect();

		/**
		* Sets the simulated client match type.
		* @param aMatchType the type of the simulated match
		* @param aClientUid the simulated client UID 
		* @return KErrNone or KErrNoMemory  
		*/
        IMPORT_C TInt SetClientMatchType( 
            TMatchType aMatchType, 
            const TUid& aClientUid );
            
        IMPORT_C TInt SetClientSupportedContentTypesL( 
           RPointerArray<CSIPContentTypeHeader> aContentTypeHeaders,
           const TUid& aClientUid );

		IMPORT_C TInt SetClientSupportedSdpMediasL( 
           RPointerArray<CSdpMediaField> aSdpMediaFields,
           const TUid& aClientUid );

		/**
		* Gets the simulated client match type.
		* @param aMatchType the type of the simulated match
		* @param aClientUid the simulated client UID
		* @return KErrNone or KErrNoMemory  
		*/        
        IMPORT_C TInt GetClientMatchType( 
            TMatchType& aMatchType,
            TUid& aClientUid );
		
		
		/**
		* Gets the simulated client SupportedContentTypesL.
		* @param aClientUid the simulated client UID
		* @return  
		*/        
        IMPORT_C RPointerArray<CSIPContentTypeHeader> 
        				ClientSupportedContentTypesL( TUid& aClientUid );
        				
        /**
		* Gets the simulated client SupportedMediaFieldsL.
		* @param aClientUid the simulated client UID
		* @return  
		*/        
        IMPORT_C RPointerArray<CSdpMediaField> 
        				ClientSupportedMediaFieldsL( TUid& aClientUid );
        				
		/**
		* Forces the SIP Client Simulator server to create 
		* a new CSIP instance with the given client UID.
		* There is 0..1 CSIP instances in the server.
		* If this function is called twice, 
		* there will still be only one instance of CSIP 
		* with the client UID given in the second call. 
		* @param aClientUid the simulated client UID 
		* @return KErrNone or KErrNoMemory 
		*/
        IMPORT_C TInt ConnectClient( const TUid& aClientUid );
        
		/**
		* Deletes the CSIP instance in the server. 
		*/         
        IMPORT_C TInt DisconnectClient();
        
        
	private: // New methods

		void StartServerL();
		TBool IsServerStarted() const;
		CBufFlat* ExternalizeLC(
	                RPointerArray<CSIPContentTypeHeader> aContentTypeHeaders );
	                
	    CBufFlat* ExternalizeLC(
	                RPointerArray<CSdpMediaField> aSdpMediaFields );
	                
	    void InternalizeArrayL(	
    				RPointerArray<CSIPContentTypeHeader>& aContentTypeHeaders,
    				RReadStream& aStream);							 
    								 
    	void InternalizeArrayL(	
    				RPointerArray<CSdpMediaField>& aSdpMediaFields,
    				RReadStream& aStream);

	private: // Data

		/// ETrue if there is an open connection to the server, 
		//  EFalse otherwise.
		TBool iIsConnected;
    };

#endif  // RSIPCLIENTSIMULATOR_H
