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
* Description:  Project wide definitions.
*
*/

#ifndef NSPDEFS_H
#define NSPDEFS_H

#include "nsppluginreturndef.h"
#include <sdpconnectionfield.h>
#include <sdpmediafield.h>

#ifndef COMPONENT
#define COMPONENT
_LIT( KComponent, "NATFW SDP Provider" );
#endif // COMPONENT

#define __PANIC( aInt ) User::Panic( KComponent, aInt )

#define NSP_ERROR( status ) ( KErrNone > status )
#define NSP_ICMP( aError ) ( KErrCommsLineFail == aError )
#define NSP_TRIGGER( status ) ( KNatReady == status )
#define NSP_DELETE( status ) ( KNatReady == status )

_LIT( KNSPPrintFormat, "%S: %S" );
_LIT( KNSPPrintFormat2, "%S: %S %S" );
_LIT( KNSPPrintFormat3, "%S: %S %d" );
_LIT( KNSPPrintFormat4, "%S: %S %u" );

inline static TInetAddr ConvertTDesC8ToTInetAddrL( const TDesC8& aDesc, TUint aPort )
    {
    RBuf address;
    address.CreateL( aDesc.Length() );
    address.CleanupClosePushL();
    address.Copy( aDesc );
    
    TInetAddr addr;
    User::LeaveIfError( addr.Input( address ) );
    CleanupStack::PopAndDestroy( &address );
    
    addr.SetPort( aPort );
    return addr;
    }

inline static void ConvertTInetAddrToTDes8AndTUintL( const TInetAddr& aAddress,
    TDes8& aDes, TUint& aPort )
    {
    RBuf address;
    address.CreateL( 40 );
    address.CleanupClosePushL();
    
    aAddress.Output( address );
    aDes.Copy( address );
    CleanupStack::PopAndDestroy( &address );
    
    aPort = aAddress.Port();
    }

inline static const TDesC8& Address( CSdpMediaField& aMediafield )
    {
    return ( aMediafield.ConnectionFields().Count() ?
            aMediafield.ConnectionFields()[0]->Address() : KNullDesC8 );
    }

#define CONN_ADDR( mediafield, sessionAddr ) \
        ( Address( mediafield ).Length() ? Address( mediafield ) : sessionAddr )


#if (defined _DEBUG)

#include <e32debug.h>
#include <e32base.h>
#include <s32mem.h>
#include <sdpdocument.h>

class RBufWriteStream;

const TText* const KWarning = _S( "Text contains not allowed characters, log ignored" );
const TText8* const KCRLF = _S8( "\r\n" );
const TUint KCRLFLength = 2;
const TUint KMaxLengthOfSDPMessage = 1000;

static inline void PrintL( const TDesC& aStr )
	{
	HBufC* line = HBufC::NewLC( 256 );
	TPtr ptr( line->Des() );
	
	if ( KErrNotFound == aStr.Locate( TChar( '%' ) ) )
		{
		ptr.Format( KNSPPrintFormat(), &KComponent(), &aStr );
		}
	else
		{
		TPtrC warning( KWarning );
		ptr.Format( KNSPPrintFormat(), &KComponent(), &warning );
		}
	
	RDebug::Print( line->Des() );
	CleanupStack::PopAndDestroy( line );
	}
static inline void Print( const TDesC& aStr )
	{
	TRAP_IGNORE( PrintL( aStr ) );
	}

static inline void PrintL( const TDesC& aStr1, const TDesC& aStr2 )
	{
	HBufC* line = HBufC::NewLC( 256 );
	TPtr ptr( line->Des() );
	
	if ( KErrNotFound == aStr1.Locate( TChar( '%' ) ) )
		{
		ptr.Format( KNSPPrintFormat2(), &KComponent(), &aStr1, &aStr2 );
		}
	else
		{
		TPtrC warning( KWarning );
		ptr.Format( _L( "%S: %S" ), &KComponent(), &warning );
		}
	
	RDebug::Print( line->Des() );
	CleanupStack::PopAndDestroy( line );
	}
static inline void Print( const TDesC& aStr1, const TDesC& aStr2 )
	{
	TRAP_IGNORE( PrintL( aStr1, aStr2 ) );
	}

static inline void PrintL( const TDesC& aStr, TInt aTInt )
	{
	HBufC* line = HBufC::NewLC( 256 );
	TPtr ptr( line->Des() );
	
	if ( KErrNotFound == aStr.Locate( TChar( '%' ) ) )
		{
		ptr.Format( KNSPPrintFormat3(), &KComponent(), &aStr, aTInt );
		}
	else
		{
		TPtrC warning( KWarning );
		ptr.Format( _L( "%S: %S" ), &KComponent(), &warning );
		}
	
	RDebug::Print( line->Des() );
	CleanupStack::PopAndDestroy( line );
	}
