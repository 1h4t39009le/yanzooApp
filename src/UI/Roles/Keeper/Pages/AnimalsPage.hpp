#ifndef ANIMALSPAGE_HPP
#define ANIMALSPAGE_HPP

#include <QList>
#include <memory>

#include "KeeperData.hpp"
#include "Widgets/TrWidget.hpp"

namespace Ui {
class AnimalsPage;
}

// Список животных выбранного вольера.
//
// Данные странице передаёт KeeperMainWidget: Core отдаёт их сигналом
// KeeperAccessView::animalsLoaded() после selectEnclosure().
class AnimalsPage : public TrWidget
{
    Q_OBJECT

public:
    explicit AnimalsPage(QWidget *parent = nullptr);
    ~AnimalsPage() override;

    void setAnimals(const QList<KeeperData::AnimalInfo> &animals);

signals:
    void animalSelected(int animalId);

protected:
    void showEvent(QShowEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void retranslateUi() override;

private:
    void rebuild();
    void openCurrentItem();

    std::unique_ptr<Ui::AnimalsPage> ui;
    QList<KeeperData::AnimalInfo> m_animals;
};

#endif // ANIMALSPAGE_HPP
