#include "About.hpp"

namespace radiosim
{
About::About(QObject * parent)
	: QAction(parent)
{
	setText(tr("About"));
	setStatusTip(tr("Radio-Sim"));
}
}
