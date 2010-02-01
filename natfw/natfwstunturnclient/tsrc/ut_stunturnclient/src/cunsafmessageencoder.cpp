/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    ?Description
*
*/




#include <e32base.h>

_LIT8( KPassword,"salasana");

#include "cunsafmessageencoder.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUnsafMessageEncoder::CUnsafMessageEncoder()
    {
    }


// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUnsafMessageEncoder::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUnsafMessageEncoder* CUnsafMessageEncoder::NewL()
    {
    CUnsafMessageEncoder* self = CUnsafMessageEncoder::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUnsafMessageEncoder* CUnsafMessageEncoder::NewLC()
    {
    CUnsafMessageEncoder* self = new( ELeave ) CUnsafMessageEncoder;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUnsafMessageEncoder::~CUnsafMessageEncoder()
    {
    }
    

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
HBufC8* CUnsafMessageEncoder::EncodeMessageL( 
    const CNATFWUNSAFMessage& aRequest, const TBool aUseSharedSecret,
    const TBool aAddFingerPrint )
    {
    CBufBase* message = NULL;       
         
    if ( !aUseSharedSecret )
        {
        message = aRequest.EncodeL(); 
        }
    else if ( !aAddFingerPrint )
        {
        message = aRequest.EncodeL( KPassword );
        }
    else
        {
        // ETrue indicates that fingerprint attribute will be added
        message = aRequest.EncodeL( KPassword, ETrue );
        }
        
    CleanupStack::PushL( message );
    TInt length = message->Size();
    HBufC8* readBuf = HBufC8::NewLC( length );
    TPtr8 writable( readBuf->Des() );
    message->Read( 0, writable, length );

    CleanupStack::Pop( readBuf );
    CleanupStack::PopAndDestroy( message );
    
    return readBuf;    
    }



