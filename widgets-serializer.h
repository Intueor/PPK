#ifndef WIDGETSERIALIZER_H
#define WIDGETSERIALIZER_H

//== ВКЛЮЧЕНИЯ.
#include <QSettings>
#include <QList>
#include <QSplitter>
#include <QRadioButton>
#include <QLineEdit>
#include <QWidget>
#include <QSpinBox>
#include <QMap>
#include <QSet>

//== МАКРОСЫ.
#define MkWidgetName(widget,suffix)				widget+"-"+suffix
#define MkChildName(parent,widget,suffix)		parent+"-"+MkWidgetName(widget,suffix)

//== КЛАССЫ.
/// Класс сериализатора виджетов.
class WidgetsSerializer
{
private:
	std::shared_ptr<QSettings> _sp_Settings; ///< Указатель на объект установок для чтения и записи состояний.
	QVector<QWidget*> _vpWidgetsExcluded; ///< Вектор с указателями на исключаемые из сериализации виджеты.
	// Обрабатываемые типы.
	QMap<QString, QSet<QSplitter*>> mpWNameToSplitters; ///< Мап имён виджетов к сету указателей на разделители.
	QMap<QString, QSet<QRadioButton*>> mpWNameToRadioButtons; ///< Мап имён виджетов к сету указателей на переключатели.
	QMap<QString, QSet<QLineEdit*>> mpWNameToLineEdits; ///< Мап имён виджетов к сету указателей на редактируемые строки.
	QMap<QString, QSet<QSpinBox*>> mpWNameToSpinBoxes; ///< Мап имён виджетов к сету указателей на спинбоксы.
	// Другие требуемые типы...

private:
	/// Удаление виджетов из листа по типам родителей.
	template <typename T>
	void EraseByParentTypes(QList<T>& r_lpWidgets, const QList<QString>& r_lTypes)
										//! \param[in,out] r_lpWidgets Ссылка на лист зарегистрированных виджетов данного типа.
										//! \param[in] r_lTypes Ссылка на лист с именами типов для вычистки.
	{
		auto itW = r_lpWidgets.begin();
		while(itW != r_lpWidgets.end())
		{
			bool bEr  = false;
			for(QString strType : r_lTypes)
			{
				if(strType == (*itW)->parentWidget()->metaObject()->className())
				{
					itW = r_lpWidgets.erase(itW);
					bEr = true;
					break;
				}
			}
			if(!bEr) itW++;
		}
	}

	/// Создание сета детей виджета по типу.
	template <typename T>
	QSet<T> MakeChildrenListOfType(const QWidget& r_Widget) const
										//! \param[in] r_Widget Ссылка на виджет для работы.
										//! \return Сет детей по типу.
	{
		QList<T> lpResult = r_Widget.template findChildren<T>();
		return {lpResult.begin(), lpResult.end()};
	}

	/// Создание сета детей виджета по типу с исключением листа неприемлемых родителей.
	template <typename T>
	QSet<T> MakeChildrenListOfType(const QWidget& r_Widget, const QList<QString>&& r_lTypes)
										//! \param[in] r_Widget Ссылка на виджет для работы.
										//! \param[in] r_lTypes Ссылка на временный лист с именами типов родителей, детьми которых не могут являться члены итогового списка.
										//! \return Сет детей по типу с исключением листа неприемлемых родителей.
	{
		QList<T> lpResult = r_Widget.template findChildren<T>();
		EraseByParentTypes<T>(lpResult, r_lTypes);
		return {lpResult.begin(), lpResult.end()};
	}

public:
	/// Конструктор от разделяемого указателя.
	WidgetsSerializer(std::shared_ptr<QSettings> sp_Settings)
										//! \param[in] sp_Settings Указатель на настройки.
		: _sp_Settings(sp_Settings) {}

	/// Конструктор.
	WidgetsSerializer(QSettings& r_Settings)
										//! \param[in] r_Settings Ссылка на настройки.
		: _sp_Settings(std::make_shared<QSettings>(&r_Settings)) {}

