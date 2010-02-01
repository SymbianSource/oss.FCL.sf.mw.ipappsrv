/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Connection multiplexer session abstraction.
*
*/




#ifndef C_CNCMLOCALADDRESSRESOLVER_H
#define C_CNCMLOCALADDRESSRESOLVER_H

#include <e32base.h>
#include <in_sock.h>

class RSocketServ;
class TSoInetInterfaceInfo;
class TSoInetIfQuery;

/**
 *  Connection multiplexer local IP resolver.
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmLocalAddressResolver ) : public CBase
    {

    friend class UT_CNcmLocalAddressResolver;
    
public:

    /**
     * Two-phased constructor
     *
     * @param    aSocketServ        Socket server
     */
    static CNcmLocalAddressResolver* NewL( RSocketServ& aSocketServ );

    /**
     * Two-phased constructor
     *
     * @param    aSocketServ        Socket server
     */
    static CNcmLocalAddressResolver* NewLC( RSocketServ& aSocketServ );
    
    /**
     * Destructor
     */
    virtual ~CNcmLocalAddressResolver();

    /**
     * Resolve local IP address
     *
     * @since    S60 v3.2
     * @param    aIPv4Addr          Resolved local IPv4 address
     * @param    aIPv6Addr          Resolved local IPv6 address
     * @param    aIap               IAP
     * @return   void
     */
    void GetLocalAddrL( TInetAddr& aIPv4Addr, TInetAddr& aIPv6Addr,
        TUint32 aIap );

private: 
        
    CNcmLocalAddressResolver( RSocketServ& aSocketServ );

    void CheckAndSetAddr( const TSoInetInterfaceInfo& aInfo,
        const TSoInetIfQuery& aQuery, TUint32 aSpecifiedIap );
                         
    TBool Match( const TSoInetInterfaceInfo& aInfo,
        const TSoInetIfQuery& aQuery, TUint32 aSpecifiedIap ) const;


private: // data

    /**
     * Socket server
     * Not own.
     */
    RSocketServ& iSocketServ;
    
    /**
     * IPv4 address
     */
    TInetAddr iIPv4Addr;
    
    /**
     * IPv6 address
     */
    TInetAddr iIPv6Addr; 
    };


#endif // C_CNCMLOCALADDRESSRESOLVER_H

