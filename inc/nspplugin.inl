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
* Description:    Plugin interface inline implementation.
*
*/


#include <ecom/ecom.h>

// -----------------------------------------------------------------------------
// CNSPPlugin::CNSPPlugin
// -----------------------------------------------------------------------------
//
inline CNSPPlugin* CNSPPlugin::NewL( TUid aUid )
    {
    return ( reinterpret_cast<CNSPPlugin*>( REComSession::CreateImplementationL(
            aUid, _FOFF( CNSPPlugin, iDtor_ID_Key ) ) ) );
    }

// -----------------------------------------------------------------------------
// CNSPPlugin::~CNSPPlugin
// -----------------------------------------------------------------------------
//
inline CNSPPlugin::~CNSPPlugin()
    {
	REComSession::DestroyedImplementation( iDtor_ID_Key );
    }

// -----------------------------------------------------------------------------
// CNSPPlugin::SetSessionParam
// -----------------------------------------------------------------------------
//
inline TInt CNSPPlugin::SetSessionParam( TUint /*aSessionId*/,
        TNSPSessionParamKey /*aParamKey*/, TUint /*aParamValue*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CNSPPlugin::GetSessionParam
// -----------------------------------------------------------------------------
//
inline TInt CNSPPlugin::GetSessionParam( TUint /*aSessionId*/,
        TNSPSessionParamKey /*aParamKey*/ )
    {
    return KErrNotSupported;
    }

// end of file