	/// Регистрация дочерних элементов виджета.
	template <typename T>
	void RegisterChildren(const T& r_Widget);
										///< \param[in] r_Widget Ссылка на виджет для работы.
	/// Регистрация дочерних элементов виджета с исключениями.
	template <typename T>
	void RegisterChildren(const T& r_Widget, const QVector<QWidget*>& r_vpWidgetsExcluded);
										///< \param[in] r_Widget Ссылка на виджет для работы.
										///< \param[in] r_vpWidgetsExcluded Ссылка на вектор с указателями на исключаемые из регистрации виджеты.
	/// Регистрация дочерних элементов виджета с исключениями из временного объекта.
	template <typename T>
	void RegisterChildren(const T& r_Widget, const QVector<QWidget*>&& r_vpWidgetsExcluded)
										//! \param[in] r_Widget Ссылка на виджет для работы.
										//! \param[in] r_vpWidgetsExcluded Ссылка на временный вектор с указателями на исключаемые из регистрации виджеты.
	{ RegisterChildren(r_Widget, r_vpWidgetsExcluded); }

	/// Регистрация вектора элементов.
	template <typename T>
	void Register(const T& r_Widget, const QVector<QWidget*>& r_vpWidgets)
										//! \param[in] r_Widget Ссылка на виджет для работы.
										//! \param[in] r_vpWidgets Ссылка на вектор с указателями на виджеты для регистрации.
	{
		QString strWName = r_Widget.objectName();
		for(QWidget* p_WidgetToReg : r_vpWidgets)
		{
			QString strType = p_WidgetToReg->metaObject()->className();
			// Обрабатываемые типы.
			if(strType == "QSplitter") mpWNameToSplitters[strWName].insert(static_cast<QSplitter*>(r_Widget)); // Разделители.
			else if(strType == "QRadioButton") mpWNameToRadioButtons[strWName].insert(static_cast<QRadioButton*>(r_Widget)); // Переключатели.
			else if(strType == "QLineEdit") mpWNameToLineEdits[strWName].insert(static_cast<QLineEdit*>(r_Widget)); // Строчные редакторы.
			else if(strType == "QSpinBox") mpWNameToSpinBoxes[strWName].insert(static_cast<QSpinBox*>(r_Widget)); // Спинбоксы.
			// Другие требуемые типы...
		}
	}

	/// Регистрация вектора элементов из временного объекта.
	template <typename T>
	void Register(const T& r_Widget, const QVector<QWidget*>&& r_vpWidgets)
										//! \param[in] r_Widget Ссылка на виджет для работы.
										//! \param[in] r_vpWidgets Ссылка на временный вектор с указателями на виджеты для регистрации.
	{ Register(r_Widget, r_vpWidgets); }

	/// Загрузка и установка состояний из настроек.
	template <typename T>
	void LoadStates(T& r_Widget, bool bIncludeMainGeometry = true);
										///< \param[in,out] r_Widget Ссылка на виджет для работы.
										///< \param[in] bIncludeMainGeometry true - включить в загрузку геометрию основного виджета.
	/// Запись состояний в настройки.
	template <typename T>
	void SaveStates(const T& r_Widget, bool bIncludeMainGeometry = true);
										///< \param[in] r_Widget Ссылка на виджет для работы.
										///< \param[in] bIncludeMainGeometry true - включить в сохранение геометрию основного виджета.
};

//== ИМПЛЕМЕНТАЦИЯ ШАБЛОННЫХ ФУНКЦИЙ КЛАССОВ.
//== Класс сериализатора виджетов.
// Регистрация дочерних элементов виджета.
template <typename T>
void WidgetsSerializer::RegisterChildren(const T& r_Widget)
{
	QString strWName = r_Widget.objectName();
	// Обрабатываемые типы.
	mpWNameToSplitters[strWName] += MakeChildrenListOfType<QSplitter*>(r_Widget); // Разделители.
	mpWNameToRadioButtons[strWName] += MakeChildrenListOfType<QRadioButton*>(r_Widget); // Переключатели.
	mpWNameToLineEdits[strWName] += MakeChildrenListOfType<QLineEdit*>(r_Widget, {"QSpinBox"}); // Строчные редакторы, кроме детей QSpinBox.
	mpWNameToSpinBoxes[strWName] += MakeChildrenListOfType<QSpinBox*>(r_Widget); // Спинбоксы.
	// Другие требуемые типы...
}

