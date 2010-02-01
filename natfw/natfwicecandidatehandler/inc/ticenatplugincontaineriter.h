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
* Description:    Iterator for traversing NAT-plugin container elements.
*
*/





#ifndef T_TICENATPLUGINCONTAINERITER_H
#define T_TICENATPLUGINCONTAINERITER_H

#include <e32base.h>
#include "cicenatplugincontainer.h"

/**
 *  Iterator for traversing plugins contained by NAT-plugin container.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class TIceNatPluginContainerIter
    {

public:

    /**
     * Constructor.
     *
     * @since   S60 3.2
     * @param   aContainer          The container to traverse
     */
    inline TIceNatPluginContainerIter( CIceNatPluginContainer& aContainer )
        : iIndex( 0 ), iContainer( aContainer ) { }
            
    /**
     * Set iterator position to the first item.
     *
     * @since   S60 3.2
     */
    inline void SetToFirst() 
        { iIndex = 0; };
    
    /**
     * Set iterator position to the last item.
     *
     * @since   S60 3.2
     */
    inline void SetToLast() 
        { iIndex = iContainer.iPlugins.Count() - 1; };
    
    /**
     * Return ETrue if all items are traversed.
     *
     * @since   S60 3.2
     */
    inline TBool IsDone() const 
        { return iContainer.iPlugins.Count() 
            ? ( iIndex >= iContainer.iPlugins.Count() ) : ETrue; };
    
    /**
     * Return item from current position.
     *
     * @since   S60 3.2
     */
    inline CNATFWPluginApi* CurrentItem()
        { 
        if ( IsDone() )
            {
            return NULL;
            }
        else
            {
            return iContainer.iPlugins[iIndex];
            }
        };

    inline CNATFWPluginApi* operator++( TInt )
        { return iIndex < iContainer.iPlugins.Count() 
            ? iContainer.iPlugins[iIndex++] : NULL; };

private:

    TIceNatPluginContainerIter();

private: // data

    /**
     * The position in the container.
     */
    TInt iIndex;

    /**
     * Container for which iterator is created.
     * Not own.
     */
    CIceNatPluginContainer& iContainer;
    };

#endif // T_TICENATPLUGINCONTAINERITER_H
