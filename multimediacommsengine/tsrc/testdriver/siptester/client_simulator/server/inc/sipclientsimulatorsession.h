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
* Description:  Server side Client-Server session
*
*/

#ifndef CSIPCLIENTSIMULATORSESSION_H
#define CSIPCLIENTSIMULATORSESSION_H

//  INCLUDES
#include "sipclientsimulatorconsts.h"
#include <e32base.h>

//  FORWARD DECLARATIONS
class CSipClientSimulatorServer;

//  CLASS DEFINITION
/**
* CSipClientSimulatorSession implements the client-server session
* of the SIP Client Simulator Server.
*/
class CSipClientSimulatorSession : public CSession2
    {
    public: // Constructors and destructor

		/**
		* Constructor.
		* @param aServer the server object owning this session.
		* @return An initialised instance of this class.
		*/
		static CSipClientSimulatorSession* NewL( 
		    CSipClientSimulatorServer& aServer );

		/**
		* Destructor.
		*/
		~CSipClientSimulatorSession();

	public: // from CSession2

		void ServiceL( const RMessage2& aMessage );

    private: // Constructors

        CSipClientSimulatorSession( CSipClientSimulatorServer& aServer );
        void ConstructL();
       
    private: // New functions

        void SetClientMatchTypeL( const RMessage2& aMessage );
        
        void SetClientSupportedContentTypesL( const RMessage2& aMessage );
        
        HBufC8* GetIpcArgDesValueLC( TSipClientSimulatorIpcArgs aIpcArgIndex,
    								 const RMessage2&  aMessage ) const;
    								 
    	void InternalizeArrayL(	
    				RPointerArray<CSIPContentTypeHeader>& aContentTypeHeaders,
    				RReadStream& aStream);							 
    								 
    	void InternalizeArrayL(	
    				RPointerArray<CSdpMediaField>& aSdpMediaFields,
    				RReadStream& aStream);
        
        void SetClientSupportedSdpMediasL( const RMessage2& aMessage );
        
        void GetClientMatchTypeL( const RMessage2& aMessage );
        
        void GetClientSupportedContentTypesL( const RMessage2& aMessage );
        
        void GetClientSupportedSdpMediasL( const RMessage2& aMessage );

        void ConnectClientL( const RMessage2& aMessage );
        
        void DisconnectClient();

        void ReadUidL( 
            TUid& aValue,
            const RMessage2& aMessage ) const;

        void GetIpcArgIntValueL(
            TSipClientSimulatorIpcArgs aIpcArgIndex,
            TInt& aValue,
            const RMessage2&  aMessage) const;
            
        void WriteToClientL(
            TSipClientSimulatorIpcArgs aIpcArgIndex,
            TInt aValue,
            const RMessage2& aMessage) const; 
            
        /**
        * Writes serialized object as return value 
		* @param aItcArgIndex index of argument
		* @param aMessage handle for ITC communication
		* @param aDes serialized object
        */
		void WriteL (TSipClientSimulatorIpcArgs aArg, const RMessage2& aMessage, 
					const TDesC8& aDes);	
					
		CBufFlat* ExternalizeLC(
	                RPointerArray<CSIPContentTypeHeader> aContentTypeHeaders );
	                
	    CBufFlat* ExternalizeLC(
	                RPointerArray<CSdpMediaField> aSdpMediaFields );
				  						  
    

	private: // Data

		CSipClientSimulatorServer& iMyServer;
    };

#endif // CSIPCLIENTSIMULATORSESSION_H
