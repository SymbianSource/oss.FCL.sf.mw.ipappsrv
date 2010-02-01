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
* Description:    AVC Payloadization/depayloadization base class.
*
*/




// ============================ INCLUDES =======================================

#include"rfc3984base.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRFC3984Base::CRFC3984Base()
// Default Constructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRFC3984Base::CRFC3984Base()
    {
	iPacketizationMode = 0;	
    }

// -----------------------------------------------------------------------------
// CRFC3984Base::~CRFC3984Base()
// Default Destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRFC3984Base::~CRFC3984Base()
    {
    }



// -----------------------------------------------------------------------------
// CRFC3984Base::SetPacketizationMode()
// Setting value of Packetization Mode as required
// -----------------------------------------------------------------------------
//

void CRFC3984Base::SetPacketizationMode(TInt aPacketizationMode)
    {
	iPacketizationMode = aPacketizationMode;	
    }


// EOF

