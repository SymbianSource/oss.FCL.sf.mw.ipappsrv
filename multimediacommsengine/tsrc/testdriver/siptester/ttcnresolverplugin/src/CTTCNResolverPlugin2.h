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
* Description:  See class definition below.
*
*/

#ifndef __CTTCNResolverPlugin2_H__
#define __CTTCNResolverPlugin2_H__

// INCLUDES
#include <apgcli.h>
#include "CTTCNResolverPluginBase.h"


// CONSTANTS ( implementation 2 )
const TInt32 KTTCNResolverPluginUID2 = 0x10203346;

// CLASS DEFINITION
/**
 * CTTCNResolverPlugin2 implements a simple plugin for testing ClientResolver.
 */
class CTTCNResolverPlugin2 : public CTTCNResolverPluginBase
	{
	public:
		/**
		 * Static constructor
		 * @return An initialized instance of this class.
		 */
		static CTTCNResolverPlugin2* NewL();
		
		/**
		 * Destructor
		 */
		~CTTCNResolverPlugin2();
		
    public: // from CTTCNResolverPluginBase
        TInt32 MyUid() const;

	 public: // from CSIPResolvedClient
		const TDesC8& Capabilities();
		
    private:
    
        void ConstructL();
        
        void CapabilitiesL();
		
	private: // data
	
	    HBufC8* iCapabilities;
	};

#endif // __CTTCNResolverPlugin2_H__
