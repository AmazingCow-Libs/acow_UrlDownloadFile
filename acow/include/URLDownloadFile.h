#pragma once

// std
#include <string>
// AmazingCow Libs
#include "acow/cpp_goodies.h"

namespace acow { namespace net { namespace URLDownloadFile {

//----------------------------------------------------------------------------//
// Types                                                                      //
//----------------------------------------------------------------------------//
typedef unsigned long errorcode_t;


//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//
///-----------------------------------------------------------------------------
bool DownloadData(
    const std::string &url,
    u8              **pOutValue,
    size_t           *pValueSize,
    errorcode_t      *pOutError = nullptr) noexcept;

///-----------------------------------------------------------------------------
bool DownloadString(
    const std::string &url,
    std::string          *pOutString,
    errorcode_t          *pOutError = nullptr) noexcept;

///-----------------------------------------------------------------------------
bool DownloadToFile(
    const std::string &url,
    const std::string &fullpath,
    errorcode_t          *pOutError = nullptr) noexcept;

} // namepace URLDownloadFile
} // namespace net
} // namespace acow