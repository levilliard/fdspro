
/*-----------------------------------------
 * @levilliard
 * FDS SISMO
 * 2016
 *-----------------------------------------
 */

#include "plotter.h"
#include <QToolButton>
#include <QStylePainter>
#include <QMouseEvent>
#include <QStyleOptionFocusRect>
#include <QPushButton>
#include <QLabel>
#include "stationconfig.h"

#include "../../global/global.h"

Plotter::Plotter(QWidget *parent, std::string data_file): QWidget(parent)
{
    TIME_INTERVAL = 1;
    MAX_INTERVAL = 1;
    file_name = data_file;

    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
    rubberBandIsShown = false;

    zoomInButton = new QToolButton(this);
    zoomInButton->setIcon(QIcon(":/img/zoomin.png"));
    zoomInButton->adjustSize();
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));

    zoomOutButton = new QToolButton(this);
    zoomOutButton->setIcon(QIcon(":/img/zoomout.png"));
    zoomOutButton->adjustSize();
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));

    QLabel *net_label = new QLabel("Networt:", this);
    net_label->setStyleSheet("color: white;");
    QLabel *station_label = new QLabel("Station ID:", this);
    station_label->setStyleSheet("color: #fff;");
    QLabel *z_label = new QLabel("Z[12, 57]:", this);
    z_label->setStyleSheet("color: white;");
    QLabel *n_label = new QLabel("Z[72, 57]:", this);
    n_label->setStyleSheet("color: white;");
    QLabel *s_label = new QLabel("Z[82, 77]:", this);
    s_label->setStyleSheet("color: white;");

    net_label->move(10, 10);
    station_label->move(110, 10);
    z_label->move(210, 10);
    n_label->move(310, 10);
    s_label->move(410, 10);

    setPlotSettings(PlotSettings());
    this->resize(700, 600);
    mTimer = new QTimer(this);
    QObject::connect(mTimer, SIGNAL(timeout()), this, SLOT(timeOut()));
    mTimer->start(100);
}

void Plotter::setPlotSettings(const PlotSettings &settings)
{
    zoomStack.clear();
    zoomStack.append(settings);
    curZoom = 0;
    zoomStack[curZoom].scroll(0, -3);
    zoomInButton->hide();
    zoomOutButton->hide();
    refreshPixmap();
}

void Plotter::zoomOut()
{
    if (curZoom > 0){
        --curZoom;
        zoomOutButton->setEnabled(curZoom > 0);
        zoomInButton->setEnabled(true);
        zoomInButton->show();
        refreshPixmap();
    }
}

void Plotter::zoomIn()
{
    if (curZoom < zoomStack.count() - 1) {
         ++curZoom;

        zoomInButton->setEnabled(curZoom < zoomStack.count() - 1);
        zoomOutButton->setEnabled(true);
        zoomOutButton->show();
        refreshPixmap();
    }
}

void Plotter::setCurveData(int id, const QVector<QPointF> &data)
{
    curveMap[id] = data;
    MAX_INTERVAL = curveMap[id].size();
    refreshPixmap();
}

void Plotter::clearCurve(int id)
{
    curveMap.remove(id);
    refreshPixmap();
}

QSize Plotter::minimumSizeHint() const
{
    return QSize(6 * Margin, 4 * Margin);
}

QSize Plotter::sizeHint() const
{
    return QSize(12 * Margin, 8 * Margin);
}

void Plotter::paintEvent(QPaintEvent * /* event */)
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, pixmap);

    if (rubberBandIsShown){
        painter.setPen(palette().light().color());
        painter.drawRect(rubberBandRect.normalized().adjusted(0, 0, -1, -1));
    }

    if (hasFocus()){
          QStyleOptionFocusRect option;
          option.initFrom(this);
          option.backgroundColor = palette().dark().color();
          painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
     }
}

void Plotter::resizeEvent(QResizeEvent * /* event */)
{
    int x = width() - (zoomInButton->width() + zoomOutButton->width() + 10);
    zoomInButton->move(x, 5);
    zoomOutButton->move(x + zoomInButton->width() + 5, 5);
    refreshPixmap();
}

