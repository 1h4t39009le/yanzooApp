#ifndef ENCLOSUREPAGE_HPP
#define ENCLOSUREPAGE_HPP

#include <QList>
#include <memory>

#include "KeeperData.hpp"
#include "Widgets/TrWidget.hpp"

namespace Ui {
class EnclosurePage;
}

// Список вольеров, закреплённых за текущим сотрудником.
// Данные странице передаёт MainWidget: сейчас это фейковый список из
// KeeperData, позже — результат запроса к БД через Core.
class EnclosurePage : public TrWidget
{
    Q_OBJECT

public:
    explicit EnclosurePage(QWidget *parent = nullptr);
    ~EnclosurePage() override;

    void setEnclosures(const QList<KeeperData::EnclosureInfo> &enclosures);

signals:
    void enclosureSelected(int enclosureId);

protected:
    void showEvent(QShowEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void retranslateUi() override;

private:
    void rebuild();
    void openCurrentItem();

    std::unique_ptr<Ui::EnclosurePage> ui;
    QList<KeeperData::EnclosureInfo> m_enclosures;
};

#endif // ENCLOSUREPAGE_HPP
