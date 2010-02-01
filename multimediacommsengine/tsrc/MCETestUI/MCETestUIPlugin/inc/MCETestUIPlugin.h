/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    MCETestUIPlugin interface
*
*/




#ifndef CMCETestUIPLUGIN_H
#define CMCETestUIPLUGIN_H

//  INCLUDES
#include <apgcli.h>
#include <e32base.h>
#include <ECom/ImplementationProxy.h>
#include "SIPResolvedClient.h"
#include "SIPContentTypeHeader.h"

/**
*  Implementation API for SIP Client Resolver.
*  Routes incoming MCETestUI calls to MMCE
*
*  @lib
*  @since Series
*/
class CMCETestUIPlugin : CSIPResolvedClient
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMCETestUIPlugin* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMCETestUIPlugin();

    public: // Functions from base classes

       	/**
		* Returns the SIP client's channel UID that indicates client's
		* channel UID for connection with SIP e.g. CSIP::NewL()
		* @param aMethod the method of the SIP request
        * @param aRequestUri the request-URI of the SIP request
        * @param aHeaders all the headers in the SIP request
        * @param aContent SIP request body; zero-length descriptor if not present
        * @param aContentType the content-type of the SIP request. 
        *        Zero-pointer if body is not present.
		* @return SIP client's communication channel's UID
		*/
		virtual TUid ChannelL(RStringF aMethod,
                              const TDesC8& aRequestUri,
                              const RPointerArray<CSIPHeaderBase>& aHeaders,
                              const TDesC8& aContent,
                              const CSIPContentTypeHeader* aContentType=0);

		/**
		* Requests the client to connect to SIP with resolved
		* UID in case there's no connection with resolved channel UID.
		* @param aUid previously resolved channel UID
		* @leave KErrNoMemory if out of memory
		* @leave KErrNotFound in case non-existing channel UID was provided
		*/
		virtual void ConnectL(TUid aUid);

		/**
		* Requests the implementation to provide capabilities in XML format
		* in case they were not defined in the ECOM resource file. The cabablities
		* to be returned must be defined according to the same DTD.
		* This function will be invoked only if the capabilities are not
		* defined in the ECOM resource file.
		*/
		virtual const TDesC8& Capabilities();


    private:

        /**
        * C++ default constructor.
        */
        CMCETestUIPlugin();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
		// Needed for cleanup of a RImplInfoPtrArray:
		static void ResetAndDestroy( TAny* anArray );

		void RefreshCapabilitiesL();

    private:    // Data

	    // MCETestUIPlugin's application UID        
    	TUid iApplicationUID;
    	
    	HBufC8* iCapabilitiesFromFile;
    };

#endif      // CMCETestUIPlugin_H   
            
// End of File

