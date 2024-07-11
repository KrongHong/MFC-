
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication4.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView* CChildView::GetDocument() const
{
	pThread = new CWinThread * [1000];//스레드 배열
	arg = new ThreadArg[1000];//전달인자 배열
	TCount = 0;//스레드 번호
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}


void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	POSITION pos = list.GetHeadPosition();
	CPoint nPos;

	while (pos != NULL)
	{
		nPos = list.GetNext(pos);
		dc.Ellipse(nPos.x - 20, nPos.y - 20, nPos.x + 20, nPos.y + 20);
	}
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}

// CLab0722012246홍승표View 메시지 처리기
UINT MyDraw(LPVOID arg) // arg는 스레드가 시작될 때 전달된 데이터
{
	ThreadArg* pArg = (ThreadArg*)arg;
	CClientDC dc(pArg->pWnd);
	CPoint point = pArg->pos;
	CCriticalSection g_cs;

	(pArg->cpoint)->AddTail(point); // 리스트 마지막에 위치 추가
	InvalidateRect(*pArg->pWnd, NULL, true); // 무효화-> 다시 그리기

	Sleep(3000); // 3초 뒤에 제거하기

	POSITION pos = (pArg->cpoint)->GetHeadPosition(); // 리스트에 저장된 첫번째 위치 반환

	g_cs.Lock(); // 공유자원에 하나의 스레드가 접근할때 다른 스레드가 접근하지 못하도록 한다
	(pArg->cpoint)->RemoveAt(pos); // pos에 사용된 원을 제거(공유 자원에 접근)
	g_cs.Unlock();

	InvalidateRect(*pArg->pWnd, NULL, true);
	return 0;


}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnLButtonDown(nFlags, point);
	CClientDC dc(this);

	arg[number].pos = point;
	arg[number].pWnd = this;
	arg[number].cpoint = &list;

	pThread[number] = AfxBeginThread(MyDraw, &arg[number], THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pThread[number]->ResumeThread();
	number++;
}
