#include "platform.hpp"
#include "version.hpp"
#include <QCoreApplication>

namespace vhfsim
{
QString platform_translations_path(const QCoreApplication & app)
{
	return app.applicationDirPath() + "/../share/" + version::project_name() + "/lang";
}

QString platform_resource_path(const QCoreApplication & app)
{
	return app.applicationDirPath() + "/../share/" + version::project_name() + "/";
}
}

