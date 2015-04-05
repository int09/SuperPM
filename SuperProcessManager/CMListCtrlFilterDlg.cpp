// CMListCtrlFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "superprocessmanager.h"
#include "CMListCtrlFilterDlg.h"
#include "deelx.h"

#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMListCtrlFilterDlg dialog

enum{RULE_DAYU,RULE_XIAOYU,RULE_DENGYU,RULE_BUDENGYU};
enum{RULE_BAOHAN,RULE_BUBAOHAN,RULE_BAOHANI,RULE_BUBAOHANI,RULE_ZHENGZE};
enum{OPERATE_SELECTED,OPERATE_DELETE};

CCMListCtrlFilterDlg::CCMListCtrlFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMListCtrlFilterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMListCtrlFilterDlg)
	m_szParameter = _T("");
	//}}AFX_DATA_INIT
}


void CCMListCtrlFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMListCtrlFilterDlg)
	DDX_Control(pDX, IDC_RESULT_TEXT, m_Result);
	DDX_Control(pDX, IDC_COMBO_RULE, m_Rule);
	DDX_Control(pDX, IDC_COMBO_COLUMN, m_Column);
	DDX_Text(pDX, IDC_PARAMETER_TEXT, m_szParameter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMListCtrlFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CCMListCtrlFilterDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_COLUMN, OnSelchangeComboColumn)
	ON_BN_CLICKED(IDC_OPERATE_SELECT, OnOperateSelect)
	ON_BN_CLICKED(IDC_OPERATE_DELETE, OnOperateDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMListCtrlFilterDlg message handlers

BOOL CCMListCtrlFilterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	for(DWORD i = 0; i<m_dwColDataSize;i++)
	{
		m_Column.SetItemData(m_Column.AddString(m_lpColData[i].szText),m_lpColData[i].ColType);
	}
	m_Column.SetCurSel(0);
	ShowRule();

	CString szText;
	szText.Format(TEXT("当前总行数:%d\t"),m_lpCMListCtrl->GetItemCount());
	m_Result.SetWindowText(szText);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMListCtrlFilterDlg::ShowRule()
{
	m_Rule.ResetContent();

	int nIndex = m_Column.GetCurSel();
	switch(m_Column.GetItemData(nIndex))
	{
	case TYPE_FLOAT:
	case TYPE_INTGER:
		m_Rule.SetItemData(m_Rule.AddString(TEXT("小于")),RULE_XIAOYU);
		m_Rule.SetItemData(m_Rule.AddString(TEXT("大于")),RULE_DAYU);
		m_Rule.SetItemData(m_Rule.AddString(TEXT("等于")),RULE_DENGYU);
		m_Rule.SetItemData(m_Rule.AddString(TEXT("不等于")),RULE_BUDENGYU);
		break;
	case TYPE_STRING:
	case TYPE_DATE:
		m_Rule.SetItemData(m_Rule.AddString(TEXT("包含")),RULE_BAOHANI);
		m_Rule.SetItemData(m_Rule.AddString(TEXT("不包含")),RULE_BUBAOHANI);
		m_Rule.SetItemData(m_Rule.AddString(TEXT("包含(区分大小写)")),RULE_BAOHAN);
		m_Rule.SetItemData(m_Rule.AddString(TEXT("不包含(区分大小写)")),RULE_BUBAOHAN);
		//m_Rule.SetItemData(m_Rule.AddString(TEXT("正则表达式")),RULE_ZHENGZE);
		break;
	}

	m_Rule.SetCurSel(0);
}


void CCMListCtrlFilterDlg::OnSelchangeComboColumn() 
{
	// TODO: Add your control notification handler code here
	ShowRule();
}


void CCMListCtrlFilterDlg::OnOperateSelect() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateData(TRUE);
	DWORD dwCount = StartFilter(m_Column.GetCurSel(),
		m_Column.GetItemData(m_Column.GetCurSel()),
		m_Rule.GetItemData(m_Rule.GetCurSel()),
		m_szParameter,
		OPERATE_SELECTED);

	CString szText;
	szText.Format(TEXT("当前总行数:%d\t已经选中:%d条"),m_lpCMListCtrl->GetItemCount(),dwCount);
	m_Result.SetWindowText(szText);
}



