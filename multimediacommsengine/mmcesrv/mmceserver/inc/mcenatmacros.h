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



#ifndef CMCENATMACROS_H
#define CMCENATMACROS_H

#include "mcenatstate.h"
#include "mcesipsession.h"

#define NAT_SESSION( session ) session.NatSession()
#define NAT_STATE( session, state ) ( state == session.NatState() )

#define NAT_WAIT( session )\
    ( NAT_SESSION( session ) &&\
      ( NAT_STATE( session, KMceNatStateCreateOffer ) ||\
        NAT_STATE( session, KMceNatStateCreateAnswer ) ||\
        NAT_STATE( session, KMceNatStateWait ) ||\
        NAT_STATE( session, KMceNatStateDecodeOffer ) ||\
        NAT_STATE( session, KMceNatStateConnected ) ) )

#define NAT_WAIT_NO_EXEC( session, exec )\
    if( !(NAT_WAIT( session )) ){ exec; }

#endif // CMCENATMACROS_H

// End of File
