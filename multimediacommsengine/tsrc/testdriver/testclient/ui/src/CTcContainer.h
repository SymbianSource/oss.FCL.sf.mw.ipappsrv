/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  See class definition below.
*
*/

#ifndef __CTCCONTAINER_H__
#define __CTCCONTAINER_H__

// INCLUDES
#include <coecntrl.h>

#ifdef __SERIES60_
#include <aknutils.h>
#endif

// FORWARD DECLARATIONS
class CEikLabel;

// CLASS DEFINITION
/**
 * CTcContainer implements the application main view control.
 * It is responsible for displaying application state information
 * (e.g. test case name and CTRL request).
 */
class CTcContainer
	: public CCoeControl
	{
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aRect View area for this control.
		 * @return An initialized instance of this class
		 */
		static CTcContainer* NewL( const TRect& aRect );

		/// Destructor
		~CTcContainer();

	private: // Constructors

		/// Default constructor
		CTcContainer();

		/**
		 * 2nd phase constructor.
		 *
		 * @param aRect View area for this control.
		 */
		void ConstructL( const TRect& aRect );

	public:  // from CCoeControl

		void Draw( const TRect& aRect ) const;

	protected: //  from CCoeControl

		TInt CountComponentControls() const;
		CCoeControl* ComponentControl( TInt aIndex ) const;
		
#ifdef __SERIES60_

		void SizeChanged();
		void HandleResourceChange( TInt aType );
#endif

	public: // New methods

		/**
		 * Query state of a TestRunner instance and update status
		 * displays accordingly.
		 * This is called from CTcAppUi::NotifyStatusChange()
		 *
		 * @param aRunner TestRunner reference
		 */
		void Update( CTcTestRunner& aRunner );

		/**
		 * Update the Status label with the given text
		 * This is called from CTcAppUi::SetStatus()
		 *
		 * @param aStatus New status text
		 */
		void UpdateStatus( const TDesC8& aStatus );

	private: // New methods

		/**
		 * This is an internal helper function for creating and laying out
		 * CEikLabel controls.
		 *
		 * @param aRect View area for the created label
		 * @param aText Default text for the label
		 * @return An initialized label control.
		 */
		CEikLabel* CreateLabelL( const TRect& aRect, const TDesC& aText );

		/**
		 * Changes the text of one of the labels in the array
		 *
		 * @param aLabelId Label array index
		 * @param aText new text for the label
		 */
		void SetLabelText( TInt aLabelId, const TDesC8& aText );
		
		void CreateLabelsL( const TRect& aRect );
		
#ifdef __SERIES60_
	
	    AknLayoutUtils::SAknLayoutTextMultiline GetLabelLayout( const TRect& aRect, 
	                                                            TInt aLabelBase, 
	                                                            TAknLogicalFontId aFontId ) const;
	                                                            
	    TInt FontTotalHeightInPixels( TAknLogicalFontId aFontId ) const;
	    
	    void ReLayoutAllLabels( const TRect& aRect );
	    
#endif

	private: // Data

		/// Label controls. Owned.
		CArrayPtrSeg< CEikLabel > iLabels;

	};

#endif // __CTCCONTAINER_H__
