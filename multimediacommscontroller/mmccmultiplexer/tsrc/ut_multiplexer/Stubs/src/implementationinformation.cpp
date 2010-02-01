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




#include <ecom/implementationinformation.h>

CImplementationInformation* CImplementationInformation::NewL(TUid /*aUid*/, 
											TInt	/*aVersion*/, 
											HBufC*  aName,
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

CImplementationInformation::CImplementationInformation()
	{
	}

CImplementationInformation::~CImplementationInformation()
	{
	delete iData;
	delete iDisplayName;
	delete iOpaqueData;
	}

void CImplementationInformation::SetRomBased(TBool /*aRomBased*/)
	{
	}
