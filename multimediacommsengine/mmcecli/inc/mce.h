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





#ifndef RMCE_H
#define RMCE_H

#include <e32std.h>
#include "mceitc.h"


class RMce : public RSessionBase, public MMceItc
	{
public: // Constructor

	RMce();

public: // From MSIPITC

    TInt Send (TMceItcFunctions aFunction, TIpcArgs& aArgs);
    void Receive (TIpcArgs& aArgs, TRequestStatus& aStatus);
    TInt Receive (TIpcArgs& aArgs, TPtr8 aContext, TPtr8 aContent );
    void CancelReceive ( TIpcArgs& aArgs );

public: // New functions

	TInt Connect (const TUid& aUid);
    TInt Connect ();
	void Close ();
	TVersion Version () const;

	TInt SendClientUid (const TUid& aUid) const;
	
private:
    void Panic (TInt aReason) const;

    TBool iConnected;

	};

#endif // end of RMCE_H

// End of File
