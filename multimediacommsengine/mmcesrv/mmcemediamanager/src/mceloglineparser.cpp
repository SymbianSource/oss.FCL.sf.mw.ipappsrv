/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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



#include "mceloglineparser.h"

// -----------------------------------------------------------------------------
// TMceLogLineParser::TMceLogLineParser
// -----------------------------------------------------------------------------
//
TMceLogLineParser::TMceLogLineParser(
    const TDesC8& aMessage,
    TUint aMaxLineLength)
 : iMaxLineLength(aMaxLineLength),
   iMessage(aMessage),
   iCurrentPos(0)
	{
	}

// -----------------------------------------------------------------------------
// TMceLogLineParser::End
// -----------------------------------------------------------------------------
//
TBool TMceLogLineParser::End()
	{
	return (iCurrentPos >= iMessage.Length());
	}

// -----------------------------------------------------------------------------
// TMceLogLineParser::GetLine
// -----------------------------------------------------------------------------
//
TPtrC8 TMceLogLineParser::GetLine()
	{
	if (End())
	    {
	    return TPtrC8();
	    }
    TPtrC8 ptr(iMessage.Mid(iCurrentPos));
    TUint lineEndOffset = 0;
    TInt nextLineEndPos = FindNextLineEndPos(ptr,lineEndOffset);
    TUint length;
    if (nextLineEndPos == KErrNotFound)
		{
        if (ptr.Length() < iMaxLineLength) 
			{
            length = ptr.Length();
			}
        else
			{
            length = iMaxLineLength;
			}
		}
    else
		{
        if (nextLineEndPos <= iMaxLineLength)
			{
            length = nextLineEndPos;
            iCurrentPos += lineEndOffset;
			}
        else
			{
            length = iMaxLineLength;
			}
		}
    iCurrentPos += length;
    return ptr.Left(length);
	}

// -----------------------------------------------------------------------------
// TMceLogLineParser::FindNextLineEndPos
// -----------------------------------------------------------------------------
//
TInt TMceLogLineParser::FindNextLineEndPos(
    const TDesC8& aDes,
    TUint& aLineEndOffset)
	{
    TInt crPos = aDes.Locate ('\r');
    TInt lfPos = aDes.Locate ('\n');

    TInt pos = KErrNotFound;
    if (!(crPos == KErrNotFound))
		{
        aLineEndOffset = 1;
        if (!(lfPos == KErrNotFound))
			{
            if (crPos < lfPos)
				{
                if (crPos+1 == lfPos) aLineEndOffset = 2;
                pos = crPos;
				}
            else
				{
                pos = lfPos;
				}
			}
        else
			{
            pos = crPos;
			}
		}
    else
		{
        if (!(lfPos == KErrNotFound))
			{
            aLineEndOffset = 1;
            pos = lfPos;
			}
		}
    return pos;
	}
