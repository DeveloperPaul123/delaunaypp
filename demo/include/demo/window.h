#pragma once
#include <QMainWindow>
#include <QEvent>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>

#include <delaunaypp/edge.h>
#include <delaunaypp/point.h>

#include <vector>

class Canvas : public QWidget {
	Q_OBJECT 
public:
	using PointType = delaunaypp::point<double>;
	using EdgeType = delaunaypp::internal::edge<PointType, double>;

	explicit Canvas(QWidget *parent);
	~Canvas();
	std::vector<PointType> getPoints() const;
	void addPoints(std::vector<PointType> points, bool append);
	void drawSegments(std::vector<EdgeType> segments);

protected:
	void mouseReleaseEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	
private:
	std::vector<PointType> points;
	std::vector<QLineF> lines;
};

class Window : public QMainWindow
{
    Q_OBJECT
public:
	using PointType = Canvas::PointType;
    explicit Window(QWidget* parent=0);

public slots:
    void onOpen();
    void onAbout();
	void onTriangulate() const;
    void enableOpen() const;
    void disableOpen() const;
	void onSavePoints();

private:
    QAction* const open_action;
    QAction* const about_action;
	QAction* const quit_action;
	QAction* const triangulate_action;
	QAction* const save_points_action;
	Canvas *canvas;
	std::vector<PointType> loadPoints(QString filename) const;
};
