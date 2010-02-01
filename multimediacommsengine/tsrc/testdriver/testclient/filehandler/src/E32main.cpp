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
* Description:  Implementation
*
*/

#include "CTcFileHandlerServer.h"

#if ( ( defined (__WINS__) || defined(__WINSCW__) ) && !defined (EKA2) )

EXPORT_C TInt WinsMain()
    {
	return reinterpret_cast<TInt>( &CTcFileHandlerServer::ThreadMain );
    }

TInt E32Dll( TDllReason ) 
    {
    return KErrNone; 
    }

#else

TInt E32Main() 
    { 
    return CTcFileHandlerServer::ThreadMain( NULL );
    }

#endif
