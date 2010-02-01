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




inline CFCInterface::CFCInterface()
	{
	}

inline CFCInterface::~CFCInterface()
	{
	// Destroy any instance variables and then
	// inform the framework that this specific 
	// instance of the interface has been destroyed.
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}

inline CFCInterface* CFCInterface::NewL( const TUid& aImplUid, 
                                         TFCInterfaceInitParams& aParams )
	{
	// The CreateImplementationL method will return
	// the created item.
	return REINTERPRET_CAST( CFCInterface*, 
	    REComSession::CreateImplementationL( aImplUid,
										     _FOFF( CFCInterface, iDtor_ID_Key ),
											 &aParams ) );
	}

