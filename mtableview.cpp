//== ВКЛЮЧЕНИЯ.
#include "mtableview.h"
#include <QSqlRelationalDelegate>
#include <QMouseEvent>
#include <QScrollArea>

//== ФУНКЦИИ КЛАССОВ.
//== Класс модифицированного заголовка виджета вида таблицы.
// Переопределение обработчика события нажатия мыши.
void MHorizontalHeaderView::mousePressEvent(QMouseEvent* p_E)
{
	QHeaderView::mousePressEvent(p_E);
	if(_p_v_p_MHorizontalHeaderViewsRelated) bMPressed = true;
}

// Переопределение обработчика события смещения мыши.
void MHorizontalHeaderView::mouseMoveEvent(QMouseEvent* p_E)
{
	QHeaderView::mouseMoveEvent(p_E);
	if(bMPressed)
	{
		int iVHWidth = static_cast<MTableView*>(parent())->verticalHeader()->width();
		for(MHorizontalHeaderView* p_MHorizontalHeaderViewRelated : *_p_v_p_MHorizontalHeaderViewsRelated)
		{
			int iVHWidthRelated = static_cast<MTableView*>(p_MHorizontalHeaderViewRelated->parent())->verticalHeader()->width();
			bool bFirstVisibleFound = false;
			int iFirstVisibleSection = 0;
			for(int iC = 0; iC < this->model()->columnCount(); iC++)
			{
				if(!p_MHorizontalHeaderViewRelated->isSectionHidden(iC) && !bFirstVisibleFound)
				{
					bFirstVisibleFound = true;
					iFirstVisibleSection = iC;
				}
				int iSize = sectionSize(iC);
				if(bFirstVisibleFound && iFirstVisibleSection == iC)
				{
					iSize += iVHWidth - iVHWidthRelated;
					int iD = static_cast<MTableView*>(parent())->width() - static_cast<MTableView*>(p_MHorizontalHeaderViewRelated->parent())->width();
					if(iD != 0) iSize -= iD;
				}
				int iSizeRelated = p_MHorizontalHeaderViewRelated->sectionSize(iC);
				if(iSizeRelated != iSize)
				{
					p_MHorizontalHeaderViewRelated->resizeSection(iC, iSize);
				}
			}
		}
	}
}

// Переопределение обработчика события отпускания мыши.
void MHorizontalHeaderView::mouseReleaseEvent(QMouseEvent* p_E)
{
	bMPressed = false;
	QHeaderView::mouseReleaseEvent(p_E);
}

//== Класс модифицированного виджета вида таблицы.
// Конструктор.
MTableView::MTableView(QWidget* p_Parent) : QTableView(p_Parent)
{
	connect(&oAntiSpamTimer, SIGNAL(timeout()), this, SLOT(AntiSpamTimerSlot()));
	oAntiSpamTimer.start(STARTUP_UPDATE_ANTISPAM_DELAY_MS);
	MHorizontalHeaderView* p_MHorizontalHeaderView = new MHorizontalHeaderView(Qt::Horizontal, this);
	p_MHorizontalHeaderView->SetRelatedHorizontalHeaderViews(&v_p_MHorizontalHeaderViewsRelated);
	setHorizontalHeader(p_MHorizontalHeaderView);
}

void MTableView::AntiSpamTimerSlot()
{
	bStartupSpamIsOver = true;
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
				setFixedHeight(iAllRowsHeight + horizontalHeader()->height() + verticalHeader()->height());
			}
		}
	}
	if(bStartupSpamIsOver)
	{
		if(model() && bOverridedStretchLastSection)
		{
			int iHContentWidth = 0;
			int iLastVisible = 0;
			for(int iC = 0; iC < model()->columnCount(); iC++)
			{
				if(!isColumnHidden(iC)) iLastVisible = iC;
				iHContentWidth += columnWidth(iC);
			}
			int iHorizontalRightSpace = horizontalHeader()->width() - iHContentWidth;
			if(iHorizontalRightSpace > 0)
			{
				setColumnWidth(iLastVisible, columnWidth(iLastVisible) + iHorizontalRightSpace);
			}
		}
	}
	else oAntiSpamTimer.start(STARTUP_UPDATE_ANTISPAM_DELAY_MS);
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
void MTableView::AddRelatedHorizontalHeaderView(MHorizontalHeaderView* p_MHorizontalHeaderViewRelated)
{
	v_p_MHorizontalHeaderViewsRelated.push_back(p_MHorizontalHeaderViewRelated);
}

// Удаление зависимого виджета вида заголовка таблицы.
void MTableView::RemoveRelatedHorizontalHeaderView(MHorizontalHeaderView* p_MHorizontalHeaderViewRelated)
{
	v_p_MHorizontalHeaderViewsRelated.erase(std::find(v_p_MHorizontalHeaderViewsRelated.begin(), v_p_MHorizontalHeaderViewsRelated.end(), p_MHorizontalHeaderViewRelated));
}
