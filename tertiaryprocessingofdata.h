#ifndef TERTIARYPROCESSINGOFDATA_H
#define TERTIARYPROCESSINGOFDATA_H

#include "area.h"
#include "etalon.h"
#include "track.h"

#include <QMap>
#include <QtMath>

namespace ThreatLevel
{

/// Класс третичной обработки данных
class TertiaryProcessingOfData
{
public:
    explicit            TertiaryProcessingOfData();
    ~TertiaryProcessingOfData();

    /// Установление ЗКВ
    inline void                 setAreas(const QMap <int, Area> &_areas);

    /// Установление эталонов
    inline void                 setEtalons(const QMap <int, Etalon> &_etalons);

    /// Возврат указателя на словарь трасс
    inline QMap <int, Track>    *getTracks();

    /// Вычислительный процесс
    void    run();

private:
    /// Определение попадания эталона в общую зону обнаружения всех ЗКВ
    bool    etalonIsDetected(const QMap <int, Area> &_areas, const Etalon &_etalon);

    /// Добавление трассы в словарь трасс по эталону
    void    addTrack(const int _num, const Etalon &_etalon);

    /// Обнуление рекурентных параметров
    void    resettingOfRecurrenceParameters(Track &_track);

    /// Обновление информации о трассе
    void    updateTrack(const int _num, const Etalon &_etalon);

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Указатели на словари параметров ЗКВ и эталонов
    const QMap <int, Area>      *areas;     /// Словарь ЗКВ         <номер, структура параметров>
    const QMap <int, Etalon>    *etalons;   /// Словарь эталонов    <номер, структура параметров>

    /// Словарь параметров трасс
    QMap <int, Track>           tracks;     /// Словарь трасс   <номер, структура параметров>
};

/// Установление ЗКВ
void TertiaryProcessingOfData::setAreas(const QMap <int, Area> &_areas)
{
    areas = &_areas;
}

/// Установление эталонов
void TertiaryProcessingOfData::setEtalons(const QMap <int, Etalon> &_etalons)
{
    etalons = &_etalons;
}

/// Возврат указателя на словарь трасс
QMap <int, Track> *TertiaryProcessingOfData::getTracks()
{
    return &tracks;
}

}

#endif // TERTIARYPROCESSINGOFDATA_H
