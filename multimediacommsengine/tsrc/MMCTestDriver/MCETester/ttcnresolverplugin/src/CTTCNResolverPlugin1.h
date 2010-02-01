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



#ifndef __CTTCNResolverPlugin1_H__
#define __CTTCNResolverPlugin1_H__

// INCLUDES
#include <apgcli.h>
#include "CTTCNResolverPluginBase.h"

// CONSTANTS ( implementation 1 )
const TInt32 KTTCNResolverPluginUID1 = 0x1020E4C8;

// CLASS DEFINITION
/**
 * CTTCNResolverPlugin1 implements a simple plugin for testing ClientResolver.
 */
class CTTCNResolverPlugin1 : public CTTCNResolverPluginBase
	{
	public:
		/**
		 * Static constructor
		 * @return An initialized instance of this class.
		 */
		static CTTCNResolverPlugin1* NewL();
		
    public: // from CTTCNResolverPluginBase
        TInt32 MyUid() const;
    
    public: // from CSIPResolvedClient
		const TDesC8& Capabilities();
	};

#endif // __CTTCNResolverPlugin1_H__
