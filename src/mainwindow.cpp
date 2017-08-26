#include <iostream>
#include <string>
#include <QWidget>
#include <QMessageBox>
#include <QLineEdit>
#include <QtGui>

#include "mainwindow.h"
#include "integequ.h"


MainWindow::MainWindow(QWidget *parent)
	:QWidget(parent)
{
	setupUi(this);

	pixCS = 0;
	scene_full = 0;
	scene_f = 0;
	scene_K = 0;
	scene_CS = 0;
	scene_y = 0;

	int eqcount = Inteq::eqcount;
	comboBox_Eq->addItem("");
	for (int i=0; i<eqcount; i++)
		comboBox_Eq->addItem(tr("Equation") + " " + QVariant(i+1).toString() + ": " + Inteq::eqstring(i+1));

	comboBox_Form->addItem("");
	comboBox_Form->addItem(tr("Trapezium formula"));
	comboBox_Form->addItem(tr("Simpsons's formula"));

	tableWidget_x->setColumnCount(1);
	tableWidget_x->setColumnWidth(0,60);
	tableWidget_x->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	tableWidget_x->verticalHeader()->setDefaultAlignment(Qt::AlignHCenter);

	tableWidget_f->setColumnCount(1);
	tableWidget_f->setColumnWidth(0,60);
	tableWidget_f->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	tableWidget_f->verticalHeader()->setDefaultAlignment(Qt::AlignHCenter);

	tableWidget_y->setColumnCount(1);
	tableWidget_y->setColumnWidth(0,60);
	tableWidget_y->horizontalHeader()->setResizeMode(QHeaderView::Fixed);

	tableWidget_K->setColumnCount(1);
	tableWidget_K->setColumnWidth(0,60);

	connect(pushButton_calc, SIGNAL(clicked()), this, SLOT(calculateEq()));
	connect(tableWidget_x, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(SetHeaders()));
	connect(tableWidget_y, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(SetHeaders()));
	connect(tableWidget_f, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(SetHeaders()));

	SetHeaders();
};


void MainWindow::on_comboBox_Eq_currentIndexChanged(int index)
{
	graphicsView_y->setScene(0);
	graphicsView_CS->setScene(0);

	tableWidget_x->setRowCount(0);
	tableWidget_f->setRowCount(0);
	tableWidget_K->setColumnCount(1);
	tableWidget_K->setRowCount(0);
	tableWidget_y->setRowCount(0);
	textBrowser_h->clear();

	if ( (index==0) )
	{
		graphicsView_Eq->setScene(0);
		graphicsView_f->setScene(0);
		graphicsView_K->setScene(0);

		doubleSpin_n->setValue(3);
		textBrowser_a->clear();
		textBrowser_b->clear();
		textBrowser_l->clear();

		pushButton_calc->setEnabled(false);
	}
	else
	{
		Inteq::set_eq(index);

		pix.load(":/images/images/formula_" + QVariant(index).toString() + "_full.png");
		if (scene_full != 0) delete(scene_full);
		scene_full = new QGraphicsScene;
		scene_full->addPixmap(pix);
		graphicsView_Eq->setScene(scene_full);

		pix.load(":/images/images/formula_" + QVariant(index).toString() + "_K.png");
		if (scene_K != 0) delete(scene_K);
		scene_K = new QGraphicsScene;
		scene_K->addPixmap(pix);
		graphicsView_K->setScene(scene_K);

		pix.load(":/images/images/formula_" + QVariant(index).toString() + "_f.png");
		if (scene_f != 0) delete(scene_f);
		scene_f = new QGraphicsScene;
		scene_f->addPixmap(pix);
		graphicsView_f->setScene(scene_f);

		textBrowser_a->setText(QVariant(Inteq::a).toString());
		textBrowser_b->setText(QVariant(Inteq::b).toString());
		textBrowser_l->setText(QVariant(Inteq::l).toString());

		if (this->comboBox_Form->currentIndex()!=0)
			pushButton_calc->setEnabled(true);
	}

};


void MainWindow::on_comboBox_Form_currentIndexChanged(int index)
{
	graphicsView_y->setScene(0);
	graphicsView_CS->setScene(0);

	tableWidget_x->setRowCount(0);
	tableWidget_f->setRowCount(0);
	tableWidget_K->setColumnCount(1);
	tableWidget_K->setRowCount(0);
	tableWidget_y->setRowCount(0);
	textBrowser_h->clear();

	if ( (index==0) )
	{
		pushButton_calc->setEnabled(false);
	}
	else
	{
		if (this->comboBox_Eq->currentIndex()!=0)
			pushButton_calc->setEnabled(true);

		Inteq::set_form(index);
	}

}


void MainWindow::SetHeaders()
{
	QStringList HeaderLabs;

	HeaderLabs.clear();
	HeaderLabs<<"xi";
	tableWidget_x->setHorizontalHeaderLabels(HeaderLabs);

	HeaderLabs.clear();
	HeaderLabs<<"fi";
	tableWidget_f->setHorizontalHeaderLabels(HeaderLabs);

	HeaderLabs.clear();
	HeaderLabs<<"yi";
	tableWidget_y->setHorizontalHeaderLabels(HeaderLabs);
}



