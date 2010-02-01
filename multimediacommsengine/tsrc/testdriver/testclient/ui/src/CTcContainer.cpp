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
* Description:  Implementation.
*
*/

#include <bldvariant.hrh>
#include <eikenv.h>
#include <eiklabel.h>

#include "CTcTestRunner.h"
#include "CTcContainer.h"
#include "TestClientConstants.h"

#ifdef __SERIES90_
#include <cknenv.h>	// skin support
#endif

#ifdef __SERIES60_
#include <akndef.h>
#endif


CTcContainer* CTcContainer::NewL( const TRect& aRect )
	{
	CTcContainer* self = new( ELeave ) CTcContainer;

	CleanupStack::PushL( self );
	self->ConstructL( aRect );
	CleanupStack::Pop( self );

	return self;
	}

CTcContainer::~CTcContainer()
	{
	// Delete all labels
	iLabels.ResetAndDestroy();
	}

CTcContainer::CTcContainer()
	: iLabels( 1 )	// Minimum granularity
	{
	}

void CTcContainer::ConstructL( const TRect& aRect )
	{
	// Create a window for this application view
	CreateWindowL();

#ifdef __SERIES90_
	// Set skin configuration
	CCknSkinBase& skin = CknEnv().Skin();
	skin.SetAppViewType( ESkinAppViewNoCbaNoToolband );
#endif
    
    CreateLabelsL( aRect );

	// Set windows size and activate it
	SetRect( aRect );
	ActivateL();
	}

void CTcContainer::Draw( const TRect& aRect ) const
	{
	// Clear the view area
	CWindowGc& gc = SystemGc();
	gc.Clear( aRect );
#ifdef __SERIES90_
	CknEnv().Skin().DrawAppView( gc, aRect );
#endif
	}

TInt CTcContainer::CountComponentControls() const
	{
	// Return amount of label control
	return iLabels.Count();
	}

CCoeControl* CTcContainer::ComponentControl( TInt aIndex ) const
	{
	// Return pointer to a label control
	return iLabels[ aIndex ];
	}

void CTcContainer::Update( CTcTestRunner& aRunner )
	{
	// Update "Value" labels
	SetLabelText( ETestCaseValue, aRunner.CurrentTestCaseName() );
	SetLabelText( ERequestValue, aRunner.CurrentRequestName() );

	// Force screen update NOW
	DrawDeferred();
	CEikonEnv::Static()->WsSession().Flush();
	}

void CTcContainer::UpdateStatus( const TDesC8& aStatus )
	{
	// Update "Status" label
	SetLabelText( EStatusValue, aStatus );

	// Force screen update NOW
	DrawNow();
	CEikonEnv::Static()->WsSession().Flush();
	}

CEikLabel* CTcContainer::CreateLabelL( const TRect& aRect, const TDesC& aText )
	{
	// Create new label and add it to the label array
	CEikLabel* label = new( ELeave ) CEikLabel;
	CleanupStack::PushL( label );
	iLabels.AppendL( label );
	CleanupStack::Pop( label );

	// Set label container, area and text
	label->SetContainerWindowL( *this );
	label->SetRect( aRect );
	label->SetTextL( aText );

	return label;
	}

void CTcContainer::SetLabelText( TInt aLabelId, const TDesC8& aText )
	{
	// 8bit to unicode conversion
	TBuf< KMaxLabelSize > tmp16;
	tmp16.Copy( aText );

	// replace an empty text with "<none>"
	if( tmp16 == KNullDesC )
		{
		tmp16 = KNone;
		}

	// Set text, ignore any errors
	TRAP_IGNORE( iLabels[ aLabelId ]->SetTextL( tmp16 ) )
	}
	
#ifdef __SERIES60_

void CTcContainer::SizeChanged()
    {
    ReLayoutAllLabels( Rect() );
    }

void CTcContainer::HandleResourceChange( TInt aType )
	{
	CCoeControl::HandleResourceChange( aType );
	
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        TRect mainPaneRect;
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, 
                                           mainPaneRect );
        SetRect( mainPaneRect );
        DrawNow();
        }
	}
	
