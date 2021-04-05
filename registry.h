#ifndef REGISTRY_H_INCLUDED
#define REGISTRY_H_INCLUDED

std::string GetStringValueFromHKCU(const std::wstring& regSubKey, const std::wstring& regValue);

#endif // REGISTRY_H_INCLUDED
