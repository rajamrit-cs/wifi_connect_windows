#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <iomanip>

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


void p_scan(){
//    string cmd_str = "netsh wlan show networks bssid > win_wifi_out.txt 2>&1";
//    const char *cmd = cmd_str.c_str();
//    system(cmd);
    ifstream file("win_wifi_out.txt");
    string str ;
    while (getline(file, str)) {
        if (strstr(str.c_str(), "SSID")){
            char *token = ::strtok(const_cast<char *>(str.c_str()), ":");
            while (token != NULL){
                cout<< token <<endl;
                token = ::strtok(NULL, "");
            }
//            cout<<str<<endl;
        }
//        cout << str << "\n";
    }
}

void a_scan(){
    ifstream file("active_scan.txt");
    string str ;
    vector<string> v;
    int i = 1;
    cout<< left << "SSID\t\tChannel\t\tInfo\t\tAuth\tBSS\tSignal\tFrequency\tCountry"<<endl;
    while (getline(file, str)) {
        stringstream ss(str);
        while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            v.push_back(substr);
        }
//        for (size_t i = 0; i < v.size(); i++){
//            cout << v[i] << endl;
//        }
//        cout<< v[33*i] << "\t" << v[(33*i) + 8]  << "\t" << v[(33*i) + 2] << "\t" << v[(33*i) + 13] << "\t" << v[(33*i) + 1] << "\t" << v[(33*i) + 3] << "\t" << "0" << "\t" << v[(33*i) + 30] << "\t" << "0"<<endl;
//        i++;
    }
    for (int i = 1; i < 20; ++i) {

            cout<< left << setw(15) << setfill(' ') << v[33*i] << v[(33*i) + 8]  << "\t" << setw(13) << \
            setfill(' ') <<  v[(33*i) + 2] << setw(6) << setfill(' ') <<  "\t" << v[(33*i) + 13] << setw(16) << setfill(' ') <<
            "" << v[(33*i) + 1] << "\t" << setw(6) << setfill(' ') << v[(33*i) + 3] << "\t" << v[(33*i) + 30] <<endl;
    }
//    cout << v[33*4] << endl; // SSID =
//    cout << v[33 +1 ] << endl; // BSSI = AA-DA-0C-B8-58-02
//    cout << v[33 + 2] << endl; // PHY Type = 802.11n/ac
//    cout << v[33 + 3] << endl; // RSSI = -68
//    cout << v[33 + 13] << endl; // Auth = WPA2-PSK
//    cout << v[33 + 7] << endl; // Freq = WPA2-PSK
//    cout << v[33 + 8] << endl; // Channel = WPA2-PSK
//    cout << v[33 + 30] << endl; // Country = WPA2-PSK


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

//    add_profile("win_wifi_out.txt");
//    connect(essid);
    a_scan();
    return 0;
}
