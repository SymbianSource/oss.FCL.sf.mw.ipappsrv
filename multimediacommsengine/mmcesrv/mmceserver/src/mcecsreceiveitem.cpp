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
* Description:  
*
*/





#include "mcecsreceiveitem.h"

// -----------------------------------------------------------------------------
// TMceCSReceiveItem::TMceCSReceiveItem
// -----------------------------------------------------------------------------
//
TMceCSReceiveItem::TMceCSReceiveItem (const TMceIds& aIds,
                                      HBufC8* aContext,
                                      HBufC8* aContent,
                                      TInt aError)
 : iIds (aIds),
   iContext (aContext),
   iContent (aContent),
   iError (aError)
	{
	}

// -----------------------------------------------------------------------------
// TMceCSReceiveItem::TMceCSReceiveItem
// -----------------------------------------------------------------------------
//
TMceCSReceiveItem::TMceCSReceiveItem (const TMceIds& aIds, TInt aError)
 : iIds (aIds),
   iContext (0),
   iContent (0),
   iError (aError)
	{
	}

// -----------------------------------------------------------------------------
// TMceCSReceiveItem::Content
// -----------------------------------------------------------------------------
//
HBufC8* TMceCSReceiveItem::Context ()
    {
    return iContext;
    }
    
// -----------------------------------------------------------------------------
// TMceCSReceiveItem::Content
// -----------------------------------------------------------------------------
//
HBufC8* TMceCSReceiveItem::Content ()
    {
    return iContent;
    }

// -----------------------------------------------------------------------------
// TMceCSReceiveItem::Ids
// -----------------------------------------------------------------------------
//
const TMceIds& TMceCSReceiveItem::Ids () const
	{
    return iIds;
	}

// -----------------------------------------------------------------------------
// TMceCSReceiveItem::Error
// -----------------------------------------------------------------------------
//
TInt TMceCSReceiveItem::Error () const
	{
    return iError;
	}

// -----------------------------------------------------------------------------
// TMceCSReceiveItem::BufSizes
// -----------------------------------------------------------------------------
//
TMceMessageBufSizes TMceCSReceiveItem::BufSizes () const
    {
    TMceMessageBufSizes sizes;
    if (iContext) 
        {
        sizes.iContextBufSize = iContext->Length();
        }
    if (iContent)
        {
        sizes.iContentBufSize = iContent->Length();
        }
    return sizes;
    }
