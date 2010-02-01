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
* Description:  Implementation
*
*/

#include "CTcSerialConnection.h"
#include "debuglog.h"
#include "ErrorHandling.h"

CTcSerialConnection::CTcSerialConnection( 
    RCommServ& aCommServ, 
    const TDesC& aPortName, 
    TUint aPortSpeed ) :
    iCommServ( aCommServ ),
    iPortName( aPortName ),
    iPortSpeed( aPortSpeed )
	{
	}

CTcSerialConnection::~CTcSerialConnection()
	{
	Cancel();
	iCommPort.Close();
	}
	
void CTcSerialConnection::DoCancel()
    {
    iCommPort.Cancel();
    }

void CTcSerialConnection::ConnectL()
    {
    LOG( _L("CTcSerialConnection::ConnectL() start") );
	if( iState != EIdle )
		{
		User::Leave( KErrNotReady );
		}

	iState = EConnecting;

	// Open serial port
	SetupPortL();
	
    // Serial port can be used immediately after port opening, do some nasty
    // RunL call to get in connected state
    iStatus = KErrNone;
	RunL();

	LOG( _L("CTcSerialConnection::ConnectL() end") );
    }

void CTcSerialConnection::Close()
    {
    Cancel();
    iCommPort.Close();
    }

void CTcSerialConnection::Send( const TDesC8& aDes )
    {
   	LOG( _L("CTcSerialConnection::Send() start") );
	// Make sure we're in correct state
	__ASSERT_ALWAYS( iState == EConnected, Panic( KErrNotReady ) );
	__ASSERT_ALWAYS( !IsActive(), Panic( KErrInUse ) );

	// Start writing
	iState = ESending;
	iCommPort.Write( iStatus, aDes );
	SetActive();

	LOG( _L("CTcSerialConnection::Send() end") );
    }

void CTcSerialConnection::Receive( TDes8& aDes )
    {
   	LOG( _L("CTcSerialConnection::Receive() start") );
	// Make sure we're in correct state
	__ASSERT_ALWAYS( iState == EConnected, Panic( KErrNotReady ) );
	__ASSERT_ALWAYS( !IsActive(), Panic( KErrInUse ) );

	// Start reading
	iState = EReceiving;
	iCommPort.Read( iStatus, aDes );
	SetActive();

	LOG( _L("CTcSerialConnection::Receive() end") );
    }

void CTcSerialConnection::ReceiveOneOrMore( TDes8& aDes )
    {
    LOG( _L("CTcSerialConnection::ReceiveOneOrMore() start") );
	// Make sure we're in correct state
	__ASSERT_ALWAYS( iState == EConnected, Panic( KErrNotReady ) );
	__ASSERT_ALWAYS( !IsActive(), Panic( KErrInUse ) );

	// Start reading
	iState = EReceiving;
	iCommPort.ReadOneOrMore( iStatus, aDes );
	SetActive();

	LOG( _L("CTcSerialConnection::ReceiveOneOrMore() end") );
    }

void CTcSerialConnection::SetupPortL()
	{
	LOG( _L("CTcSerialConnection::SetupPortL()") );
	
	SetupPortBaudrateValuesL();

	User::LeaveIfError( iCommPort.Open( iCommServ, iPortName, ECommShared ) );

    // Configure port, chack first whether ports supports settings we want
	TCommCaps ourCapabilities;
	iCommPort.Caps( ourCapabilities );

	if ( ( ( ourCapabilities().iRate & iBaudrateCap ) == 0 ) ||
		 ( ( ourCapabilities().iDataBits & KCapsData8 ) == 0 ) ||
		 ( ( ourCapabilities().iStopBits & KCapsStop1 ) == 0 ) ||
		 ( ( ourCapabilities().iParity & KCapsParityNone ) == 0 ) )
	    {
		User::Leave( KErrNotSupported );
	    }

	TCommConfig portSettings;
	iCommPort.Config( portSettings );
	portSettings().iRate = iBaudrate;
	portSettings().iParity = EParityNone;
	portSettings().iDataBits = EData8;
	portSettings().iStopBits = EStop1;

	// as well as the physical characteristics, we need to set various logical ones
	// to do with handshaking, behaviour of reads and writes and so so
	portSettings().iFifo = EFifoEnable;
	portSettings().iHandshake = ( KConfigObeyCTS | KConfigFreeRTS ); // for cts/rts

    portSettings ().iTerminator[ 0 ] = 10;
	portSettings ().iTerminatorCount = 1; // so that we terminate a read on each line feed arrives

	User::LeaveIfError( iCommPort.SetConfig( portSettings ) );

	// now turn on CTS and RTS 
	iCommPort.SetSignals( KSignalCTS, 0 );
	iCommPort.SetSignals( KSignalRTS, 0 );
	
	// (perhaps set our receive buffer size also)
	//TInt curlenth = commPort.ReceiveBufferLength();
	//commPort.SetReceiveBufferLength( KTcRequestMaxLength );
	
	LOG( _L("CTcSerialConnection::ConnectL() done") );
	}
	
void CTcSerialConnection::SetupPortBaudrateValuesL()
    {
    LOG( _L("CTcSerialConnection::SetupPortBaudrateValuesL()") );
    
    switch ( iPortSpeed )
        {
        case 1200:
            {
            iBaudrate = EBps1200;
            iBaudrateCap = KCapsBps1200;
            break;
            }
        case 1800:
            {
            iBaudrate = EBps1800;
            iBaudrateCap = KCapsBps1800;
            break;
            }
        case 2000:
            {
            iBaudrate = EBps2000;
            iBaudrateCap = KCapsBps2000;
            break;
            }
        case 2400:
            {
            iBaudrate = EBps2400;
            iBaudrateCap = KCapsBps2400;
            break;
            }
        case 3600:
            {
            iBaudrate = EBps3600;
            iBaudrateCap = KCapsBps3600;
            break;
            }
        case 4800:
            {
            iBaudrate = EBps4800;
            iBaudrateCap = KCapsBps4800;
            break;
            }
        case 7200:
            {
            iBaudrate = EBps7200;
            iBaudrateCap = KCapsBps7200;
            break;
            }
        case 9600:
            {
            iBaudrate = EBps9600;
            iBaudrateCap = KCapsBps9600;
            break;
            }
        case 19200:
            {
            iBaudrate = EBps19200;
            iBaudrateCap = KCapsBps19200;
            break;
            }
        case 38400:
            {
            iBaudrate = EBps38400;
            iBaudrateCap = KCapsBps38400;
            break;
            }
        case 57600:
            {
            iBaudrate = EBps57600;
            iBaudrateCap = KCapsBps57600;
            break;
            }        
         case 115200:
            {
            iBaudrate = EBps115200;
            iBaudrateCap = KCapsBps115200;
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }
    }

