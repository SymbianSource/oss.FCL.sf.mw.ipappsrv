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
* Description:   
*
*/




#include "tnatfwunsafmsgstateinit.h"
#include "mnatfwunsafmsgassemblerctx.h"
#include "natfwunsafmessagefactory.h"
#include "natfwunsafmessage.h"

// ----------------------------------------------------------------------------
// TNATFWUNSAFMsgStateInit::DataReceivedL
// ----------------------------------------------------------------------------
//
TBool TNATFWUNSAFMsgStateInit::DataReceivedL( TPtr8 aData, TUint& aNextLength )
    {
    TBool retVal( EFalse );

    if ( iMsgAssembler.MsgBuffer().Size() > 0 ||
         aData.Length() < CNATFWUNSAFMessage::EHeaderSize )
        {
        iMsgAssembler.MsgBuffer().InsertL( iMsgAssembler.MsgBuffer().Size(),
                                           aData );
        iMsgAssembler.MsgBuffer().Compress();

        // delete content of received data.
        aData.Delete( 0, aData.Length() );
        TPtr8 bufPtr = iMsgAssembler.MsgBuffer().Ptr( 0 );

        if ( bufPtr.Length() >= CNATFWUNSAFMessage::EHeaderSize )
            {
            // There's enough data for determining NATFWUNSAF msg length

            HBufC8* newData = HBufC8::NewLC( bufPtr.Length() );

            // copy the msg buffer data to new data
            TPtr8 newDataPtr = newData->Des();
            newDataPtr.Append( bufPtr );

            // clean the msg buffer
            iMsgAssembler.MsgBuffer().Reset();
            iMsgAssembler.MsgBuffer().Compress();

            retVal = HandleNATFWUNSAFMsgLenL( newDataPtr, aNextLength );

            CleanupStack::PopAndDestroy( newData );
            }
        }
    else
        {
        // There's enough data for determining NATFWUNSAF msg length
        retVal = HandleNATFWUNSAFMsgLenL( aData, aNextLength );
        }

    return retVal;
    }

// ----------------------------------------------------------------------------
// TNATFWUNSAFMsgStateInit::DataReceivedL
// ----------------------------------------------------------------------------
//
TBool TNATFWUNSAFMsgStateInit::HandleNATFWUNSAFMsgLenL( const TPtr8& aData,
    TUint& aNextLength )
    {
    TInt len = iMsgAssembler.MsgFactory().MessageLength( aData );

    User::LeaveIfError( len );

    iMsgAssembler.SetMessageLength( len + CNATFWUNSAFMessage::EHeaderSize );

    iMsgAssembler.ChangeState( TNATFWUNSAFMsgStateBase::EMsgHeaderStart );

    return iMsgAssembler.CurrentState().DataReceivedL( aData, aNextLength );
    }

// End of File

