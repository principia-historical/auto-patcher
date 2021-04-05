#include <iostream>
#include <string>
#include <filesystem>
#include <unistd.h>
#if _WIN32
#include <windows.h>
#include "registry.h"
#endif // _WIN32

using namespace std;

int main() {
	int mod_count;
	filesystem::directory_entry mods[256];
	string temp_cmd;

	string path_mods;
	string path_principia;

	#if _WIN32
	path_mods = "Z:/home/administrator/.principia/mods";

	// get principia path from registry...
	try {
		path_principia = GetStringValueFromHKCU(L"SOFTWARE\\Bithack\\Principia\\", L"");
	} catch (exception& e) {
		cout << "AAAAA REGISTRY THING WENT BOOM HELP ";
		cout << e.what();
	}
	#else
	path_mods = "/home/administrator/.principia/mods";
	path_principia = "/usr/share/principia";
	#endif // _WIN32

	cout << "Principia AutoPatcher" << endl;
	cout << "=====================" << endl;

	cout << "Principia mod path: " << path_mods << endl;
	cout << "Principia exe path: " << path_principia << endl;

	cout << "Searching for mods..." << endl;

	for (const auto & entry : filesystem::directory_iterator(path_mods)) {
		if (entry.path().extension() == ".xdelta") {
			cout << "Got mod " << entry.path().stem() << "." << endl;
			mods[mod_count] = entry;
			mod_count++;
		}
	}
	cout << "Found " << mod_count << " mods!" << endl;

	cout << "Preparing for patching..." << endl;

	// TODO: Replace this stupid command shit with some sort of native IO functions.
	#ifdef _WIN32
	temp_cmd = "copy /Y \""+path_principia+"\\principia.exe\" \""+path_principia+"\\.principia.exe\"";
	#else
	temp_cmd = "cp \""+path_principia+"/principia.exe\" \""+path_principia+"/.principia.exe\"";
	#endif // _WIN32

	cout << temp_cmd << endl;
	system(temp_cmd.c_str());

	for (filesystem::path mod : mods) {
		if (mod == "") break;

		cout << "Patching mod " << mod.stem() << "." << endl;

		#ifdef _WIN32
		temp_cmd = "xdelta3.dll -d -n -f -s \""+path_principia+"\\.principia.exe\" "+mod.string()+" \""+path_principia+"\\.principia.exe\"";
		#else
		temp_cmd = "xdelta3 -d -n -f -s \""+path_principia+"/.principia.exe\" "+mod.string()+" \""+path_principia+"/.principia.exe\"";
		#endif // _WIN32

		cout << temp_cmd << endl;
		system(temp_cmd.c_str());
	}

	cout << "Launching patched executable!" << endl;

	#ifdef _WIN32
	temp_cmd = path_principia+"\\.principia.exe";
	#else
	temp_cmd = "wine "+path_principia+"/.principia.exe";
	#endif // _WIN32

	cout << temp_cmd << endl;
	system(temp_cmd.c_str());

	return 0;
}






