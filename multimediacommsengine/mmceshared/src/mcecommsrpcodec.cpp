/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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




#include "mcemsrpcodec.h"
#include "mcecommsrpcodec.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <MmccCodecInformation.h>
#include "mcesrvstream.h"
#include "mcesrvsource.h"

#include <MmccCodecInformationFactory.h>
#include <MmccCodecInformation.h>

//levels two redundancy is default
_LIT8( KDefaultMsrpFmtp, "a=Message dummy codec" );

#endif//MCE_COMMON_SERVER_SIDE


// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CMceComMsrpCodec::NewL
// -----------------------------------------------------------------------------
//
CMceComMsrpCodec* CMceComMsrpCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComMsrpCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceComMsrpCodec::NewLC
// -----------------------------------------------------------------------------
//
CMceComMsrpCodec* CMceComMsrpCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComMsrpCodec* self = new (ELeave) CMceComMsrpCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComMsrpCodec::~CMceComMsrpCodec
// -----------------------------------------------------------------------------
//
CMceComMsrpCodec::~CMceComMsrpCodec()
    {
    }
    

// -----------------------------------------------------------------------------
// CMceComMsrpCodec::CMceComMsrpCodec
// -----------------------------------------------------------------------------
//
CMceComMsrpCodec::CMceComMsrpCodec()
 : CMceComMessageCodec()
    {

    }



// -----------------------------------------------------------------------------
// CMceComMsrpCodec::ConstructL
// -----------------------------------------------------------------------------
//
CMceComMessageCodec* CMceComMsrpCodec::CloneL()
    {
    CMceComMsrpCodec* copy = new (ELeave) CMceComMsrpCodec();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    
    }


// -----------------------------------------------------------------------------
// CMceComMsrpCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComMsrpCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComMessageCodec::ConstructL( aSdpName );
    
    }


// -----------------------------------------------------------------------------
// CMceComMsrpCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComMsrpCodec::ConstructL( CMceComMsrpCodec& aCodec )
    {
    CMceComMessageCodec::ConstructL( aCodec );
    }


// -----------------------------------------------------------------------------
// CMceComMsrpCodec::SetBitrate
// -----------------------------------------------------------------------------
//
TInt CMceComMsrpCodec::SetBitrate( TUint /*aBitrate*/ )
    {
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMceComMsrpCodec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMceComMsrpCodec::SetAllowedBitrates( TUint /*aBitrates*/ )
    {
    return KErrNone;	
    }

// ---------------------------------------------------------
// CMceComMsrpCodec::GetMaxBitRate
// ---------------------------------------------------------
TUint CMceComMsrpCodec::GetMaxBitRate()
	{
    return 0;	
	}
	
// ---------------------------------------------------------
// CMceComMsrpCodec::ResolveAllowedBitrates
// ---------------------------------------------------------
//
TInt CMceComMsrpCodec::ResolveAllowedBitrates( 
    TUint /*aBitrate*/, TUint& /*aAllowedBitrates*/ )
    {
    return KErrNone;
    }

#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComMsrpCodec::DoMccValidateL
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceComMsrpCodec::DoMccValidateL( CMccCodecInformation& /*aMccCodec*/,
                                               CMceSrvStream& /*aStream*/,
                                               TMceNegotiationRole /*aRole*/ )
    {	
	return NULL;
    }

// -----------------------------------------------------------------------------
// CMceComMsrpCodec::DoSetDefaultFmtpAttributeL
// -----------------------------------------------------------------------------
//
void CMceComMsrpCodec::DoSetDefaultFmtpAttributeL()
    {
    SetFmtpAttributeL( KDefaultMsrpFmtp );   
    }

#endif

// End of File
