
// MFCApplication5View.cpp: CMFCApplication5View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCApplication5.h"
#endif

#include "MFCApplication5Doc.h"
#include "MFCApplication5View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication5View

IMPLEMENT_DYNCREATE(CMFCApplication5View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication5View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCApplication5View 생성/소멸

CMFCApplication5View::CMFCApplication5View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	//스레드의 전달인자와 스레드 배열을 미리 선언해놓는다.
	pThread = new CWinThread * [1000];//스레드 배열 선언
	arg = new ThreadArg[1000];//전달인자 배열
	TCount = 0;//스레드 번호

}

CMFCApplication5View::~CMFCApplication5View()
{
}

BOOL CMFCApplication5View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCApplication5View 그리기

void CMFCApplication5View::OnDraw(CDC* /*pDC*/)
{
	CMFCApplication5Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	CClientDC dc(this);

	POSITION pos = cpt.GetHeadPosition(), old; // 리스트의 헤더에 저장단 위치 가져오기
	CPoint p;

	// 처음부터 다 다시 그리기
	while (pos != NULL) //처음부터 끝까지 그릴것
	{
		p = cpt.GetNext(pos); // 다음 노드를 불러와서 그리기
		dc.Ellipse(p.x - 20, p.y - 20, p.x + 20, p.y + 20); //원 그리기
	}
}


// CMFCApplication5View 인쇄

BOOL CMFCApplication5View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCApplication5View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCApplication5View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFCApplication5View 진단

#ifdef _DEBUG
void CMFCApplication5View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication5View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication5Doc* CMFCApplication5View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication5Doc)));
	return (CMFCApplication5Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication5View 메시지 처리기
UINT Mydraw(LPVOID arg) { //thread로 부르는 함수
	ThreadArg* pArg = (ThreadArg*)arg; //값들을 받아오기
	CClientDC dc(pArg->pWnd); 
	CPoint point = pArg->pos;
	CCriticalSection cs; // 임계 구역 변수 생성

	(pArg->cpoint)->AddTail(point); //그린것을 list에 추가한다.

	InvalidateRect(*pArg->pWnd, NULL, true); //무효화(다시 그리기)

	Sleep(3000); //3초뒤에 제거하려고

	POSITION pos = (pArg->cpoint)->GetHeadPosition(); // 가장 먼저 만들어진것 부터 지우기 위해 딜레이 후 처음 포지션부터 반환

	cs.Lock(); //critical section진입
	(pArg->cpoint)->RemoveAt(pos); //원 삭제(공유 자원에 접근)
	cs.Unlock();//critical section 해제

	InvalidateRect(*pArg->pWnd, NULL, true);//무효화 -> 다시 그리기
	return 0;
}

void CMFCApplication5View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
	CClientDC dc(this);
	//스레드 값 설정
	arg[TCount].pos = point;//현재 CPoint(원)
	arg[TCount].pWnd = this;//뷰 객체의 주소
	arg[TCount].cpoint = &cpt;//원을 담을 리스트 전달

	//스레드를 중지상태로 생성
	pThread[TCount] = AfxBeginThread(Mydraw, &arg[TCount], THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

	//스레드 시작
	pThread[TCount]->ResumeThread();

	//Tcount를 증가시켜서 원마다 스레드 하나씩
	TCount += 1;
}
