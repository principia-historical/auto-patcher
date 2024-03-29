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
	int mod_count = 0;
	filesystem::directory_entry mods[256];
	string temp_cmd;

	string path_mods;
	string path_principia;
	string xdelta;

	#if _WIN32 // Windows code for getting necessary paths
		// get principia home folder using the USERPROFILE envvar, emulating principia behavior.
		string userprofile = getenv("USERPROFILE");
		path_mods = userprofile+"/Principia/mods";

		// get principia path from registry...
		try {
			path_principia = GetStringValueFromHKCU(L"SOFTWARE\\Bithack\\Principia\\", L"");
		} catch (exception& e) {
			cout << "AAAAA REGISTRY THING WENT BOOM HELP - ";
			cout << e.what();
		}

		xdelta = "xdelta3.dll";
	#else // Linux-ish. While it should account for all possible setups, your mileage may vary.
		string user = getenv("USER");
		// Check how Principia is installed (as a package, or in the default wine prefix)
		if (filesystem::exists("/usr/share/principia")) {
			path_mods = "/home/"+user+"/.principia/mods";
			path_principia = "/usr/share/principia";
		} else {
			path_mods = "/home/"+user+"/.wine/drive_c/Users/[username]/Principia/mods";
			path_principia = "/home/"+user+"/.wine/drive_c/Program Files (x86)/Principia";
		}

		xdelta = "xdelta3";
	#endif // _WIN32

	cout << "Principia AutoPatcher" << endl;
	cout << "=====================" << endl;

	cout << "Principia mod path: " << path_mods << endl;
	cout << "Principia exe path: " << path_principia << endl;

	cout << "Searching for mods..." << endl;

	if (!filesystem::exists(path_mods)) {
		cout << "No mod folder found, let's just launch the regular executable." << endl;

		#ifdef _WIN32
			temp_cmd = "\""+path_principia+"\\principia.exe\"";
		#else // Use wine for non-windows
			temp_cmd = "wine \""+path_principia+"/principia.exe\"";
		#endif // _WIN32

		cout << temp_cmd << endl;
		system(temp_cmd.c_str());
	}
	for (const auto & entry : filesystem::directory_iterator(path_mods)) {
		if (entry.path().extension() == ".xdelta") {
			cout << "Got mod " << entry.path().stem() << "." << endl;
			mods[mod_count] = entry;
			mod_count++;
		}
	}
	cout << "Found " << mod_count << " mods!" << endl;

	cout << "Merging xdelta patches into one." << endl;

	int i = 0;
	temp_cmd = xdelta+" merge -f "; // Do "force" to overwrite a possible existing merged xdelta file.
	for (filesystem::path mod : mods) {
		if (mod == "") break;
		i++;

		// workaround for an xdelta quirk where the last xdelta patch to be merged shouldn't be preceeded with an -m
		if (i != mod_count) {
			temp_cmd += " -m ";
		}

		temp_cmd += " \""+mod.string()+"\" ";
	}
	temp_cmd += " \""+path_principia+"/.merged.xdelta\"";
	system(temp_cmd.c_str());

	cout << "Patching executable with merged xdelta..." << endl;

	temp_cmd = xdelta+" -d -n -f -s \""+path_principia+"/principia.exe\" \""+path_principia+"/.merged.xdelta\" \""+path_principia+"/.principia.exe\"";
	system(temp_cmd.c_str());

	cout << "Launching patched executable!" << endl;

	#ifdef _WIN32
		temp_cmd = "\""+path_principia+"\\.principia.exe\"";
	#else // Use wine for non-windows
		temp_cmd = "wine \""+path_principia+"/.principia.exe\"";
	#endif // _WIN32

	system(temp_cmd.c_str());

	return 0;
}