void MainWindow::calculateEq()
{
	int n = this->doubleSpin_n->value();
	textBrowser_h->setText(QVariant(Inteq::h).toString());
	graphicsView_y->setScene(0);
	graphicsView_CS->setScene(0);

	tableWidget_x->clear();
	tableWidget_f->clear();
	tableWidget_K->clear();
	tableWidget_y->clear();

	tableWidget_x->setRowCount(1);
	tableWidget_f->setRowCount(1);
	tableWidget_K->setRowCount(1);
	tableWidget_K->setColumnCount(1);
	tableWidget_y->setRowCount(1);

	tableWidget_x->repaint();
	tableWidget_f->repaint();
	tableWidget_K->repaint();
	tableWidget_y->repaint();

	QTableWidgetItem* pTWItem = 0;

	try
	{
		Inteq::calc_eq(n);
	}
	catch (Inteq::DetZero())
	{
		QMessageBox::information(0, tr("Integral equations"),  tr("Single result is absent."), QMessageBox::Ok);
		return;
	}

	tableWidget_x->setRowCount(n);
	tableWidget_f->setRowCount(n);
	tableWidget_K->setRowCount(n);
	tableWidget_K->setColumnCount(n);
	tableWidget_y->setRowCount(n);

	for (int i = 0; i<n; ++i)
	{

		pTWItem = new QTableWidgetItem(QString::number(Inteq::Vx[i], 'f', 4));
		tableWidget_x->setItem(i, 0, pTWItem);

		pTWItem = new QTableWidgetItem(QString::number(Inteq::Vf[i], 'f', 4));
		tableWidget_f->setItem(i, 0, pTWItem);

		for (int t = 0; t<n; ++t)
		{
			pTWItem = new QTableWidgetItem(QString::number(Inteq::MK[i][t], 'f', 4));
			tableWidget_K->setItem(i, t, pTWItem);
		}

		pTWItem = new QTableWidgetItem(QString::number(Inteq::Vy[i], 'f', 4));
		tableWidget_y->setItem(i, 0, pTWItem);

	}

	if (scene_y != 0) delete(scene_y);
	scene_y = new QGraphicsScene;
	scene_y->addText(Inteq::ystr());
	graphicsView_y->setScene(scene_y);

	drawEq();

}


void MainWindow::drawEq()
{

	int h, w, ny, nx, dy, dx, grw, x0, y0;
	grw = 5;
	h = 200;
	w = 500;
	ny = 6;
	nx = 18;
	x0 = w/2 + grw;
	y0 = h/2 + grw;
	dy=h/ny;
	dx=w/nx;

	if (pixCS != 0) delete(pixCS);
	pixCS = new QPixmap( (w+2*grw), (h+2*grw));
	pixCS->fill();
	QPainter mypainter( pixCS );

	// horizontal axis
	mypainter.drawLine(grw, h/2+grw, w+grw, h/2+grw);
	// horizontal axis's arrow
	mypainter.drawLine(w-grw, h/2, w+grw, h/2+grw);
	mypainter.drawLine(w-grw, h/2+2*grw, w+grw, h/2+grw);
	// vertical graduation
	for (int y=((h/2)-(ny/2-1)*dy)+grw; y<=(h+grw)-dy; y+=dy)
	{
		mypainter.drawLine(w/2, y, w/2+2*grw, y);
		if (((ny/2) - (y/dy)) >= 0)
			mypainter.drawText ((w/2)-(3*grw), y + 2*grw, QVariant((ny/2) - (y/dy)).toString());
		else
			mypainter.drawText ((w/2)-(3.5*grw), y + 2*grw, QVariant((ny/2) - (y/dy)).toString());
	}

	// vertical axis
	mypainter.drawLine(w/2+grw, grw, w/2+grw, h+grw);
	// vertical axis's arrow
	mypainter.drawLine(w/2+grw, grw, w/2, grw*3);
	mypainter.drawLine(w/2+grw, grw, w/2+2*grw, grw*3);
	// horizontal graduation
	for (int x=((w/2)-(nx/2-1)*dx)+grw ; x<=(w+grw)-dx; x+=dx)
	{
		mypainter.drawLine(x, h/2, x, h/2+2*grw);
		if ((-(nx/2) + (x/dx)) <= 0)
			mypainter.drawText ( x-2*grw, (h/2)+(5*grw), QVariant(-(nx/2) + (x/dx)).toString());
		else
			mypainter.drawText ( x-grw, (h/2)+(5*grw), QVariant(-(nx/2) + (x/dx)).toString());
	}

	mypainter.drawText ( w/2-grw, grw, "y");
	mypainter.drawText ( w-grw, h/2+4*grw, "x");

	std::vector<double> Vxd(0);
	std::vector<double> Vyd(0);

	for (double x = -nx+w/2/dx+1+grw/dx; x<=-(nx/2)+((w+grw)-dx)/dx; x+=0.1)
	{
		Vxd.push_back(x);
		Vyd.push_back(Inteq::y(x));
	}

	for (unsigned int i = 0; i<Vxd.size()-1; ++i)
	{
		mypainter.drawLine(x0+Vxd[i]*dx, y0-Vyd[i]*dy, x0+Vxd[i+1]*dx, y0-Vyd[i+1]*dy);
	}

	scene_CS = new QGraphicsScene;
	scene_CS->addPixmap(*pixCS);
	graphicsView_CS->setScene(scene_CS);
}
