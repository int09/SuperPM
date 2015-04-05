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
	filename: 	E:\界面编程\SortListCtrlDemo\SortListCtrl.h
	file path:	E:\界面编程\SortListCtrlDemo
	file base:	SortListCtrl
	file ext:	h
	author:		coding狂人 QQ:1159863174
	
	purpose:	带三角箭头可自动排序的CListCtrl. 未对CListCtrl内部做任何改动，也未对CListCtrl做任何重绘操作
	，可以随意移植到您的工程项目中，绝对不影响效率，支持ASCII,UNICODE
*********************************************************************/



//列的数据类型
enum COL_TYPE
{
	TYPE_FLOAT,		//浮点数
	TYPE_INTGER,	//数字-整型
	TYPE_STRING,	//字符串
	TYPE_DATE		//时间 时间格式为:YYYY*MM*DD*HH*MM*SS  *为任意分隔符 
					//如 2012/11/22 22:11::00 ,2012-11-22 22:11::00
};

typedef struct _COLUMN_DATA
{
	INT nCol;			//列号
	LPCTSTR szText;		//列标题
	int nFormat;		//列对齐方式
	BOOL bAutoSize;		//是否自动调整宽度
	INT nLenght;		//宽度
	COL_TYPE ColType;	//列数据类型
}COLUMN_DATA,*LPCOLUMN_DATA;




//双缓冲。
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
	//因为排序的特殊性,故为public权限,请勿访问以下数据
	CMMAP m_ColSortMark;
	CMMAP m_ColType;
	INT m_nCurCol;
// Operations
public:
	//////////////////////////////////////////////////////////////////////////
	/*	
		InsertColumn:
		参数除了多了个ColType 其他均与CListCtrl一样。
		ColType 表示该列是什么数据类型,排序好按照特定类型排序。可以取以下值:
		 1 TYPE_FLOAT
		 2 TYPE_INTGER
		 3 TYPE_STRING
		 4 TYPE_DATE
		 默认为TYPE_STRING 即字符串
		 参数含义请看上面一点点。
	*/

	//亲，平时怎么用这个函数，就怎么用哦，注意第三个参数..
	int InsertColumn(
		int nCol,
		const LVCOLUMN* pColumn,
		COL_TYPE ColType=TYPE_STRING/*该列的数据类型 默认为字符串*/,
		BOOL bAutoSize=TRUE/*是否自动调整列的大小 默认为自动*/);

	
	//亲，平时怎么用这个函数，就怎么用哦，注意第三个参数，和最后一个参数
	int InsertColumn( int nCol, 
		LPCTSTR lpszColumnHeading, 
		COL_TYPE ColType=TYPE_STRING/*该列的数据类型 默认为字符串*/,
		int nFormat = LVCFMT_LEFT,/*对齐方式默认是 左对齐*/
		int nWidth = -1,
		int nSubItem = -1,
		BOOL bAutoSize=TRUE/*是否自动调整列的大小 默认为自动*/);

	int InsertColumn(LPCOLUMN_DATA lpColumnData,DWORD dwSize);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//亲，SetItemText平时怎么用的就怎么用...
	BOOL SetItemText( int nItem, int nSubItem, LPCTSTR lpszText );
	BOOL SetItemTextA(int nItem, int nSubItem, LPCSTR lpszText);

	//亲，这个SetItemText能帮你格式化数据哦,还记得可爱的printf吗.你应该会用它吧。
	//比如 SetItemText(0,1,"今天是:%d年%d月%日",2012,11,22);
	BOOL SetItemFormatText( int nItem, int nSubItem, LPCTSTR szFormat,...);
	BOOL SetItemFormatTextA( int nItem, int nSubItem, LPCSTR szFormat,...);

	//亲，可以直接设置整数，您不需要自己格式化了.
	BOOL SetItemInt(int nItem, int nSubItem,INT nValue,INT radix=10 /*默认为10，表示是10进制，可以是2 8 10 16 等等.*/);	
	//////////////////////////////////////////////////////////////////////////
	
	void SetResizingInfo(INT nMinY,INT nMaxY,HWND hMsgWnd,INT nMsg);

	//自动调整列的宽度 非常非常的实用
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
