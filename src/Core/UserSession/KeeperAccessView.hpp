#ifndef KEEPERACCESSVIEW_HPP
#define KEEPERACCESSVIEW_HPP

#include "RoleAccessView.hpp"

#include <QList>
#include <QString>

class KeeperAccessView : public RoleAccessView {
    Q_OBJECT
public:
    // Сначала полностью определяем тела структур внутри класса
    struct EnclosureInfo {
        int id;
        QString name;
        QString countText;
        int maxHoursSinceFed;
    };

    struct AnimalInfo {
        int id;
        QString species;
        QString nickname;
        QString gender;
        int age;
        int hoursSinceFed;
        int rationId;
    };

    struct AnimalFullDetails {
        AnimalInfo base;
        QString feedingFrequency;
        int fedTodayCount;
        QString rationName;
    };

    struct RationIngredient {
        QString name;
        QString amountWithUnit;
    };

    // Теперь компилятор знает их точный размер и может использовать в методах
    explicit KeeperAccessView(QObject *parent, EmployeeID empid, QSqlDatabase db);

    QList<EnclosureInfo> enclosures() const { return m_enclosures; }
    QList<AnimalInfo> animals() const { return m_animals; }
    const AnimalFullDetails &currentAnimal() const { return m_currentAnimal; }
    QList<RationIngredient> currentRationDetails() const { return m_currentRationDetails; }

signals:
    void enclosuresRefreshed(const QList<KeeperAccessView::EnclosureInfo> &enclosures);
    void animalsLoaded(const QList<KeeperAccessView::AnimalInfo> &animals);
    void animalDetailsLoaded(const KeeperAccessView::AnimalFullDetails &animal, const QList<KeeperAccessView::RationIngredient> &ration);
    void feedingRegistered();

public slots:
    void refreshData();
    void selectEnclosure(int enclosureId);
    void selectAnimal(int animalId);
    void registerFeeding(int animalId, const QString &notes);

private:
    QList<RationIngredient> getRationDetails(int rationId);

    int m_selectedEnclosureId;
    
    QList<EnclosureInfo> m_enclosures;
    QList<AnimalInfo> m_animals;
    AnimalFullDetails m_currentAnimal; // Теперь MSVC видит размер этой структуры
    QList<RationIngredient> m_currentRationDetails;
};

#endif // KEEPERACCESSVIEW_HPP
