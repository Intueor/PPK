//== ВКЛЮЧЕНИЯ.
#include "msqlrelationaldelegate.h"

//== ФУНКЦИИ КЛАССОВ.
//== Класс модифицированного для сортировки sql-соотносимого делегата виджета вида таблицы.
// Копия приватной функции получения индекса поля из базового класса.
int MSqlRelationalDelegate::fieldIndex(const QSqlTableModel* const p_Model, const QSqlDriver* const p_Driver, const QString& r_strFieldName)
{
	const QString strStripped = p_Driver->isIdentifierEscaped(r_strFieldName, QSqlDriver::FieldName)
			? p_Driver->stripDelimiters(r_strFieldName, QSqlDriver::FieldName)
			: r_strFieldName;
	return p_Model->fieldIndex(strStripped);
}

// Переопределённая функция создания редактора для подстановки сортировки в комбо.
QWidget* MSqlRelationalDelegate::createEditor(QWidget* p_Parent, const QStyleOptionViewItem& r_Option, const QModelIndex& r_Index) const
{
	const QSqlRelationalTableModel* p_SqlModel = qobject_cast<const QSqlRelationalTableModel *>(r_Index.model());
	QSqlTableModel* p_ChildModel = p_SqlModel ? p_SqlModel->relationModel(r_Index.column()) : nullptr;
	if(!p_ChildModel) return QStyledItemDelegate::createEditor(p_Parent, r_Option, r_Index);
	p_ChildModel->sort(1, Qt::SortOrder::AscendingOrder);
	const QSqlDriver* const p_Driver = p_ChildModel->database().driver();
	QComboBox* p_Combo = new QComboBox(p_Parent);
	p_Combo->setModel(p_ChildModel);
	p_Combo->setModelColumn(fieldIndex(p_ChildModel, p_Driver, p_SqlModel->relation(r_Index.column()).displayColumn()));
	p_Combo->installEventFilter(const_cast<MSqlRelationalDelegate *>(this));
	return p_Combo;
}
