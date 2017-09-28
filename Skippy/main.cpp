#include "Skippy.h"
#include <QtWidgets/QApplication>
#include <qfile.h>
#include <qdir.h>
#include <qdebug.h>


void myMessageHandler(QtMsgType msgType, const QMessageLogContext &context, const QString &msg);


int main(int argc, char *argv[])
{
	qInstallMessageHandler(myMessageHandler);
	QApplication a(argc, argv);
	Skippy w;
	w.show();
	return a.exec();
}



void myMessageHandler(QtMsgType msgType, const QMessageLogContext &context, const QString &msg)
{
	QString text;
	QFile *outFile;

	switch (msgType) {
	case QtDebugMsg:
		text = QString("Debug: (%1:%2, %3)\n%4\n").arg(context.file).arg(context.line).arg(context.function).arg(msg);
		outFile = new QFile("..//Log//debug.log");
		break;
	case QtInfoMsg:
		text = QString("Info: (%1:%2, %3)\n%4\n").arg(context.file).arg(context.line).arg(context.function).arg(msg);
		outFile = new QFile("..//Log//info.log");
		break;
	case QtWarningMsg:
		text = QString("Warning: (%1:%2, %3)\n%4\n").arg(context.file).arg(context.line).arg(context.function).arg(msg);
		outFile = new QFile("..//Log//warning.log");
		break;
	case QtCriticalMsg:
		text = QString("Critical: (%1:%2, %3)\n%4\n").arg(context.file).arg(context.line).arg(context.function).arg(msg);
		outFile = new QFile("..//Log//critical.log");
		break;
	case QtFatalMsg:
		text = QString("Fatal: (%1:%2, %3)\n%4\n").arg(context.file).arg(context.line).arg(context.function).arg(msg);
		outFile = new QFile("..//Log//fatal.log");
		abort();
	}

	outFile->open(QIODevice::WriteOnly | QIODevice::Append);
	//QTextStream textStream(outFile);
	//textStream << text;
	outFile->write(text.toStdString().c_str(), qstrlen(text.toStdString().c_str()));
	outFile->close();

	delete outFile;
}
