#include <iostream>
#include <string>
#include <filesystem>
#include <unistd.h>

using namespace std;

int main() {
	int mod_count;
	filesystem::directory_entry mods[256];
	string temp_cmd;

	string path_mods = "/home/administrator/.principia/mods/";
	//string path_temp_principia = "/home/administrator/.principia/cache/principia.exe";
	string path_temp_principia = "/usr/share/principia/.principia.exe";
	string path_exec_principia = "/usr/share/principia/principia.exe";

	cout << endl << endl;
	cout << "Principia AutoPatcher" << endl;
	cout << "=====================" << endl;

	cout << "Principia mod path: " << path_mods << endl;
	cout << "Principia txe path: " << path_temp_principia << endl;
	cout << "Principia exe path: " << path_exec_principia << endl;

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
	temp_cmd = "cp "+path_exec_principia+" "+path_temp_principia;
	system(temp_cmd.c_str());

	for (filesystem::path mod : mods) {
		if (mod == "") break;

		cout << "Patching mod " << mod.stem() << "." << endl;
		temp_cmd = "xdelta3 -d -n -f -s "+path_temp_principia+" "+mod.string()+" "+path_temp_principia;
		system(temp_cmd.c_str());
	}

	cout << "Launching patched executable!" << endl;
	chdir("/usr/share/principia/");
	temp_cmd = "wine "+path_temp_principia;
	system(temp_cmd.c_str());

	return 0;
}






