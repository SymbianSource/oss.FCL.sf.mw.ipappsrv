/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
#include "MccRtpDataSource.h"
#include "MccRtpDataSink.h"

#define KImplUidRtpDataSource           0x1020740B
const TUid KRtpDataSource =  { KImplUidRtpDataSource };

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
                            TUid aInterfaceUid,
							RImplInfoPtrArray& aImplInfoArray)
    {
    TUid uid(aInterfaceUid); 
	TInt version = 1; 
	HBufC*  name = _L("dummy").AllocLC();
	HBufC8* dataType = _L8("0_IETF").AllocLC();
	HBufC8* opaqueData = _L8("dummy").AllocLC();
    TDriveUnit drive;
	TBool romOnly(ETrue);
	TBool romBased(ETrue);
    CImplementationInformation* info = 
    				CImplementationInformation::NewL(uid,version,name,dataType,
    								opaqueData,drive,romOnly,romBased);
    CleanupStack::Pop(3);
    CleanupStack::PushL(info);
    aImplInfoArray.AppendL(info);
    CleanupStack::Pop(info);
	
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
                            TUid aImplementationUid, 
							TInt32 /*aKeyOffset*/)
    {
    if ( aImplementationUid == KRtpDataSource )
    	{
    	return CMccRtpDataSource::NewSourceL(aImplementationUid,KNullDesC8 );
    	}
    else
    	{
    	return CMccRtpDataSink::NewSinkL( aImplementationUid, KNullDesC8 );
    	}
    
    }
    
    
