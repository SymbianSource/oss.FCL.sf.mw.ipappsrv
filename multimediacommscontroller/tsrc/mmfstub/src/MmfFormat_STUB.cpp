/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include <mmf/server/mmfformat.h>
#include "AmrPayloadFormatRead.h"
#include "AmrPayloadFormatWrite.h"
#include "dtmfpayloadformatread.h"
#include "dtmfpayloadformatwrite.h"
//#include "mccredpayloadread.h"
//#include "mccredpayloadwrite.h"
#include "mccuids.hrh"

EXPORT_C CMMFFormatDecode* CMMFFormatDecode::NewL( TUid aUid, MDataSource* aSource )
    {
    if ( aUid.iUid == KImplUidAmrPayloadFormatDecode || 
         aUid.iUid == KImplUidAnyPayloadFormatDecode ||
         aUid.iUid == KImplUidRedPayloadFormatDecode ||
         aUid.iUid == KImplUidiLBCPayloadFormatDecode )
        {
        return CAmrPayloadFormatRead::NewL( aSource );
        }
    else if ( aUid.iUid == KImplUidDTMFPayloadFormatDecode )
        {
        return CDTMFPayloadFormatRead::NewL( aSource );
        }
//    else if ( aUid.iUid == KImplUidRedPayloadFormatDecode )
//        {
//        return CMccRedPayloadRead::NewL( aSource );
//        }
    User::Leave( KErrNotSupported );
    return NULL;
    }

EXPORT_C CMMFFormatEncode* CMMFFormatEncode::NewL( TUid aUid, MDataSink* aSink )
    {
    if ( aUid.iUid == KImplUidAmrPayloadFormatEncode )
        {
        return CAmrPayloadFormatWrite::NewL( aSink );
        }
    else if ( aUid.iUid == KImplUidDTMFPayloadFormatEncode )
        {
        return CDTMFPayloadFormatWrite::NewL( aSink );
        }        
    User::Leave( KErrNotSupported );
    return NULL;
    }