static inline void Print( const TDesC& aStr, TInt aTInt )
	{
	TRAP_IGNORE( PrintL( aStr, aTInt ) );
	}

static inline void PrintL( const TDesC& aStr, TUint aTUint )
	{
	HBufC* line = HBufC::NewLC( 256 );
	TPtr ptr( line->Des() );
	
	if ( KErrNotFound == aStr.Locate( TChar( '%' ) ) )
		{
		ptr.Format( KNSPPrintFormat4(), &KComponent(), &aStr, aTUint );
		}
	else
		{
		TPtrC warning( KWarning );
		ptr.Format( _L( "%S: %S" ), &KComponent(), &warning );
		}
	
	RDebug::Print( line->Des() );
	CleanupStack::PopAndDestroy( line );
	}
static inline void Print( const TDesC& aStr, TUint aTUint )
	{
	TRAP_IGNORE( PrintL( aStr, aTUint ) );
	}

static inline void Log8L( const TDesC8& aDesC8 )
	{
	HBufC* buffer = HBufC::NewL( aDesC8.Length() );
	TPtr ptr( buffer->Des() );
	ptr.Copy( aDesC8 );
	Print( buffer->Des() );
	delete buffer;
	}

static inline void Log8L( const TDesC& aStr, const TDesC8& aDesC8 )
	{
	HBufC* buffer = HBufC::NewL( aDesC8.Length() );
	TPtr ptr( buffer->Des() );
	ptr.Copy( aDesC8 );
	Print( aStr, buffer->Des() );
	delete buffer;
	}

static inline HBufC8* EncodeSdpDocumentL( const CSdpDocument& aDoc )
	{
	CBufFlat* encBuf = CBufFlat::NewL( KMaxLengthOfSDPMessage );
	CleanupStack::PushL( encBuf );
	RBufWriteStream writeStream;
	CleanupClosePushL( writeStream );
	writeStream.Open( *encBuf );
	aDoc.EncodeL( writeStream );
	CleanupStack::PopAndDestroy( &writeStream );
	HBufC8* buff = encBuf->Ptr(0).AllocL();
	CleanupStack::PopAndDestroy( encBuf );
	return buff;
	}

#define NSPLOG_STR( aLit ) \
	{ _LIT( KStr, aLit ); Print( KStr() ); }
#define NSPLOG_STR2( aLit, b16 ) \
	{ _LIT( KStr, aLit ); Print( KStr(), b16 ); }
#define NSPLOG_STR3( aLit, b16, c16 ) \
	{ _LIT( KStr, aLit ); Print( KStr(), b16, c16 ); }
#define NSPLOG_INT( aLit, bInt ) \
	{ _LIT( KStr, aLit ); Print( KStr(), bInt ); }
#define NSPLOG_UINT( aLit, bUInt ) \
	{ _LIT( KStr, aLit ); Print( KStr(), bUInt ); }

#define NSPLOG8_STR( a8 ) \
	{ Log8L( a8 ); }
#define NSPLOG8_STR2( aLit, b8 ) \
	{ _LIT( KStr, aLit ); Log8L( KStr(), b8 ); }

#define NSPLOG8_DOC_L( aDoc ) \
	{\
	HBufC8* buffer = EncodeSdpDocumentL( aDoc );\
	CleanupStack::PushL( buffer );\
	TPtrC8 ptr( buffer->Des() );\
	while( ptr.Length() )\
		{\
		const TInt length = ptr.Length();\
		const TInt index = ptr.Find( TPtrC8( KCRLF ) );\
		const TInt left = ( KErrNotFound != index ? index : length );\
		const TInt right = ( length - KCRLFLength - left );\
		HBufC8* line = ptr.Left( left ).AllocLC();\
		Log8L( line->Des() );\
		CleanupStack::PopAndDestroy( line );\
        ptr.Set( ptr.Right( 0 < right ? right : 0 ) );\
	  }\
	CleanupStack::PopAndDestroy( buffer );\
	}

#define NSPDEBUG_PANIC( aInt ) \
	NSPLOG_INT( "NATFW SDP Provider!! code:", aInt ) __PANIC( aInt )

#if (defined NSP_PRINT_INPUT)

#define NSP_INPUT_OFFER_L( aOffer ) \
    {\
	NSPLOG_STR( "============ INPUT OFFER =============" )\
    NSPLOG8_DOC_L( aOffer )\
    NSPLOG_STR( "======================================" )\
    }
#define NSP_INPUT_OFFER( aOffer ) \
    TRAP_IGNORE( NSP_INPUT_OFFER_L( aOffer ) );
#define NSP_INPUT_ANSWER_L( aAnswer ) \
    {\
    NSPLOG_STR( "============ INPUT ANSWER ============" )\
    NSPLOG8_DOC_L( aAnswer )\
    NSPLOG_STR( "======================================" )\
    }
