#include "Skippy.h"

Skippy::Skippy(QWidget *parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);
	m_ui.myGLWidget->setRenderFramework(&m_rf);
}
