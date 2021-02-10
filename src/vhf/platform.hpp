#ifndef VHFSIM_VHF_PLATFORM_HPP
#define VHFSIM_VHF_PLATFORM_HPP

#include <QString>

class QCoreApplication; // forward

namespace vhfsim
{
QString platform_translations_path(const QCoreApplication & app);
QString platform_resource_path(const QCoreApplication & app);
}

#endif
