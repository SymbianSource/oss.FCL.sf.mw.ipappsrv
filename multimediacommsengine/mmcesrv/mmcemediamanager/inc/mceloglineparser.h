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




#ifndef TMCELOGLINEPARSER_H
#define TMCELOGLINEPARSER_H

#include <e32std.h>

class TMceLogLineParser
	{
    public:

    	TMceLogLineParser(const TDesC8& aMessage, TUint aMaxLineLength);
    	TBool End();
        TPtrC8 GetLine();

    private:

    	TInt FindNextLineEndPos(const TDesC8& aDes, TUint& aLineEndOffset);

    private: // Data

    	TInt iMaxLineLength;
    	TPtrC8 iMessage;
    	TInt iCurrentPos;
	};

#endif // TMCELOGLINEPARSER_H

// End of File
