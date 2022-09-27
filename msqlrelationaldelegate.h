#ifndef MSQLRELATIONALDELEGATE_H
#define MSQLRELATIONALDELEGATE_H

//== ВКЛЮЧЕНИЯ.
#include <QSqlRelationalDelegate>

//== КЛАССЫ.
/// Класс модифицированного для сортировки sql-соотносимого делегата виджета вида таблицы.
class MSqlRelationalDelegate : public QSqlRelationalDelegate
{
	Q_OBJECT

public:
	/// Перечисление типов делегатов по виду данных.
	enum CustomDelegateType
	{
		Positive,
		FormattedTime
	};

private:
	const std::map<uchar, CustomDelegateType>* const _p_mpColumnsDataTypes; ///< Внутренний константный указатель на константную карту 'колонка в тип данных'.

private:
	/// Копия приватной функции получения индекса поля из базового класса.
	static int fieldIndex(const QSqlTableModel* const p_Model, const QSqlDriver* const p_Driver, const QString& r_strFieldName);
														///< \param[in] p_Model Константный указатель на константный объект модели.
														///< \param[in] p_Driver Константный указатель на константный объект драйвера.
														///< \param[in] p_Driver Константная ссылка на строку с именем поля.
														///< \return Индекс.
public:
	/// Конструктор.
	explicit MSqlRelationalDelegate(QObject* p_Parent = nullptr, const std::map<uchar, CustomDelegateType>* const p_mpColumnsDataTypes = nullptr)
		: QSqlRelationalDelegate(p_Parent), _p_mpColumnsDataTypes(p_mpColumnsDataTypes)
														///< \param[in] p_Parent Указатель на родительский объект.
														///< \param[in] p_mpColumnsDataTypes Константный указатель на константную карту 'колонка в тип данных'.
	{}
	/// Переопределённая функция создания редактора для подстановки сортировки в комбо.
	QWidget* createEditor(QWidget* p_Parent, const QStyleOptionViewItem& r_Option, const QModelIndex& r_Index) const override;
														///< \param[in] p_Parent Указатель на родительский виджет.
														///< \param[in] r_Option Ссылка на опции.
														///< \param[in] r_Index Ссылка на индекс.
														///< \return Указатель на виджет.
	/// Переопределённая функция установки данных модели для проверки и отсечения некорректных данных по колонке и типу из карты.
	void setModelData(QWidget* p_Editor, QAbstractItemModel* p_Model, const QModelIndex& r_Index) const override;
														///< \param[in] p_Editor Указатель на редактор.
														///< \param[in] p_Model Указатель на модель.
														///< \param[in] r_Index Константная ссылка на индекс.
};

#endif // MSQLRELATIONALDELEGATE_H
