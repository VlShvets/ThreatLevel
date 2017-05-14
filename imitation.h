#ifndef IMITATION_H
#define IMITATION_H

#include "area.h"
#include "etalon.h"
#include "parametersofareas.h"
#include "parametersofetalons.h"

#include <QMap>
#include <QtMath>

namespace ThreatLevel
{

/// Класс имитации
class Imitation
{
public:
    explicit                    Imitation(ParametersOfAreas *_parametersOfAreas, ParametersOfEtalons *_parametersOfEtalons);
    ~Imitation();

    /// Возвращение словаря ЗКВ
    inline QMap <int, Area>             *getAreas();

    /// Возвращение словаря эталонов
    inline const QMap <int, Etalon>     *getEtalons();

    /// Вычислительный процесс
    void    run(const float _currentTime);

private:
    /// Начальная инициализация параметров ЗКВ
    void    initializationOfParametersAreas();

    /// Обнуление рекурентных параметров ЗКВ
    void    resettingOfAreasRecurrenceParameters(Area &_area);

    /// Начальная инициализация параметров эталонов
    void    initializationOfParametersEtalons();

    /// --------------------------------------------------
    /// Статические функции
    /// --------------------------------------------------

    /// Равномерное распределение
    static float    uniformDistribution(const float _mean, const float _dev);

    /// Распределение Гаусса
    static float    gaussDistribution(const float _mean, const float _dev);

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Словарь параметров ЗКВ и эталонов
    QMap <int, Area>    areas;      /// Словарь ЗКВ         <номер, структура параметров>
    QMap <int, Etalon>  etalons;    /// Словарь эталонов    <номер, структура параметров>

    /// --------------------------------------------------
    /// Указатели на объекты классов
    /// --------------------------------------------------

    ParametersOfAreas       *parametersOfAreas;     /// Класс виджета редактирования параметров ЗКВ
    ParametersOfEtalons     *parametersOfEtalons;   /// Класс виджета редактирования параметров эталонов
};

/// Возвращение словаря ЗКВ
QMap <int, Area> *Imitation::getAreas()
{
    return &areas;
}

/// Возвращение словаря эталонов
const QMap <int, Etalon> *Imitation::getEtalons()
{
    return &etalons;
}

}

#endif // IMITATION_H
