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
* Description:    Interface proxy for G711 encoder.
*
*/




// INCLUDE FILES
#include "G711EncoderIntfcProxy_STUB.h"
//#include "G711EncoderIntfcMsgs.h"
#include <CustomCommandUtility.h>
#include <CustomInterfaceUtility.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CG711EncoderIntfcProxy_STUB::CG711EncoderIntfcProxy_STUB
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CG711EncoderIntfcProxy_STUB::CG711EncoderIntfcProxy_STUB( )
    {
    }


// -----------------------------------------------------------------------------
// CG711EncoderIntfcProxy_STUB::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CG711EncoderIntfcProxy_STUB::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CG711EncoderIntfcProxy_STUB::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CG711EncoderIntfcProxy_STUB* CG711EncoderIntfcProxy_STUB::NewL()
    {
    CG711EncoderIntfcProxy_STUB* self = new(ELeave) CG711EncoderIntfcProxy_STUB();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// Destructor
EXPORT_C CG711EncoderIntfcProxy_STUB::~CG711EncoderIntfcProxy_STUB()
    {
    }

// ---------------------------------------------------------
// CG711EncoderIntfcProxy_STUB::SetEncoderMode
// Sends the custom command for this function to its message handler.
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CG711EncoderIntfcProxy_STUB::SetEncoderMode(TEncodeMode /*aEncodeMode*/)
    {
	return KErrNone;
    }
    
// ---------------------------------------------------------
// CG711EncoderIntfcProxy_STUB::SetVadMode
// Sends the custom command for this function to its message handler.
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CG711EncoderIntfcProxy_STUB::SetVadMode(TBool /*aVadMode*/)
	{
	return KErrNone;
	}

// ---------------------------------------------------------
// CG711EncoderIntfcProxy_STUB::GetVadMode
// Sends the custom command for this function to its message handler.
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CG711EncoderIntfcProxy_STUB::GetVadMode(TBool& /*aVadMode*/)
	{
	return KErrNone;
	}

// End of File
