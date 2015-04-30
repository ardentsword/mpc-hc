#include "stdafx.h"
#include "PlayerToolbarBarClassic.h"
#include "mplayerc.h"


CPlayerToolbarBarClassic::CPlayerToolbarBarClassic(CMainFrame* pMainFrame) 
	:CPlayerToolBar(pMainFrame)
{
}


CPlayerToolbarBarClassic::~CPlayerToolbarBarClassic()
{
}

BOOL CPlayerToolbarBarClassic::Create(CWnd* pParentWnd)
{
	VERIFY(__super::CreateEx(pParentWnd,
		TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_AUTOSIZE | TBSTYLE_CUSTOMERASE,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM | CBRS_TOOLTIPS,
		CRect(2, 2, 0, 1)));

	VERIFY(LoadToolBar(IDB_PLAYERTOOLBAR));

	// Should never be RTLed
	ModifyStyleEx(WS_EX_LAYOUTRTL, WS_EX_NOINHERITLAYOUT);

	CToolBarCtrl& tb = GetToolBarCtrl();
	tb.DeleteButton(tb.GetButtonCount() - 1);
	tb.DeleteButton(tb.GetButtonCount() - 1);

	SetMute(AfxGetAppSettings().fMute);

	UINT styles[] = {
		TBBS_CHECKGROUP, TBBS_CHECKGROUP, TBBS_CHECKGROUP,
		TBBS_SEPARATOR,
		TBBS_BUTTON, TBBS_BUTTON, TBBS_BUTTON, TBBS_BUTTON,
		TBBS_SEPARATOR,
		TBBS_BUTTON,
		TBBS_SEPARATOR,
		TBBS_SEPARATOR,
		TBBS_CHECKBOX,
	};

	for (int i = 0; i < _countof(styles); ++i) {
		// This fixes missing separator in Win 7
		if (styles[i] & TBBS_SEPARATOR) {
			SetButtonInfo(i, GetItemID(i), styles[i], -1);
		}
		else {
			SetButtonStyle(i, styles[i] | TBBS_DISABLED);
		}
	}

	m_volctrl.Create(this);
	m_volctrl.SetRange(0, 100);

	m_nButtonHeight = 16; // reset m_nButtonHeight
	CImage image;
	if (LoadExternalToolBar(&image)) {
		CBitmap* bmp = CBitmap::FromHandle(image);
		int width = image.GetWidth();
		int height = image.GetHeight();
		int bpp = image.GetBPP();
		if (width == height * 15) {
			// the manual specifies that sizeButton should be sizeImage inflated by (7, 6)
			SetSizes(CSize(height + 7, height + 6), CSize(height, height));

			m_pButtonsImages = DEBUG_NEW CImageList();
			if (bpp == 32) {
				m_pButtonsImages->Create(height, height, ILC_COLOR32 | ILC_MASK, 1, 0);
				m_pButtonsImages->Add(bmp, nullptr); // alpha is the mask
			}
			else {
				m_pButtonsImages->Create(height, height, ILC_COLOR24 | ILC_MASK, 1, 0);
				m_pButtonsImages->Add(bmp, RGB(255, 0, 255));
			}
			m_nButtonHeight = height;
			GetToolBarCtrl().SetImageList(m_pButtonsImages);
		}
		image.Destroy();
	}
	
	return TRUE;
}