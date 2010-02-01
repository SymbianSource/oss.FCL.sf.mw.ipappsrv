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



#include <ecom/ecom.h>
#include <ecom/implementationinformation.h>
#include "mccrtpdatasink.h"
#include "mccrtpdatasource.h"
// REComSession:

// -----------------------------------------------------------------------------
// REComSession::REComSession
// -----------------------------------------------------------------------------
//
REComSession::REComSession()
    {
    }
    
// -----------------------------------------------------------------------------
// REComSession::CreateImplementationL
// -----------------------------------------------------------------------------
//
TAny* REComSession::CreateImplementationL( TUid aImplementationUid, 
TInt32 /*aKeyOffset*/ )
    {
    if (aImplementationUid.iUid == KImplUidRtpDataSink)
    	{
    	return CMccRtpDataSink::NewSinkL( aImplementationUid,KNullDesC8 );
    	}
    if (aImplementationUid.iUid == KImplUidRtpDataSource)
    	{
    	return CMccRtpDataSource::NewSourceL( aImplementationUid,KNullDesC8 );
    	}
    User::Leave(KErrArgument);
    
    return NULL;
    }
     
// -----------------------------------------------------------------------------
// REComSession::ListImplementationsL
// -----------------------------------------------------------------------------
//
void REComSession::ListImplementationsL(
                            TUid /*aInterfaceUid*/,
							RImplInfoPtrArray& /*aImplInfoArray*/)
    {
    	
    }


// -----------------------------------------------------------------------------
// REComSession::DestroyedImplementation
// -----------------------------------------------------------------------------
//
void REComSession::DestroyedImplementation(
                            TUid /*aDtorIDKey*/)
    {
    }

    
// -----------------------------------------------------------------------------
// REComSession::FinalClose
// -----------------------------------------------------------------------------
//
void REComSession::FinalClose()
    {
    }
   

    
