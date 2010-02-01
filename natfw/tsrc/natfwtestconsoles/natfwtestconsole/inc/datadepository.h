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
* Description:    
*
*/




#ifndef __DATADEPOSITORY_H__
#define __DATADEPOSITORY_H__

#include "e32base.h" 

//Forward declare
class CRepository;

class CDataDepository : public CBase
{
public:
    static CDataDepository* NewL( CRepository* aRepository );
    static CDataDepository* NewLC( CRepository* aRepository );
        
    TUint32 CreateNewTableKeyL( TUint aTableMask, TUint32 aFieldMask ) const;
        
    void StoreL( const TUint32 aKey, const TInt aData );
    void StoreL( const TUint32 aKey, const TDesC8& aData );
        
    TInt Read( const TUint32 aKey, TInt& aData );
    TInt ReadL( const TUint32 aKey, HBufC8** aData );
        
    TInt EraseL( TUint32 aTableMask, TUint32 aFieldMask );

    ~CDataDepository( );

private: 
    CDataDepository( CRepository* aRepository );
    
private: //DATA
    CRepository* iRepository;
};

#endif //__DATADEPOSITORY_H__
