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

#include <ilbcDecoderIntfc.h>
#include <ilbcDecoderIntfcproxy.h>
#include <mmf/server/sounddevice.h>


EXPORT_C CIlbcDecoderIntfc* CIlbcDecoderIntfc::NewL(CMMFDevSound& /*aDevSound*/)
{
    TMMFMessageDestinationPckg handler;
    MCustomCommand* command = 0;
    CCustomInterfaceUtility* utility = 0;

    return CIlbcDecoderIntfcProxy::NewL(handler, *command, utility);
}