AknLayoutUtils::SAknLayoutTextMultiline CTcContainer::GetLabelLayout( 
    const TRect& aRect, TInt aLabelBase, TAknLogicalFontId aFontId ) const
    {
    /*
    SAknLayoutTextMultiline struct:
    
    @param iFont      font id, ELatinPlain12, ELatinBold12, ELatinBold13 etc..
	@param iC         colour index, 0..255
	@param iL         left margin
	@param iR         right margin
	@param iB         Baseline from top of the parent rectangle
	@param iW         text width in pixels
	@param iJ         justification. ELayoutAlignNone; ELayoutAlignCenter;
	                  ELayoutAlignLeft; ELayoutAlignRight; ELayoutAlignBidi
	                  
	@param iNumberOfLinesShown
	@param iNextLineB baseline of 2nd line for multi-line labels/editors
	*/

    const TInt KFontColorIndexBlack( 200 );	

	AknLayoutUtils::SAknLayoutTextMultiline labelLayout;
	labelLayout.iFont = aFontId;
    labelLayout.iC = KFontColorIndexBlack;
    labelLayout.iL = KLabelTlX;
    labelLayout.iR = KLabelTlX;
    labelLayout.iB = aLabelBase;
    labelLayout.iW = aRect.Width() - labelLayout.iL - labelLayout.iR;
    labelLayout.iJ = ELayoutAlignLeft;
    labelLayout.iNumberOfLinesShown = 1;
    return labelLayout;
    }

TInt CTcContainer::FontTotalHeightInPixels( TAknLogicalFontId aFontId ) const
    {
    const CFont* font = AknLayoutUtils::FontFromId( aFontId );
    
   	_LIT( KPanicCntx, "TestClient" );
	__ASSERT_ALWAYS( font, User::Panic( KPanicCntx(), KErrNotFound ) );
	
	return  font->HeightInPixels();
    }

void CTcContainer::CreateLabelsL( const TRect& aRect )
    {
    // Create labels
	CEikLabel* label;
	
	TInt primaryFontTotalHeight( FontTotalHeightInPixels( EAknLogicalFontPrimaryFont ) );
	TInt secondaryFontTotalHeight( FontTotalHeightInPixels( EAknLogicalFontSecondaryFont ) );
	
	TInt labelBase = KLabelTlY + primaryFontTotalHeight;	
    AknLayoutUtils::SAknLayoutTextMultiline labelLayout = 
        GetLabelLayout( aRect, labelBase, EAknLogicalFontPrimaryFont );
	TRect rect( KLabelTlX, KLabelTlY, 
	            aRect.Width() - KLabelTlX, KLabelTlY + primaryFontTotalHeight );

    label = CreateLabelL( rect, KTestCase );
    AknLayoutUtils::LayoutLabel( label, rect, labelLayout );
    
	rect.Move( 0, secondaryFontTotalHeight );
	labelBase += secondaryFontTotalHeight;
	labelLayout = GetLabelLayout( aRect, labelBase, EAknLogicalFontSecondaryFont );
	label = CreateLabelL( rect, KNone );
    AknLayoutUtils::LayoutLabel( label, rect, labelLayout );

	rect.Move( 0, primaryFontTotalHeight );
	labelBase += primaryFontTotalHeight;
	labelLayout = GetLabelLayout( aRect, labelBase, EAknLogicalFontPrimaryFont );
	label = CreateLabelL( rect, KRequest );
	AknLayoutUtils::LayoutLabel( label, rect, labelLayout );
	rect.Move( 0, secondaryFontTotalHeight );
	labelBase += secondaryFontTotalHeight;
	labelLayout = GetLabelLayout( aRect, labelBase, EAknLogicalFontSecondaryFont );
	label = CreateLabelL( rect, KNone );
	AknLayoutUtils::LayoutLabel( label, rect, labelLayout );


	rect.Move( 0, primaryFontTotalHeight );
	labelBase += primaryFontTotalHeight;
	labelLayout = GetLabelLayout( aRect, labelBase, EAknLogicalFontPrimaryFont );
	label = CreateLabelL( rect, KStatus );
	AknLayoutUtils::LayoutLabel( label, rect, labelLayout );
	rect.Move( 0, secondaryFontTotalHeight );
	labelBase += secondaryFontTotalHeight;
	labelLayout = GetLabelLayout( aRect, labelBase, EAknLogicalFontSecondaryFont );
	label = CreateLabelL( rect, KNone );
	AknLayoutUtils::LayoutLabel( label, rect, labelLayout );
    }
    
 