void Plotter::mousePressEvent(QMouseEvent *event)
{
    QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);

    if (event->button() == Qt::LeftButton) {
        if (rect.contains(event->pos())) {
            rubberBandIsShown = true;
            rubberBandRect.setTopLeft(event->pos());
            rubberBandRect.setBottomRight(event->pos());
            updateRubberBandRegion();
            setCursor(Qt::CrossCursor);
        }
    }
}

void Plotter::mouseMoveEvent(QMouseEvent *event)
{
    if (rubberBandIsShown) {
        updateRubberBandRegion();
        rubberBandRect.setBottomRight(event->pos());
        updateRubberBandRegion();
    }
}

void Plotter::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton) && rubberBandIsShown) {
        rubberBandIsShown = false;
        updateRubberBandRegion();
        unsetCursor();

        QRect rect = rubberBandRect.normalized();

        if (rect.width() < 4 || rect.height() < 4)
            return;

        rect.translate(-Margin, -Margin);
        PlotSettings prevSettings = zoomStack[curZoom];
        PlotSettings settings;
        double dx = prevSettings.spanX() / (width() - 2 * Margin);
        double dy = prevSettings.spanY() / (height() - 2 * Margin);
        settings.minX = prevSettings.minX + dx * rect.left();
        settings.maxX = prevSettings.minX + dx * rect.right();
        settings.minY = prevSettings.maxY - dy * rect.bottom();
        settings.maxY = prevSettings.maxY - dy * rect.top();
        settings.adjust();
        zoomStack.resize(curZoom + 1);
        zoomStack.append(settings);
        zoomIn();
    }
}

void Plotter::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Plus:
        zoomIn();
        break;

        case Qt::Key_Minus:
        zoomOut();
        break;


        case Qt::Key_Left:
        zoomStack[curZoom].scroll(-1, 0);
        refreshPixmap();
        break;

        case Qt::Key_Right:
        zoomStack[curZoom].scroll(+1, 0);
        refreshPixmap();
        break;

        case Qt::Key_Down:
        zoomStack[curZoom].scroll(0, -1);
       refreshPixmap();
       break;

       case Qt::Key_Up:
       zoomStack[curZoom].scroll(0, +1);
       refreshPixmap();
       break;

       default:
       QWidget::keyPressEvent(event);
   }
}

void Plotter::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numTicks = numDegrees / 15;
    if (event->orientation() == Qt::Horizontal) {
        zoomStack[curZoom].scroll(numTicks, 0);
    } else {
        zoomStack[curZoom].scroll(0, numTicks);
    }

    refreshPixmap();
}

void Plotter::updateRubberBandRegion()
{
    QRect rect = rubberBandRect.normalized();
    update(rect.left(), rect.top(), rect.width(), 1);
    update(rect.left(), rect.top(), 1, rect.height());
    update(rect.left(), rect.bottom(), rect.width(), 1);
    update(rect.right(), rect.top(), 1, rect.height());
}

void Plotter::refreshPixmap()
{
    pixmap = QPixmap(size());
    pixmap.fill(Qt::black);
    QPainter painter(&pixmap);
    painter.initFrom(this);
    drawGrid(&painter);
    drawCurves(&painter);
    update();
}

void Plotter::timeOut(){
    TIME_INTERVAL++;

    if(TIME_INTERVAL == MAX_INTERVAL -1)
        mTimer->disconnect(mTimer, SIGNAL(timeout()), this, SLOT(timeOut()));
    refreshPixmap();
}

