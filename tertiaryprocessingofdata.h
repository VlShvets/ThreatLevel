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

    /// Возврат указателя на словарь трасс
    QMap <int, Track>   getTracks(const QMap <int, Area> *_areas, const QMap <int, Etalon> *_etalons);

private:
    /// Определение попадания эталона в общую зону обнаружения всех ЗКВ
    bool    etalonIsDetected(const QMap <int, Area> *_areas, const Etalon *_etalon);

    /// Добавление трассы в словарь трасс по эталону
    void    addTrack(const int _num, const Etalon &_etalon);

    /// Обнуление рекурентных параметров
    void    resettingOfRecurrenceParameters();

    /// --------------------------------------------------
    /// Статические функции
    /// --------------------------------------------------

    /// Вычисление расстояния между двумя точками
    static float    calcDistance(const QPointF &_p1, const QPointF &_p2);

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Словарь параметров трасс
    QMap <int, Track>   tracks;     /// Словарь трасс   <номер, структура параметров>
};

}

#endif // TERTIARYPROCESSINGOFDATA_H
