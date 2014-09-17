#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include "Terrain.h"
#include "GLViewport.h"
#include "GLRenderer.h"
#include <QFileInfo>
#include <QMessageBox>

#define _USE_MATH_DEFINES
#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	terrain = new Terrain;

	connect(ui->glViewport, SIGNAL(initialized()), this, SLOT(openglInitialized()));
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));

	connect(ui->glViewport, SIGNAL(cameraXChanged(float)), this, SLOT(cameraXChanged(float)));
	connect(ui->glViewport, SIGNAL(cameraYChanged(float)), this, SLOT(cameraYChanged(float)));
	connect(ui->glViewport, SIGNAL(cameraZChanged(float)), this, SLOT(cameraZChanged(float)));
	connect(ui->glViewport, SIGNAL(cameraPitchChanged(float)), this, SLOT(cameraPitchChanged(float)));
	connect(ui->glViewport, SIGNAL(cameraYawChanged(float)), this, SLOT(cameraYawChanged(float)));


	connect(ui->xSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateCameraX(double)));
	connect(ui->ySpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateCameraY(double)));
	connect(ui->zSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateCameraZ(double)));
	connect(ui->pitchSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateCameraPitch(double)));
	connect(ui->yawSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateCameraYaw(double)));
	connect(ui->movSpeedSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateCameraSpeed(double)));

	ui->statusBar->showMessage("Use ZQSD or arrows to move, C/space to go up/downward, hold shift for 10x speed, mouse is like official: right click + drag move");

	title = "Rappelz Map Viewer ~ Glandu2";
	setWindowTitle(title);

	if(!terrain->loadTexturesList("terraintextureinfo.cfg")) {
		QMessageBox::warning(0, title, "Failed to load ./terraintextureinfo.cfg\nMap will be invisible (no texture)");
	}
}

MainWindow::~MainWindow()
{
	delete terrain;
	delete ui;
}

void MainWindow::openglInitialized() {
	ui->glViewport->getRenderer()->addMesh(terrain);

	updateCameraX(ui->xSpinBox->value());
	updateCameraY(ui->ySpinBox->value());
	updateCameraZ(ui->zSpinBox->value());
	updateCameraPitch(ui->pitchSpinBox->value());
	updateCameraYaw(ui->yawSpinBox->value());
	updateCameraSpeed(ui->movSpeedSpinBox->value());
}

void MainWindow::openFile() {
	QString filename = QFileDialog::getOpenFileName(this, "Open map file (NFM)", QString(), "Rappelz Map (*.nfm);;All files (*)");
	if(!filename.isEmpty()) {
		QString currentCaption = windowTitle();
		QByteArray filename8bits = filename.toLocal8Bit();

		setWindowTitle(QString("Loading map %1 ...").arg(QString::fromLocal8Bit(filename8bits)));
		QApplication::processEvents();
		terrain->unloadFromGpu();
		bool ok = terrain->loadFromNfm(filename8bits.constData());
		terrain->loadToGpu();
		setWindowTitle(currentCaption);
		if(ok) {
			ui->statusBar->showMessage(QString("Loaded map %1").arg(QString::fromLocal8Bit(filename8bits)));
			setWindowTitle(title + QString(" - %1").arg(QFileInfo(QString::fromLocal8Bit(filename8bits)).fileName()));
		} else {
			QString message = QString("Failed to load %1").arg(QString::fromLocal8Bit(filename8bits));
			ui->statusBar->showMessage(message);
			QMessageBox::warning(this, title, message);
			setWindowTitle(title);
		}
	}
}



void MainWindow::cameraXChanged(float x) {
	ui->xSpinBox->setValue(x);
}

void MainWindow::cameraYChanged(float y) {
	ui->ySpinBox->setValue(y);
}

void MainWindow::cameraZChanged(float z) {
	ui->zSpinBox->setValue(z);
}

void MainWindow::cameraPitchChanged(float pitch) {
	ui->pitchSpinBox->setValue(pitch/M_PI*180);
}

void MainWindow::cameraYawChanged(float yaw) {
	ui->yawSpinBox->setValue(yaw/M_PI*180);
}


void MainWindow::updateCameraX(double x) {
	ui->glViewport->setCameraX(x);
}

void MainWindow::updateCameraY(double y) {
	ui->glViewport->setCameraY(y);
}

void MainWindow::updateCameraZ(double z) {
	ui->glViewport->setCameraZ(z);
}

void MainWindow::updateCameraPitch(double pitch) {
	ui->glViewport->setCameraPitch(pitch/180*M_PI);
}

void MainWindow::updateCameraYaw(double yaw) {
	ui->glViewport->setCameraYaw(yaw/180*M_PI);
}

void MainWindow::updateCameraSpeed(double modspd) {
	ui->glViewport->setSpeed(modspd/7/30*100);
}
