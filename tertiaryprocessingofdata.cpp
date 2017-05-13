#include "tertiaryprocessingofdata.h"

namespace ThreatLevel
{

/// Класс третичной обработки данных
TertiaryProcessingOfData::TertiaryProcessingOfData()
{
}

TertiaryProcessingOfData::~TertiaryProcessingOfData()
{
    tracks.clear();
}

/// Возврат указателя на словарь трасс
QMap <int, Track> TertiaryProcessingOfData::getTracks(const QMap <int, Area> *_areas, const QMap <int, Etalon> *_etalons)
{
    QMap <int, Etalon>::const_iterator etalon = _etalons->begin();
    for(; etalon != _etalons->end(); ++etalon)
    {
        if(etalonIsDetected(_areas, &etalon.value()))
            addTrack(etalon.key(), etalon.value());
    }

    /// Обнуление рекурентных параметров
    resettingOfRecurrenceParameters();

    return tracks;
}

/// Определение попадания эталона в общую зону обнаружения всех ЗКВ
bool TertiaryProcessingOfData::etalonIsDetected(const QMap <int, Area> *_areas, const Etalon *_etalon)
{
    QMap <int, Area>::const_iterator area = _areas->begin();
    for(; area != _areas->end(); ++area)
    {
        if(calcDistance(area.value().initPos, _etalon->exactPos) < area.value().initDetectRange)
            return true;
    }

    return false;
}

/// Добавление трассы в словарь трасс по эталону
void TertiaryProcessingOfData::addTrack(const int _num, const Etalon &_etalon)
{
    /// Априорные параметры
    tracks[_num].initIsBG        = _etalon.initIsBG;
    tracks[_num].initFinalPos    = _etalon.initFinalPos;
    tracks[_num].initQuant       = _etalon.initQuant;

    /// Точные значения текущих параметров
    tracks[_num].exactPos        = _etalon.exactPos;

    /// Параметры измерения с погрешностью
    tracks[_num].measSpeed       = _etalon.measSpeed;
    tracks[_num].measCourse      = _etalon.measCourse;
}

/// Обнуление рекурентных параметров
void TertiaryProcessingOfData::resettingOfRecurrenceParameters()
{
    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
    {
        /// Обнуление координат начальной точки и экстраполированного конца траектории
        track.value().startPos      = QPointF();
        track.value().finalPos      = QPointF();

        /// Обнуление количества измерений
        track.value().countMeas     = 0;

        /// Обнуление рекурентно использующихся при сглаживании проекций вектора скорости
        track.value().smoothVx      = 0.0;
        track.value().smoothVy      = 0.0;

        /// Очистка номера идентифицированного ПР
        track.value().numArea       = -1;

        /// Обнуление суммы квадратов разности вычисленного и измеренного времени поражения ПР
        track.value().sumDiffTime   = 0.0;

        /// Обнуление среднеквадратической разности вычисленного и измеренного времени поражения ПР
        track.value().rmsDiffTime   = 0.0;
    }
}

/// --------------------------------------------------
/// Статические функции
/// ---------------------------------------------------

/// Вычисление расстояния между двумя точками
float TertiaryProcessingOfData::calcDistance(const QPointF &_p1, const QPointF &_p2)
{
    return qSqrt((_p1.x() - _p2.x()) * (_p1.x() - _p2.x()) +
                 (_p1.y() - _p2.y()) * (_p1.y() - _p2.y()));
}

}
