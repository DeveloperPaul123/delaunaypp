#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

#include "demo/window.h"
#include "delaunaypp/delaunay.h"

/**
* Canvas class to draw points and triangles.
*/
Canvas::Canvas(QWidget *parent) : QWidget(parent) {
}

void Canvas::drawSegments(std::vector<EdgeType> edges) {
	for (const auto& edge : edges) {
		QPoint st(edge.start().x(), edge.start().y());
		QPoint e(edge.end().x(), edge.end().y());
		QLine l(st, e);
		lines.push_back(l);
	}
	update();
}

void Canvas::paintEvent(QPaintEvent *event) {
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(Qt::black, 2.0));
	for (auto l : lines) {
		painter.drawLine(l);
	}
	painter.setPen(QPen(Qt::blue, 2.0));
	for (auto p : points) {
		painter.drawEllipse({ p.x(), p.y() }, 3.5f, 3.5f);
		QString points("(%1, %2)");
		QPoint textPos(p.x(), p.y() - 10);
		painter.drawText(textPos, points.arg(QString::number(p.x()), 
			QString::number(p.y())));
	}
}

void Canvas::addPoints(std::vector<PointType> nPoints, bool append) {
	if (!append){
		points.clear();
		points = nPoints;
		update();
	}
	else {
		for (auto p : nPoints) {
			points.emplace_back(p);
		}
		update();
	}
}

std::vector <Canvas::PointType> Canvas::getPoints() const
{
	return points;
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		auto p = event->localPos();
		points.emplace_back(PointType{ p.x(), p.y() });
		update();
	}
	else if (event->button() == Qt::RightButton) {
		points.clear();
		lines.clear();
		update();
	}
}

Canvas::~Canvas() {
}

/**
* Main window of the application.
* @param parent the QWidget parent of the window.
*/
Window::Window(QWidget *parent) :
	QMainWindow(parent),
	open_action(new QAction("Open", this)),
	about_action(new QAction("About", this)),
	quit_action(new QAction("Quit", this)),
	triangulate_action(new QAction("Triangulate", this)),
	save_points_action(new QAction("Save Points", this))
{
	canvas = new Canvas(parent);
	setCentralWidget(canvas);

	QString title("Mesh-%1");
    setWindowTitle(title.arg("1.0.0"));
    setAcceptDrops(true);

    open_action->setShortcut(QKeySequence::Open);
    QObject::connect(open_action, &QAction::triggered,
                     this, &Window::onOpen);

    quit_action->setShortcut(QKeySequence::Quit);
    QObject::connect(quit_action, &QAction::triggered,
                     this, &Window::close);

    QObject::connect(about_action, &QAction::triggered,
                     this, &Window::onAbout);

	QObject::connect(triangulate_action, &QAction::triggered,
		this, &Window::onTriangulate);

	QObject::connect(save_points_action, &QAction::triggered,
		this, &Window::onSavePoints);

    auto file_menu = menuBar()->addMenu("File");
    file_menu->addAction(open_action);
    file_menu->addAction(quit_action);
	file_menu->addAction(triangulate_action);
	file_menu->addAction(save_points_action);
    auto help_menu = menuBar()->addMenu("Help");
    help_menu->addAction(about_action);

    resize(800, 550);
}

/**
* Slot for when File->Open is clicked. 
*/
void Window::onOpen()
{
	auto filename = QFileDialog::getOpenFileName(
                this, "Load point .txt file", QString(), "*.txt");

	auto points = loadPoints(filename);	
	canvas->addPoints(points, false);
}

std::vector<Window::PointType> Window::loadPoints(QString filename) const
{
	std::vector<PointType> points;
	QFile file(filename);
	file.open(QIODevice::ReadOnly);
	QTextStream stream(&file);
	while (!stream.atEnd()){
		auto line = stream.readLine();
		auto list = line.split('\t');
		if (list.length() > 0) {
			auto x = list.at(0).toDouble();
			auto y = list.at(1).toDouble();
			PointType p(x, y);
			points.push_back(p);
		}
	}
	file.close();
	return points;
}

void Window::onSavePoints() {
	auto points = canvas->getPoints();
	QFile output("points.txt");
	output.open(QIODevice::WriteOnly);
	QTextStream ts(&output);
	for (auto p : points) {
		ts << QString::number(p.x()) << '\t' << QString::number(p.y()) << endl;
	}
	output.close();
	QMessageBox::about(this, "Success", "Point file saved successfully");
}

/**
* Slot for when Help->About is clicked. 
*/
void Window::onAbout()
{
    QMessageBox::about(this, "",
        "<p align=\"center\"><b>fstl</b></p>"
        "<p>A fast mesh generator.<br>"
        "<p>© 2016 Paul Tsouchlos<br>");
}

void Window::onTriangulate() const
{
	//get points and triangulate and redraw.
	auto points = canvas->getPoints();
	delaunaypp::delaunay<PointType> delaunay(points);
	auto tris = delaunay.triangulate();
	std::vector<Canvas::EdgeType> edges;
	for(const auto& tri: tris)
	{
		const auto tri_edges = tri.edges();
		for(const auto& t_e: tri_edges)
		{
			edges.emplace_back(t_e);
		}
	}
	canvas->drawSegments(edges);
}

/**
* Enables the open action. 
*/
void Window::enableOpen() const
{
    open_action->setEnabled(true);
}

/**
* Disables the open action. 
*/
void Window::disableOpen() const
{
    open_action->setEnabled(false);
}
