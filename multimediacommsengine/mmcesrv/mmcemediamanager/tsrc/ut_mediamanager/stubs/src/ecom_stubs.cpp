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
    TUid aInterfaceUid,
    RImplInfoPtrArray& aImplInfoArray)
    {
    TUid uid(aInterfaceUid); 
	TInt version = 1; 
	HBufC* name = _L("dummy").AllocLC();
	HBufC8* dataType = _L8("datatype").AllocLC();
	HBufC8* opaqueData = KNullDesC8().AllocLC();
    TDriveUnit drive;
	TBool romOnly(ETrue);
	TBool romBased(ETrue);
    CImplementationInformation* info = 
        CImplementationInformation::NewL(uid,version,name,dataType,
    								opaqueData,drive,romOnly,romBased);
    CleanupStack::Pop(opaqueData);
    CleanupStack::Pop(dataType);
    CleanupStack::Pop(name);
    CleanupStack::PushL(info);
    aImplInfoArray.AppendL(info);
    CleanupStack::Pop(info);
    }

// -----------------------------------------------------------------------------
// REComSession::DestroyedImplementation
// -----------------------------------------------------------------------------
//
void REComSession::DestroyedImplementation(TUid /*aDtorIDKey*/)
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
    return CPluginStub::NewL();
    }

// -----------------------------------------------------------------------------
// CImplementationInformation::NewL
// -----------------------------------------------------------------------------
//    
CImplementationInformation* CImplementationInformation::NewL(
    TUid /*aUid*/, 
	TInt /*aVersion*/, 
	HBufC* aName,
    HBufC8* aDataType,
	HBufC8* aOpaqueData,
	TDriveUnit /*aDrive*/,
	TBool /*aRomOnly*/,
	TBool /*aRomBased*/)
	{
	CImplementationInformation* self = new(ELeave)CImplementationInformation();
	CleanupStack::PushL(self);
	self->iData = aDataType;
	self->iDisplayName = aName;
	self->iOpaqueData = aOpaqueData;
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CImplementationInformation::CImplementationInformation
// -----------------------------------------------------------------------------
//
CImplementationInformation::CImplementationInformation()
	{
	}

// -----------------------------------------------------------------------------
// CImplementationInformation::~CImplementationInformation
// -----------------------------------------------------------------------------
//
CImplementationInformation::~CImplementationInformation()
	{
	delete iData;
	delete iDisplayName;
	delete iOpaqueData;
	}
