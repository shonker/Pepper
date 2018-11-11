#include "stdafx.h"
#include "ViewRightBottomRight.h"

IMPLEMENT_DYNCREATE(CViewRightBottomRight, CScrollView)

CViewRightBottomRight::CViewRightBottomRight()
{
}

CViewRightBottomRight::~CViewRightBottomRight()
{
}

BEGIN_MESSAGE_MAP(CViewRightBottomRight, CScrollView)
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CViewRightBottomRight::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	m_pChildFrame = (CChildFrame*)GetParentFrame();

	m_pMainDoc = (CPepperDoc*)GetDocument();
	m_pLibpe = m_pMainDoc->m_pLibpe;
	if (!m_pLibpe)
		return;

	CreateListTLSCallbacks();
}

void CViewRightBottomRight::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (!m_pChildFrame)
		return;

	if (m_pActiveList)
		m_pActiveList->ShowWindow(SW_HIDE);

	CRect rect;
	GetClientRect(&rect);
	m_pChildFrame->m_stSplitterRight.GetPane(1, 0)->GetClientRect(&rect);

	switch (LOWORD(lHint))
	{
	case IDC_LIST_TLS:
		m_stListTLSCallbacks.SetWindowPos(this, 0, 0, rect.Width(), rect.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
		m_pActiveList = &m_stListTLSCallbacks;
		m_pChildFrame->m_stSplitterRightBottom.SetColumnInfo(0, rect.Width() / 2, 0);
		break;
	default:
		m_pChildFrame->m_stSplitterRightBottom.SetColumnInfo(0, rect.Width(), 0);
	}

	m_pChildFrame->m_stSplitterRightBottom.RecalcLayout();
}

void CViewRightBottomRight::OnDraw(CDC* pDC)
{
}

void CViewRightBottomRight::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	if (m_pActiveList)
		m_pActiveList->SetWindowPos(this, 0, 0, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER);
}

int CViewRightBottomRight::CreateListTLSCallbacks()
{
	PCLIBPE_TLS_TUP pTLSDir { };
	if (m_pLibpe->GetTLSTable(&pTLSDir) != S_OK)
		return -1;

	m_stListTLSCallbacks.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | LVS_OWNERDRAWFIXED | LVS_REPORT,
		CRect(0, 0, 0, 0), this, IDC_LIST_TLS_CALLBACKS);
	m_stListTLSCallbacks.SendMessageW(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
	m_stListTLSCallbacks.InsertColumn(0, L"TLS Callbacks", LVCFMT_CENTER | LVCFMT_FIXED_WIDTH, 300);

	int listindex { };
	TCHAR str[9] { };

	auto vecCallbacks = std::get<3>(*pTLSDir);
	for (auto& i : vecCallbacks)
	{
		swprintf_s(str, 9, L"%08X", i);
		m_stListTLSCallbacks.InsertItem(listindex, str);
		listindex++;
	}

	return 0;
}
