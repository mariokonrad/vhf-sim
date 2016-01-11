#ifndef __RADIO_SIM__ACTION__FULLSCREEN__HPP__
#define __RADIO_SIM__ACTION__FULLSCREEN__HPP__

#include <QAction>

namespace radiosim
{
class Fullscreen : public QAction
{
	Q_OBJECT

public:
	Fullscreen(QObject * parent);

signals:
	void showFullScreen();
	void showNormal();

private slots:
	void on_change();
};
}

#endif
