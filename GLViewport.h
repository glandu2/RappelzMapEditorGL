#ifndef GLVIEWPORT_H
#define GLVIEWPORT_H

#include <GL/glew.h>
#include <QGLWidget>

class GLViewport : public QGLWidget
{
	Q_OBJECT

public:
	explicit GLViewport(QWidget *parent = 0);
	~GLViewport();

	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();
	virtual void keyPressEvent(QKeyEvent *keyEvent);

public slots:
	virtual void updateGL();

protected:
	GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);

private:
	QTimer *updateTimer;
	GLuint VertexArrayID;
	GLuint vertexbuffer;
};

#endif // GLVIEWPORT_H
