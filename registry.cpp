#ifdef _WIN32
#include <iostream>
#include <string>
#include <exception>
#include <windows.h>

using namespace std;

/*! \brief						  Returns a value from HKLM as string.
	\exception     runtime_error  Replace with your error handling.
*/
string GetStringValueFromHKCU(const wstring& regSubKey, const wstring& regValue) {
	size_t bufferSize = 0xFFF; // If too small, will be resized down below.
	wstring valueBuf; // Contiguous buffer since C++11.
	valueBuf.resize(bufferSize);
	auto cbData = static_cast<DWORD>(bufferSize * sizeof(wchar_t));
	auto rc = RegGetValueW(
		HKEY_CURRENT_USER,
		regSubKey.c_str(),
		regValue.c_str(),
		RRF_RT_REG_SZ,
		nullptr,
		static_cast<void*>(valueBuf.data()),
		&cbData
	);
	while (rc == ERROR_MORE_DATA) {
		// Get a buffer that is big enough.
		cbData /= sizeof(wchar_t);
		if (cbData > static_cast<DWORD>(bufferSize)) {
			bufferSize = static_cast<size_t>(cbData);
		} else {
			bufferSize *= 2;
			cbData = static_cast<DWORD>(bufferSize * sizeof(wchar_t));
		}
		valueBuf.resize(bufferSize);
		rc = RegGetValueW(
			HKEY_CURRENT_USER,
			regSubKey.c_str(),
			regValue.c_str(),
			RRF_RT_REG_SZ,
			nullptr,
			static_cast<void*>(valueBuf.data()),
			&cbData
		);
	}
	if (rc == ERROR_SUCCESS) {
		cbData /= sizeof(wchar_t);
		valueBuf.resize(static_cast<size_t>(cbData - 1)); // remove end null character
		return string(valueBuf.begin(), valueBuf.end());
	} else {
		throw runtime_error("Windows system error code: " + to_string(rc));
	}
}
#endif
