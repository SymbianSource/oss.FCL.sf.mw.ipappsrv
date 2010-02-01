/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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






// INCLUDES
#include <uri8.h>
#include "sipdialog.h"
#include "sipfromheader.h"
#include "siptoheader.h"
#include <sipcallidheader.h>



CSIPDialog* CSIPDialog::NewL()
    {
    CSIPDialog* self = new (ELeave) CSIPDialog();
    CleanupStack::PushL( self );
    
    const CUri8* remoteUri = NULL;
    const CSIPFromHeader* from = NULL;
    const CSIPToHeader* to = NULL;
    self->ConstructL( *remoteUri, *from, *to );
    
    CleanupStack::Pop( self );
    return self;
    }

CSIPDialog* CSIPDialog::NewL( const CUri8& aRemoteUri,
                              const CSIPFromHeader& aFrom,
                              const CSIPToHeader& aTo )
                              
    {
    CSIPDialog* self = new (ELeave) CSIPDialog();
    CleanupStack::PushL( self );
    self->ConstructL( aRemoteUri, aFrom, aTo );
    CleanupStack::Pop( self );
    return self;
    
    }
    
CSIPDialog::~CSIPDialog()
    {
    delete iRemoteUri;
    delete iFrom;
    delete iTo;
    delete iCallId;
    }

CSIPDialog::CSIPDialog()
 : iState( EConfirmed ),
   iReferenceCount( 0 )
    {
    }

EXPORT_C CSIPDialog::TState CSIPDialog::State() const
    {
    return iState;
    
    }

void CSIPDialog::ConstructL( const CUri8& aRemoteUri,
                              const CSIPFromHeader& aFrom,
                              const CSIPToHeader& aTo)
    {
    if ( &aRemoteUri )
        {
        iRemoteUri = CUri8::NewL( aRemoteUri.Uri() );
        }
    else
        {
        iRemoteUri = CUri8::NewL();
        }
    if ( &aFrom )
        {
        iFrom = static_cast<CSIPFromHeader*>( aFrom.CloneL() );
        }
    else
        {
        iFrom = CSIPFromHeader::DecodeL(_L8("sip:me@fromhere.com"));
        }
    if ( &aTo )
        {
        iTo = static_cast<CSIPToHeader*>( aTo.CloneL() );
        }
    else
        {
        iTo = CSIPToHeader::DecodeL(_L8("sip:me@tohere.com"));
        }
        
    iCallId = CSIPCallIDHeader::DecodeL( _L8("ab2x@zb7y") );

    }

EXPORT_C TBool CSIPDialog::operator==(const CSIPDialog& aDialog) const
    {
	return (this == &aDialog);
    }

EXPORT_C const CSIPFromHeader& CSIPDialog::FromHeader() const
    {
	return *iFrom;
    }
    
EXPORT_C const CSIPToHeader& CSIPDialog::ToHeader() const
    {
	return *iTo;
    }
    
EXPORT_C const CUri8& CSIPDialog::RemoteURI() const
    {
	return *iRemoteUri;
    }

EXPORT_C const CSIPCallIDHeader& CSIPDialog::CallIdL() const
    {
    return *iCallId;
    }

/////////
EXPORT_C void CSIPDialog::SetState( CSIPDialog::TState aState )
    {
    iState = aState;
    }
