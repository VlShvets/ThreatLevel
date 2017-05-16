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
void TertiaryProcessingOfData::run()
{
    QMap <int, Etalon>::const_iterator etalon = etalons->constBegin();
    for(; etalon != etalons->end(); ++etalon)
    {
        /// Трасса обнаружена
        if(etalonIsDetected(etalon.value()))
        {
            /// Трасса не содержится в словаре трасс
            if(!tracks.contains(etalon.key()))
            {
                /// Добавление трассы в словарь трасс по эталону
                addTrack(etalon.key(), etalon.value());
            }

            /// Обновление информации о трассе
            updateTrack(etalon.key(), etalon.value());
        }
        /// Трасса не обнаружена, но содержится в словаре трасс
        else if(tracks.contains(etalon.key()))
        {
            /// Удаление трассы из словаря по ключу эталона
            tracks.remove(etalon.key());
        }
    }
}

/// Определение попадания эталона в общую зону обнаружения всех ЗКВ
bool TertiaryProcessingOfData::etalonIsDetected(const Etalon &_etalon)
{
    QPointF     pArea;      /// Координаты ЗКВ
    QPointF     pEtalon;    /// Текущие коодинаты эталона

    QMap <int, Area>::const_iterator area = areas->constBegin();
    for(; area != areas->end(); ++area)
    {
        pArea       = area.value().initPos;
        pEtalon     = _etalon.exactPos;
        if(qSqrt((pArea.x() - pEtalon.x()) * (pArea.x() - pEtalon.x()) +
                 (pArea.y() - pEtalon.y()) * (pArea.y() - pEtalon.y())) <
           area.value().initDetectRange)
        {
            return true;
        }
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

    /// Координаты начальной точки траектории
    tracks[_num].startPos = _etalon.exactPos;

    /// Обнуление рекурентных параметров
    zeroingOfRecurrenceParameters(tracks[_num]);
}

/// Обнуление рекурентных параметров
void TertiaryProcessingOfData::zeroingOfRecurrenceParameters(Track &_track)
{
    /// Обнуление координат экстраполированного конца траектории
    _track.finalPos      = QPointF();

    /// Обнуление количества измерений
    _track.countMeas     = 0;

    /// Обнуление рекурентно использующихся при сглаживании проекций вектора скорости
    _track.smoothVx      = 0.0;
    _track.smoothVy      = 0.0;

    /// Очистка номера идентифицированного ПР
    _track.numArea       = -1;

    /// Обнуление суммы квадратов разности вычисленного и измеренного времени поражения ПР
    _track.sumDiffTime   = 0.0;

    /// Обнуление среднеквадратической разности вычисленного и измеренного времени поражения ПР
    _track.rmsDiffTime   = 0.0;
}

/// Обновление информации о трассе
void TertiaryProcessingOfData::updateTrack(const int _num, const Etalon &_etalon)
{
    /// Точные значения текущих параметров
    tracks[_num].exactPos        = _etalon.exactPos;

    /// Параметры измерения с погрешностью
    tracks[_num].measSpeed       = _etalon.measSpeed;
    tracks[_num].measCourse      = _etalon.measCourse;
}

}
