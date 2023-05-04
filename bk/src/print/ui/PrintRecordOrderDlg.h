#ifndef __PRINTRECORDORDERDLG_H__
#define __PRINTRECORDORDERDLG_H__

#include <afxwin.h>
#include "../../resource.h"
#include "../options/DocumentCfgDescription.h"
#include <configuration\print\PrintProfile.h>
#include "PrintProfileEditor.h"
#include "../../ctrls/ownrdrwlistctrl/OwnrDrwListCtrl.h"
#include "../../ctrls/contentlistctrl/NullListRow.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include "../../ctrls/arrowbutton/ArrowButton.h"

class PrintRecordOrderDlg : public CDialog
{
	enum SortintOrderListColumn
	{
		SortintOrderListColumn_FieldName,
		SortintOrderListColumn_Collation
	};

	class SortingOrderListRow : public NullListRow
	{
	public:
		SortingOrderListRow(const OrderSpecifier* specifier, const std::wstring& fieldName);
		~SortingOrderListRow();

		virtual std::wstring CellValue(uint32_t colId);
		const OrderSpecifier* Specifier() const;
		void SetCollation(bool bAsc);

	private:
		OrderSpecifier* m_Specifier;
		std::wstring m_fieldName;

	};
	
	class SortingOrderListContent : public ListContent
	{

		struct RowKeyExtractor
		{
			typedef std::wstring result_type;
			result_type operator()(SortingOrderListRow* pRow) const
			{
				return pRow->Specifier()->fieldCode();
			}
		};

		struct index_by_code{};

		typedef boost::multi_index_container
			<
			SortingOrderListRow*,
			boost::multi_index::indexed_by<
			boost::multi_index::random_access<>,
			boost::multi_index::ordered_unique < boost::multi_index::tag<index_by_code>, RowKeyExtractor >
			>
			> ContentIndex;

	public:
		~SortingOrderListContent();
		
		SortingOrderListRow* AddRow(const OrderSpecifier* specifier, const std::wstring& fieldName);
		void DeleteRow(int row);
		void MoveRow(int row, int positions);
		void DeleteAllRows();
		void SetCollation(int row, bool bAsc);
		const SortingOrderListRow* GetRowForField(const std::wstring& fieldCd) const;
		const SortingOrderListRow* GetSortingOrderRowAt(int row) const;

		virtual int GetRowCount() const;
		virtual ListRow* GetRow(int row);
		virtual void SortContent(const SortAttributes& attributes);			
		

	private:
		ContentIndex m_content;

	};

public:
	PrintRecordOrderDlg(const DocumentCfgDescription* configuration, PrintProfileEditor* pEditor, CWnd* pParent = NULL);
	enum { IDD = IDD_PRINTRECORDORDER };
	void Create(CWnd* pParent);
	void Apply();
protected:
	CDialog::DoModal;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	void ReloadComparableFieldsCombo();
	void OnCellDblClick(int row, uint32_t colId, CellDescriptor& cell);
	void OnCollationChanged(int row, uint32_t collationId);
	
	CArrowButton m_moveUpBtn;
	CArrowButton m_moveDownBtn;
	CComboBox m_ComparableFieldsCb;
	COwnrDrwListCtrl m_sortingOrderList;


	const DocumentCfgDescription* m_configuration;
	PrintProfileEditor* m_pEditor;
	
	SortingOrderListContent m_Content;
	
protected:
	
	afx_msg void OnAdd();
	afx_msg void OnMoveUp();
	afx_msg void OnDelete();
	afx_msg void OnMoveDown();
	afx_msg void OnDestroy();
	
	DECLARE_MESSAGE_MAP()
};

#endif