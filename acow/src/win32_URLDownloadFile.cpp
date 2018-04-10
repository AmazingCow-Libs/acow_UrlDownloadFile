// Header
#include "include/URLDownloadFile.h"

#if (ACOW_OS_IS_WINDOWS)

// std
#include <cstdio>
#include <cstdlib>
// Windows
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <Windows.h>
#include <Urlmon.h>

#pragma comment(lib, "Urlmon.lib")

// Usings
using namespace acow;
using namespace std;

//----------------------------------------------------------------------------//
// Macros                                                                     //
//----------------------------------------------------------------------------//
#define SAFE_ASSIGN(_var_, _value_)  \
	do {							 \
		if (_var_) *_var_ = _value_; \
	} while (0) 


//----------------------------------------------------------------------------//
// Helper Functions                                                           //
//----------------------------------------------------------------------------//
acow_internal_function std::string 
GetTempFilename() noexcept
{
	// COWTODO(n2omatt): Implement...
	return "_temp_file_";
}

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
	u8   			 **pOutValue, 
	size_t			  *pValueSize,
	errorcode_t		  *pOutError) noexcept
{
	auto temp_filename = GetTempFilename();
	if(DownloadToFile(url, temp_filename, pOutError)) {
		// Read entire file.
		auto p_file	= fopen(temp_filename.c_str(), "rb"); 
		// COWTODO(n2omatt): Check if we could open the file...

        //COWTODO(n2omatt): acow_c_goodies has some stuff like that?
		auto file_len = FindFileSize(p_file);

		// COWTOTOD(n2omatt); Check if we could alloc memory...
		auto buffer = (u8 *)malloc(file_len * sizeof(u8));;
		*pOutValue = buffer;

		// COWTODO(n2omatt): Check if read_size == file_len;
		auto read_size = fread(*pOutValue, file_len, 1, p_file);
		
		fclose(p_file);
		
		SAFE_ASSIGN(pValueSize, file_len);
		SAFE_ASSIGN(pOutError,  0);
		
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------
bool 
URLDownloadFile::DownloadString(
	const std::string &url,
	std::string		  *pOutString,
	errorcode_t		  *pOutError) noexcept
{
	u8     *p_contents    = nullptr;
	size_t  contents_size = 0;
	
	if(DownloadData(url, &p_contents, &contents_size, pOutError)) {
		
		pOutString->resize(contents_size + 1);
		pOutString->assign(reinterpret_cast<char *>(p_contents), contents_size);

		free(p_contents);
	}
	
	return false;
}

//------------------------------------------------------------------------------
bool  
URLDownloadFile::DownloadToFile(
	const std::string &url,
	const std::string &fullpath, 
	errorcode_t		  *pOutError) noexcept
{	
	auto hresult = URLDownloadToFile(
		NULL, 
		url		.c_str(), 
		fullpath.c_str(), 
		0, 
		NULL
	);

	if(SUCCEEDED(hresult)) {
		SAFE_ASSIGN(pOutError, 0);
		return true;
	} else {
		SAFE_ASSIGN(pOutError, hresult);
		return false;
	}
}

#endif // ACOW_OS_IS_WINDOWS