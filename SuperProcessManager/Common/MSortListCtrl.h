#if !defined(AFX_MSORTLISTCTRL_H__1888833F_72A3_4628_B984_593563E3FAA7__INCLUDED_)
#define AFX_MSORTLISTCTRL_H__1888833F_72A3_4628_B984_593563E3FAA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MSortListCtrl.h : header file
//

#if _MSC_VER ==1200 
#pragma warning(disable:4786)
#endif
#include <iostream>
#include <map>
using namespace std;



/////////////////////////////////////////////////////////////////////////////
// CMSortListCtrl window
/********************************************************************
	created:	2012/11/22
	created:	22:11:2012   0:04
	filename: 	E:\������\SortListCtrlDemo\SortListCtrl.h
	file path:	E:\������\SortListCtrlDemo
	file base:	SortListCtrl
	file ext:	h
	author:		coding���� QQ:1159863174
	
	purpose:	�����Ǽ�ͷ���Զ������CListCtrl. δ��CListCtrl�ڲ����κθĶ���Ҳδ��CListCtrl���κ��ػ����
	������������ֲ�����Ĺ�����Ŀ�У����Բ�Ӱ��Ч�ʣ�֧��ASCII,UNICODE
*********************************************************************/



//�е���������
enum COL_TYPE
{
	TYPE_FLOAT,		//������
	TYPE_INTGER,	//����-����
	TYPE_STRING,	//�ַ���
	TYPE_DATE		//ʱ�� ʱ���ʽΪ:YYYY*MM*DD*HH*MM*SS  *Ϊ����ָ��� 
					//�� 2012/11/22 22:11::00 ,2012-11-22 22:11::00
};

typedef struct _COLUMN_DATA
{
	INT nCol;			//�к�
	LPCTSTR szText;		//�б���
	int nFormat;		//�ж��뷽ʽ
	BOOL bAutoSize;		//�Ƿ��Զ��������
	INT nLenght;		//���
	COL_TYPE ColType;	//����������
}COLUMN_DATA,*LPCOLUMN_DATA;




//˫���塣
#define	 LVS_EX_DOUBLEBUFFER				0x00010000


typedef map<INT,BOOL> CMMAP;
typedef pair<INT,BOOL> CMPAIR;

class CMSortListCtrl : public CListCtrl
{
// Construction
public:
	CMSortListCtrl();

// Attributes
public:
	//��Ϊ�����������,��ΪpublicȨ��,���������������
	CMMAP m_ColSortMark;
	CMMAP m_ColType;
	INT m_nCurCol;
// Operations
public:
	//////////////////////////////////////////////////////////////////////////
	/*	
		InsertColumn:
		�������˶��˸�ColType ��������CListCtrlһ����
		ColType ��ʾ������ʲô��������,����ð����ض��������򡣿���ȡ����ֵ:
		 1 TYPE_FLOAT
		 2 TYPE_INTGER
		 3 TYPE_STRING
		 4 TYPE_DATE
		 Ĭ��ΪTYPE_STRING ���ַ���
		 ���������뿴����һ��㡣
	*/

	//�ף�ƽʱ��ô���������������ô��Ŷ��ע�����������..
	int InsertColumn(
		int nCol,
		const LVCOLUMN* pColumn,
		COL_TYPE ColType=TYPE_STRING/*���е��������� Ĭ��Ϊ�ַ���*/,
		BOOL bAutoSize=TRUE/*�Ƿ��Զ������еĴ�С Ĭ��Ϊ�Զ�*/);

	
	//�ף�ƽʱ��ô���������������ô��Ŷ��ע������������������һ������
	int InsertColumn( int nCol, 
		LPCTSTR lpszColumnHeading, 
		COL_TYPE ColType=TYPE_STRING/*���е��������� Ĭ��Ϊ�ַ���*/,
		int nFormat = LVCFMT_LEFT,/*���뷽ʽĬ���� �����*/
		int nWidth = -1,
		int nSubItem = -1,
		BOOL bAutoSize=TRUE/*�Ƿ��Զ������еĴ�С Ĭ��Ϊ�Զ�*/);

	int InsertColumn(LPCOLUMN_DATA lpColumnData,DWORD dwSize);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//�ף�SetItemTextƽʱ��ô�õľ���ô��...
	BOOL SetItemText( int nItem, int nSubItem, LPCTSTR lpszText );
	BOOL SetItemTextA(int nItem, int nSubItem, LPCSTR lpszText);

	//�ף����SetItemText�ܰ����ʽ������Ŷ,���ǵÿɰ���printf��.��Ӧ�û������ɡ�
	//���� SetItemText(0,1,"������:%d��%d��%��",2012,11,22);
	BOOL SetItemFormatText( int nItem, int nSubItem, LPCTSTR szFormat,...);
	BOOL SetItemFormatTextA( int nItem, int nSubItem, LPCSTR szFormat,...);

	//�ף�����ֱ������������������Ҫ�Լ���ʽ����.
	BOOL SetItemInt(int nItem, int nSubItem,INT nValue,INT radix=10 /*Ĭ��Ϊ10����ʾ��10���ƣ�������2 8 10 16 �ȵ�.*/);	
	//////////////////////////////////////////////////////////////////////////
	
	void SetResizingInfo(INT nMinY,INT nMaxY,HWND hMsgWnd,INT nMsg);

	//�Զ������еĿ�� �ǳ��ǳ���ʵ��
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSortListCtrl)
	//}}AFX_VIRTUAL

protected:
	void SetHeaderSortImage(HWND listView, int columnIndex, BOOL isAscending);
// Implementation
public:
	virtual ~CMSortListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMSortListCtrl)
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	DECLARE_MESSAGE_MAP()
private:
	CHAR m_szFormatBufferA[2048];
	TCHAR m_szFormatBuffer[2048];
	HWND m_hMsgWnd;
	INT  m_nMsg;
	BOOL m_bNcHitTest,m_bNcHitTested;
	INT  m_nMinY;
	INT  m_nMaxY;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSORTLISTCTRL_H__1888833F_72A3_4628_B984_593563E3FAA7__INCLUDED_)
