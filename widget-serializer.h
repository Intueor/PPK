#ifndef WIDGETSERIALIZER_H
#define WIDGETSERIALIZER_H

//== ВКЛЮЧЕНИЯ.
#include <QSettings>
#include <QList>
#include <QSplitter>
#include <QRadioButton>
#include <QLineEdit>
#include <QWidget>

//== МАКРОСЫ.
#define MkWidgetName(v,s)		v+"-"+s
#define MkChildName(p,v,s)		p+"-"+MkWidgetName(v,s)

//== КЛАССЫ.
/// Класс сериализатора виджетов.
class WidgetSerializer
{
private:
	std::shared_ptr<QSettings> _sp_Settings; ///< Указатель на объект установок для чтения и записи состояний.
	QVector<QWidget*> _vpExcludedWidgets; ///< Вектор с указателями на исключаемые из сериализации виджеты.
	// Обрабатываемые типы.
	QList<QSplitter*> lpSplitters; ///< Лист с указателями на разделители.
	QList<QRadioButton*> lpRadioButtons; ///< Лист с указателями на переключатели.
	QList<QLineEdit*> lpLineEdits; ///< Лист с указателями на редактируемые строки.
	// Другие требуемые типы...

public:
	/// Конструктор.
	WidgetSerializer(std::shared_ptr<QSettings> p_Settings)
										//! \param[in] p_Settings Указатель на настройки.
		: _sp_Settings(p_Settings) {}

	/// Регистрация дочерних элементов виджета.
	template <typename T>
	void RegisterChildren(T* p_Widget);
										///< \param[in] r_Widget Указатель на виджет для работы.
	/// Регистрация дочерних элементов виджета с исключениями.
	template <typename T>
	void RegisterChildren(T* p_Widget, QVector<QWidget*>& r_vpExcludedWidgets);
										///< \param[in] r_Widget Указатель на виджет для работы.
										///< \param[in] r_vpExcludedWidgets Ссылка на вектор с указателями на исключаемые из регистрации виджеты.
	/// Регистрация дочерних элементов виджета с исключениями из временного объекта.
	template <typename T>
	void RegisterChildren(T* p_Widget, QVector<QWidget*>&& r_vpExcludedWidgets)
										//! \param[in] r_Widget Указатель на виджет для работы.
										//! \param[in] r_vpExcludedWidgets Ссылка на временный вектор с указателями на исключаемые из регистрации виджеты.
	{ RegisterChildren(p_Widget, r_vpExcludedWidgets); }

	/// Регистрация вектора элементов.
	void Register(QVector<QWidget*>& vpWidgets)
										//! \param[in] vpWidgets Ссылка на вектор с указателями на виджеты для регистрации.
	{
		for(QWidget* p_Widget : vpWidgets)
		{
			QString strType = p_Widget->metaObject()->className();
			// Обрабатываемые типы.
			if(strType == "QSplitter") lpSplitters.append(static_cast<QSplitter*>(p_Widget)); // Разделители.
			else if(strType == "QRadioButton") lpRadioButtons.append(static_cast<QRadioButton*>(p_Widget)); // Переключатели.
			else if(strType == "QLineEdit") lpLineEdits.append(static_cast<QLineEdit*>(p_Widget)); // Строчные редакторы.
			// Другие требуемые типы...
		}
	}

	/// Регистрация вектора элементов из временного объекта.
	void Register(QVector<QWidget*>&& vpWidgets)
										//! \param[in] vpWidgets Ссылка на временный вектор с указателями на виджеты для регистрации.
	{ Register(vpWidgets); }

	/// Загрузка и установка состояний из настроек.
	template <typename T>
	void LoadStates(T* p_Widget, bool bIncludeMainStates = true);
										///< \param[in] r_Widget Указатель на виджет для работы.
										///< \param[in] bIncludeMainStates true - включить в загрузку данные по основному виджету.
	/// Запись состояний в настройки.
	template <typename T>
	void SaveStates(const T* p_Widget, bool bIncludeMainStates = true);
										///< \param[in] p_Widget Указатель на виджет для работы.
										///< \param[in] bIncludeMainStates true - включить в сохранение данные по основному виджету.
};

