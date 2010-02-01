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





#include "mcecsreceivequeue.h"

// -----------------------------------------------------------------------------
// CMceCsReceiveQueue::CMceCsReceiveQueue
// -----------------------------------------------------------------------------
//
CMceCsReceiveQueue::CMceCsReceiveQueue ()
	{
    iReceiveItems.Reset();
	}	

// -----------------------------------------------------------------------------
// CMceCsReceiveQueue::~CMceCsReceiveQueue
// -----------------------------------------------------------------------------
//
CMceCsReceiveQueue::~CMceCsReceiveQueue ()
	{
	while (iReceiveItems.Count() > 0) 
        {
        RemoveFirst();
        }
	iReceiveItems.Close();
	}

// -----------------------------------------------------------------------------
// CMceCsReceiveQueue::IsEmpty
// -----------------------------------------------------------------------------
//
TBool CMceCsReceiveQueue::IsEmpty () const
	{ 
	return (iReceiveItems.Count() == 0); 
	}

// -----------------------------------------------------------------------------
// CMceCsReceiveQueue::First
// -----------------------------------------------------------------------------
//
TMceCSReceiveItem& CMceCsReceiveQueue::First ()
	{
    __ASSERT_ALWAYS (iReceiveItems.Count() > 0, 
                     User::Panic(KNullDesC, KErrGeneral));
    return iReceiveItems[0];
	}

// -----------------------------------------------------------------------------
// CMceCsReceiveQueue::AddLast
// -----------------------------------------------------------------------------
//
TInt CMceCsReceiveQueue::AddLast (const TMceCSReceiveItem& aItem)
    {
	return iReceiveItems.Append(aItem);
    }
    
// -----------------------------------------------------------------------------
// CMceCsReceiveQueue::RemoveFirst
// -----------------------------------------------------------------------------
//
void CMceCsReceiveQueue::RemoveFirst ()
	{
	if (iReceiveItems.Count() > 0) 
        {
        TMceCSReceiveItem& item = iReceiveItems[0];
        delete item.Context();
        delete item.Content();
	    iReceiveItems.Remove(0);
        }
	}

