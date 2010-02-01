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

#include <G729EncoderIntfc.h>
#include <G729EncoderIntfcproxy.h>

EXPORT_C CG729EncoderIntfcProxy* CG729EncoderIntfcProxy::NewL(
		                    TMMFMessageDestinationPckg aMessageHandler,
                            MCustomCommand& aCustomCommand,
                            CCustomInterfaceUtility* aCustomInterfaceUtility)
{
	return new CG729EncoderIntfcProxy(aMessageHandler, aCustomCommand, aCustomInterfaceUtility);
}

EXPORT_C CG729EncoderIntfcProxy::~CG729EncoderIntfcProxy()
{
}

EXPORT_C TInt CG729EncoderIntfcProxy::SetVadMode(TBool aVadMode)
{
}

EXPORT_C TInt CG729EncoderIntfcProxy::GetVadMode(TBool& aVadMode)
{
}


CG729EncoderIntfcProxy::CG729EncoderIntfcProxy(
		                    TMMFMessageDestinationPckg aMessageHandler, 
                            MCustomCommand& aCustomCommand,
                            CCustomInterfaceUtility* aCustomInterfaceUtility):iCustomCommand(aCustomCommand)
{
}

