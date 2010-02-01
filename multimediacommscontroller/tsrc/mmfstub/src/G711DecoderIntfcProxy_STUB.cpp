/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
#include <G711DecoderIntfc.h>
#include <G711DecoderIntfcproxy.h>



EXPORT_C CG711DecoderIntfcProxy* CG711DecoderIntfcProxy::NewL(
		                    TMMFMessageDestinationPckg aMessageHandler,
                            MCustomCommand& aCustomCommand,
                            CCustomInterfaceUtility* aCustomInterfaceUtility)
	{
  	return new CG711DecoderIntfcProxy(aMessageHandler, aCustomCommand, aCustomInterfaceUtility);
	}

EXPORT_C CG711DecoderIntfcProxy::~CG711DecoderIntfcProxy()
	{
	}

CG711DecoderIntfcProxy::CG711DecoderIntfcProxy(
		                    TMMFMessageDestinationPckg aMessageHandler, 
                            MCustomCommand& aCustomCommand,
                            CCustomInterfaceUtility* aCustomInterfaceUtility):iCustomCommand(aCustomCommand)
	{
	}

EXPORT_C TInt CG711DecoderIntfcProxy::SetDecoderMode(TDecodeMode aDecodeMode)
	{
	return KErrNone;
	}

EXPORT_C TInt CG711DecoderIntfcProxy::SetCng(TBool aCng)
	{
	return KErrNone;
	}

EXPORT_C TInt CG711DecoderIntfcProxy::GetCng(TBool& aCng)
	{
	return KErrNone;
	}

EXPORT_C TInt CG711DecoderIntfcProxy::SetPlc(TBool aPlc)
	{
	return KErrNone;
	}


