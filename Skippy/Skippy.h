#ifndef SKIPPY_H
#define SKIPPY_H

#include <QtWidgets/QMainWindow>
#include "ui_Skippy.h"
#include "MyGeometry/RenderFramework.h"

class Skippy : public QMainWindow
{
	Q_OBJECT

public:
	Skippy(QWidget *parent = Q_NULLPTR);

private:
	Ui::SkippyUi m_ui;
	RenderFramework m_rf;

};


#endif