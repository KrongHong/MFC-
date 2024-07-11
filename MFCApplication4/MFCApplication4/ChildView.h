
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once
struct ThreadArg
{
	CWnd* pWnd;//출력할 대상 윈도우 객체의 주소
	CPoint pos;
	CList<CPoint>* cpoint;//리스트 위치 전달
};

// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	CChildView* GetDocument() const;
	CWinThread** pThread; // 스레드 객체 주소
	ThreadArg* arg; // 스레드 전달 인자
	CList<CPoint> list;// 원이 주소를 저장할 리스트
	int number;
// 작업입니다.
public:

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

