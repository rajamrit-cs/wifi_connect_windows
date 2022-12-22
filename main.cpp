#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void add_profile(string file_name){
    cout<<"Adding Profiles"<<endl;
    string cmd_str = "netsh wlan add profile filename=" + file_name + " interface=\"Wi-Fi\"";
    const char *cmd = cmd_str.c_str();
    system(cmd);
}

void connect(string essid){
    string cmd_str = "netsh wlan connect name=" + essid + " ssid=" + essid + " interface=Wi-Fi";
    const char *cmd = cmd_str.c_str();
    system(cmd);
}

string scan(){
    string cmd_str = "netsh wlan show networks bssid > win_wifi_out.txt 2>&1";
    const char *cmd = cmd_str.c_str();
    system(cmd);
}

int main(int argc, char** argv) {

//    if (argc <= 1){
//        std::cout<<"[Error] Argument not provided properly"<<endl;
//        exit(0);
//    }
    string essid = "CG"; // argv[1];
    string password = "Password@123"; // argv[2];

    string encryption = "AES";
    string tmpf = "tmp-wlan-profile.xml";
    ofstream xmlProfileFile;
    xmlProfileFile.open(tmpf);

    xmlProfileFile << "<?xml version=\"1.0\"?>\n"
                   << "<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">\n"
                   << "\t<name>" << essid << "</name>\n";

    // SSIDConfig
    xmlProfileFile << "\t<SSIDConfig>\n"
                   <<    "\t\t<SSID>\n"
                   <<       "\t\t\t<hex>";
    for(int i=0 ; i < essid.length(); i++) { // Convert SSID to hex
        xmlProfileFile << hex << (int) essid.at(i);
    }
    xmlProfileFile <<       "</hex>\n"
                   <<       "\t\t\t<name>" << essid << "</name>\n"
                   <<    "\t\t</SSID>\n"
                   << "\t</SSIDConfig>\n";

    // Connection info
    xmlProfileFile << "\t<connectionType>ESS</connectionType>\n"
                   << "\t<connectionMode>manual</connectionMode>\n";

    // Security-related
    xmlProfileFile << "\t<MSM>\n"
                   <<    "\t\t<security>\n"
                   <<       "\t\t\t<authEncryption>\n"
                   <<          "\t\t\t\t<authentication>WPA2PSK</authentication>\n"     // TODO: Revisit authentication type?
                   <<          "\t\t\t\t<encryption>" << encryption << "</encryption>\n"  // TODO: How to get encryption?
                   <<          "\t\t\t\t<useOneX>false</useOneX>\n"
                   <<       "\t\t\t</authEncryption>\n"
                   <<       "\t\t\t<sharedKey>\n"
                   <<          "\t\t\t\t<keyType>passPhrase</keyType>\n"
                   <<          "\t\t\t\t<protected>false</protected>\n"
                   <<          "\t\t\t\t<keyMaterial>" << password << "</keyMaterial>\n"
            <<       "\t\t\t</sharedKey>\n"
            <<    "\t\t</security>\n"
            << "\t</MSM>\n";

    // MAC Randomization
    xmlProfileFile << "\t<MacRandomization xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v3\">\n"
    <<    "\t\t<enableRandomization>false</enableRandomization>\n"
    <<    "\t\t<randomizationSeed>2232527858</randomizationSeed>\n"
            << "\t</MacRandomization>\n";

    // End profile config
    xmlProfileFile << "</WLANProfile>\n";
    xmlProfileFile.close();

    add_profile("win_wifi_out.txt");
    connect(essid);

    return 0;
}
