//== ВКЛЮЧЕНИЯ.
#include "mtableview.h"
#include <QSqlRelationalDelegate>

//== ФУНКЦИИ КЛАССОВ.
//== Класс модифицированного заголовка виджета вида таблицы.
// Переопределение обработчика события нажатия мыши.
void MHeaderView::mousePressEvent(QMouseEvent* p_E)
{
	QHeaderView::mousePressEvent(p_E);
	bMouseDown = true;
}
// Переопределение обработчика события смещения мыши.
void MHeaderView::mouseMoveEvent(QMouseEvent* p_E)
{
	QHeaderView::mouseMoveEvent(p_E);
	if(bMouseDown && _p_v_p_MHorizontalHeaderViewsRelated)
	{
		for(MHeaderView* p_MHorizontalHeaderViewRelated : *_p_v_p_MHorizontalHeaderViewsRelated)
		{
			for(int iC = 0; iC < this->model()->columnCount(); iC++)
			{
				p_MHorizontalHeaderViewRelated->resizeSection(iC, sectionSize(iC));
			}
			p_MHorizontalHeaderViewRelated->updateGeometry();
		}
	}
}
// Переопределение обработчика события отпускания мыши.
void MHeaderView::mouseReleaseEvent(QMouseEvent* p_E)
{
	bMouseDown = false;
	QHeaderView::mouseReleaseEvent(p_E);
}

//== Класс модифицированного виджета вида таблицы.
// Конструктор.
MTableView::MTableView(QWidget* p_Parent) : QTableView(p_Parent)
{
	MHeaderView* p_MHeaderView = new MHeaderView(Qt::Horizontal, this);
	p_MHeaderView->SetRelatedHorizontalHeaderViews(&v_p_MHorizontalHeaderViewsRelated);
	setHorizontalHeader(p_MHeaderView);
}

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
	UpdateRelatedTableViews();
}

// Перезагрузка зависимых видов таблиц.
void MTableView::UpdateRelatedTableViews()
{
	for(auto p_MTableView : v_p_MTableViewsRelated)
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
		p_MTableView->setItemDelegate(new MSqlRelationalDelegate(p_QSqlRelationalTableModel));
		if(p_MTableView->_iColumnForSort)
		{
			p_MTableView->sortByColumn(p_MTableView->_iColumnForSort, Qt::AscendingOrder);
		}
		p_QSqlRelationalTableModel->select();
	}
	if(_iColumnForSort)
	{
		this->model()->sort(_iColumnForSort);
	}
}

// Установка колонки для автосортировки или 0.
void MTableView::SetColumnForSort(int iColumnForSort)
{
	_iColumnForSort = iColumnForSort;
	if(_iColumnForSort) this->model()->sort(_iColumnForSort);
}

// Добавление зависимого виджета вида таблицы.
void MTableView::AddRelatedTableView(MTableView* p_MTableViewRelated)
{
	v_p_MTableViewsRelated.push_back(p_MTableViewRelated);
}

// Удаление зависимого виджета вида таблицы.
void MTableView::RemoveRelatedTableView(MTableView* p_MTableViewRelated)
{
	v_p_MTableViewsRelated.erase(std::find(v_p_MTableViewsRelated.begin(), v_p_MTableViewsRelated.end(), p_MTableViewRelated));
}

// Добавление зависимого виджета вида заголовка таблицы.
void MTableView::AddRelatedHorizontalHeaderView(MHeaderView* p_MHeaderViewRelated)
{
	v_p_MHorizontalHeaderViewsRelated.push_back(p_MHeaderViewRelated);
}

// Удаление зависимого виджета вида заголовка таблицы.
void MTableView::RemoveRelatedHorizontalHeaderView(MHeaderView* p_MHeaderViewRelated)
{
	v_p_MHorizontalHeaderViewsRelated.erase(std::find(v_p_MHorizontalHeaderViewsRelated.begin(), v_p_MHorizontalHeaderViewsRelated.end(), p_MHeaderViewRelated));
}
