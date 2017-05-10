#ifndef IMITATION_H
#define IMITATION_H

#include "etalon.h"
#include "parametersofetalons.h"

#include <QMap>

namespace ThreatLevel
{

/// Класс имитации
class Imitation
{
public:
    explicit Imitation();
    ~Imitation();

    /// Начальная инициализация параметров эталонов
    void initialization();

    /// Возврат словаря соответствующих заданному времени эталонов
    QMap <int, Etalon>  getEtalons(const float _currentTime);

private:
    /// Равномерное распределение
    static float            uniformDistribution(const float _mean, const float _dev);

    /// Распределение Гаусса
    static float            gaussDistribution(const float _mean, const float _dev);

    QMap <int, Etalon>      etalons;                /// Словарь эталонов <номер, структура параметров>

    ParametersOfEtalons     *parametersOfEtalons;   /// Класс редактирования параметров трасс
};

#endif // IMITATION_H
