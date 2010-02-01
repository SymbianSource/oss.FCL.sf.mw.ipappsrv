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
* Description:  
*
*/

#ifndef CTTCN2GRESOLVERPLUGIN_H
#define CTTCN2GRESOLVERPLUGIN_H

// INCLUDES
#include "sipclientsimulatorcli.h"
#include <sipresolvedclient2.h>


// CONSTANTS
const TInt32 KTTCN2GResolverPluginUID = 0x101F5D42;

// CLASS DEFINITION
/**
 * CTTCN2GResolverPlugin implements a 2G plug-in 
 * for testing SIP Client Resolver with TTCN.
 */
class CTTCN2GResolverPlugin : public CSIPResolvedClient2
	{
	public: // Constructors and destructor
	
		/**
		* Constructor
		* @return An initialized instance of this class.
		*/
		static CTTCN2GResolverPlugin* NewL();
		
		/**
		* Destructor
		*/
		~CTTCN2GResolverPlugin();
    
    public: // From CSIPResolvedClient2
    
        TBool MatchAcceptContactsL(
            RStringF aMethod,
            const CUri8& aRequestUri,
            const RPointerArray<CSIPHeaderBase>& aHeaders,
            const TDesC8& aContent,
            const CSIPContentTypeHeader* aContentType,
            TUid& aClientUid);

        TBool MatchEventL(
            RStringF aMethod,
            const CUri8& aRequestUri,
            const RPointerArray<CSIPHeaderBase>& aHeaders,
            const TDesC8& aContent,
            const CSIPContentTypeHeader* aContentType,
            TUid& aClientUid);

        TBool MatchRequestL(
            RStringF aMethod,
            const CUri8& aRequestUri,
            const RPointerArray<CSIPHeaderBase>& aHeaders,
            const TDesC8& aContent,
            const CSIPContentTypeHeader* aContentType,
            TUid& aClientUid );

        TBool ConnectSupported();

        void ConnectL( const TUid& aClientUid );
        
        void CancelConnect( const TUid& aClientUid );
        
        RPointerArray<CSIPContentTypeHeader> SupportedContentTypesL();
        
        RPointerArray<CSdpMediaField> SupportedSdpMediasL();
          
        void AddClientSpecificHeadersForOptionsResponseL(
            				RPointerArray<CSIPHeaderBase>& aHeaders );
		
    private: // Constructors
    
        CTTCN2GResolverPlugin();
        void ConstructL();
		
    private: // New functions
    
        TBool Match(
            RSipClientSimulator::TMatchType aMatchType,
            TUid& aClientUid );
		
	private: // data
	
	    RSipClientSimulator iSimulator;
	};

#endif // CTTCN2GRESOLVERPLUGIN_H
