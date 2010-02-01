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





#ifndef CMCELOCALADDRRESOLVER_H
#define CMCELOCALADDRRESOLVER_H

#include <in_sock.h>
#include "mcesrv.h"


class CMceLocalAddrResolver : public CBase
	{ 
    public: // Constructors and destructor

    	static CMceLocalAddrResolver* NewL( 
    	    RSocketServ& aSocketServ,
    	    TUint32 aIapId );
    	
    	static CMceLocalAddrResolver* NewLC(
    	    RSocketServ& aSocketServ,
    	    TUint32 aIapId );    	
    	
    	~CMceLocalAddrResolver();
    	
    public: // New functions	
    	
    	const TInetAddr& Addr() const;
    	
    	TBool HasIPv4Addr()	const;
    	
    	const TInetAddr& IPv4Addr() const;
    	
    	void RefreshLocalAddressesL();

    private: // Construction

    	CMceLocalAddrResolver( RSocketServ& aSocketServ );
        void ConstructL( TUint32 aIapId );

    private: // New functions

        TBool Match( const TSoInetInterfaceInfo& aInfo,
                     const TSoInetIfQuery& aQuery ) const;
        
        void SetAddressesIfNotSet( const TInetAddr& aAddr );
        
    private: // Data

        RSocketServ& iSocketServ;
        TUint32 iIapId;
    	TInetAddr iAddr;
    	TInetAddr iIPv4Addr;
    	
    private: // For testing purposes

    	MCESRV_UT_DEFINITIONS
	};
	
#endif // CMCELOCALADDRRESOLVER_H

// End of File
