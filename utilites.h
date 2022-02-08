#ifndef UTILITES_H
#define UTILITES_H

//== ВКЛЮЧЕНИЯ.
#include <QDialog>

//== ФУНКЦИИ.
/// Исполнение диалога с возможностью отмены результатов.
//! \file utilites.h
void CancelableDialogExec(QDialog& r_Dialog);
										///< \param[in,out] r_Dialog Ссылка на диалог для исполнения.

#endif // UTILITES_H