void CCMListCtrlFilterDlg::OnOperateDelete() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	INT dwItenCount = m_lpCMListCtrl->GetItemCount();

	DWORD dwCount = StartFilter(m_Column.GetCurSel(),
		m_Column.GetItemData(m_Column.GetCurSel()),
		m_Rule.GetItemData(m_Rule.GetCurSel()),
		m_szParameter,
		OPERATE_DELETE);

	CString szText;
	szText.Format(TEXT("当前总行数:%d\t原行数:%d\t删除:%d条"),m_lpCMListCtrl->GetItemCount(),dwItenCount,dwCount);
	m_Result.SetWindowText(szText);
}


DWORD CCMListCtrlFilterDlg::StartFilter(INT nColumn,INT nType, INT nRule, LPCTSTR szParam,INT nOperate)
{
	INT nItem = m_lpCMListCtrl->GetItemCount();
	CString szColumnText;
	bool bResult = false;
	DWORD dwCount=0;
	

	for(INT i=0;i<nItem;i++)
	{
		szColumnText = m_lpCMListCtrl->GetItemText(i,nColumn);
		
		bResult = false;

		switch(nType)
		{
			case TYPE_FLOAT:
			{
				switch(nRule)
				{
				case RULE_DAYU:
					bResult = STOF(szColumnText) > STOF(szParam);
					break;
				case RULE_XIAOYU:
					bResult = STOF(szColumnText) < STOF(szParam);
					break;
				case RULE_DENGYU:
					bResult = STOF(szColumnText) == STOF(szParam);
					break;
				case RULE_BUDENGYU:
					bResult = STOF(szColumnText) != STOF(szParam);
					break;
				}
			}break;
			case TYPE_INTGER:
				{
					switch(nRule)
					{
					case RULE_DAYU:
						bResult = _ttoi(szColumnText) > _ttoi(szParam);
						break;
					case RULE_XIAOYU:
						bResult = _ttoi(szColumnText) < _ttoi(szParam);
						break;
					case RULE_DENGYU:
						bResult = _ttoi(szColumnText) == _ttoi(szParam);
						break;
					case RULE_BUDENGYU:
						bResult = _ttoi(szColumnText) != _ttoi(szParam);
						break;
					}
				}break;

			case TYPE_DATE:
			case TYPE_STRING:
				{
					switch(nRule)
					{
					case RULE_BAOHAN:
						bResult = (NULL==StrStr(szColumnText,szParam))?false:true;
							break;
					case RULE_BAOHANI:
						bResult = (NULL==StrStrI(szColumnText,szParam))?false:true;
							break;
					case RULE_BUBAOHAN:
						bResult = (NULL == StrStr(szColumnText,szParam))?true:false;
						break;
					case RULE_BUBAOHANI:
						bResult = (NULL == StrStrI(szColumnText,szParam))?true:false;
							break;
					case RULE_ZHENGZE:
						{
							CRegexpT <TCHAR> regexp(szParam);
							MatchResult result = regexp.MatchExact(szColumnText);
							bResult = result.IsMatched()?true:false;
						}break;
					}
				}break;
		}

		switch(nOperate)
		{
		case OPERATE_SELECTED:
			m_lpCMListCtrl->SetCheck(i,bResult);
			break;
		case OPERATE_DELETE:
			if(bResult)
			{
				m_lpCMListCtrl->DeleteItem(i);
				nItem = m_lpCMListCtrl->GetItemCount();
				i--;
			}
			break;
		}
		
		if(bResult) dwCount++;
	}

	return dwCount;
}