//== ИМПЛЕМЕНТАЦИЯ ШАБЛОННЫХ ФУНКЦИЙ КЛАССОВ.
//== Класс сериализатора виджетов.
// Регистрация дочерних элементов виджета.
template <typename T>
void WidgetSerializer::RegisterChildren(T* p_Widget)
{
	// Обрабатываемые типы.
	lpSplitters = p_Widget->template findChildren<QSplitter*>(); // Разделители.
	lpRadioButtons = p_Widget->template findChildren<QRadioButton*>(); // Переключатели.
	lpLineEdits = p_Widget->template findChildren<QLineEdit*>(); // Строчные редакторы.
	// Другие требуемые типы...
}

// Регистрация дочерних элементов виджета с исключениями.
template <typename T>
void WidgetSerializer::RegisterChildren(T* p_Widget, QVector<QWidget*>& r_vpExcludedWidgets)
{
	RegisterChildren(p_Widget);
	// Обрабатываемые типы.
	for(QWidget* p_QWidget : r_vpExcludedWidgets) lpSplitters.removeOne(p_QWidget); // Разделители.
	for(QWidget* p_QWidget : r_vpExcludedWidgets) lpRadioButtons.removeOne(p_QWidget); // Переключатели.
	for(QWidget* p_QWidget : r_vpExcludedWidgets) lpLineEdits.removeOne(p_QWidget); // Строчные редакторы.
	// Другие требуемые типы...
}

// Загрузка и установка состояний.
template <typename T>
void WidgetSerializer::LoadStates(T* p_Widget, bool bIncludeMainStates)
{
	QString strWName = p_Widget->objectName();
	if(bIncludeMainStates)
	{
		// Основной виджет.
		p_Widget->restoreGeometry(_sp_Settings->value(MkWidgetName(strWName, "G")).toByteArray());
		p_Widget->restoreState(_sp_Settings->value(MkWidgetName(strWName, "S")).toByteArray());
	}
	QVariant oVariant;
	// Обрабатываемые типы.
	// Дочерние разделители.
	for(QSplitter* p_Splitter : lpSplitters)
	{
		p_Splitter->restoreGeometry(_sp_Settings->value(MkChildName(strWName, p_Splitter->objectName(), "G")).toByteArray());
		p_Splitter->restoreState(_sp_Settings->value(MkChildName(strWName, p_Splitter->objectName(), "S")).toByteArray());
	}
	// Дочерние переключатели.
	for(QRadioButton* p_RadioButton : lpRadioButtons)
	{
		oVariant = _sp_Settings->value(MkChildName(strWName, p_RadioButton->objectName(), "V"));
		if(!oVariant.isNull()) p_RadioButton->setChecked(oVariant.toBool());
	}
	// Дочерние строчные редакторы.
	for(QLineEdit* p_LineEdit : lpLineEdits)
	{
		oVariant = _sp_Settings->value(MkChildName(strWName, p_LineEdit->objectName(), "T"));
		if(!oVariant.isNull()) p_LineEdit->setText(oVariant.toString());
	}
	// Другие требуемые типы...
}

// Запись состояний.
template <typename T>
void WidgetSerializer::SaveStates(const T* p_Widget, bool bIncludeMainStates)
{
	QString strWName = p_Widget->objectName();
	if(bIncludeMainStates)
	{
		// Основной виджет.
		_sp_Settings->setValue(MkWidgetName(strWName, "G"), p_Widget->saveGeometry());
		_sp_Settings->setValue(MkWidgetName(strWName, "S"), p_Widget->saveState());
	}
	// Обрабатываемые типы.
	// Дочерние разделители.
	for(QSplitter* p_Splitter : lpSplitters)
	{
		_sp_Settings->setValue(MkChildName(strWName, p_Splitter->objectName(), "G"), p_Splitter->saveGeometry());
		_sp_Settings->setValue(MkChildName(strWName, p_Splitter->objectName(), "S"), p_Splitter->saveState());
	}
	// Дочерние переключатели.
	for(QRadioButton* p_RadioButton : lpRadioButtons)
	{
		_sp_Settings->setValue(MkChildName(strWName, p_RadioButton->objectName(), "V"), p_RadioButton->isChecked());
	}
	// Дочерние строчные редакторы.
	for(QLineEdit* p_LineEdit : lpLineEdits)
	{
		_sp_Settings->setValue(MkChildName(strWName, p_LineEdit->objectName(), "T"), p_LineEdit->text());
	}
	// Другие требуемые типы...
}

#endif // WIDGETSERIALIZER_H
