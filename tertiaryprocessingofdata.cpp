#include "tertiaryprocessingofdata.h"

namespace ThreatLevel
{

/// Класс третичной обработки данных
TertiaryProcessingOfData::TertiaryProcessingOfData()
{
}

/// Определение попадания в зону обнаружения
bool TertiaryProcessingOfData::trackIsDetected(const Track &_track)
{
    QMap <int, Area>::const_iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        if(calcDistance(_track.exactPos, area.value().initPos) < area.value().initDetectRange)
            return true;
    }

    return false;
}


/// Сброс трасс
void TertiaryProcessingOfData::resetTracks()
{
    bool isReset;   /// Флаг сброса трассы

    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end();)
    {
        isReset = false;

        int numArea = track.value().numArea;
        if(numArea != -1 &&
           areas.contains(numArea))
        {
            /// Сброс трассы при пересечении границ ПР
            if(track.value().distToPA < areas[numArea].initRadius)
                isReset = true;

            /// Сброс трассы при значении времени преодоления расстояния до ПР меньше значения критического времени ПР
            if(track.value().timeToPA < areas[numArea].initCritTime)
                isReset = true;
        }

        if(isReset)
        {
            /// Удаление номера трассы из списка номеров ассоциированных с ПР трасс
            if(areas.contains(numArea) &&
               areas[numArea].numTrack.contains(track.key()))
            {
                areas[numArea].numTrack.removeOne(track.key());
            }

            /// Удаление трассы
            track = tracks.erase(track);
         }
        else
             ++track;
    }
}

}
