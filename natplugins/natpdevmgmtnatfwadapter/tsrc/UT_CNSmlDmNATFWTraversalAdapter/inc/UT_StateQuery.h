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
#ifndef UT_STATES
#define UT_STATES

#include <e32base.h>

class TStates
    {

public:
    TStates() : iVoipSupport( ETrue )
        {
        }
    
    void Clear()
        {
        iErrArray.Reset();
        }
    
    void SetErr( TInt aErr )
        {
        iErrArray.Append( aErr );
        }
    
    TInt Err()
        {
        TInt temp( 0 );
        if ( iErrArray.Count() != 0 )
            {
            temp = iErrArray[ 0 ];
            iErrArray.Remove( 0 );
            if ( iErrArray.Count() == 0 )
                {
                iErrArray.Reset();
                }
            }
        return temp;
        }
    
    void Clean()
        {
        iErrArray.Close();
        }

private:
    RArray<TInt> iErrArray;
public:
    TBool iVoipSupport;    
    };

#endif // UT_TSTATES
