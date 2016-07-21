#include "painter.h"

namespace ThreatLevel
{

Painter::Painter(AreaParameters *_areaParameters, TrackParameters *_trackParameters, Results *_results, QWidget *_parent)
    : Grapher2D(_parent), areaParameters(_areaParameters), trackParameters(_trackParameters), results(_results)
{
    setCSAbsMeasure(10000);
    setCSOrdMeasure(10000);
    setCSZoom(10);

    loadAreaPar();
    loadTrackPar();
    calcStaticPar();
}

Painter::~Painter()
{
    for(int i = 0; i < area.count(); ++i)
        area[i].target.clear();
    area.clear();
    track.clear();
}

void Painter::reStart()
{
    loadAreaPar();
    loadTrackPar();
    calcStaticPar();

    repaint();
}

void Painter::paintEvent(QPaintEvent * _pEvent)
{
    Grapher2D::paintEvent(_pEvent);

    QPainter p(this);
    p.translate(getCSAbsTranslate(), getCSOrdTranslate());
    p.scale(getCSAbsScale(), getCSOrdScale());

    QPen pen;
    pen.setCosmetic(true);

    /// Позиционные районы
    for(int i = 0; i < area.count(); ++i)
    {
        /// Отрисовка местоположения
        pen.setStyle(Qt::SolidLine);
        pen.setColor(Qt::darkBlue);
        pen.setWidth(3);
        p.setPen(pen);
        p.drawEllipse(area.at(i).pos, area.at(i).radius, area.at(i).radius);

        /// Отрисовка критического времени при минимальной скорости
        pen.setStyle(Qt::DashLine);
        pen.setColor(Qt::darkRed);
        pen.setWidth(1);
        p.setPen(pen);
        p.drawEllipse(area.at(i).pos, area.at(i).critTime * Track::minModV, area.at(i).critTime * Track::minModV);

        /// Отрисовка критического времени при максимальной скорости
        pen.setColor(Qt::darkGreen);
        p.setPen(pen);
        p.drawEllipse(area.at(i).pos, area.at(i).critTime * Track::maxModV, area.at(i).critTime * Track::maxModV);

        /// Отрисовка района обнаружения
        pen.setColor(Qt::darkBlue);
        p.setPen(pen);
        p.drawEllipse(area.at(i).pos, area.at(i).radarRange, area.at(i).radarRange);

        /// Отображение номера
        p.save();
        p.resetTransform();
        p.translate(width() / 2.0 + getCSAbsShift(), height() / 2.0 + getCSOrdShift());
        p.drawText(area.at(i).pos.x() * getCSAbsScale() - 20, area.at(i).pos.y() * getCSOrdScale() - 5,
                   QString::number(area.at(i).num + 1));
        p.restore();
    }

    /// Трассы
    pen.setColor(Qt::darkCyan);
    pen.setStyle(Qt::SolidLine);
    for(int j = 0; j < track.count(); ++j)
    {
        /// Отрисовка местоположения
        pen.setWidth(6);
        p.setPen(pen);
        p.drawPoint(track.at(j).pos);

        /// Отрисовка курса
        pen.setWidth(1);
        p.setPen(pen);
        if(!track.at(j).endPos.isNull())
            p.drawLine(track.at(j).startPos, track.at(j).endPos);

        /// Отображение номера
        p.save();
        p.resetTransform();
        p.translate(width() / 2.0 + getCSAbsShift(), height() / 2.0 + getCSOrdShift());
        p.drawText(track.at(j).pos.x() * getCSAbsScale() - 20, track.at(j).pos.y() * getCSOrdScale() - 5,
                   QString::number(track.at(j).num + 1));
        p.restore();
    }

    /// Отрисовка расстояний от трасс до центров ПР и углов видимости ПР
    for(int i = 0; i < area.count(); ++i)
    {
        for(int j = 0; j < track.count(); ++j)
        {
            /// Кратчайшее растояние до ПР
            pen.setColor(Qt::darkRed);
            p.setPen(pen);
//            p.drawLine(area.at(i).pos, track.at(area.at(i).target.at(j).num).pos);

            /// Угол видимости
            pen.setColor(Qt::darkBlue);
            p.setPen(pen);
            if(!area.at(i).target.at(j).p1.isNull())
                p.drawLine(track.at(area.at(i).target.at(j).num).pos, area.at(i).target.at(j).p1);
            if(!area.at(i).target.at(j).p2.isNull())
                p.drawLine(track.at(area.at(i).target.at(j).num).pos, area.at(i).target.at(j).p2);
        }
    }

    p.end();
}

void Painter::timerEvent(QTimerEvent *_tEvent)
{
    /// Перемещение цели
    /// ==================================================
    for(int j = 0; j < track.count(); ++j)
    {
        /// Точные значения
        track[j].modV += track.at(j).boost * DELTAT;
        track[j].pos += QPointF(track.at(j).modV * qSin(track.at(j).angV),
                                track.at(j).modV * qCos(track.at(j).angV)) * DELTAT;

        /// Внесение погрешностей
        track[j].errModV = track.at(j).modV + gaussDistribution(0, ERRMODV);
        track[j].errAngV = track.at(j).angV + qDegreesToRadians(uniformDistribution(0, ERRANGV));

        /// Сглаживание проекции вектора скорости на ось абсцисс
        if(track.at(j).errVx == 0.0)
            track[j].errVx = track.at(j).errModV * qSin(track.at(j).errAngV);
        else
            track[j].errVx = track.at(j).errVx                                  * WEIGHT +
                            (track.at(j).errModV * qSin(track.at(j).errAngV))   * (1.0 - WEIGHT);

        /// Сглаживание проекции вектора скорости на ось ординат
        if(track.at(j).errVy == 0.0)
            track[j].errVy = track.at(j).errModV * qCos(track.at(j).errAngV);
        else
            track[j].errVy = track.at(j).errVy                                  * WEIGHT +
                            (track.at(j).errModV * qCos(track.at(j).errAngV))   * (1.0 - WEIGHT);

        /// Возврат к модулю скорости и курсу
        track[j].errModV = qSqrt(track.at(j).errVx * track.at(j).errVx + track.at(j).errVy * track.at(j).errVy);
        track[j].errAngV = qAtan2(track.at(j).errVx, track.at(j).errVy);
    }
    /// ==================================================

    /// Расчет времени поражения ПР
    /// ==================================================
    for(int i = 0; i < area.count(); ++i)
    {
        for(int j = 0; j < track.count(); ++j)
        {
            /// Определение номера рассматриваемой трассы
            int num = area.at(i).target.at(j).num;

            /// Вычисление точек касания угла видимости
//            calcTanPoints(area.at(i).pos, track.at(num).pos, area.at(i).radius, area[i].target[j].p1, area[i].target[j].p2);

            /// Расчет точных значений переменных
            /// ------------------------------------------

            /// Вычисление текущего расстояния от центра ПР до проекции цели
            area[i].target[j].dist = area.at(i).target.at(j).startDist -
                                     calcDistance(track.at(num).startPos, track.at(num).pos) * qCos(area.at(i).target.at(j).angToV);

            /// Расчет времени поражения ПР
            area[i].target[j].time = area.at(i).target.at(j).dist /
                                    (track.at(num).modV * qCos(area.at(i).target.at(j).angToV));
            /// ------------------------------------------

            /// Расчет значений переменных с погрешностями
            /// ------------------------------------------

            /// Определение угла между прямой от центра ПР до цели и вектором скорости цели с погрешностью
            area[i].target[j].errAngToV = track.at(num).errAngV - qAtan2(area.at(i).pos.x() - track.at(num).startPos.x(),
                                                                         area.at(i).pos.y() - track.at(num).startPos.y());

            /// Вычисление текущего расстояния от центра ПР до проекции цели с погрешностью
            area[i].target[j].errDist = area.at(i).target.at(j).startDist -
                                        calcDistance(track.at(num).startPos, track.at(num).pos) * qCos(area.at(i).target.at(j).errAngToV);

            /// Расчет времени поражения ПР с погрешностью
            area[i].target[j].errTime = area.at(i).target.at(j).errDist /
                                        (track.at(num).errModV * qCos(area.at(i).target.at(j).errAngToV));
            /// ------------------------------------------

            /// Инкрементация количества измерений
            ++area[i].target[j].countMeasure;
        }
    }
    /// ==================================================

    /// Вычисление параметров для отрисовки курса
    /// ==================================================
    for(int j = 0; j < track.count(); ++j)
    {
        /// Определение номера рассматриваемой трассы по первому ПР
        int num = area.at(0).target.at(j).num;

        /// Поиск ближайшего по времени ПР и определение номера рассматриваемой трассы в списке целей данного ПР
        int nNearArea = 0;      /// Ближайший по времени с погрешностью ПР
        int nNearTarget = j;    /// Номер трассы в списке целей данного ПР
        for(int i = 1; i < area.count(); ++i)
        {
            /// Определение трассы по номеру в спике целей рассматриваемого ПР
            int numTarget = -1;
            for(int j = 0; j < track.count(); ++j)
            {
                if(area.at(i).target.at(j).num == num)
                    numTarget = j;
            }

            if(area.at(nNearArea).target.at(nNearTarget).errTime > area.at(i).target.at(numTarget).errTime)
            {
                nNearArea = i;
                nNearTarget = numTarget;
            }
        }

        /// Вычисление координат экстраполированного конца траектории
        track[num].endPos = calcEndPos(track.at(num).startPos, area.at(nNearArea).target.at(nNearTarget).startDist,
                                       area.at(nNearArea).target.at(nNearTarget).errAngToV, track.at(num).errModV,
                                       area.at(nNearArea).critTime, track.at(num).errAngV);

        /// Условие окончания
        if(area.at(nNearArea).target.at(nNearTarget).errTime < area.at(nNearArea).critTime && idTimer != -1)
        {
            killTimer(_tEvent->timerId());
            idTimer = -1;
        }
    }
    /// ==================================================

    /// Погрешность времени поражения ПР
    /// ==================================================
    for(int i = 0; i < area.count(); ++i)
    {
        /// Сортировка целей по времени поражения ПР с погрешностью
        quickSortTargets(i, 0, area.at(i).target.count() - 1);

        /// Вычисление погрешности времени поражения в текущий момент времени
        area[i].diffTime = area.at(i).target.at(0).errTime - area.at(i).target.at(0).time;

        /// Вычисление суммы квадратов погрешностей времени поражения
        area[i].sumDiffTime += area.at(i).diffTime * area.at(i).diffTime;

        /// Вычисление среднеквадратической погрешности времени поражения
        area[i].sigmaT = qSqrt(area.at(i).sumDiffTime / area.at(i).target.at(0).countMeasure);
    }

    /// Отображение результатов вычислений
    results->loadTable(area);
    /// ==================================================

    repaint();
}

void Painter::loadAreaPar()
{
    area.resize(areaParameters->getCount());

    for(int i = 0; i < area.count(); ++i)
    {
        /// Присвоение позиционному району номера
        area[i].num = i;

        /// Загрузка исходных данных из таблицы
        area[i].pos.setX(areaParameters->getPar(i, 0));
        area[i].pos.setY(areaParameters->getPar(i, 1));
        area[i].radius = areaParameters->getPar(i, 2);
        area[i].critTime = areaParameters->getPar(i, 3);
        area[i].radarRange = areaParameters->getPar(i, 4);

        /// Обнуление рекурентно использующейся переменной
        /// при подсчете среднеквадратической погрешности времени поражения
        area[i].sumDiffTime = 0.0;
    }
}

void Painter::loadTrackPar()
{
    track.resize(trackParameters->getCount());

    for(int j = 0; j < track.count(); ++j)
    {
        /// Присовение трассе номера
        track[j].num = j;

        /// Загрузка исходных данных из таблицы
        track[j].startPos.setX(trackParameters->getPar(j, 0));
        track[j].startPos.setY(trackParameters->getPar(j, 1));
        track[j].modV = trackParameters->getPar(j, 2);
        track[j].angV = qDegreesToRadians(trackParameters->getPar(j, 3));
        track[j].boost = trackParameters->getPar(j, 4);

        /// Присвоение текущим координатам начальных координат трассы
        track[j].pos = track.at(j).startPos;

        /// Обнуление координат экстраполированного конца траектории
        track[j].endPos = QPointF();

        /// Обнуление рекурентно использующихся при сглаживании переменных
        track[j].errVx = 0.0;
        track[j].errVy = 0.0;
    }
}

void Painter::calcStaticPar()
{
    for(int i = 0; i < area.count(); ++i)
    {
        area[i].target.resize(track.count());

        for(int j = 0; j < track.count(); ++j)
        {
            /// Присвоение цели номера
            area[i].target[j].num = j;

            /// Обнуление количества измерений
            area[i].target[j].countMeasure = 0;

            /// Определение расстояния от центра ПР до начальной точки трассы
            area[i].target[j].startDist = calcDistance(area.at(i).pos, track.at(j).startPos);

            /// Определение угла между прямой от центра ПР до трассы и вектором скорости трассы
            area[i].target[j].angToV = track.at(j).angV - qAtan2(area.at(i).pos.x() - track.at(j).startPos.x(),
                                                                 area.at(i).pos.y() - track.at(j).startPos.y());
        }
    }
}

void Painter::quickSortTargets(const int _numArea, const int _first, const int _last)
{
    int i = _first;
    int j = _last;
    float x = area.at(_numArea).target.at((_first + _last) / 2).errTime;

    do
    {
        while(area.at(_numArea).target.at(i).errTime < x)   i++;
        while(area.at(_numArea).target.at(j).errTime > x)   j--;

        if(i <= j)
        {
            if(area.at(_numArea).target.at(i).errTime > area.at(_numArea).target.at(j).errTime)
                swapTargets(_numArea, i, j);
            i++;
            j--;
        }
    }
    while(i <= j);

    if(i < _last)
        quickSortTargets(_numArea, i, _last);
    if(_first < j)
        quickSortTargets(_numArea, _first, j);
}

void Painter::swapTargets(const int _numArea, const int _numTarget1, const int _numTarget2)
{
    Area::Target tmpTarget = area[_numArea].target.at(_numTarget1);
    area[_numArea].target[_numTarget1] = area.at(_numArea).target.at(_numTarget2);
    area[_numArea].target[_numTarget2] = tmpTarget;
}

float Painter::calcDistance(const QPointF &_p1, const QPointF &_p2)
{
    return qSqrt((_p1.x() - _p2.x()) * (_p1.x() - _p2.x()) + (_p1.y() - _p2.y()) * (_p1.y() - _p2.y()));
}

const QPointF Painter::calcEndPos(const QPointF &_startPos, const float _startDist, const float _errAngToV,
                                  const float _errModV, const float _critTime, const float _errAngV)
{
    return _startPos + QPointF((_startDist / qCos(_errAngToV) - _errModV * _critTime) * qSin(_errAngV),
                               (_startDist / qCos(_errAngToV) - _errModV * _critTime) * qCos(_errAngV));
}

void Painter::calcTanPoints(const QPointF &_area, const QPointF &_track, const float _radius, QPointF &_p1, QPointF &_p2)
{
    float distance = calcDistance(_area, _track) - _radius;
    if(distance < 0)
    {
        _p1 = QPointF();
        _p2 = QPointF();

        return;
    }

    QPointF midDistTargetBase((_track - _area) / 2.0);

    float a = midDistTargetBase.x() * midDistTargetBase.x() +
            midDistTargetBase.y() * midDistTargetBase.y();
    float E = 0.5 * (_radius * _radius - 0.25 * (distance + _radius) * (distance + _radius) + a);

    if(qAbs(midDistTargetBase.x()) < 1.e-6)
    {
        _p1.setX(qSqrt(_radius * _radius - E * E / (midDistTargetBase.y() * midDistTargetBase.y())));
        _p1.setY(E / midDistTargetBase.y());

        _p2.setX(- qSqrt(_radius * _radius - E * E / (midDistTargetBase.y() * midDistTargetBase.y())));
        _p2.setY(E / midDistTargetBase.y());
    }
    else
    {
        float b = - E * midDistTargetBase.y();
        float c = E * E - (_radius * _radius * midDistTargetBase.x() * midDistTargetBase.x());
        float D = b * b - a * c;

        _p1.setY((- b + qSqrt(D)) / a);
        _p2.setY((- b - qSqrt(D)) / a);

        _p1.setX((E - _p1.y() * midDistTargetBase.y()) / midDistTargetBase.x());
        _p2.setX((E - _p2.y() * midDistTargetBase.y()) / midDistTargetBase.x());
    }

    _p1 += _area;
    _p2 += _area;
}

float Painter::uniformDistribution(const float _mean, const float _dev)
{
    return 2.0 * _dev * ((float) qrand() / RAND_MAX) - _dev + _mean;
}

float Painter::gaussDistribution(const float _mean, const float _dev)
{
    static bool ready = false;
    static float second = 0.0;

    if(ready)
    {
        ready = false;
        return second * _dev + _mean;
    }

    float u, v, s;
    do
    {
        u = 2.0 * ((float) qrand() / RAND_MAX) - 1.0;
        v = 2.0 * ((float) qrand() / RAND_MAX) - 1.0;
        s = u * u + v * v;
    }
    while(s > 1.0 || s == 0.0);

    float r = qSqrt(-2.0 * qLn(s) / s);
    second = r * u;

    if(qAbs(second) < 1.5)
        ready = true;

    if(qAbs(r * v) < 1.5)
        return r * v * _dev + _mean;

    return gaussDistribution(_mean, _dev);
}

}