#define NSP_INPUT_ANSWER( aAnswer ) \
    TRAP_IGNORE( NSP_INPUT_ANSWER_L( aAnswer ) );

#else // NSP_PRINT_INPUT

#define NSP_INPUT_OFFER_L( aOffer )
#define NSP_INPUT_OFFER( aOffer )
#define NSP_INPUT_ANSWER_L( aAnswer )
#define NSP_INPUT_ANSWER( aAnswer )

#endif // NSP_PRINT_INPUT

#if (defined NSP_PRINT_OUTPUT)

#define NSP_OUTPUT_INITIALIZED \
	{\
	NSPLOG_STR( "============ INITIALIZED =============" )\
	NSPLOG_STR( "======================================" )\
	}
#define NSP_OUTPUT_OFFER_L( aStatus, aOffer ) \
    if ( KNatReady == aStatus )\
		{\
		NSPLOG_STR( "============ OUTPUT OFFER ============" )\
	    NSPLOG8_DOC_L( aOffer )\
	    NSPLOG_STR( "======================================" )\
		}
#define NSP_OUTPUT_OFFER( aStatus, aOffer ) \
    TRAP_IGNORE( NSP_OUTPUT_OFFER_L( aStatus, aOffer ) );
#define NSP_OUTPUT_ANSWER_L( aStatus, aAnswer ) \
    if ( KNatReady == aStatus )\
		{\
	    NSPLOG_STR( "============ OUTPUT ANSWER ===========" )\
	    NSPLOG8_DOC_L( aAnswer )\
	    NSPLOG_STR( "======================================" )\
		}
#define NSP_OUTPUT_ANSWER( aStatus, aAnswer )  \
    TRAP_IGNORE( NSP_OUTPUT_ANSWER_L( aStatus, aAnswer ) );
#define NSP_OUTPUT_UPDATESDP_L( aStatus, aOffer ) \
	if ( KNatReady == aStatus )\
		{\
		NSPLOG_STR( "============ UPDATED SDP =============" )\
	    NSPLOG8_DOC_L( aOffer )\
		NSPLOG_STR( "======================================" )\
		}
#define NSP_OUTPUT_UPDATESDP( aStatus, aOffer ) \
	TRAP_IGNORE( NSP_OUTPUT_UPDATESDP_L( aStatus, aOffer ) );
#define NSP_OUTPUT_ERROR( aError ) \
	{\
	NSPLOG_STR( "============ OUTPUT ERROR ============" )\
	NSPLOG_INT( ":", aError )\
	NSPLOG_STR( "======================================" )\
	}


#else // NSP_PRINT_OUTPUT

#define NSP_OUTPUT_INITIALIZED
#define NSP_OUTPUT_OFFER_L( aStatus, aOffer )
#define NSP_OUTPUT_OFFER( aStatus, aOffer )
#define NSP_OUTPUT_ANSWER_L( aStatus, aAnswer )
#define NSP_OUTPUT_ANSWER( aStatus, aAnswer )
#define NSP_OUTPUT_UPDATESDP_L( aStatus, aOffer )
#define NSP_OUTPUT_UPDATESDP( aStatus, aOffer )
#define NSP_OUTPUT_ERROR( aError )

#endif // NSP_PRINT_OUTPUT

#else // _DEBUG

#define NSPLOG_STR( aLit )
#define NSPLOG_STR2( aLit, b16 )
#define NSPLOG_STR3( aLit, b16, c16 )
#define NSPLOG_INT( aLit, bInt )
#define NSPLOG_UINT( aLit, bUInt )
#define NSPLOG8_STR( a8 )
#define NSPLOG8_STR2( aLit, b8 )
#define NSPLOG8_DOC_L( aDoc )
#define NSPDEBUG_PANIC( aInt )

#define NSP_INPUT_OFFER_L( aOffer )
#define NSP_INPUT_OFFER( aOffer )
#define NSP_INPUT_ANSWER_L( aAnswer )
#define NSP_INPUT_ANSWER( aAnswer )
#define NSP_OUTPUT_INITIALIZED
#define NSP_OUTPUT_OFFER_L( aStatus, aOffer )
#define NSP_OUTPUT_OFFER( aStatus, aOffer )
#define NSP_OUTPUT_ANSWER_L( aStatus, aAnswer )
#define NSP_OUTPUT_ANSWER( aStatus, aAnswer )
#define NSP_OUTPUT_UPDATESDP_L( aStatus, aOffer )
#define NSP_OUTPUT_UPDATESDP( aStatus, aOffer )
#define NSP_OUTPUT_ERROR( aError )

#endif // _DEBUG

#endif // NSPDEFS_H

// end of file