void CTcContainer::ReLayoutAllLabels( const TRect& aRect )
    {
    if ( iLabels.Count() != 6 )
        {
        return;
        }
        
    CEikLabel* label;
    
    TInt primaryFontTotalHeight( FontTotalHeightInPixels( EAknLogicalFontPrimaryFont ) );
	TInt secondaryFontTotalHeight( FontTotalHeightInPixels( EAknLogicalFontSecondaryFont ) );
	
	TInt labelBase = KLabelTlY + primaryFontTotalHeight;	
    AknLayoutUtils::SAknLayoutTextMultiline labelLayout = 
        GetLabelLayout( aRect, labelBase, EAknLogicalFontPrimaryFont );
	TRect rect( KLabelTlX, KLabelTlY, 
	            aRect.Width() - KLabelTlX, KLabelTlY + primaryFontTotalHeight );

    label = iLabels[ 0 ];
    AknLayoutUtils::LayoutLabel( label, rect, labelLayout );
    
	rect.Move( 0, secondaryFontTotalHeight );
	labelBase += secondaryFontTotalHeight;
	labelLayout = GetLabelLayout( aRect, labelBase, EAknLogicalFontSecondaryFont );
	label = iLabels[ 1 ];
    AknLayoutUtils::LayoutLabel( label, rect, labelLayout );

	rect.Move( 0, primaryFontTotalHeight );
	labelBase += primaryFontTotalHeight;
	labelLayout = GetLabelLayout( aRect, labelBase, EAknLogicalFontPrimaryFont );
	label = iLabels[ 2 ];
	AknLayoutUtils::LayoutLabel( label, rect, labelLayout );
	rect.Move( 0, secondaryFontTotalHeight );
	labelBase += secondaryFontTotalHeight;
	labelLayout = GetLabelLayout( aRect, labelBase, EAknLogicalFontSecondaryFont );
	label = iLabels[ 3 ];
	AknLayoutUtils::LayoutLabel( label, rect, labelLayout );


	rect.Move( 0, primaryFontTotalHeight );
	labelBase += primaryFontTotalHeight;
	labelLayout = GetLabelLayout( aRect, labelBase, EAknLogicalFontPrimaryFont );
	label = iLabels[ 4 ];
	AknLayoutUtils::LayoutLabel( label, rect, labelLayout );
	rect.Move( 0, secondaryFontTotalHeight );
	labelBase += secondaryFontTotalHeight;
	labelLayout = GetLabelLayout( aRect, labelBase, EAknLogicalFontSecondaryFont );
	label = iLabels[ 5 ];
	AknLayoutUtils::LayoutLabel( label, rect, labelLayout );
    }
    
#else
    
void CTcContainer::CreateLabelsL( const TRect& /*aRect*/ )
    {
   	// Create labels
	CEikLabel* label;
	TRect rect( KLabelTlX, KLabelTlY, KLabelBrX, KLabelBrY );
	label = CreateLabelL( rect, KTestCase );
	rect.Move( 0, KLabelSpacing );
	label = CreateLabelL( rect, KNone );
	// Use smaller for for this label
	label->SetFont( CEikonEnv::Static()->AnnotationFont() );

	rect.Move( 0, KLabelSpacing );
	label = CreateLabelL( rect, KRequest );
	rect.Move( 0, KLabelSpacing );
	label = CreateLabelL( rect, KNone );
	// Use smaller for for this label
	label->SetFont( CEikonEnv::Static()->AnnotationFont() );

	rect.Move( 0, KLabelSpacing );
	label = CreateLabelL( rect, KStatus );
	rect.Move( 0, KLabelSpacing );
	label = CreateLabelL( rect, KNone );
	// Use smaller for for this label
	label->SetFont( CEikonEnv::Static()->AnnotationFont() );
    }

#endif // __SERIES60_


