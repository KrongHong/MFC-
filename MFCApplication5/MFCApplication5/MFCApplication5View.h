
// MFCApplication5View.h: CMFCApplication5View 클래스의 인터페이스
//

#pragma once


struct ThreadArg
{
	CWnd* pWnd; //출력할 대상 윈도우 객체의 주소
	CPoint pos; 
	CList<CPoint>* cpoint;//리스트의 위치 주소 전달
};

class CMFCApplication5View : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFCApplication5View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication5View)

// 특성입니다.
public:
	CMFCApplication5Doc* GetDocument() const;
	CWinThread** pThread;//스레드 객체 주소 전달
	CList<CPoint> cpt; //원의 위치 저장 리스트 전달
	ThreadArg* arg;//스레드 전달 인자 전달
	int TCount; // 스레드의 배열의 주소 저장

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFCApplication5View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MFCApplication5View.cpp의 디버그 버전
inline CMFCApplication5Doc* CMFCApplication5View::GetDocument() const
   { return reinterpret_cast<CMFCApplication5Doc*>(m_pDocument); }
#endif

