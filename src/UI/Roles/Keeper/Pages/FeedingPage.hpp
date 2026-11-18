#ifndef FEEDINGPAGE_HPP
#define FEEDINGPAGE_HPP

#include <QList>
#include <memory>

#include "KeeperData.hpp"
#include "Widgets/TrWidget.hpp"

namespace Ui {
class FeedingPage;
}

// Карточка выбранного животного и регистрация кормления.
//
// Данные приходят из KeeperMainWidget (адаптер Core -> UI): Core отдаёт их
// сигналом KeeperAccessView::animalDetailsLoaded(). Сам факт кормления
// страница не записывает — она только сообщает о нём сигналом.
class FeedingPage : public TrWidget
{
    Q_OBJECT

public:
    explicit FeedingPage(QWidget *parent = nullptr);
    ~FeedingPage() override;

    void setAnimalDetails(const KeeperData::AnimalDetails &animal,
                          const QList<KeeperData::RationIngredient> &ration);

signals:
    void feedingRequested(int animalId, const QString &notes);

protected:
    void showEvent(QShowEvent *event) override;
    void retranslateUi() override;

private slots:
    void onFeedClicked();

private:
    void rebuild();
    void clearRationRows();
    void populateRation();

    std::unique_ptr<Ui::FeedingPage> ui;
    KeeperData::AnimalDetails m_animal;
    QList<KeeperData::RationIngredient> m_ration;
    int m_animalId = -1;
};

#endif // FEEDINGPAGE_HPP