void Plotter::drawGrid(QPainter *painter)
{
    QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);

    if (!rect.isValid())
        return;

    PlotSettings settings = zoomStack[curZoom];
    QPen quiteDark = palette().dark().color().light();
    QPen light = palette().light().color();

    for (int i = 0; i <= settings.numXTicks; ++i){
        int x = rect.left() + (i * (rect.width() - 1) / settings.numXTicks);
        double label = settings.minX + (i * settings.spanX() / settings.numXTicks);
        painter->setPen(quiteDark);
        painter->drawLine(x, rect.top(), x, rect.bottom());
        painter->setPen(light);
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);

        if(i%2 == 0){
            QFont font;
            font.setPixelSize(8);
            /* set the modified font to the painter */
            painter->setFont(font);
            painter->drawText(x - 50, rect.bottom() + 5, 100, 20, Qt::AlignHCenter | Qt::AlignTop, getDataTime((int)label));
        }
    }

    for (int j = 0; j <= settings.numYTicks; ++j) {
        int y = rect.bottom() - (j * (rect.height() - 1) / settings.numYTicks);
        // double label = settings.minY + (j * settings.spanY() / settings.numYTicks);
        painter->setPen(quiteDark);
        painter->drawLine(rect.left(), y, rect.right(), y);
        painter->setPen(light);
        painter->drawLine(rect.left(), y, rect.left(), y);
     }

    painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

void Plotter::drawCurves(QPainter *painter)
{
    static const QColor colorForIds[6] = {
        Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow
     };

    PlotSettings settings = zoomStack[curZoom];
    QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);

    if (!rect.isValid())
        return;

    painter->setClipRect(rect.adjusted(+1, +1, -1, -1));
    QMapIterator<int, QVector<QPointF> > i(curveMap);
    ///int counter = 0;
    int temp[] = {-2, 0, 2};

    while (i.hasNext()){
        i.next();
        int id = i.key();

        QVector<QPointF> data;
        for(int k = 0; k < TIME_INTERVAL; ++k)
            data.push_back(i.value()[k]);

        QPolygonF polyline(data.count());

        for (int j = 0; j < TIME_INTERVAL; ++j) {
            double dx = data[j].x() - settings.minX;
            double dy = data[j].y() - settings.minY;
            double x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
            double y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY()) + (temp[id] - 1)*2.8*Margin;
            polyline[j] = QPointF(x/4, y);
        }

        painter->setPen(colorForIds[uint(id) % 6]);
        painter->drawPolyline(polyline);
        painter->setPen(Qt::white);

    }
}

void Plotter::setMetaData(std::string &mdata){

}

QString Plotter::getDataTime(int second){

    //[0] get seismic waves data
    //[0]

    StationConfig scfg;
    std::vector<float> data = scfg.getStationData(file_name);
    DateTime dt;
    std::vector<std::string> meta_data = GLOBAL_FUNCTION::split(scfg.getMetaData(), ',');
    std::string mdtime = GLOBAL_FUNCTION::trim(meta_data[3]);  //2010-02-27T06:50:00.069539
    //std::cout<<" dt hr = "<<mdtime<<"\n";

    dt.year = std::stoi(mdtime.substr(0, 4));
    dt.month = std::stoi(mdtime.substr(5, 2));
    dt.day = std::stoi(mdtime.substr(8, 2));

    dt.hour = std::stoi(mdtime.substr(11, 2));
    dt.minute = std::stoi(mdtime.substr(14, 2));
    dt.second = std::stoi(mdtime.substr(17, 2));

    dt.second += second;

    if(dt.second >= 60){
        int temp = dt.second/60;
        dt.second = dt.second % 60;
        dt.minute += temp;
    }

    if(dt.minute >= 60){
        int temp = dt.minute / 60;
        dt.minute = dt.minute % 60;
        dt.hour += temp;
    }

    if(dt.hour >= 24){
        int temp = dt.hour / 24;
        dt.hour = dt.hour % 24;
        dt.day += temp;
    }

    return QString::number(dt.year) + "-" + QString::number(dt.month).rightJustified(2, '0') + "-" + QString::number(dt.day).rightJustified(2, '0') + " " + QString::number(dt.hour).rightJustified(2, '0') + ":" + QString::number(dt.minute).rightJustified(2, '0') + ":" + QString::number(dt.second).rightJustified(2, '0');
}
