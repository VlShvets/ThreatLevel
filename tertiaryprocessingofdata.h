#ifndef TERTIARYPROCESSINGOFDATA_H
#define TERTIARYPROCESSINGOFDATA_H

namespace ThreatLevel
{

/// Класс третичной обработки данных
class TertiaryProcessingOfData
{
public:
    TertiaryProcessingOfData();

    /// Определение попадания в зону обнаружения
    bool    trackIsDetected(const Track &_track);

    /// Сброс трасс
    void    resetTracks();
};

}

#endif // TERTIARYPROCESSINGOFDATA_H
