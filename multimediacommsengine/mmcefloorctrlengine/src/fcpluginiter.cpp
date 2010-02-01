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




#include "fcpluginiter.h"
#include "fcplugininfo.h"

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
//
TFCPlugInIter::TFCPlugInIter(
	RPointerArray<CFCPlugInInfo>& aFCPlugIn) :
	iPos(0),
	iFCPlugIn(aFCPlugIn)	
	{
	}

// ----------------------------------------------------------------------------
// TFCPlugInIter::Previous
// ----------------------------------------------------------------------------
//
CFCPlugInInfo* TFCPlugInIter::Previous()
	{
	if (Count() > 0 && iPos > 0)
		{
		iPos--;
		return iFCPlugIn[iPos];
		}
	return NULL;
	}

// ----------------------------------------------------------------------------
// TFCPlugInIter::Next
// ----------------------------------------------------------------------------
//
CFCPlugInInfo* TFCPlugInIter::Next()
	{
	if (Count() > 0 && iPos < (Count() - 1))
		{
		iPos++;
		return iFCPlugIn[iPos];
		}
	return NULL;
	}

// ----------------------------------------------------------------------------
// TFCPlugInIter::First
// ----------------------------------------------------------------------------
//
CFCPlugInInfo* TFCPlugInIter::First()
	{
	if (Count() > 0)	
		{
		iPos = 0;
		return iFCPlugIn[iPos];
		}
	return NULL;
	}

// ----------------------------------------------------------------------------
// TFCPlugInIter::Last
// ----------------------------------------------------------------------------
//
CFCPlugInInfo* TFCPlugInIter::Last()
	{
	if (Count() > 0)
		{
		iPos = Count() - 1;
		return iFCPlugIn[iPos];
		}
	return NULL;
	}
	
// ----------------------------------------------------------------------------
// TFCPlugInIter::Current
// ----------------------------------------------------------------------------
//	
CFCPlugInInfo* TFCPlugInIter::Current()
	{
	if (Count() > 0)
		{
		return iFCPlugIn[iPos];
		}
	return NULL;
	}
	
// ----------------------------------------------------------------------------
// TFCPlugInIter::Count
// ----------------------------------------------------------------------------
//
TInt TFCPlugInIter::Count() const
	{
	return iFCPlugIn.Count();
	}
//  End of File
