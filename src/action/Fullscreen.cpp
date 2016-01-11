#include "Fullscreen.hpp"

namespace radiosim
{
Fullscreen::Fullscreen(QObject * parent)
	: QAction(parent)
{
	setCheckable(true);
	setChecked(false);
	setText(tr("Fullscreen"));
	setStatusTip(tr("Shows the window in fullscreen"));
	connect(this, SIGNAL(triggered()), this, SLOT(on_change()));
}

void Fullscreen::on_change()
{
	if (isChecked()) {
		emit showFullScreen();
	} else {
		emit showNormal();
	}
}
}
