//== ВКЛЮЧЕНИЯ.
#include "mtableview.h"
#include <QHeaderView>
#include <QSqlRelationalDelegate>

//== ФУНКЦИИ КЛАССОВ.
//== Класс модифицированного виджета вида таблицы.
// Переопределение обновления геометрии для авторасширения без вертикального скроллинга.
void MTableView::updateGeometries()
{
	if(!_bCanScroll)
	{
		int iRQ = 0;
		int iAllRowsHeight = 0;
		QAbstractItemModel* p_Model = model();
		if(p_Model)
		{
			iRQ = p_Model->rowCount();
			if(iLastRQ != iRQ)
			{
				for(int iCurRow = 0; iCurRow < iRQ; iCurRow++)
				{
					iAllRowsHeight += rowHeight(iCurRow);
				}
				iLastRQ = iRQ;
				setFixedHeight(iAllRowsHeight + horizontalHeader()->height() + this->verticalHeader()->height());
			}
		}
	}
	QTableView::updateGeometries();
}

// Переопределение отправки данных после редактирования для перезагрузки зависимых видов таблиц.
void MTableView::commitData(QWidget* p_Editor)
{
	QTableView::commitData(p_Editor);
	for(auto p_MTableView : v_p_MTableViews)
	{
		QSqlRelationalTableModel* p_QSqlRelationalTableModel = static_cast<QSqlRelationalTableModel*>(p_MTableView->model());
		QString strTableName = p_QSqlRelationalTableModel->tableName();
		QString strFilter = p_QSqlRelationalTableModel->filter();
		std::vector<QSqlRelation> vRelations;
		for(int iC = 0; iC < p_QSqlRelationalTableModel->columnCount(); iC++) vRelations.push_back(p_QSqlRelationalTableModel->relation(iC));
		delete p_QSqlRelationalTableModel;
		p_QSqlRelationalTableModel = new QSqlRelationalTableModel(this);
		p_QSqlRelationalTableModel->setTable(strTableName);
		p_QSqlRelationalTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);
		p_QSqlRelationalTableModel->setFilter(strFilter);
		p_MTableView->setModel(p_QSqlRelationalTableModel);
		for(int iC = 0; iC < p_QSqlRelationalTableModel->columnCount(); iC++)
		{
			if(vRelations.begin()->isValid()) p_QSqlRelationalTableModel->setRelation(iC, *vRelations.begin());
			vRelations.erase(vRelations.begin());
		}
		p_MTableView->setItemDelegate(new QSqlRelationalDelegate(p_QSqlRelationalTableModel));
		p_QSqlRelationalTableModel->select();
	}
	if(_iColumnForSort) this->sortByColumn(_iColumnForSort, Qt::SortOrder::AscendingOrder);
}

// Установка колонки для автосортировки или 0.
void MTableView::SetColumnForSort(int iColumnForSort)
{
	_iColumnForSort = iColumnForSort;
	if(_iColumnForSort) this->sortByColumn(_iColumnForSort, Qt::SortOrder::AscendingOrder);
}

// Добавление зависимого виджета вида таблицы.
void MTableView::AddRelatedTableView(MTableView* p_MTableView)
{
	v_p_MTableViews.push_back(p_MTableView);
}

// Удаление зависимого виджета вида таблицы.
void MTableView::RemoveRelatedTableView(MTableView* p_MTableView)
{
	v_p_MTableViews.erase(std::find(v_p_MTableViews.begin(), v_p_MTableViews.end(), p_MTableView));
}
