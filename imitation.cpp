#include "imitation.h"

namespace ThreatLevel
{

/// Класс имитации
Imitation::Imitation(ParametersOfAreas *_parametersOfAreas, ParametersOfEtalons *_parametersOfEtalons)
    : parametersOfAreas(_parametersOfAreas), parametersOfEtalons(_parametersOfEtalons)
{
    /// Начальная инициализация параметров ЗКВ
    initializationOfParametersAreas();

    /// Начальная инициализация параметров эталонов
    initializationOfParametersEtalons();
}

Imitation::~Imitation()
{
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
        area.value().numTrack.clear();

    areas.clear();
    etalons.clear();
}

/// Возвращение словаря соответствующих заданному времени эталонов
void Imitation::run(const float _currentTime)
{
    QMap <int, Etalon>::iterator etalon = etalons.begin();
    for(; etalon != etalons.end(); ++etalon)
    {
        /// Изменение точных значений текущих координат эталона
        etalon.value().exactPos  = etalon.value().initStartPos +
                QPointF(etalon.value().initSpeed  * qSin(etalon.value().initCourse),
                        etalon.value().initSpeed  * qCos(etalon.value().initCourse)) * _currentTime;

        /// Внесение погрешностей измерения
        etalon.value().measSpeed    = etalon.value().initSpeed
                + gaussDistribution(0, Etalon::ERR_SPEED);
        etalon.value().measCourse   = etalon.value().initCourse
                + qDegreesToRadians(uniformDistribution(0, Etalon::ERR_COURSE));
    }
}

/// Начальная инициализация параметров ЗКВ
void Imitation::initializationOfParametersAreas()
{
    /// Загрузка параметров ЗКВ
    for(int i = 0; i < parametersOfAreas->getCount(); ++i)
    {
        /// Определение номера ЗКВ
        int num                     = (int) parametersOfAreas->getPar(i, 0);

        /// Загрузка начальных данных из таблицы
        areas[num].initPos.setX(parametersOfAreas->getPar(i, 1));
        areas[num].initPos.setY(parametersOfAreas->getPar(i, 2));
        areas[num].initRadius       = parametersOfAreas->getPar(i, 3);
        areas[num].initCritTime     = parametersOfAreas->getPar(i, 4);
        areas[num].initDetectRange  = parametersOfAreas->getPar(i, 5);
    }

    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        /// Обнуление рекурентных параметров ЗКВ
        resettingOfAreasRecurrenceParameters(*area);
    }

    /// Обнуление максимального суммарного количества крылатых ракет
    Area::CMMaxSumCount     = 0;

    /// Обнуление максимального суммарного количества баллистических целей
    Area::BGMaxSumCount     = 0;

    /// Обнуление максимального количественного состава налета по всем ЗКВ
    Area::raidMaxSumCount   = 0;
}

/// Обнуление рекурентных параметров ЗКВ
void Imitation::resettingOfAreasRecurrenceParameters(Area &_area)
{
    /// Обнуление максимального количества идентифицированных с ЗКВ крылатых ракет
    _area.CMMaxCount     = 0;

    /// Обнуление максимального количества идентифицированных с ЗКВ баллистических целей
    _area.BGMaxCount     = 0;

    /// Обнуление максимального количества идентифицированных с ЗКВ трасс
    _area.raidMaxCount   = 0;
}

/// Начальная инициализация параметров эталонов
void Imitation::initializationOfParametersEtalons()
{
    /// Загрузка параметров КР
    for(int j = 0; j < parametersOfEtalons->getCMCount(); ++j)
    {
        /// Определение номера трассы
        int num = (int) parametersOfEtalons->getCMPar(j, 0);

        /// Загрузка начальных данных из таблицы
        etalons[num].initIsBG       = false;
        etalons[num].initStartPos.setX(parametersOfEtalons->getCMPar(j, 1));
        etalons[num].initStartPos.setY(parametersOfEtalons->getCMPar(j, 2));
        etalons[num].initSpeed      = parametersOfEtalons->getCMPar(j, 3);
        etalons[num].initFinalPos   = QPointF();
        etalons[num].initCourse     = qDegreesToRadians(parametersOfEtalons->getCMPar(j, 4));
        etalons[num].initQuant      = parametersOfEtalons->getCMPar(j, 5);
    }

    /// Загрузка параметров БЦ
    for(int j = 0; j < parametersOfEtalons->getBGCount(); ++j)
    {
        /// Определение номера трассы
        int num                     = (int) parametersOfEtalons->getBGPar(j, 0);

        /// Загрузка начальных данных из таблицы
        etalons[num].initIsBG       = true;
        etalons[num].initStartPos.setX(parametersOfEtalons->getBGPar(j, 1));
        etalons[num].initStartPos.setY(parametersOfEtalons->getBGPar(j, 2));
        etalons[num].initSpeed      = parametersOfEtalons->getBGPar(j, 3);
        etalons[num].initFinalPos.setX(parametersOfEtalons->getBGPar(j, 4));
        etalons[num].initFinalPos.setY(parametersOfEtalons->getBGPar(j, 5));
        etalons[num].initCourse     = qAtan2(etalons[num].initFinalPos.x() - etalons[num].initStartPos.x(),
                                             etalons[num].initFinalPos.y() - etalons[num].initStartPos.y());
        etalons[num].initQuant      = 1;
    }
}

/// --------------------------------------------------
/// Статические функции
/// --------------------------------------------------

/// Равномерное распределение
float Imitation::uniformDistribution(const float _mean, const float _dev)
{
    return 2.0 * _dev * ((float) qrand() / RAND_MAX) - _dev + _mean;
}

/// Распределение Гаусса
float Imitation::gaussDistribution(const float _mean, const float _dev)
{
    static bool ready   = false;
    static float second = 0.0;

    if(ready)
    {
        ready   = false;
        return second * _dev + _mean;
    }

    float u, v, s;
    do
    {
        u   = 2.0 * ((float) qrand() / RAND_MAX) - 1.0;
        v   = 2.0 * ((float) qrand() / RAND_MAX) - 1.0;
        s   = u * u + v * v;
    }
    while(s > 1.0 || s == 0.0);

    float r = qSqrt(-2.0 * qLn(s) / s);
    second  = r * u;

    if(qAbs(second) < 1.5)
        ready   = true;

    if(qAbs(r * v) < 1.5)
        return r * v * _dev + _mean;

    return gaussDistribution(_mean, _dev);
}

}
