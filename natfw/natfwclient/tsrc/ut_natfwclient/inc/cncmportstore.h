/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Class for reserving ports on specific range
*
*/




#ifndef NATFWPORTSTORE_H
#define NATFWPORTSTORE_H

#include <e32base.h>

/**
 *  Class for reserving ports on specific range
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class CNcmPortStore : public CBase
    {

public:

    /**
     * Two-phased constructor
     *
     * @param   aStartPort     Port range start for session
     * @param   aStopPort      Port range stop for session
     */
    static CNcmPortStore* NewL( TUint aStartPort, TUint aStopPort );

    /**
     * Two-phased constructor
     *
     * @param   aStartPort     Port range start for session
     * @param   aStopPort      Port range stop for session
     */
    static CNcmPortStore* NewLC( TUint aStartPort, TUint aStopPort );

    /**
     * Destructor
     */
    virtual ~CNcmPortStore();

    /**
     * Retrives a free port for usage
     *
     * @since   S60 v3.2
     * @return  Usable port, or zero on error
     */
    IMPORT_C TUint Port();

    /**
     * Frees reserved port
     *
     * @since S60 v3.2
     * @param aPort         Port to be freed
     */
    IMPORT_C void FreePort( TUint aPort );

private:

    CNcmPortStore( TUint aStartPort, TUint aStopPort );

private: // data

    /**
     * Reserved ports are stored here
     */
    RArray<TUint> iReservedPorts;

    /**
     * First port on range
     */
    TUint iStartPort;

    /**
     * Last port on range
     */
    TUint iStopPort;

    };


#endif // NATFWPORTSTORE_H
