#pragma once
#include "PlayerToolBar.h"
class CPlayerToolbarBarClassic :
	public CPlayerToolBar
{
public:
	CPlayerToolbarBarClassic(CMainFrame* pMainFrame);
	virtual ~CPlayerToolbarBarClassic();
	
	virtual BOOL Create(CWnd* pParentWnd);
};

