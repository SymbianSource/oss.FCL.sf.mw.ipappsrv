/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    implementation
*
*/




// INCLUDE FILES
#include "cnatfwunsafmsgassembler.h"
#include "mnatfwunsafmsgassemblerobserver.h"
#include "tnatfwunsafmsgstateinit.h"
#include "tnatfwunsafmsgstateheaderstart.h"
#include "tnatfwunsafmsgstateheaderend.h"
#include "tnatfwunsafmsgstatecomplete.h"
#include "natfwunsafmessagefactory.h"
#include "natfwunsafmessage.h"

const TInt KBufExpandSize = 100;

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMsgAssembler* CNATFWUNSAFMsgAssembler::NewL(
    MNATFWUNSAFMsgAssemblerObserver& aObserver )
    {
    CNATFWUNSAFMsgAssembler* self =
        CNATFWUNSAFMsgAssembler::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMsgAssembler* CNATFWUNSAFMsgAssembler::NewLC(
    MNATFWUNSAFMsgAssemblerObserver& aObserver )
    {
    CNATFWUNSAFMsgAssembler* self =
        new ( ELeave ) CNATFWUNSAFMsgAssembler( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::CNATFWUNSAFMsgAssembler
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMsgAssembler::CNATFWUNSAFMsgAssembler(
    MNATFWUNSAFMsgAssemblerObserver& aObserver )
    : iObserver( aObserver ),
      iStates( TNATFWUNSAFMsgStateBase::EMsgMaxStates ),
      iStateValue( TNATFWUNSAFMsgStateBase::EMsgInit )
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMsgAssembler::ConstructL()
    {
    iMsgFactory = CNATFWUNSAFMessageFactory::NewL();

    iMsgBuf = CBufFlat::NewL( KBufExpandSize );

    iStates.AppendL( TNATFWUNSAFMsgStateInit( *this ),
                     sizeof( TNATFWUNSAFMsgStateInit ) );

    iStates.AppendL( TNATFWUNSAFMsgStateHeaderStart( *this ),
                     sizeof( TNATFWUNSAFMsgStateHeaderStart ) );

    iStates.AppendL( TNATFWUNSAFMsgStateHeaderEnd( *this ),
                     sizeof( TNATFWUNSAFMsgStateHeaderEnd ) );

    iStates.AppendL( TNATFWUNSAFMsgStateComplete( *this ),
                     sizeof( TNATFWUNSAFMsgStateComplete ) );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::~CNATFWUNSAFMsgAssembler
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMsgAssembler::~CNATFWUNSAFMsgAssembler()
    {
    delete iMsgFactory;
    delete iMsgBuf;
    delete iMessage;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::MsgObserver
// From MNATFWUNSAFMsgAssemblerCtx
// -----------------------------------------------------------------------------
//
MNATFWUNSAFMsgAssemblerObserver& CNATFWUNSAFMsgAssembler::MsgObserver()
    {
    return iObserver;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::MsgFactory
// From MNATFWUNSAFMsgAssemblerCtx
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessageFactory& CNATFWUNSAFMsgAssembler::MsgFactory()
    {
    return *iMsgFactory;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::MsgBuffer
// From MNATFWUNSAFMsgAssemblerCtx
// -----------------------------------------------------------------------------
//
CBufFlat& CNATFWUNSAFMsgAssembler::MsgBuffer()
    {
    return *iMsgBuf;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::Message
// From MNATFWUNSAFMsgAssemblerCtx
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage* CNATFWUNSAFMsgAssembler::Message()
    {
    return iMessage;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::SetMessage
// From MNATFWUNSAFMsgAssemblerCtx
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMsgAssembler::SetMessage( CNATFWUNSAFMessage* aMessage )
    {
    delete iMessage;
    iMessage = aMessage;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::DetachMessage
// From MNATFWUNSAFMsgAssemblerCtx
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMsgAssembler::DetachMessage()
    {
    iMessage = 0;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::SetMessageLength
// From MNATFWUNSAFMsgAssemblerCtx
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMsgAssembler::SetMessageLength( TInt aMessageLength )
    {
    iMessageLength = aMessageLength;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::MessageLength
// From MNATFWUNSAFMsgAssemblerCtx
// -----------------------------------------------------------------------------
//
TInt CNATFWUNSAFMsgAssembler::MessageLength()
    {
    return iMessageLength;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::ChangeState
// From MNATFWUNSAFMsgAssemblerCtx
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMsgAssembler::ChangeState(
    TNATFWUNSAFMsgStateBase::TNATFWUNSAFMsgStateValue aState )
    {
    iStateValue = aState;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::CurrentState
// From MNATFWUNSAFMsgAssemblerCtx
// -----------------------------------------------------------------------------
//
TNATFWUNSAFMsgStateBase& CNATFWUNSAFMsgAssembler::CurrentState()
    {
    _LIT(KPanicCategory, "CNATFWUNSAFMsgAssembler::CurrentState");
    __ASSERT_ALWAYS( iStateValue < iStates.Count(),
        User::Panic( KPanicCategory, KErrTotalLossOfPrecision ) );

    return iStates.At( iStateValue );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMsgAssembler::InputL
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFMsgAssembler::InputL( HBufC8* aData, TUint& aNextLength )
    {
    __ASSERT_ALWAYS( aData, User::Leave( KErrArgument ) );

    __ASSERT_ALWAYS( aData->Length() != 0, User::Leave( KErrArgument ) );

    aNextLength = CNATFWUNSAFMsgAssembler::EDefaultBufferSize;

    TPtr8 dataPtr = aData->Des();
    TBool retVal = CurrentState().DataReceivedL( dataPtr, aNextLength );

    delete aData;

    return retVal;
    }

// End of File