// Регистрация дочерних элементов виджета с исключениями.
template <typename T>
void WidgetsSerializer::RegisterChildren(const T& r_Widget, const QVector<QWidget*>& r_vpWidgetsExcluded)
{
	QString strWName = r_Widget.objectName();
	RegisterChildren(r_Widget);
	for(QWidget* p_QWidget : r_vpWidgetsExcluded)
	{
		// Обрабатываемые типы.
		mpWNameToSplitters[strWName].remove(static_cast<QSplitter*>(p_QWidget)); // Разделители.
		mpWNameToRadioButtons[strWName].remove(static_cast<QRadioButton*>(p_QWidget)); // Переключатели.
		mpWNameToLineEdits[strWName].remove(static_cast<QLineEdit*>(p_QWidget)); // Строчные редакторы.
		mpWNameToSpinBoxes[strWName].remove(static_cast<QSpinBox*>(p_QWidget)); // Спинбоксы.
		// Другие требуемые типы...
	}
}

// Загрузка и установка состояний.
template <typename T>
void WidgetsSerializer::LoadStates(T& r_Widget, bool bIncludeMainGeometry)
{
	QString strWName = r_Widget.objectName();
	if(bIncludeMainGeometry) r_Widget.restoreGeometry(_sp_Settings->value(MkWidgetName(strWName, "G")).toByteArray());
	QVariant oVariant;
	// Обрабатываемые типы.
	// Дочерние разделители.
	for(QSplitter* p_Splitter : mpWNameToSplitters[strWName])
	{
		p_Splitter->restoreGeometry(_sp_Settings->value(MkChildName(strWName, p_Splitter->objectName(), "G")).toByteArray());
		p_Splitter->restoreState(_sp_Settings->value(MkChildName(strWName, p_Splitter->objectName(), "S")).toByteArray());
	}
	// Дочерние переключатели.
	for(QRadioButton* p_RadioButton : mpWNameToRadioButtons[strWName])
	{
		oVariant = _sp_Settings->value(MkChildName(strWName, p_RadioButton->objectName(), "V"));
		if(!oVariant.isNull()) p_RadioButton->setChecked(oVariant.toBool());
	}
	// Дочерние строчные редакторы.
	for(QLineEdit* p_LineEdit : mpWNameToLineEdits[strWName])
	{
		oVariant = _sp_Settings->value(MkChildName(strWName, p_LineEdit->objectName(), "V"));
		if(!oVariant.isNull()) p_LineEdit->setText(oVariant.toString());
	}
	// Дочерние строчные cпинбоксы.
	for(QSpinBox* p_SpinBox : mpWNameToSpinBoxes[strWName])
	{
		oVariant = _sp_Settings->value(MkChildName(strWName, p_SpinBox->objectName(), "V"));
		if(!oVariant.isNull()) p_SpinBox->setValue(oVariant.toInt());
	}
	// Другие требуемые типы...
}

// Запись состояний.
template <typename T>
void WidgetsSerializer::SaveStates(const T& r_Widget, bool bIncludeMainGeometry)
{
	QString strWName = r_Widget.objectName();
	if(bIncludeMainGeometry) _sp_Settings->setValue(MkWidgetName(strWName, "G"), r_Widget.saveGeometry());
	// Обрабатываемые типы.
	// Дочерние разделители.
	for(QSplitter* p_Splitter : mpWNameToSplitters[strWName])
	{
		_sp_Settings->setValue(MkChildName(strWName, p_Splitter->objectName(), "G"), p_Splitter->saveGeometry());
		_sp_Settings->setValue(MkChildName(strWName, p_Splitter->objectName(), "S"), p_Splitter->saveState());
	}
	// Дочерние переключатели.
	for(QRadioButton* p_RadioButton : mpWNameToRadioButtons[strWName])
	{
		_sp_Settings->setValue(MkChildName(strWName, p_RadioButton->objectName(), "V"), p_RadioButton->isChecked());
	}
	// Дочерние строчные редакторы.
	for(QLineEdit* p_LineEdit : mpWNameToLineEdits[strWName])
	{
		_sp_Settings->setValue(MkChildName(strWName, p_LineEdit->objectName(), "V"), p_LineEdit->text());
	}
	// Дочерние cпинбоксы.
	for(QSpinBox* p_SpinBox : mpWNameToSpinBoxes[strWName])
	{
		_sp_Settings->setValue(MkChildName(strWName, p_SpinBox->objectName(), "V"), p_SpinBox->value());
	}
	// Другие требуемые типы...
}

#endif // WIDGETSERIALIZER_H
