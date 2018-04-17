// Header
#include "acow/include/URLDownloadFile.h"

#if (ACOW_OS_IS_WINDOWS)

// std
#include <cstdio>
#include <cstdlib>
// Windows
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <Windows.h>
#include <Urlmon.h>
// AmazingCow Libs
#include "acow/IO.h"

//----------------------------------------------------------------------------//
// Library Linking                                                            //
//----------------------------------------------------------------------------//
#pragma comment(lib, "Urlmon.lib")

// Usings
using namespace acow;
using namespace net;
using namespace std;

acow_internal_function size_t
FindFileSize(FILE *pFile) noexcept
{
    // COWTODO(n2omatt): Error handling...
    auto curr = ftell(pFile);
    fseek(pFile, 0, SEEK_END);
    auto len = ftell(pFile);
    fseek(pFile, curr, SEEK_SET);

    return len;
}

//----------------------------------------------------------------------------//
// Implementation                                                             //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
bool
URLDownloadFile::DownloadData(
    const std::string &url,
    u8               **pOutValue,
    size_t            *pValueSize,
    errorcode_t       *pOutError) noexcept
{
    auto temp_filename = acow::IO::Path::GetTempFileName();
    if(DownloadToFile(url, temp_filename, pOutError)) {
        auto file_bytes      = acow::IO::File::ReadAllBytes(temp_filename);        
        auto file_bytes_size = file_bytes.size();
        auto buffer          = acow_malloc(ACOW_SIZEOF(u8, file_bytes_size));
        
        acow_memcpy(buffer, file_bytes.data(), file_bytes_size);
        *pOutValue = (u8*)buffer;

        ACOW_SAFE_ASSIGN(pValueSize, file_bytes_size);
        ACOW_SAFE_ASSIGN(pOutError,  0);

        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
bool
URLDownloadFile::DownloadString(
    const std::string &url,
    std::string          *pOutString,
    errorcode_t          *pOutError) noexcept
{
    u8     *p_contents    = nullptr;
    size_t  contents_size = 0;

    if(DownloadData(url, &p_contents, &contents_size, pOutError)) {
        pOutString->resize(contents_size + 1);
        pOutString->assign(reinterpret_cast<char *>(p_contents), contents_size);

        acow_free(p_contents);
    }

    return false;
}

//------------------------------------------------------------------------------
bool
URLDownloadFile::DownloadToFile(
    const std::string &url,
    const std::string &fullpath,
    errorcode_t       *pOutError) noexcept
{
    auto hresult = URLDownloadToFile(
        NULL,
        url     .c_str(),
        fullpath.c_str(),
        0,
        NULL
    );

    if(SUCCEEDED(hresult)) {
        ACOW_SAFE_ASSIGN(pOutError, 0);
        return true;
    } else {
        ACOW_SAFE_ASSIGN(pOutError, hresult);
        return false;
    }
}

#endif // ACOW_OS_IS_WINDOWS