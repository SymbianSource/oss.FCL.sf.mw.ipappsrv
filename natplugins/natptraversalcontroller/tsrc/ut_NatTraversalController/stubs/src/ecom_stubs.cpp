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
#include "plugin_stub.h"


// REComSession:

// -----------------------------------------------------------------------------
// REComSession::REComSession
// -----------------------------------------------------------------------------
//
REComSession::REComSession()
    {
    }
    
     
// -----------------------------------------------------------------------------
// REComSession::ListImplementationsL
// -----------------------------------------------------------------------------
//
void REComSession::ListImplementationsL(
                            TUid /*aInterfaceUid*/,
							RImplInfoPtrArray& /*aImplInfoArray*/)
    {
	delete HBufC::NewL( 1 );
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
   
// -----------------------------------------------------------------------------
// REComSession::CreateImplementationL
// -----------------------------------------------------------------------------
//
TAny* REComSession::CreateImplementationL(
                            TUid /*aImplementationUid*/, 
							TInt32 /*aKeyOffset*/)
    {
    delete HBufC::NewL( 1 );
    return CPluginStub::NewL();    
    }
