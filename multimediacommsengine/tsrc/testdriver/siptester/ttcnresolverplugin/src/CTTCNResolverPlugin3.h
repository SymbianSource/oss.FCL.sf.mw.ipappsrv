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

#ifndef __CTTCNResolverPlugin3_H__
#define __CTTCNResolverPlugin3_H__

// INCLUDES
#include <apgcli.h>
#include "CTTCNResolverPluginBase.h"

// CONSTANTS ( implementation 3 )
const TInt32 KTTCNResolverPluginUID3 = 0x10203347;

// CLASS DEFINITION
/**
 * CTTCNResolverPlugin3 implements a simple plugin for testing ClientResolver.
 */
class CTTCNResolverPlugin3 : public CTTCNResolverPluginBase
	{
	public:
		/**
		 * Static constructor
		 * @return An initialized instance of this class.
		 */
		static CTTCNResolverPlugin3* NewL();
		
    public: // from CTTCNResolverPluginBase
        TInt32 MyUid() const;
       
	};

#endif // __CTTCNResolverPlugin3_H__
