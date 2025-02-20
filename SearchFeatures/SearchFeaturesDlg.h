
// SearchFeaturesDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "FeatureCode.h"
#include "afxcmn.h"
#include <vector>

// CSearchFeaturesDlg 对话框
class CSearchFeaturesDlg : public CDialogEx
{
// 构造
public:
	CSearchFeaturesDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SEARCHFEATURES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();

    virtual BOOL PreTranslateMessage(MSG* pMsg);

    //选择进程下来框内容做出改变响应函数
    afx_msg void OnCbnSelchangeComboProcesslist();
    
    //测试按钮响应函数
    afx_msg void OnBnClickedBtnTest();

    //搜索按钮响应函数
    afx_msg void OnBnClickedBtnSearch();

    //添加到列表按钮响应函数
    afx_msg void OnBnClickedBtnAddlist();

    //保存特征码到文件按钮响应函数
    afx_msg void OnBnClickedBtnSave();

    //加载特征码文件按钮响应函数
    afx_msg void OnBnClickedBtnLoad();
    //移除字符串中的换行符
    void RemoveNewlines(CString& str);

    //生成代码按钮响应函数
    afx_msg void OnBnClickedBtnCreatecode();

private:
    /*
    GetProcessHandle
    通过进程id获取进程句柄
    参数为进程id
    返回值为进程句柄
    */
    inline HANDLE GetProcessHandle(DWORD dwPid);
    /// <summary>
    /// 获取下拉框的选择进程名
    /// </summary>
    /// <param name="str"></param>
    /// <returns></returns>
    CString GetSelectProcessName(CString str);

    //用于获取当前所有进程
    void GetAllProcess();

    /*
    SplitString 字符串分割函数
    第一个参数 要分割的字符串
    第二个参数 以什么字符串作为分割依据 
    返回值：分割出来的字符串数组
    */
    std::vector<CString> SplitString(const CString& str, TCHAR delimiter);
    
    void CreateCppCode();
    void CreateECode();
    /// <summary>
    /// 下拉框搜索 并选中
    /// </summary>
    /// <param name="pComboBox"></param>
    /// <param name="lpszText"></param>
    /// <returns></returns>
    int SelectComboItemByText(CComboBox* pComboBox, LPCTSTR lpszText);

    //初始化UI
    void InitUI();

private:
    // 用于显示进程列表的ComboBox控件
    CComboBox m_comboProcessList;

    //存储进程句柄
    HANDLE m_hProcess;

    // 特征码名字 如果要生成C++代码的话就是C++宏名称
    CString m_strName;

    // 特征码注释 说明
    CString m_strNotes;

    // 特征码
    CString m_strMarkCode;

    // 要读取的偏移 如果是往上就是整数 往下是负数
    int m_nOffset;

    // 要读取的长度
    UINT m_uLen;

    // 特征码搜索起始地址
    CString m_dwBeginAddr;

    //特征码搜索结束地址
    CString m_dwEndAddr;

    //特征码列表
    CString m_strMarkCodeList;
    
    // 表明用户搜索类型，0==基址 1==call 2==偏移 3==地址
    int m_btnType;
    
    // 显示特征码搜索结果
    CListCtrl m_listResult;

    // 表明要生成什么语言的代码
    CComboBox m_comboBoxLanguage;
    
    // 特征码列表edit控件变量
    CEdit m_editMarkCodeList;
public:
    afx_msg void OnBnClickedButtonRefresh();
};
