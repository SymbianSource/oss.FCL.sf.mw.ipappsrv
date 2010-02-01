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




#ifndef __CFCPLUGININFO_H__
#define __CFCPLUGININFO_H__

#include <e32base.h>

// CLASS DECLARATION
/**
* Information of FC plug-in. 
*/
class CFCPlugInInfo: public CBase
    {
    public:  // Constructors and destructor
        
	    /**
		* Constructs an object
		* @param aUid the plug-in implementation UID
	    * @param aName The name of the plugin. 
		* @returns new instance
		*/
		static CFCPlugInInfo* NewL( const TUid& aUid, const TDesC8& aName );

		/**
		* Destructor.
		*/
		~CFCPlugInInfo();
	    
	public: 
	    /**
	    * Return the name of the plug-in
	    * @return The name
	    */    
	    const TDesC8& Name() const;
	    
	    /**
	    * Return the implementation UID of the plug-in
	    * @return The UID
	    */    
	    const TUid& Uid() const;
	    
    private:
	
	    /**
        * Constructor
	    * @param aUid the plug-in implementation UID
	    */
	    CFCPlugInInfo( const TUid& aUid );
	    
        /**
        * Symbian 2nd phase constructor.
        * @param aName The name of the plugin.
        */
		void ConstructL( const TDesC8& aName );
	    
    private://Data
    
        // The plug-in implementation UID
	    const TUid iUid;
	    
	    // The name of the plug-in
	    HBufC8* iName;
    };

#endif

//  End of File
