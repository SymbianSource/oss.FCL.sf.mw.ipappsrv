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



#ifndef __CTTCNRESOLVERPLUGINBASE_H__
#define __CTTCNRESOLVERPLUGINBASE_H__

// INCLUDES
#include <apgcli.h>
#include "SipResolvedClient.h"

// CLASS DEFINITION
/**
 * CTTCNResolverPluginBase base class for testing plugins of ClientResolver.
 */
class CTTCNResolverPluginBase : public CSIPResolvedClient
	{
	public:	// Constructors and destructor

		/// Destructor
		virtual ~CTTCNResolverPluginBase();

	public: // from CSIPResolvedClient
	
	    TUid ChannelL( RStringF aMethod,
                       const TDesC8& aRequestUri,
                       const RPointerArray<CSIPHeaderBase>& aHeaders,
                       const TDesC8& aContent,
                       const CSIPContentTypeHeader* aContentType=0);

		void ConnectL( TUid aUid );

	protected: // Constructors

		inline CTTCNResolverPluginBase() {}
		
		// Second phase constructor
		void ConstructL(); 
	
	protected: // derived class must implement:
	
	    virtual TInt32 MyUid() const = 0;
	
	private: // New functions

		// Needed for cleanup of a RImplInfoPtrArray:
		static void ResetAndDestroy( TAny* anArray );

		// Name
		RApaLsSession iApaSession;
		TApaAppInfo iAppInfo;
		
		TUid iApplicationUID;
	};

#endif // __CTTCNRESOLVERPLUGIN_H__
