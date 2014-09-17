#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class Terrain;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void openFile();
	void openglInitialized();

	void cameraXChanged(float x);
	void cameraYChanged(float y);
	void cameraZChanged(float z);
	void cameraPitchChanged(float pitch);
	void cameraYawChanged(float yaw);

	void updateCameraX(double x);
	void updateCameraY(double y);
	void updateCameraZ(double z);
	void updateCameraPitch(double pitch);
	void updateCameraYaw(double yaw);

	void updateCameraSpeed(double modspd);

private:
	Ui::MainWindow *ui;
	Terrain* terrain;
	QString title;
};

#endif // MAINWINDOW_H
