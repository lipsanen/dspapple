#pragma once

#include <string>

namespace dspapple
{
    struct Error
    {
        std::string m_sErrorMessage;
        bool m_bError = false;
    };
}
