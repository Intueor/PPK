//== ВКЛЮЧЕНИЯ.
#include "msqlrelationaldelegate.h"
#include <QLineEdit>

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
	const QSqlRelationalTableModel* p_SqlModel = qobject_cast<const QSqlRelationalTableModel*>(r_Index.model());
	QSqlTableModel* p_ChildModel = p_SqlModel ? p_SqlModel->relationModel(r_Index.column()) : nullptr;
	if(!p_ChildModel) return QSqlRelationalDelegate::createEditor(p_Parent, r_Option, r_Index);
	p_ChildModel->sort(1, Qt::SortOrder::AscendingOrder);
	const QSqlDriver* const p_Driver = p_ChildModel->database().driver();
	QComboBox* p_Combo = new QComboBox(p_Parent);
	p_Combo->setModel(p_ChildModel);
	p_Combo->setModelColumn(fieldIndex(p_ChildModel, p_Driver, p_SqlModel->relation(r_Index.column()).displayColumn()));
	// Установка фильтра из карты по колонке индекса.
	if(_p_mpColumnsFilters)
	{
		int iICol = r_Index.column();
		for(const auto& r_prType : *_p_mpColumnsFilters)
		{
			if(r_prType.first == iICol)
			{
				p_ChildModel->setFilter(r_prType.second);
				break;
			}
		}
	}
	p_Combo->installEventFilter(const_cast<MSqlRelationalDelegate*>(this));
	return p_Combo;
}

// Переопределённая функция установки данных модели для проверки и отсечения некорректных данных по колонке и типу из карты.
void MSqlRelationalDelegate::setModelData(QWidget* p_Editor, QAbstractItemModel* p_Model, const QModelIndex& r_Index) const
{
	QLineEdit* p_LineEdit = qobject_cast<QLineEdit*>(p_Editor);
	if(p_LineEdit && _p_mpColumnsDataTypes)
	{
		QString strText = p_LineEdit->text();
		int iICol = r_Index.column();
		for(const auto& r_prType : *_p_mpColumnsDataTypes)
		{
			if(r_prType.first == iICol)
			{
				switch(r_prType.second)
				{
					case CustomDelegateType::FormattedTime:
					{
						int iLength = strText.length();
						if(iLength > 5 || iLength < 3 || !strText.contains(':')) return;
						QStringList strlHM = strText.split(':', Qt::SkipEmptyParts);
						if(strlHM.length() != 2) return;
						int iH = strlHM.at(0).toInt();
						if((iH < 0) || (iH > 23)) return;
						int iM = strlHM.at(1).toInt();
						if((iM < 0) || (iM > 59)) return;
						QString strH = QString::number(iH);
						QString strM = QString::number(iM);
						if(strH.length() < 2) strH.prepend('0');
						if(strM.length() < 2) strM.prepend('0');
						p_LineEdit->setText(strH + ':' + strM);
						break;
					}
					case CustomDelegateType::PositiveNotNull:
					{
						if(strText.toInt() <= 0) return;
						break;
					}
					case CustomDelegateType::StringNotEmpty:
					{
						if(strText.isEmpty()) return;
						break;
					}
				}
				break;
			}
		}

	}
	QSqlRelationalDelegate::setModelData(p_Editor, p_Model, r_Index);
}
