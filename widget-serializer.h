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
	std::shared_ptr<QSettings> _sp_QSettings; ///< Указатель на объект установок для чтения и записи состояний.
	QList<QSplitter*> lpQSplitters; ///< Лист с указателями на разделители.
	QList<QRadioButton*> lpQRadioButtons; ///< Лист с указателями на переключатели.
	QList<QLineEdit*> lpQLineEdits; ///< Лист с указателями на редактируемые строки.
	QVector<QWidget*> _vpExcludedWidgets; ///< Вектор с указателями на исключаемые из сериализации виджеты.

public:
	/// Конструктор.
	WidgetSerializer(std::shared_ptr<QSettings> p_QSettings)
										//! \param[in] p_QSettings Указатель на настройки.
		: _sp_QSettings(p_QSettings) {}

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
	void RegisterChildren(T* p_Widget, QVector<QWidget*>&& r_vpExcludedWidgets);
										///< \param[in] r_Widget Указатель на виджет для работы.
										///< \param[in] r_vpExcludedWidgets Ссылка на временный вектор с указателями на исключаемые из регистрации виджеты.
	/// Загрузка и установка состояний из настроек.
	template <typename T>
	void LoadStates(T* p_Widget);
										///< \param[in] r_Widget Указатель на виджет для работы.
	/// Запись состояний в настройки.
	template <typename T>
	void SaveStates(const T* p_Widget);
										///< \param[in] p_Widget Указатель на виджет для работы.
};

//== ИМПЛЕМЕНТАЦИЯ ФУНКЦИЙ КЛАССОВ.
//== Класс сериализатора виджетов.
// Регистрация дочерних элементов виджета.
template <typename T>
void WidgetSerializer::RegisterChildren(T* p_Widget)
{
	lpQSplitters = p_Widget->template findChildren<QSplitter*>();
	lpQRadioButtons = p_Widget->template findChildren<QRadioButton*>();
	lpQLineEdits = p_Widget->template findChildren<QLineEdit*>();
}

// Регистрация дочерних элементов виджета с исключениями.
template <typename T>
void WidgetSerializer::RegisterChildren(T* p_Widget, QVector<QWidget*>& r_vpExcludedWidgets)
{
	RegisterChildren(p_Widget);
	for(QWidget* p_QWidget : r_vpExcludedWidgets) lpQSplitters.removeOne(p_QWidget);
	for(QWidget* p_QWidget : r_vpExcludedWidgets) lpQRadioButtons.removeOne(p_QWidget);
	for(QWidget* p_QWidget : r_vpExcludedWidgets) lpQLineEdits.removeOne(p_QWidget);
}

// Регистрация дочерних элементов виджета с исключениями из временного объекта.
template <typename T>
void WidgetSerializer::RegisterChildren(T* p_Widget, QVector<QWidget*>&& r_vpExcludedWidgets) { RegisterChildren(p_Widget, r_vpExcludedWidgets); }

// Загрузка и установка состояний.
template <typename T>
void WidgetSerializer::LoadStates(T* p_Widget)
{
	QString strWName = p_Widget->objectName();
	// Основной виджет.
	p_Widget->restoreGeometry(_sp_QSettings->value(MkWidgetName(strWName, "G")).toByteArray());
	p_Widget->restoreState(_sp_QSettings->value(MkWidgetName(strWName, "S")).toByteArray());
	QVariant oQVariant;
	// Дочерние разделители.
	for(QSplitter* p_QSplitter : lpQSplitters)
	{
		p_QSplitter->restoreGeometry(_sp_QSettings->value(MkChildName(strWName, p_QSplitter->objectName(), "G")).toByteArray());
		p_QSplitter->restoreState(_sp_QSettings->value(MkChildName(strWName, p_QSplitter->objectName(), "S")).toByteArray());
	}
	// Дочерние переключатели.
	for(QRadioButton* p_QRadioButton : lpQRadioButtons)
	{
		oQVariant = _sp_QSettings->value(MkChildName(strWName, p_QRadioButton->objectName(), "V"));
		if(!oQVariant.isNull()) p_QRadioButton->setChecked(oQVariant.toBool());
	}
	// Дочерние строчные редакторы.
	for(QLineEdit* p_QLineEdit : lpQLineEdits)
	{
		oQVariant = _sp_QSettings->value(MkChildName(strWName, p_QLineEdit->objectName(), "T"));
		if(!oQVariant.isNull()) p_QLineEdit->setText(oQVariant.toString());
	}
}

// Запись состояний.
template <typename T>
void WidgetSerializer::SaveStates(const T* p_Widget)
{
	QString strWName = p_Widget->objectName();
	// Основной виджет.
	_sp_QSettings->setValue(MkWidgetName(strWName, "G"), p_Widget->saveGeometry());
	_sp_QSettings->setValue(MkWidgetName(strWName, "S"), p_Widget->saveState());
	// Дочерние разделители.
	for(QSplitter* p_QSplitter : lpQSplitters)
	{
		_sp_QSettings->setValue(MkChildName(strWName, p_QSplitter->objectName(), "G"), p_QSplitter->saveGeometry());
		_sp_QSettings->setValue(MkChildName(strWName, p_QSplitter->objectName(), "S"), p_QSplitter->saveState());
	}
	// Дочерние переключатели.
	for(QRadioButton* p_QRadioButton : lpQRadioButtons)
	{
		_sp_QSettings->setValue(MkChildName(strWName, p_QRadioButton->objectName(), "V"), p_QRadioButton->isChecked());
	}
	// Дочерние строчные редакторы.
	for(QLineEdit* p_QLineEdit : lpQLineEdits)
	{
		_sp_QSettings->setValue(MkChildName(strWName, p_QLineEdit->objectName(), "T"), p_QLineEdit->text());
	}
}

#endif // WIDGETSERIALIZER_H
