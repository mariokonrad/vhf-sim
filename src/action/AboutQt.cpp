#include "AboutQt.hpp"

namespace radiosim
{
AboutQt::AboutQt(QObject * parent)
	: QAction(parent)
{
	setText(tr("AboutQt"));
	setStatusTip(tr("Shows the AboutQt dialog"));
}
}
