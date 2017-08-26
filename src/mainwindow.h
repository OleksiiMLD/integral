#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "ui/ui_mainwindow.h"

class MainWindow : public QWidget, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent =0 );

private:
//
	QPixmap pix;
	QPixmap* pixCS;
	QGraphicsScene* scene_full;
	QGraphicsScene* scene_f;
	QGraphicsScene* scene_K;
	QGraphicsScene* scene_CS;
	QGraphicsScene* scene_y;
	void drawEq();

private slots:
	void on_comboBox_Eq_currentIndexChanged(int);
	void on_comboBox_Form_currentIndexChanged(int);
	void calculateEq();
	void SetHeaders();
};


#endif
