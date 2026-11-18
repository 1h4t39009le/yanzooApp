#include "KeeperData.hpp"

namespace KeeperData {

QList<EnclosureInfo> enclosures()
{
    // Заглушка вместо запроса к БД. Названия вольеров не переводятся
    // (как и данные из БД), а количество животных приходит числом:
    // подпись "Animals: %1" формирует EnclosurePage.
    return {
        {1, QStringLiteral("Savanna"), 12},
        {2, QStringLiteral("Primates"), 7},
        {3, QStringLiteral("Aquarium"), 23},
        {4, QStringLiteral("Aviary"), 15},
        {5, QStringLiteral("Predators"), 5},
        {6, QStringLiteral("Terrarium"), 9},
    };
}

} // namespace KeeperData
