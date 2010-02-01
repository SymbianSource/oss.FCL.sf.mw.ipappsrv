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




#ifndef AVCPAYLOADFORMATLOGS_H
#define AVCPAYLOADFORMATLOGS_H

#include "mcclogs.h"

_LIT(KAvcPayloadFormat, "Mcc/AvcPayloadFormat:");

#ifdef _DEBUG
	#define __MCC_AVCPLFORMAT_CONTROLL
#endif // end of _DEBUG

#ifdef __MCC_AVCPLFORMAT_CONTROLL
	#define __AVCPLFORMAT_CONTROLL( a ) \
		{  _LIT( KStr, a ); TMccLog::Print( KAvcPayloadFormat, KStr ); }
	#define __AVCPLFORMAT_CONTROLL_INT1( a, b ) \
		{  _LIT( KStr, a ); TMccLog::Print( KAvcPayloadFormat, KStr, b ); }
#else
	#define __AVCPLFORMAT_CONTROLL( a )
	#define __AVCPLFORMAT_CONTROLL_INT1( a, b )
#endif // end of ifdef __MCC_AVCPLFORMAT_CONTROLL

#endif // End of define AVCPAYLOADFORMATLOGS_H

// end of file 