#ifndef MSQLRELATIONALDELEGATE_H
#define MSQLRELATIONALDELEGATE_H

//== ВКЛЮЧЕНИЯ.
#include <QSqlRelationalDelegate>

//== КЛАССЫ.
/// Класс модифицированного для сортировки sql-соотносимого делегата виджета вида таблицы.
class MSqlRelationalDelegate : public QSqlRelationalDelegate
{
	Q_OBJECT

private:
	/// Копия приватной функции получения индекса поля из базового класса.
	static int fieldIndex(const QSqlTableModel* const p_Model, const QSqlDriver* const p_Driver, const QString& r_strFieldName);
														///< \param[in] p_Model Константный указатель на константный объект модели.
														///< \param[in] p_Driver Константный указатель на константный объект драйвера.
														///< \param[in] p_Driver Константная ссылка на строку с именем поля.
														///< \return Индекс.
public:
	/// Конструктор.
	explicit MSqlRelationalDelegate(QObject* p_Parent = nullptr) : QSqlRelationalDelegate(p_Parent)
														///< \param[in] p_Parent Указатель на родительский объект.
	{}
	/// Переопределённая функция создания редактора для подстановки сортировки в комбо.
	QWidget* createEditor(QWidget* p_Parent, const QStyleOptionViewItem& r_Option, const QModelIndex& r_Index) const override;
														///< \param[in] p_Parent Указатель на родительский виджет.
														///< \param[in] r_Option Ссылка на опции.
														///< \param[in] r_Index Ссылка на индекс.
														///< \return Указатель на виджет.

};

#endif // MSQLRELATIONALDELEGATE_H
