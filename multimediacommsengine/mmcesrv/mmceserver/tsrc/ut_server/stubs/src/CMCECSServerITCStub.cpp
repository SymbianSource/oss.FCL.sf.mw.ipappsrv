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
* Description:  
*
*/




#include <badesca.h>
#include "cmcecsserveritc.h"
#include "mceserver.pan"

#include "mceserial.h"
#include "cmcetls.h"

_LIT8(KDigest, "Digest");
_LIT8(KTestBuf, "Test");
_LIT8(KFmtAttribute,"a=fmtp:TBCPTest format parameters\r\n");


	
CMceCsServerITC* CMceCsServerITC::NewL ()
	{
    CMceCsServerITC* self = CMceCsServerITC::NewLC ();
    CleanupStack::Pop(self);
    return self;
	}


CMceCsServerITC* CMceCsServerITC::NewLC ()
	{
    CMceCsServerITC* self = new (ELeave) CMceCsServerITC;
    CleanupStack::PushL (self);
    self->ConstructL ();
    return self;
	}


CMceCsServerITC::CMceCsServerITC ()
	{
	}


void CMceCsServerITC::ConstructL ()
	{
	}


CMceCsServerITC::~CMceCsServerITC ()
	{
	}
	
void CMceCsServerITC::WriteL (const RMessage2& /*aMessage*/,
			 const TDesC8& aDes,
			 TMceItcArguments aItcArgIndex ) const
	{
    CMCETls* tls = CMCETls::Storage();
    if (!tls) return;
    
    if ( aItcArgIndex == EMceItcArgContext )
        {
        delete tls->iContextServer;
        tls->iContextServer = NULL;
        tls->iContextServer = aDes.AllocL();
        }
    else if ( aItcArgIndex == EMceItcArgMessageContent )
        {
        delete tls->iContentServer;
        tls->iContentServer = NULL;
        tls->iContentServer = aDes.AllocL();
        }
    else
        {
        //NOP
        }
	}
	
	

void CMceCsServerITC::WriteL (const RMessage2& /*aMessage*/, 
							  const TMceIds& aIds ) const
	{
    CMCETls* tls = CMCETls::Storage();
    if (!tls) return;
    if ( aIds.IsAppUIDId() )
        {
        tls->iManagerIds = aIds;
        }
    else
        {
        tls->iIds = aIds;
        }
	}

void CMceCsServerITC::WriteL (const RMessage2& /*aMessage*/,
			 const TMceMessageBufSizes aSizes ) const
	{
    CMCETls* tls = CMCETls::Storage();
    if (!tls) return;
    tls->iMessageSizes = aSizes;
	}



TMceIds CMceCsServerITC::ReadMCEIdsL (const RMessage2& /*aMessage*/) const
	{
    CMCETls* tls = CMCETls::Storage();
    if (tls) 
        {
        if ( tls->iIdsIndex == 0 )
            {
            return CMCETls::Storage()->iIds;
            }
        else
            {
            return CMCETls::Storage()->iManagerIds;
            }
        }
    else
        {
        return TMceIds();
        }
	}
	
    
HBufC8* CMceCsServerITC::ReadLC (const RMessage2&  /*aMessage*/,
                                 TMceItcArguments aItcArgIndex) const
    {
    CMCETls* tls = CMCETls::Storage();
    HBufC8* value = NULL;
    
    if (!tls)
        {
        value = KNullDesC8().AllocL();    
        }
    else if ( aItcArgIndex == EMceItcArgContext )
        {
        value = tls->iContextClient;
        tls->iContextClient = NULL;
        }
    else if ( aItcArgIndex == EMceItcArgMessageContent )
        {
        value = tls->iContentClient;
        tls->iContentClient = NULL;
        }
    else
        {
        value = KFmtAttribute().AllocL();    
        }

    if( value )
        {
        CleanupStack::PushL( value );
        }
    else
        {
        value = KNullDesC8().AllocLC();
        }
    
	return value;
     
    }
    

TUint32 CMceCsServerITC::ReadTUint32L (const RMessage2&  /*aMessage*/,
                                       TMceItcArguments /*aItcArgIndex*/) const
    {
    CMCETls* tls = CMCETls::Storage();
    if (tls)
        {
        return CMCETls::Storage()->iValue;
        }
    else
        {
        return 1200;
        }
    }

    
void CMceCsServerITC::Complete (const RMessage2& /*aMessage*/, 
                                TInt aCompletionCode ) const
	{
	
    CMCETls* tls = CMCETls::Storage();
    if (tls)
        {
        CMCETls::Storage()->iCompletionCode = aCompletionCode;
        }
	
	}

TInt CMceCsServerITC::Function (const RMessage2& /*aMessage*/) const
	{
    CMCETls* tls = CMCETls::Storage();
    if (tls)
        {
        return CMCETls::Storage()->iItcFunction;
        }
    else
        {
        return EMceItcSetAppUid;
        }
	}


void CMceCsServerITC::PanicClient(const RMessage2& /*aMessage*/,
                                  TInt /*aPanic*/) const
	{
	}

const TAny* CMceCsServerITC::ITCArgPtr (TMceItcArguments aItcArgIndex,
                                        const RMessage2&  aMessage) const
	{
    const TAny* ptr;
    switch (aItcArgIndex)
		{
        case 0: ptr = aMessage.Ptr0(); break;
        case 1: ptr = aMessage.Ptr1(); break;
        case 2: ptr = aMessage.Ptr2(); break;
        case 3: ptr = aMessage.Ptr3(); break;
        default: ptr = 0; break;
		}
    return ptr; 
	}
