// armisMusicChanger.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;
struct Fraction {
    int bunja;
    int bunmo;
};
struct Tempos {
    Fraction backja;
    int speed;
    
};
struct Expression {
    Fraction bakja;
    string exp = "";
};
struct Notes {
    string piano="";
    Fraction bakja;
};
struct ChainedNotes {
    int stOct, stTempo, stVolume, stPointer, endOct, endTempo, endVolume;
    string stsL = "4";
    Fraction stL, endL;
    Fraction sbakja,stBakja;
    string snote = "";
};
struct track {
    int v=12;
    int t=60;
    int oct = 4;
    Fraction bakja;
    string sbakja="4";
    Fraction sumbakja;
    int pointer=0,prePointer=0;
    Notes prenote;
    ChainedNotes chainNotes;
    ifstream file;
    ofstream tofile;
    string out="";

};
struct insertnotes {
    int stOct, stTempo, stVolume, endOct,endTempo,endVolume;
    Fraction stL,endL;
    string notes;
};

Fraction addFraction(Fraction x, Fraction y);
Fraction subFraction(Fraction x, Fraction y);
Fraction multFraction(Fraction x, Fraction y);
Fraction divFraction(Fraction x, Fraction y);
Fraction trimFraction(Fraction x);
string pointerMove(int i);
string pointerMove2(track& track1);
Fraction Point(Fraction& bakja);
void AssignFraction(Fraction& into, Fraction backja);
int compare(Fraction x, Fraction y);
string OptimizeBakja(Fraction x,string piano);
ChainedNotes ChainedMove(int t);
int ArrayTrack(vector<int> &TrackABC);
int CompressTrack(vector<int> TrackABC);
void NoteToR(vector<int> TrackABC);
void NoteToVibrate(vector<int> TrackABC);
void NoteToForce(vector<int> TrackABC);
void NoteToDrum(vector<int> TrackABC);
string nNotes[] = { "c", "c+","d","d+","e","f","f+","g","g+","a","a+","b" };
const int tempTrack = 199;
track tracks[200];
track abcTrack[50];
vector<Tempos> temposs;
int trackcount;

void initTrack(int count) {
    //포인터, 박자합 l박자 초기화
    for (int i = 1; i <= count; i++) {
        tracks[i].pointer = 0;
        tracks[i].sumbakja.bunja = 0;
        tracks[i].sumbakja.bunmo = 4;

        tracks[i].bakja.bunja = 1;
        tracks[i].bakja.bunmo = 4;
        tracks[i].oct = 4;
        tracks[i].sbakja = "4";
        tracks[i].chainNotes.snote = "";
    }
}
void initTrack2(track &track1) {
    //포인터, 박자합 l박자 초기화
    track1.pointer = 0;
    track1.sumbakja.bunja = 0;
    track1.sumbakja.bunmo = 4;

    track1.bakja.bunja = 1;
    track1.bakja.bunmo = 4;
    track1.oct = 4;
    track1.sbakja = "4";
    track1.chainNotes.snote = "";
}
int noteCk(string notes) {
    if (notes == "") {
        cout << "noteck error: empty var";
        return -1;
    }

    if (notes.at(0) == 'a' || notes.at(0) == 'b' || notes.at(0) == 'c' || notes.at(0) == 'd' || notes.at(0) == 'e' || notes.at(0) == 'f' || notes.at(0) == 'g') {
        return 1;
    }
    else if (notes.at(0) == 'r') {
        return 2;
    }
    else {
        return 0;
    }
}
int noteCk(char notes) {
    if (notes == 'a' || notes == 'b' || notes == 'c' || notes == 'd' || notes == 'e' || notes == 'f' || notes == 'g') {
        return 1;
    }
    else if (notes == 'r') {
        return 2;
    }
    else {
        return 0;
    }
}
void testfuc() {
    track test;
    initTrack2(test);
    cout << test.bakja.bunmo << test.bakja.bunja;
}

int main()
{
    
    int righthand = 0;
    int lefthand=0;
    int maxvolume=0;
    int extrack = 0;
    cout << "intput number of track : ";
    cin >> trackcount;

    //파일열기
    for (int i = 1; i <= trackcount; i++) {
        tracks[i].file.open("track"+ to_string(i) + ".txt", ifstream::in);
    }
    //파일 읽어오기
    for (int i = 1; i <= trackcount; i++) {
        string tmp;
        while (!tracks[i].file.eof()) {
            getline(tracks[i].file, tmp);
            tracks[i].out.append(tmp);
        }  
    }
    
    //끝을 표시
    for (int i = 1; i <= trackcount; i++) {
        tracks[i].out += "*";
    }
    
    initTrack(trackcount);

    cout << "input expression track(if not exist, input zero)";
    cin >> extrack;

    //전체 템포 설정
    ////템포박자합 순으로 정렬.
    Tempos tTempo;
    int tempoOffset = -1;
    if (extrack > 0) {
        while (tracks[extrack].pointer < tracks[extrack].out.length()) {
            string temp = "";
            
            temp = pointerMove(extrack);
            if (temp.at(0) == 't') {
                tTempo.backja = tracks[extrack].sumbakja;
                tTempo.speed = tracks[extrack].t+tempoOffset;
                temposs.push_back(tTempo);
                tracks[extrack].out.erase(tracks[extrack].prePointer, temp.length());
                tracks[extrack].pointer = tracks[extrack].prePointer;
            }

        }
    }
    else {
        for (int i = 1; i <= trackcount; i++) {
            while (tracks[i].pointer < tracks[i].out.length()) {
                string temp = "";
                temp = pointerMove(i);
                if (temp.at(0) == 't') {
                    tTempo.backja = tracks[i].sumbakja;
                    tTempo.speed = tracks[i].t+ tempoOffset;
                    temposs.push_back(tTempo);
                    tracks[i].out.erase(tracks[i].prePointer, temp.length());
                    tracks[i].pointer = tracks[i].prePointer;
                }

            }
        }
        //정렬코드
        for (int i = temposs.size() - 1; i > 0; i--) {
            for (int j = 0; j < i; j++) {
                if (compare(temposs[j].backja, temposs[j + 1].backja) == 1) {
                    tTempo = temposs[j];
                    temposs[j] = temposs[j + 1];
                    temposs[j + 1] = tTempo;
                }
            }
        }
        for (int i = 0; i < temposs.size() - 1; i++) {
            while (i + 1 < temposs.size() && compare(temposs[i].backja, temposs[i + 1].backja) == 0) {
                temposs.erase(temposs.begin() + i);
            }
        }
    }
    //포인터, 박자합 l박자 초기화 및 사전 고르기 작업
    initTrack(trackcount);
    int ROn = 0;
    for (int i = 1; i <= trackcount; i++) {
        for (int k = 1; k <= trackcount; k++) {
            while (tracks[k].pointer < tracks[k].out.length()) {
                string temp;
                temp=pointerMove(k);
                if (tracks[k].pointer >= tracks[k].out.length())break;
                if (temp.at(0) == 'r')
                    ROn = 1;
                else if (temp.at(0) == 'a' || temp.at(0) == 'b' || temp.at(0) == 'c' || temp.at(0) == 'd' || temp.at(0) == 'e' || temp.at(0) == 'f' || temp.at(0) == 'g')
                    ROn = 0;
                if (ROn==1 && tracks[k].out.at(tracks[k].pointer) == '&') {
                    tracks[k].out.erase(tracks[k].pointer,1);
                }
               
            }
        }
    }
    initTrack(trackcount);
    //정렬한 템포 실제 트랙들에 끼워넣기.
    
    for (int i = 1; i <= trackcount; i++) {
        if (temposs[0].backja.bunja == 0) {
            tracks[i].out.insert(0, "t" + to_string(temposs[0].speed));
        }
    }

    for (int i = 0; i < temposs.size(); i++) {
        if (temposs[i].backja.bunja != 0) {
            for (int k = 1; k <= trackcount; k++) {
                while (tracks[k].pointer < tracks[k].out.length()) {
                    string temp = "";
                    temp = pointerMove(k);
                    if (compare(tracks[k].sumbakja, temposs[i].backja) == 1) {
                        Fraction temp2, temp3;
                        string temp4 = "";
                        string mpercent = "&";
                        temp2 = subFraction(tracks[k].sumbakja, temposs[i].backja);
                        temp3 = subFraction(tracks[k].prenote.bakja, temp2);
                        if (tracks[k].prenote.piano == "r")mpercent = "";
                        temp4 = OptimizeBakja(temp3, tracks[k].prenote.piano) + mpercent + "t" + to_string(temposs[i].speed) + OptimizeBakja(temp2, tracks[k].prenote.piano);
                        tracks[k].out.replace(tracks[k].prePointer, temp.length(), temp4);
                        tracks[k].pointer = tracks[k].prePointer;
                        tracks[k].sumbakja = subFraction(tracks[k].sumbakja,tracks[k].prenote.bakja);
                        cout << temp4 << " 가변\n";
                        break;
                    }
                    if (compare(tracks[k].sumbakja, temposs[i].backja) == 0) {
                        tracks[k].out.insert(tracks[k].pointer, "t" + to_string(temposs[i].speed));
                        break;
                    }
                }
            }
        }

    }
    

    //포인터, 박자합 l박자 초기화
    initTrack(trackcount);
    int minvolume = 15;
    //음량 최고,최저 볼륨 확인
    for (int i = 1; i <= trackcount; i++) {
        while (tracks[i].pointer < tracks[i].out.length()) {
            string temp = "";
            if (tracks[i].out.at(tracks[i].pointer) == 'v') {
                if (isdigit(tracks[i].out.at(tracks[i].pointer + 1))) {
                    temp += tracks[i].out.at(tracks[i].pointer + 1);
                }
                else { cout << "볼륨 숫자가 없습니다."; }
                if (isdigit(tracks[i].out.at(tracks[i].pointer + 2))) {
                    temp += tracks[i].out.at(tracks[i].pointer + 2);
                }
                if (maxvolume < stoi(temp)) {
                    maxvolume = stoi(temp);
                }
                if (minvolume > stoi(temp)) {
                    minvolume = stoi(temp);
                }
            }

            tracks[i].pointer++;
        }
    }
    cout << "음악의 최고 볼륨은" + to_string(maxvolume) + "입니다\n";

    //포인터, 박자합 l박자 초기화
    initTrack(trackcount);

    //전체 볼륨 증가시키기
        int volumeup = 15 - maxvolume;
        cout << "전체볼륨을" + to_string(volumeup) + "올립니다.\n";

        for (int i = 1; i <= trackcount; i++) {
            while (tracks[i].pointer < tracks[i].out.length()) {
                string temp = "";
                int temp2 = 0;
                int j = 0;
                if (tracks[i].out.at(tracks[i].pointer) == 'v') {
                    while (isdigit(tracks[i].out.at(tracks[i].pointer + j + 1)) != 0) {
                        j++;
                        temp += tracks[i].out.at(tracks[i].pointer + j);
                    }
                    if (maxvolume-minvolume >=5) {
                        temp=to_string(stoi(temp) + (15 - stoi(temp))/ 2);
                    }
                    else {
                        temp = to_string(stoi(temp) + volumeup);
                    }
                    if (stoi(temp) < 11)temp = "11";
                    tracks[i].out.replace(tracks[i].pointer + 1, j, temp);
                    tracks[i].pointer += temp.length();
                    cout << to_string(tracks[i].pointer) + "번째 v의 음량이" + temp + "가 되었습니다\n";

                }
                tracks[i].pointer++;
            }
        }
    //포인터, 박자합 l박자 초기화
    initTrack(trackcount);
    // 트랙 입력
    vector<int> abcTrackNumber[50];
    int abcsize = 0;
    char abc = 'A';
    int counttemp = 0;

    cout << "각 트랙에서 첫번째로 입력되는 트랙을 기준으로 압축됩니다.(다만 박자가 낮을시 교체됩니다.)\n";
    for (int i = 0; i <= 36;i++) {
        char tem = abc + i;
        cout << tem << "트랙에 압축할 트랙들을 입력하시오.(0 입력시 다음트랙으로, -1 입력시 입력종료) \n ";
    
        do {
            cin >> counttemp;
            if (counttemp == -1)break;
            if (counttemp == 0)break;
            abcTrackNumber[i].push_back(counttemp);
        } while (counttemp != 0);

        if (counttemp == -1) {
            abcsize = i+1;
            break;
        }
    }
    for (int i = 0; i < abcsize; i++) {
        ArrayTrack(abcTrackNumber[i]);
       // abcTrack[i].out=tracks[abcTrackNumber[i][0]].out;
    }

    //트랙 압축
    for(int i=0;i<abcsize;i++)
        CompressTrack(abcTrackNumber[i]);

    initTrack(trackcount);
    
    //바이올린 진동 넣기.
    cout << "\n";
    char violinNumber=0;
    while (violinNumber >= 0) {
        cout << "input vilolin track capital (exit key: !)  :";
        cin >> violinNumber;
        if (violinNumber == '!')break;
        int t1 = violinNumber - 65;
        NoteToVibrate(abcTrackNumber[t1]);
    }
    //베이스 음량 최대로.
    cout << "\n";
    char baseCap = 0;
    while (baseCap >= 0) {
        cout << "input base track capital (exit key: !)  :";
        cin >> baseCap;
        if (baseCap == '!')break;
        int t2 = baseCap - 65;
        NoteToForce(abcTrackNumber[t2]);
    }
    //드럼
    cout << "\n";
    char drumCap = 0;
    while (drumCap >= 0) {
        cout << "input drum track capital (exit key: !)  :";
        cin >> drumCap;
        if (drumCap == '!')break;
        int t3 = drumCap - 65;
        NoteToDrum(abcTrackNumber[t3]);
    }

    
    
    
    
    

    //읽기 전용으로 연 파일들 닫아주기.
    for(int i = 1; i <= trackcount; i++) {
        tracks[i].file.close();
    }
    /*
    //쓰기 전용으로 파일들 다시 열어서 개조한 문자열 대입시켜주기
    for (int i = 1; i <= trackcount; i++) {
        tracks[i].tofile.open("fixedCode" + to_string(i) + ".txt", ifstream::out);
        if (tracks[i].tofile.is_open())    //파일이 열렸는지 확인
        {
            tracks[i].tofile << tracks[i].out;
        }
        tracks[i].tofile.close();
    }*/

    char abcdefg = 'A';
    for (int i = 0; i < abcsize; i++) {
        string abcdefg2;
        abcdefg2 += abcdefg;
        tracks[abcTrackNumber[i][0]].tofile.open(abcdefg2+ "Track" + ".txt", ifstream::out);
        abcdefg++;
        if (tracks[abcTrackNumber[i][0]].tofile.is_open())    //파일이 열렸는지 확인
        {
            tracks[abcTrackNumber[i][0]].tofile << tracks[abcTrackNumber[i][0]].out;
        }
        tracks[abcTrackNumber[i][0]].tofile.close();
    }
   
    
}
//i트랙의 포인터를 다음 음표로 이동하면서 전 음표를 리턴.
string pointerMove(int i) {
    string temp = "";
    string temp2="";
    if (tracks[i].pointer < tracks[i].out.length()) {
        
        int j = 1;
        string tempBakja="";
        string plus = "";
        Fraction tmp;
        tracks[i].prePointer = tracks[i].pointer;
        switch (tracks[i].out.at(tracks[i].pointer)) {
        case 'v':
            while (isdigit(tracks[i].out.at(tracks[i].pointer + j))) {
                temp += tracks[i].out.at(tracks[i].pointer + j);
                j++;
            }
            tracks[i].v = stoi(temp);
            tracks[i].pointer += temp.length();
            temp = "v" + temp;
            break;
        case 't':
            while (isdigit(tracks[i].out.at(tracks[i].pointer + j))) {
                temp += tracks[i].out.at(tracks[i].pointer + j);
                j++;
            }
            tracks[i].t = stoi(temp);
            tracks[i].pointer += temp.length();
            temp = "t" + temp;
            break;
        case 'l':
            while (isdigit(tracks[i].out.at(tracks[i].pointer + j))) {
                temp += tracks[i].out.at(tracks[i].pointer + j);
                j++;
            }
            
            tmp.bunmo = stoi(temp);
            tmp.bunja = 1;
            while (tracks[i].out.at(tracks[i].pointer + j) == '.') {
                Point(tmp);
                temp += ".";
                j++;
            }
            tracks[i].sbakja = temp;
            tracks[i].bakja = tmp;
            tracks[i].pointer += temp.length();
            temp = "l" + temp;
            break;
        case 'o':
            if (tracks[i].out.at(tracks[i].pointer + j) == '-') {
                temp += "-";
                j++;
            }
            while (isdigit(tracks[i].out.at(tracks[i].pointer + j))) {
                temp += tracks[i].out.at(tracks[i].pointer + j);
                j++;
            }
            tracks[i].oct = stoi(temp);
            tracks[i].pointer += temp.length();
            temp = "o" + temp;
            break;
        case '>':
            tracks[i].oct++;
            temp = ">";
            break;
        case '<':
            tracks[i].oct--;
            temp = "<";
            break;
        case '&':
            temp = "&";
            break;
        case 'r':
            temp = "";
            j = 1;
            while (isdigit(tracks[i].out.at(tracks[i].pointer + j)) != 0) {
                    temp += tracks[i].out.at(tracks[i].pointer + j);
                    j++;
            }
            if (temp.length() < 1) {
                 AssignFraction(tmp, tracks[i].bakja);
            }
            else {
                 tmp.bunmo = stoi(temp);
                 tmp.bunja = 1;
            }
            while (tracks[i].out.at(tracks[i].pointer + j) == '.') {
                 Point(tmp);
                 temp += ".";
                 j++;
            }
            tracks[i].pointer += temp.length();
            tracks[i].sumbakja = addFraction(tracks[i].sumbakja, tmp);
            
            tracks[i].prenote.piano = "r";
            tracks[i].prenote.bakja = tmp;
            temp = "r" + temp;
            break;
        case 'n':
            temp = "";
            while (isdigit(tracks[i].out.at(tracks[i].pointer + j))) {
                temp += tracks[i].out.at(tracks[i].pointer + j);
                j++;
            }
            if (stoi(temp) / 12 != tracks[i].oct) {
                temp2="o" + to_string(stoi(temp) / 12) + nNotes[stoi(temp) % 12]+tracks[i].sbakja;
            }
            else {
                temp2 = nNotes[stoi(temp) % 12] + tracks[i].sbakja;
            }
            tracks[i].out.replace(tracks[i].pointer, temp.length() + 1, temp2+ "o" + to_string(tracks[i].oct));
            tracks[i].pointer += temp2.length()-1;
            tracks[i].sumbakja = addFraction(tracks[i].sumbakja, tracks[i].bakja);
            temp = temp2;
            break;
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'a':
        case 'b':
            
            j = 1;
            if (tracks[i].out.at(tracks[i].pointer + j) == '+') {
                j++;
                plus = '+';
            }   else if (tracks[i].out.at(tracks[i].pointer + j) == '-') {
                j++;
                plus = '-';
            }
            

            if (tracks[i].v == 0) {
                tracks[i].out.replace(tracks[i].pointer, 1 + plus.length(), "r");
                j -= plus.length();
                plus = "";
            }
              
            while (isdigit(tracks[i].out.at(tracks[i].pointer + j))) {
                temp += tracks[i].out.at(tracks[i].pointer + j);
                j++;
            }
            if (temp.length() < 1) {
                AssignFraction(tmp, tracks[i].bakja);
            }
            else {
                tmp.bunmo = stoi(temp);
                tmp.bunja = 1;
            }
            while (tracks[i].out.at(tracks[i].pointer + j) == '.') {
                Point(tmp);
                temp += ".";
                j++;
            }
            tracks[i].pointer += temp.length()+plus.length();
            tracks[i].sumbakja = addFraction(tracks[i].sumbakja, tmp);

            tracks[i].prenote.piano = tracks[i].out.at(tracks[i].prePointer) + plus;
            tracks[i].prenote.bakja = tmp;
            temp = tracks[i].out.at(tracks[i].prePointer) + plus + temp;
            break;
        default:
            temp = "*";

        }
        tracks[i].pointer++;
    }
    return temp;

}
string pointerMove2(track &track1) {
    string temp = "";
    string temp2 = "";
    if (track1.pointer < track1.out.length()) {

        int j = 1;
        string tempBakja = "";
        string plus = "";
        Fraction tmp;
        track1.prePointer = track1.pointer;
        switch (track1.out.at(track1.pointer)) {
        case 'v':
            while (isdigit(track1.out.at(track1.pointer + j))) {
                temp += track1.out.at(track1.pointer + j);
                j++;
            }
            track1.v = stoi(temp);
            track1.pointer += temp.length();
            temp = "v" + temp;
            break;
        case 't':
            while (isdigit(track1.out.at(track1.pointer + j))) {
                temp += track1.out.at(track1.pointer + j);
                j++;
            }
            track1.t = stoi(temp);
            track1.pointer += temp.length();
            temp = "t" + temp;
            break;
        case 'l':
            while (isdigit(track1.out.at(track1.pointer + j))) {
                temp += track1.out.at(track1.pointer + j);
                j++;
            }

            tmp.bunmo = stoi(temp);
            tmp.bunja = 1;
            while (track1.out.at(track1.pointer + j) == '.') {
                Point(tmp);
                temp += ".";
                j++;
            }
            track1.sbakja = temp;
            track1.bakja = tmp;
            track1.pointer += temp.length();
            temp = "l" + temp;
            break;
        case 'o':
            if (track1.out.at(track1.pointer + j) == '-') {
                temp += "-";
                j++;
            }
            while (isdigit(track1.out.at(track1.pointer + j))) {
                temp += track1.out.at(track1.pointer + j);
                j++;
            }
            track1.oct = stoi(temp);
            track1.pointer += temp.length();
            temp = "o" + temp;
            break;
        case '>':
            track1.oct++;
            temp = ">";
            break;
        case '<':
            track1.oct--;
            temp = "<";
            break;
        case '&':
            temp = "&";
            break;
        case 'r':
            temp = "";
            j = 1;
            while (isdigit(track1.out.at(track1.pointer + j)) != 0) {
                temp += track1.out.at(track1.pointer + j);
                j++;
            }
            if (temp.length() < 1) {
                AssignFraction(tmp, track1.bakja);
            }
            else {
                tmp.bunmo = stoi(temp);
                tmp.bunja = 1;
            }
            while (track1.out.at(track1.pointer + j) == '.') {
                Point(tmp);
                temp += ".";
                j++;
            }
            track1.pointer += temp.length();
            track1.sumbakja = addFraction(track1.sumbakja, tmp);

            track1.prenote.piano = "r";
            track1.prenote.bakja = tmp;
            temp = "r" + temp;
            break;
        case 'n':
            temp = "";
            while (isdigit(track1.out.at(track1.pointer + j))) {
                temp += track1.out.at(track1.pointer + j);
                j++;
            }
            if (stoi(temp) / 12 != track1.oct) {
                temp2 = "o" + to_string(stoi(temp) / 12) + nNotes[stoi(temp) % 12] + track1.sbakja;
            }
            else {
                temp2 = nNotes[stoi(temp) % 12] + track1.sbakja;
            }
            track1.out.replace(track1.pointer, temp.length() + 1, temp2 + "o" + to_string(track1.oct));
            track1.pointer += temp2.length() - 1;
            track1.sumbakja = addFraction(track1.sumbakja, track1.bakja);
            temp = temp2;
            break;
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'a':
        case 'b':

            j = 1;
            if (track1.out.at(track1.pointer + j) == '+') {
                j++;
                plus = '+';
            }
            else if (track1.out.at(track1.pointer + j) == '-') {
                j++;
                plus = '-';
            }


            if (track1.v == 0) {
                track1.out.replace(track1.pointer, 1 + plus.length(), "r");
                j -= plus.length();
                plus = "";
            }

            while (isdigit(track1.out.at(track1.pointer + j))) {
                temp += track1.out.at(track1.pointer + j);
                j++;
            }
            if (temp.length() < 1) {
                AssignFraction(tmp, track1.bakja);
            }
            else {
                tmp.bunmo = stoi(temp);
                tmp.bunja = 1;
            }
            while (track1.out.at(track1.pointer + j) == '.') {
                Point(tmp);
                temp += ".";
                j++;
            }
            track1.pointer += temp.length() + plus.length();
            track1.sumbakja = addFraction(track1.sumbakja, tmp);

            track1.prenote.piano = track1.out.at(track1.prePointer) + plus;
            track1.prenote.bakja = tmp;
            temp = track1.out.at(track1.prePointer) + plus + temp;
            break;
        default:
            temp = "*";

        }
        track1.pointer++;
    }
    return temp;

}


//sumbackja 높은 거 메인트랙으로
int ArrayTrack(vector<int> &TrackABC) {
    if (TrackABC.size() <= 1)return 0;
    cout << "정렬\n";
    cout << "Track size : " << TrackABC.size() << "\n";
    for (int i = 0; i < TrackABC.size(); i++)
        cout << TrackABC[i];
    for (int i = 0; i < TrackABC.size(); i++) {
        while (tracks[TrackABC[i]].pointer < tracks[TrackABC[i]].out.length()) {
            pointerMove(TrackABC[i]);
        }
        cout << "\n" << tracks[TrackABC[i]].sumbakja.bunja << "/" << tracks[TrackABC[i]].sumbakja.bunmo;
    }
    
    for (int j = 1; j < TrackABC.size(); j++) {
        if (compare(tracks[TrackABC[0]].sumbakja,tracks[TrackABC[j]].sumbakja)==2) {
            int temp;
            temp = TrackABC[0];
            TrackABC[0] = TrackABC[j];
            TrackABC[j] = temp;
        }
    }
    for (int i = 0; i < TrackABC.size(); i++) {
        cout << "\n" << i << "track\n" << "orginal :" << TrackABC[i] << "\n" << tracks[TrackABC[i]].sumbakja.bunja << "/" << tracks[TrackABC[i]].sumbakja.bunmo;   
    }

    for (int i = 0; i < TrackABC.size(); i++)
        initTrack(TrackABC[i]);
}

insertnotes CutNote(ChainedNotes basic,Fraction bak,int num) {
    string result="";
    insertnotes result2;
    track a, b;
    initTrack2(a);
    a.out = basic.snote;
    a.out += "*";
    a.bakja = basic.stL;
    a.oct = basic.stOct;
    a.t = basic.stTempo;
    a.v = basic.stVolume;
    if (num == 1) {
        result2.stL = a.bakja;
        result2.stOct = a.oct;
        result2.stTempo = a.t;
        result2.stVolume = a.v;
        while (a.pointer < a.out.length()) {
            string temp;
            temp = pointerMove2(a);
            if (temp == "*")break;
            if (compare(a.sumbakja, bak) == 0) {
                result += temp;
                break;
            }
            else if (compare(a.sumbakja, bak) == 1) {
                Fraction minus;
                string cc = "";
                minus = subFraction(a.sumbakja, bak);
                minus = subFraction(a.prenote.bakja, minus);
                cc = OptimizeBakja(minus,a.prenote.piano);
                result += cc;
                break;
            }
            else if (compare(a.sumbakja, bak) == 2) {
                result += temp;
            }
        }
    }
    else if (num == 2) {
        while (a.pointer < a.out.length()) {
            string temp;
            temp = pointerMove2(a);
            if (temp == "*")break;
            if (compare(a.sumbakja, bak) == 0) {
                result2.stL = a.bakja;
                result2.stOct = a.oct;
                result2.stTempo = a.t;
                result2.stVolume = a.v;
                while (a.pointer < a.out.length()) {
                    string temp2;
                    temp2 = pointerMove2(a);
                    if (temp2 == "*")break;
                    result += temp2;
                }
                break;
            }
            else if (compare(a.sumbakja, bak) == 1) {
                result2.stL = a.bakja;
                result2.stOct = a.oct;
                result2.stTempo = a.t;
                result2.stVolume = a.v;

                Fraction minus;
                string cc = "";
                minus = subFraction(a.sumbakja, bak);
                cc = OptimizeBakja(minus, a.prenote.piano);
                result += cc;
                while (a.pointer < a.out.length()) {
                    string temp2;
                    temp2 = pointerMove2(a);
                    if (temp2 == "*")break;
                    result += temp2;
                }
                break;
            }
            else if (compare(a.sumbakja, bak) == 2) {
                ;
            }
        }
    }
    result2.notes = result;
    result2.endL = a.bakja;
    result2.endOct = a.oct;
    result2.endTempo = a.t;
    result2.endVolume = a.v;
    return result2;
    
}
insertnotes CutNote(ChainedNotes basic, Fraction stBak,Fraction bak) 
{
    insertnotes result2;
    Fraction sumbak;
    sumbak.bunja = 0;
    sumbak.bunmo = 4;
    track a, b;
    initTrack2(a);
    a.out = basic.snote;
    a.out += "*";
    a.bakja = basic.stL;
    a.oct = basic.stOct;
    a.t = basic.stTempo;
    a.v = basic.stVolume;
    while (a.pointer < a.out.length()) {
        string temp;
        temp=pointerMove2(a);
        if (temp == "*")break;
        if (compare(a.sumbakja, stBak) == 0) {
            result2.stL = a.bakja;
            result2.stOct = a.oct;
            result2.stTempo = a.t;
            result2.stVolume = a.v;
            break;
        }
        else if (compare(a.sumbakja, stBak) == 1) {
            result2.stL = a.bakja;
            result2.stOct = a.oct;
            result2.stTempo = a.t;
            result2.stVolume = a.v;
            Fraction minus,real;
            minus = subFraction(a.sumbakja, stBak);
            real = subFraction(a.prenote.bakja, minus);

            result2.notes += OptimizeBakja(real, a.prenote.piano);
            
            sumbak = real;
            break;
        }
        else if (compare(a.sumbakja, stBak) == 2) {
            ;
        }
    }
    while (a.pointer < a.out.length()) {
        string temp;
        temp = pointerMove2(a);
        if (temp == "*")break;
        sumbak = addFraction(sumbak, a.prenote.bakja);
        if (compare(sumbak, bak) == 0) {
            result2.notes += temp;
            break;
        }
        else if (compare(sumbak, bak) == 1) {
            Fraction minus, real;
            minus = subFraction(sumbak, bak);
            real = subFraction(a.prenote.bakja, minus);
            result2.notes += OptimizeBakja(real, a.prenote.piano);
            break;

        }
        else if (compare(sumbak, bak) == 2) {
            result2.notes += temp;
        }
    }
    result2.endL = a.bakja;
    result2.endOct = a.oct;
    result2.endTempo = a.t;
    result2.endVolume = a.v;
    return result2;
}
string changeChain(ChainedNotes a, insertnotes b) {
    string result = "";
    if (compare(a.stL, b.stL) != 0) { 
        if(b.stL.bunja==1)
            result += "l" + to_string(b.stL.bunmo);
        else
            result += "l" + to_string(b.stL.bunmo/2) + ".";
    }
    if (a.stOct != b.stOct)result += "o" + to_string(b.stOct);
    if (a.stTempo != b.stTempo)result += "t" + to_string(b.stTempo);
    if (a.stVolume != b.stTempo)result += "v" + to_string(b.stVolume);
    result += b.notes;
    if (compare(a.endL, b.endL) != 0) {
        if(a.endL.bunja==1)
            result += "l" + to_string(a.endL.bunmo);
        else
            result += "l" + to_string(a.endL.bunmo/2)+".";
    }
    if (a.endOct != b.endOct)result += "o" + to_string(a.endOct);
    if (a.endTempo != b.endTempo)result += "t" + to_string(a.endTempo);
    if (a.endVolume != b.endVolume)result += "v" + to_string(a.endVolume);
    return result;
}
string changeChain(insertnotes a, insertnotes b) {
    string result = "";
    if (compare(a.stL, b.stL) != 0) { 
        if (b.stL.bunja == 1)
            result += "l" + to_string(b.stL.bunmo);
        else
            result += "l" + to_string(b.stL.bunmo / 2) + ".";
    }
    if (a.stOct != b.stOct)result += "o" + to_string(b.stOct);
    if (a.stTempo != b.stTempo)result += "t" + to_string(b.stTempo);
    if (a.stVolume != b.stTempo)result += "v" + to_string(b.stVolume);
    result += b.notes;
    if (compare(a.endL, b.endL) != 0) {
        if (a.endL.bunja == 1)
            result += "l" + to_string(a.endL.bunmo);
        else
            result += "l" + to_string(a.endL.bunmo / 2) + ".";
    }
    if (a.endOct != b.endOct)result += "o" + to_string(a.endOct);
    if (a.endTempo != b.endTempo)result += "t" + to_string(a.endTempo);
    if (a.endVolume != b.endVolume)result += "v" + to_string(a.endVolume);
    return result;
}
string NoteChangetoR(string content) {
    track a;
    string result = "";
    initTrack2(a);
    a.out = content+"*";
    while (a.pointer < a.out.length() - 1) {
        string temp;
        temp=pointerMove2(a);
        if (noteCk(temp) == 0) {
            if(temp!="&")
                result += temp;
        }
        else if (noteCk(temp) == 1) {
            temp.replace(0, a.prenote.piano.length(), "r");
            result += temp;
        }
        else if (noteCk(temp) == 2) {
            result += temp;
        }
    }
    return result;
}
void NoteToNote(vector<int> TrackABC) {
    while (tracks[TrackABC[0]].out.length() > tracks[TrackABC[0]].pointer) {
        ChainedNotes mainNote;
        vector<int> notes;
        string comnotes;
        mainNote = ChainedMove(TrackABC[0]);
        if (mainNote.snote == "*")break;
        if (noteCk(mainNote.snote)==1) {
            //다른 트랙들을 쉼표가 있는 위치와 같거나 클때까지 이동.(음만 검출)
            for (int i = 1; i < TrackABC.size(); i++) {
                ChainedNotes test;
                if (tracks[TrackABC[i]].chainNotes.snote == "") {
                    test = ChainedMove(TrackABC[i]);
                }
                while (tracks[TrackABC[i]].pointer < tracks[TrackABC[i]].out.length()) {
                    if (compare(tracks[TrackABC[i]].chainNotes.stBakja, mainNote.stBakja) == 1 && compare(tracks[TrackABC[i]].chainNotes.stBakja, tracks[TrackABC[0]].sumbakja) == 2 && noteCk(tracks[TrackABC[i]].chainNotes.snote) == 1) {
                        notes.push_back(TrackABC[i]);
                        break;
                    }
                    if (compare(tracks[TrackABC[i]].chainNotes.stBakja, tracks[TrackABC[0]].sumbakja) != 2) {
                        break;
                    }
                    test = ChainedMove(TrackABC[i]);

                }
            }
            // 겹치는 음이 한개라도 있으면,
            if (notes.size() >= 1) {

                //제일 시작지점이 작은 걸로 선택.
                Fraction min;
                int minNum = notes[0];
                min = tracks[notes[0]].chainNotes.stBakja;
                for (int i = 0; i < notes.size(); i++) {
                    if (compare(tracks[notes[i]].chainNotes.stBakja, min) == 2) {
                        min = tracks[notes[i]].chainNotes.stBakja;
                        minNum = notes[i];
                    }
                }

                //삽입할 음이 쉼표를 넘는지 계산하고 쉼표에 맞게 개조한다.
                Fraction fTemp, lTemp, mTemp, insertFrac;
                insertFrac.bunmo = 4;
                insertFrac.bunja = 0;
                insertnotes insertNote;
                fTemp = subFraction(tracks[minNum].chainNotes.stBakja, mainNote.stBakja);
                lTemp = subFraction(mainNote.sbakja, fTemp);


                if (compare(lTemp, tracks[minNum].chainNotes.sbakja) == 0) {
                    insertNote.notes = tracks[minNum].chainNotes.snote;
                    insertNote.stL = tracks[minNum].chainNotes.stL;
                    insertNote.stOct = tracks[minNum].chainNotes.stOct;
                    insertNote.stTempo = tracks[minNum].chainNotes.stTempo;
                    insertNote.stVolume = tracks[minNum].chainNotes.stVolume;
                    insertNote.endL = tracks[minNum].chainNotes.endL;
                    insertNote.endOct = tracks[minNum].chainNotes.endOct;
                    insertNote.endTempo = tracks[minNum].chainNotes.endTempo;
                    insertNote.endVolume = tracks[minNum].chainNotes.endVolume;

                    insertFrac = tracks[minNum].chainNotes.sbakja;
                }
                else if (compare(lTemp, tracks[minNum].chainNotes.sbakja) == 1) {
                    insertNote.notes = tracks[minNum].chainNotes.snote;
                    insertNote.stL = tracks[minNum].chainNotes.stL;
                    insertNote.stOct = tracks[minNum].chainNotes.stOct;
                    insertNote.stTempo = tracks[minNum].chainNotes.stTempo;
                    insertNote.stVolume = tracks[minNum].chainNotes.stVolume;
                    insertNote.endL = tracks[minNum].chainNotes.endL;
                    insertNote.endOct = tracks[minNum].chainNotes.endOct;
                    insertNote.endTempo = tracks[minNum].chainNotes.endTempo;
                    insertNote.endVolume = tracks[minNum].chainNotes.endVolume;

                    insertFrac = tracks[minNum].chainNotes.sbakja;
                }
                else if (compare(lTemp, tracks[minNum].chainNotes.sbakja) == 2) {
                    insertFrac = lTemp;
                    insertNote = CutNote(tracks[minNum].chainNotes, insertFrac, 1);

                }
                lTemp = subFraction(lTemp, insertFrac);

                //쉼표에 대입시작
                //삽입할 음이 쉼표랑 같은 위치일경우
                if (compare(tracks[minNum].chainNotes.stBakja, mainNote.stBakja) == 0) {

                    //기존 chainnote에 교체할 문자열 만들기
                    if (compare(insertFrac, mainNote.sbakja) == 0) {
                        comnotes = changeChain(mainNote, insertNote);
                    }
                    else if (compare(insertFrac, mainNote.sbakja) == 1) {
                        //이런 경우 안생김
                        cout << "error: insertFrac이 mainnote의 박자를 넘을 수 없습니다";
                    }
                    else if (compare(insertFrac, mainNote.sbakja) == 2) {
                        insertnotes a, b, c;
                        a = insertNote;
                        b = CutNote(mainNote, insertFrac, 1);
                        c = CutNote(mainNote, insertFrac, 2);
                        comnotes += changeChain(b, a) + NoteChangetoR(c.notes);
                    }
                }  //삽입할 음이 쉼표의 약간뒤에 잇을경우
                else if (compare(tracks[minNum].chainNotes.stBakja, mainNote.stBakja) == 1) {
                    Fraction front, back;
                    front = subFraction(tracks[minNum].chainNotes.stBakja, mainNote.stBakja);
                    back = subFraction(mainNote.sbakja, front);
                    insertnotes a, b, c;

                    //기존 chainnote에 교체할 문자열 만들기
                    if (compare(insertFrac, subFraction(mainNote.sbakja, front)) == 0) {
                        a = CutNote(mainNote, front, 1);
                        b = CutNote(mainNote, front, 2);
                        comnotes = a.notes + changeChain(b, insertNote);
                    }
                    else if (compare(insertFrac, subFraction(mainNote.sbakja, front)) == 1) {
                        //이런일은 생길 수 없음.
                        cout << "error: insertFrac 박자 넘침 NoteToR";
                    }
                    else if (compare(insertFrac, subFraction(mainNote.sbakja, front)) == 2) {

                        a = CutNote(mainNote, front, 1);
                        b = CutNote(mainNote, front, insertFrac);
                        c = CutNote(mainNote, addFraction(front, insertFrac), 2);
                        comnotes = a.notes + changeChain(b, insertNote) + NoteChangetoR(c.notes);
                    }

                }
                // cout <<"\n" << tracks[TrackABC[0]].chainNotes.snote<<" to " << comnotes << "\n";

                tracks[TrackABC[0]].out.replace(mainNote.stPointer, mainNote.snote.length(), comnotes);

                tracks[TrackABC[0]].sumbakja = subFraction(tracks[TrackABC[0]].sumbakja, tracks[TrackABC[0]].chainNotes.sbakja);
                tracks[TrackABC[0]].bakja = tracks[TrackABC[0]].chainNotes.stL;
                tracks[TrackABC[0]].t = tracks[TrackABC[0]].chainNotes.stTempo;
                tracks[TrackABC[0]].oct = tracks[TrackABC[0]].chainNotes.stOct;
                tracks[TrackABC[0]].v = tracks[TrackABC[0]].chainNotes.stVolume;
                tracks[TrackABC[0]].pointer = mainNote.stPointer;

                string toR = NoteChangetoR(tracks[minNum].chainNotes.snote);
                tracks[minNum].out.replace(tracks[minNum].chainNotes.stPointer, tracks[minNum].chainNotes.snote.length(), toR);
                tracks[minNum].pointer = tracks[minNum].chainNotes.stPointer + toR.length();
                //tracks[TrackABC[0]].pointer = mainNote.stPointer + comnotes.length();
            }
        }
    }
}
void NoteToR(vector<int> TrackABC) {
    while (tracks[TrackABC[0]].out.length() > tracks[TrackABC[0]].pointer) {
        ChainedNotes mainNote;
        vector<int> notes;
        string comnotes;
        mainNote = ChainedMove(TrackABC[0]);
        if (mainNote.snote == "*")break;
        if (mainNote.snote.at(0) == 'r') {
            //다른 트랙들을 쉼표가 있는 위치와 같거나 클때까지 이동.(음만 검출)
            for (int i = 1; i < TrackABC.size(); i++) {
                ChainedNotes test;
                if (tracks[TrackABC[i]].chainNotes.snote == "") {
                    test=ChainedMove(TrackABC[i]);
                }
                while (tracks[TrackABC[i]].pointer < tracks[TrackABC[i]].out.length()) {
                    if (compare(tracks[TrackABC[i]].chainNotes.stBakja, mainNote.stBakja) != 2 && compare(tracks[TrackABC[i]].chainNotes.stBakja, tracks[TrackABC[0]].sumbakja) == 2 && noteCk(tracks[TrackABC[i]].chainNotes.snote) == 1){
                        notes.push_back(TrackABC[i]);
                        break;
                    }
                    if (compare(tracks[TrackABC[i]].chainNotes.stBakja, tracks[TrackABC[0]].sumbakja) != 2) {
                        break;
                    }
                    test=ChainedMove(TrackABC[i]);
                    
                }
            }
            // 겹치는 음이 한개라도 있으면,
            if (notes.size() >= 1) {

                //제일 시작지점이 작은 걸로 선택.
                Fraction min;
                int minNum = notes[0];
                min = tracks[notes[0]].chainNotes.stBakja;
                for (int i = 0; i < notes.size(); i++) {
                    if (compare(tracks[notes[i]].chainNotes.stBakja, min) == 2) {
                        min = tracks[notes[i]].chainNotes.stBakja;
                        minNum = notes[i];
                    }
                }

                //삽입할 음이 쉼표를 넘는지 계산하고 쉼표에 맞게 개조한다.
                Fraction fTemp, lTemp, mTemp, insertFrac;
                insertFrac.bunmo = 4;
                insertFrac.bunja = 0;
                insertnotes insertNote;
                fTemp = subFraction(tracks[minNum].chainNotes.stBakja, mainNote.stBakja);
                lTemp = subFraction(mainNote.sbakja, fTemp);


                if (compare(lTemp, tracks[minNum].chainNotes.sbakja) == 0) {
                    insertNote.notes = tracks[minNum].chainNotes.snote;
                    insertNote.stL = tracks[minNum].chainNotes.stL;
                    insertNote.stOct = tracks[minNum].chainNotes.stOct;
                    insertNote.stTempo = tracks[minNum].chainNotes.stTempo;
                    insertNote.stVolume = tracks[minNum].chainNotes.stVolume;
                    insertNote.endL = tracks[minNum].chainNotes.endL;
                    insertNote.endOct = tracks[minNum].chainNotes.endOct;
                    insertNote.endTempo = tracks[minNum].chainNotes.endTempo;
                    insertNote.endVolume = tracks[minNum].chainNotes.endVolume;

                    insertFrac = tracks[minNum].chainNotes.sbakja;
                }
                else if (compare(lTemp, tracks[minNum].chainNotes.sbakja) == 1) {
                    insertNote.notes = tracks[minNum].chainNotes.snote;
                    insertNote.stL = tracks[minNum].chainNotes.stL;
                    insertNote.stOct = tracks[minNum].chainNotes.stOct;
                    insertNote.stTempo = tracks[minNum].chainNotes.stTempo;
                    insertNote.stVolume = tracks[minNum].chainNotes.stVolume;
                    insertNote.endL = tracks[minNum].chainNotes.endL;
                    insertNote.endOct = tracks[minNum].chainNotes.endOct;
                    insertNote.endTempo = tracks[minNum].chainNotes.endTempo;
                    insertNote.endVolume = tracks[minNum].chainNotes.endVolume;

                    insertFrac = tracks[minNum].chainNotes.sbakja;
                }
                else if (compare(lTemp, tracks[minNum].chainNotes.sbakja) == 2) {
                    insertFrac = lTemp;
                    insertNote=CutNote(tracks[minNum].chainNotes, insertFrac, 1);
                    
                }
                lTemp = subFraction(lTemp, insertFrac);

                //쉼표에 대입시작
                //삽입할 음이 쉼표랑 같은 위치일경우
                if (compare(tracks[minNum].chainNotes.stBakja, mainNote.stBakja) == 0) {

                    //기존 chainnote에 교체할 문자열 만들기
                    if (compare(insertFrac, mainNote.sbakja) == 0) {
                        comnotes = changeChain(mainNote,insertNote);
                    }
                    else if (compare(insertFrac, mainNote.sbakja) == 1) {
                        //이런 경우 안생김
                        cout<< "error: insertFrac이 mainnote의 박자를 넘을 수 없습니다";
                    }
                    else if (compare(insertFrac, mainNote.sbakja) == 2) {
                        insertnotes a,b,c;
                        a = insertNote;
                        b = CutNote(mainNote, insertFrac, 1);
                        c = CutNote(mainNote, insertFrac, 2);
                        comnotes += changeChain(b, a) + c.notes;
                    }
                }  //삽입할 음이 쉼표의 약간뒤에 잇을경우
                else if (compare(tracks[minNum].chainNotes.stBakja, mainNote.stBakja) == 1) {
                    Fraction front,back;
                    front = subFraction(tracks[minNum].chainNotes.stBakja, mainNote.stBakja);
                    back = subFraction(mainNote.sbakja, front);
                    insertnotes a, b, c;

                    //기존 chainnote에 교체할 문자열 만들기
                    if (compare(insertFrac, subFraction(mainNote.sbakja, front)) == 0) {
                        a = CutNote(mainNote, front, 1);
                        b = CutNote(mainNote, front, 2);
                        comnotes = a.notes+changeChain(b,insertNote);
                    }
                    else if (compare(insertFrac, subFraction(mainNote.sbakja, front)) == 1) {
                        //이런일은 생길 수 없음.
                        cout<<"error: insertFrac 박자 넘침 NoteToR";
                    }
                    else if (compare(insertFrac, subFraction(mainNote.sbakja, front)) == 2) {

                        a = CutNote(mainNote, front, 1);
                        b = CutNote(mainNote,front,insertFrac);
                        c = CutNote(mainNote, addFraction(front, insertFrac),2);
                        comnotes = a.notes + changeChain(b, insertNote) + c.notes;
                    }
                    
                }
               // cout <<"\n" << tracks[TrackABC[0]].chainNotes.snote<<" to " << comnotes << "\n";
                
                tracks[TrackABC[0]].out.replace(mainNote.stPointer, mainNote.snote.length(), comnotes);
                
                tracks[TrackABC[0]].sumbakja = subFraction(tracks[TrackABC[0]].sumbakja, tracks[TrackABC[0]].chainNotes.sbakja);
                tracks[TrackABC[0]].bakja = tracks[TrackABC[0]].chainNotes.stL;
                tracks[TrackABC[0]].t = tracks[TrackABC[0]].chainNotes.stTempo;
                tracks[TrackABC[0]].oct = tracks[TrackABC[0]].chainNotes.stOct;
                tracks[TrackABC[0]].v = tracks[TrackABC[0]].chainNotes.stVolume;
                tracks[TrackABC[0]].pointer = mainNote.stPointer;
                
                string toR = NoteChangetoR(tracks[minNum].chainNotes.snote);
                tracks[minNum].out.replace(tracks[minNum].chainNotes.stPointer, tracks[minNum].chainNotes.snote.length(),toR );
                tracks[minNum].pointer = tracks[minNum].chainNotes.stPointer + toR.length();
                //tracks[TrackABC[0]].pointer = mainNote.stPointer + comnotes.length();
            }
        }
    }
}
void inputHW(vector<int> TrackABC)
{
    
    while (tracks[TrackABC[0]].pointer < tracks[TrackABC[0]].out.length() - 1) {
        vector<int> notes;
        ChainedNotes mainNote;
        mainNote = ChainedMove(TrackABC[0]);
        if (noteCk(mainNote.snote) == 1) {
            for (int i = 1; i < TrackABC.size(); i++) {
                ChainedNotes test;
                if (tracks[TrackABC[i]].chainNotes.snote == "") {
                    test = ChainedMove(TrackABC[i]);
                }
                while (tracks[TrackABC[i]].pointer < tracks[TrackABC[i]].out.length()) {     
                    if (noteCk(tracks[TrackABC[i]].chainNotes.snote) == 1 && compare(tracks[TrackABC[i]].chainNotes.stBakja, mainNote.stBakja) == 0) {
                        notes.push_back(TrackABC[i]);
                        break;
                    }
                    if (compare(tracks[TrackABC[i]].chainNotes.stBakja, mainNote.stBakja) == 1) {
                        break;
                    }
                    test = ChainedMove(TrackABC[i]);
                }
            }
            if (notes.size() >= 1) {
                string results = "";
                for (int i = 0; i < notes.size(); i++) {
                    ChainedNotes insertChain = tracks[notes[i]].chainNotes;
                    results += ":";
                    //화음시작지점 동기화
                        
                    if (insertChain.stL.bunja != 1) {
                        results += "l"+to_string(insertChain.stL.bunmo / 2) + ".";
                    }
                    else {
                        results += "l"+to_string(insertChain.stL.bunmo);
                    }
                    results += "t" + to_string(insertChain.stTempo);
                    results += "o" + to_string(insertChain.stOct);
                    results += "v" + to_string(insertChain.stVolume);

                    //박자가 같으면?
                    if (compare(mainNote.sbakja, insertChain.sbakja) == 0) {
                        results += insertChain.snote;
                    }
                    //메인이 더 크면?
                    else if (compare(mainNote.sbakja, insertChain.sbakja) == 1) {
                        //손 봐야됨
                        

                        //템포계산 후 음 연장
                        vector<Tempos> tempoT;
                        track temper,temper2;
                        initTrack2(temper);
                        temper.bakja = mainNote.stL;
                        temper.oct = mainNote.stOct;
                        temper.t = mainNote.stTempo;
                        temper.out = mainNote.snote + "*";
                        while (temper.pointer < temper.out.length() - 1) {
                            string tempda=pointerMove2(temper);
                            if (tempda.at(0) == 't') {
                                string tempoTemp = "";
                                for (int j = 1; j < tempda.length(); j++) {
                                    tempoTemp += tempda.at(j);
                                }
                                Tempos tempoTo;
                                tempoTo.speed = stoi(tempoTemp);
                                tempoTo.backja = temper.sumbakja;
                                tempoT.push_back(tempoTo);
                            }
                        }
                        initTrack2(temper2);
                        temper2.bakja = insertChain.stL;
                        temper2.oct = insertChain.stOct;
                        temper2.t = insertChain.stTempo;
                        temper2.out = insertChain.snote +"*";
                        while (temper2.pointer < temper2.out.length() - 1) {
                            pointerMove2(temper2);
                        }
                        temper2.out.insert(temper2.pointer, "&" + OptimizeBakja(subFraction(mainNote.sbakja,insertChain.sbakja), temper2.prenote.piano));
                        initTrack2(temper2);
                        temper2.bakja = insertChain.stL;
                        temper2.oct = insertChain.stOct;
                        temper2.t = insertChain.stTempo;
                        ////////여기부터 해야댐 tempoT 쑤셔넣기 및 insertchain.end 바꿔주기.
                        for (int j = 0; j < tempoT.size(); j++) {
                            if (compare(tempoT[j].backja, insertChain.sbakja) != 2) {
                                while (temper2.pointer < temper2.out.length() - 1) {
                                    string tempeResult = pointerMove2(temper2);
                                    if (compare(temper2.sumbakja, tempoT[j].backja) == 0) {
                                        temper2.out.insert(temper2.pointer, "t" + to_string(tempoT[j].speed));
                                        break;
                                    }
                                    else if (compare(temper2.sumbakja, tempoT[j].backja) == 1) {
                                        Fraction div1;
                                        string divresult = "",divresult2="";
                                        div1 = subFraction(temper2.sumbakja, tempoT[j].backja);
                                        
                                        divresult = OptimizeBakja(subFraction(temper2.prenote.bakja, div1), temper2.prenote.piano) + "&" + "t" + to_string(tempoT[j].speed);
                                        divresult2= OptimizeBakja(div1, temper2.prenote.piano) ;
                                        temper2.out.replace(temper2.prePointer, tempeResult.length(), divresult + divresult2);
                                        temper2.sumbakja = subFraction(temper2.sumbakja, div1);
                                        temper2.pointer = temper2.prePointer + divresult.length();
                                        break;
                                    }
                                    else if (compare(temper2.sumbakja, tempoT[j].backja) == 2) {
                                        ;
                                    }
                                }
                            }
                            insertChain.endTempo = tempoT[j].speed;
                        }
                        temper2.out.erase(temper2.out.length() - 1);
                        results += temper2.out;
                    }
                    else if (compare(mainNote.sbakja, insertChain.sbakja) == 2) {
                        insertnotes a;
                        a=CutNote(insertChain, mainNote.sbakja, 1);
                        results += a.notes;

                        insertChain.endL = a.endL;
                        insertChain.endOct = a.endOct;
                        insertChain.endTempo = a.endTempo;
                        insertChain.endVolume = a.endVolume;
                    }
                    //화음 끝지점 동기화
                    if (i == notes.size() - 1) {

                        if (mainNote.endL.bunja != 1) {
                            results += "l" + to_string(mainNote.endL.bunmo / 2) + ".";
                        }
                        else {
                            results += "l" + to_string(mainNote.endL.bunmo);
                        }

                        results += "o" + to_string(mainNote.endOct);
                        results += "t" + to_string(mainNote.endTempo);
                        results += "v" + to_string(mainNote.endVolume);
                    }
                }
                tracks[TrackABC[0]].out.insert(tracks[TrackABC[0]].pointer, results);
                tracks[TrackABC[0]].pointer += results.length();
                
            }
        }

    }
}
void NoteToVibrate(vector<int> TrackABC) {

    for (int i = 0; i < TrackABC.size(); i++) {
        initTrack2(tracks[TrackABC[i]]);
    }
    while (tracks[TrackABC[0]].pointer < tracks[TrackABC[0]].out.length() - 1) {
        string temp = pointerMove2(tracks[TrackABC[0]]);
        if (noteCk(temp) == 1) {
            tracks[TrackABC[0]].out.insert(tracks[TrackABC[0]].pointer, "*2");
            tracks[TrackABC[0]].pointer += 2;
        }
    }
    for (int i = 0; i < TrackABC.size(); i++) {
        initTrack2(tracks[TrackABC[i]]);
    }
}
void NoteToForce(vector<int> TrackABC) {

    for (int i = 0; i < TrackABC.size(); i++) {
        initTrack2(tracks[TrackABC[i]]);
    }
    while (tracks[TrackABC[0]].pointer < tracks[TrackABC[0]].out.length() - 1) {
        string temp = pointerMove2(tracks[TrackABC[0]]);
        if (noteCk(temp) == 1) {
            tracks[TrackABC[0]].out.insert(tracks[TrackABC[0]].pointer, "*14");
            tracks[TrackABC[0]].pointer += 2;
        }
    }
    for (int i = 0; i < TrackABC.size(); i++) {
        initTrack2(tracks[TrackABC[i]]);
    }
}
void NoteToDrum(vector<int> TrackABC) {
    for (int i = 0; i < TrackABC.size(); i++) {
        initTrack2(tracks[TrackABC[i]]);
    }
    while (tracks[TrackABC[0]].pointer < tracks[TrackABC[0]].out.length() - 1) {
        string temp = pointerMove2(tracks[TrackABC[0]]);
        if (noteCk(temp) == 1) {
            if ((tracks[TrackABC[0]].oct == 1 && tracks[TrackABC[0]].prenote.piano == "b") || (tracks[TrackABC[0]].oct == 2 && tracks[TrackABC[0]].prenote.piano == "c")) {
                tracks[TrackABC[0]].out.replace(tracks[TrackABC[0]].prePointer, 1, "o4c");
                tracks[TrackABC[0]].pointer += 2;
            }
            else if (tracks[TrackABC[0]].oct == 2 && (tracks[TrackABC[0]].prenote.piano == "d" || tracks[TrackABC[0]].prenote.piano == "e")) {
                tracks[TrackABC[0]].out.replace(tracks[TrackABC[0]].prePointer, 1, "o4c+");
                tracks[TrackABC[0]].pointer += 3;
            }
            else if (tracks[TrackABC[0]].oct == 2 && tracks[TrackABC[0]].prenote.piano == "f+") {
                tracks[TrackABC[0]].out.replace(tracks[TrackABC[0]].prePointer, 2, "o4f");
                tracks[TrackABC[0]].pointer += 1;
            }
            else if (tracks[TrackABC[0]].oct == 2 && tracks[TrackABC[0]].prenote.piano == "c+") {
                tracks[TrackABC[0]].out.replace(tracks[TrackABC[0]].prePointer, 2, "o5d");
                tracks[TrackABC[0]].pointer += 1;
            }
            else if (tracks[TrackABC[0]].oct == 2 && tracks[TrackABC[0]].prenote.piano == "a+") {
                tracks[TrackABC[0]].out.replace(tracks[TrackABC[0]].prePointer, 2, "o4f+");
                tracks[TrackABC[0]].pointer += 2;
            }
            else if (tracks[TrackABC[0]].oct == 3 && tracks[TrackABC[0]].prenote.piano == "c+") {
                tracks[TrackABC[0]].out.replace(tracks[TrackABC[0]].prePointer, 2, "o4g");
                tracks[TrackABC[0]].pointer += 1;
            }
            else if (tracks[TrackABC[0]].oct == 3 && tracks[TrackABC[0]].prenote.piano == "a") {
                tracks[TrackABC[0]].out.replace(tracks[TrackABC[0]].prePointer, 1, "o4g+");
                tracks[TrackABC[0]].pointer += 3;
            }
            else if (tracks[TrackABC[0]].oct == 3 && tracks[TrackABC[0]].prenote.piano == "d+") {
                tracks[TrackABC[0]].out.replace(tracks[TrackABC[0]].prePointer, 2, "o4a");
                tracks[TrackABC[0]].pointer += 1;
            }
            else {
                tracks[TrackABC[0]].out.replace(tracks[TrackABC[0]].prePointer, 1, "o4c+");
                tracks[TrackABC[0]].pointer += 4-tracks[TrackABC[0]].prenote.piano.length();
            }
            
        }
    }
    for (int i = 0; i < TrackABC.size(); i++) {
        initTrack2(tracks[TrackABC[i]]);
    }
}
void chaintest(int num) {
    int limit = tracks[num].out.length() - 1;
    while (tracks[num].pointer < limit) {
        ChainedNotes temp;
        temp = ChainedMove(num);
       cout << "\n" << temp.snote << "\n";
       if (temp.snote == "") {
           cout << "?";
       }
       
    }
}
int CompressTrack(vector<int> TrackABC)
{
    if (TrackABC.size() <= 1)return 0;
    //chaintest(1);
    for (int i = 0; i < TrackABC.size(); i++) {
        NoteToR(TrackABC);
        initTrack(trackcount);
        NoteToNote(TrackABC);
        initTrack(trackcount);
    }   
    inputHW(TrackABC);
        
    return 0;

}


ChainedNotes ChainedMove(int t) {
    ChainedNotes note;
    string temp = "", chaintext = "";
    int chained = 0;
   
    note.stPointer = tracks[t].pointer;
    note.stOct = tracks[t].oct;
    note.stVolume = tracks[t].v;
    note.stTempo = tracks[t].t;
    note.stBakja = tracks[t].sumbakja;
    note.sbakja.bunmo = 4;
    note.sbakja.bunja = 0;
    note.stsL = tracks[t].sbakja;
    note.stL = tracks[t].bakja;
    note.snote = "";

    int ont = 0;
    int toct, ttempo, tVolume;
    Fraction tL;
    toct = tracks[t].oct;
    ttempo = tracks[t].t;
    tVolume = tracks[t].v;
    tL = tracks[t].bakja;

    temp = pointerMove(t);
    /*
    if (tracks[t].pointer>= tracks[t].out.length()) {
        cout << "end";
        return note;
    }*/



    string exp = "";
    int exCK = 0; //표현식만 있는지 검사(볼륨,옥타브 등)
    switch (temp.at(0)) {
    case 'o':
    case '<':
    case '>':
    case 'l':
    case 't':
    case 'v':
        note.stOct = tracks[t].oct;
        note.stVolume = tracks[t].v;
        note.stTempo = tracks[t].t;
        note.stL = tracks[t].bakja;

        toct = tracks[t].oct;
        ttempo = tracks[t].t;
        tVolume = tracks[t].v;
        tL = tracks[t].bakja;
        exCK = 1;
        note.snote = temp;
        break;
    case 'r':
        note.sbakja = addFraction(note.sbakja, tracks[t].prenote.bakja);
        note.snote += temp;
        chained = 1;
        while (true) {

            if (exp == "" && ont==1) {
                toct = tracks[t].oct;
                ttempo = tracks[t].t;
                tVolume = tracks[t].v;
                tL = tracks[t].bakja;
            }
            ont = 1;
            temp=pointerMove(t);   
            if (tracks[t].pointer >= tracks[t].out.length())break;
            switch (temp.at(0)) {
            case 'o':
            case 'l':
            case 't':
            case 'v':
            case '<':
            case '>':
                exp += temp;
                if (noteCk(tracks[t].out.at(tracks[t].pointer))==1 || tracks[t].out.at(tracks[t].pointer) == '*') {
                    exp = "";
                    chained = 0;
                    break;
                }
                else if (tracks[t].out.at(tracks[t].pointer) == 'r') {
                    note.snote += exp;
                    exp = "";
                }
                
                break;
            case 'r':
                note.snote += temp;
                note.sbakja = addFraction(note.sbakja, tracks[t].prenote.bakja);
                break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
                tracks[t].sumbakja= subFraction(tracks[t].sumbakja, tracks[t].prenote.bakja);
                chained = 0;
                break;
            }
            if(chained==0)break;

        }
        break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
        note.sbakja = addFraction(note.sbakja, tracks[t].prenote.bakja);
        note.snote += temp;
        chained = 0;
        while (true) {
            if (exp == "" && ont == 1) {
                toct = tracks[t].oct;
                ttempo = tracks[t].t;
                tVolume = tracks[t].v;
                tL = tracks[t].bakja;
            }
            ont = 1;
            temp = pointerMove(t);
            if (tracks[t].pointer >= tracks[t].out.length())break;
            switch (temp.at(0)) {
            case 'o':
            case 'l':
            case 't':
            case 'v':
            case '<':
            case '>':
                exp += temp;
                if (tracks[t].out.at(tracks[t].pointer) == 'r' || tracks[t].out.at(tracks[t].pointer) == 'a' || tracks[t].out.at(tracks[t].pointer) == 'b' || tracks[t].out.at(tracks[t].pointer) == 'c' || tracks[t].out.at(tracks[t].pointer) == 'd' || tracks[t].out.at(tracks[t].pointer) == 'e' || tracks[t].out.at(tracks[t].pointer) == 'f' || tracks[t].out.at(tracks[t].pointer) == 'g' || tracks[t].out.at(tracks[t].pointer) == '*') {
                    if (chained == 1) {
                        note.snote += exp;
                        exp = "";
                    }
                    else {
                        chained = -1;
                        exp = "";
                    }
                    break;
                }
                else if(tracks[t].out.at(tracks[t].pointer) == '&'){
                    note.snote += exp;
                    exp = "";
                } 
                break;
            case '&':
                note.snote += '&';   
                chained = 1;
                break;
            case 'r':
                chained = -1;
                tracks[t].sumbakja = subFraction(tracks[t].sumbakja, tracks[t].prenote.bakja);
                break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
                if (chained == 1) {
                    note.snote += temp;
                    note.sbakja = addFraction(note.sbakja, tracks[t].prenote.bakja);
                    chained = 0;
                }
                else {
                    tracks[t].sumbakja = subFraction(tracks[t].sumbakja, tracks[t].prenote.bakja);
                    chained = -1;
                }
                break;
            case '*':
                break;

            }
            if (chained == -1)break;
        }
        break;
    case '*':
        note.snote = "*";
        break;
    default:
        cout << "ChainError :" << temp;
        note.snote = temp;
    }
    tracks[t].pointer = note.stPointer + note.snote.length();
    note.endL = tL;
    note.endOct = toct;
    note.endTempo = ttempo;
    note.endVolume = tVolume;
    tracks[t].bakja = tL;
    tracks[t].oct = toct;
    tracks[t].t = ttempo;
    tracks[t].v = tVolume;
    tracks[t].chainNotes = note;
    return note;
}


void AssignFraction(Fraction &into,Fraction backja)
{
    into.bunja = backja.bunja;
    into.bunmo = backja.bunmo;
}

Fraction Point(Fraction &bakja)
{
    Fraction temp;
    temp.bunmo = bakja.bunmo * 2;
    temp.bunja = bakja.bunja;
    temp = addFraction(temp, bakja);
    AssignFraction(bakja, temp);
    return bakja;
}


string OptimizeBakja(Fraction x,string piano) {
    x=trimFraction(x);
    string temp = "";
    Fraction cal;
    string mpercent = "&";
    if (piano == "r")mpercent = "";
    cal = x;
    for (int i = 0; i <= 7; i++) {
        if (x.bunmo < (int)pow(2, i))break;
        Fraction t1,t2;
        t1.bunmo = pow(2, i);
        t1.bunja = 1;
        t2 = t1;
        Point(t1);
        while (subFraction(cal, t2).bunja >=0) {
            if (subFraction(cal, t1).bunja >= 0) {
                cal = subFraction(cal, t1);
                if (temp != "")temp += mpercent;
                temp += piano + to_string((int)pow(2, i)) + ".";
            }
            else if (subFraction(cal, t2).bunja >= 0) {
                cal = subFraction(cal, t2);
                if (temp != "")temp += mpercent;
                temp += piano + to_string((int)pow(2, i));
            }
        }   
    }

    Fraction t1;
    t1.bunja = 1;
    t1.bunmo = cal.bunmo;
    for (int i = 0; i < cal.bunja; i++) {      
        if (temp != "")temp += mpercent;
        temp += piano + to_string(cal.bunmo);    
        cal = subFraction(cal, t1);

    }
        
    if (cal.bunja > 0)cout << " 박자 최적화에 문제가 있습니다. 해당 박자 :" + to_string(x.bunja) + "/" + to_string(x.bunmo);
    return temp;
}

Fraction addFraction(Fraction x, Fraction y) {
    Fraction z;
    z.bunmo = x.bunmo * y.bunmo;
    z.bunja = y.bunmo * x.bunja + x.bunmo * y.bunja;
    return trimFraction(z);
}
Fraction subFraction(Fraction x, Fraction y) {
    Fraction z;
    z.bunmo = x.bunmo * y.bunmo;
    z.bunja = y.bunmo * x.bunja - x.bunmo * y.bunja;
    return trimFraction(z);
}
Fraction multFraction(Fraction x, Fraction y) {
    Fraction z;
    z.bunmo = x.bunmo * y.bunmo;
    z.bunja = y.bunja * x.bunja;
    return trimFraction(z);
}
Fraction divFraction(Fraction x, Fraction y) {
    Fraction z;
    z.bunmo = x.bunmo * y.bunja;
    z.bunja = y.bunmo * x.bunja;
    return trimFraction(z);
}
int compare(Fraction x, Fraction y) {
    if (y.bunmo * x.bunja - x.bunmo * y.bunja == 0)
        return 0;
    if (y.bunmo * x.bunja - x.bunmo * y.bunja > 0)
        return 1;
    else
        return 2;
}
Fraction trimFraction(Fraction x) {
    int down = x.bunja, up = x.bunmo, com=1;
    if (x.bunja > x.bunmo) {
        down = x.bunmo;
        up = x.bunja;
    }

    for (int i = 1; i <= down; i++)
        if ((down % i) == 0 && (up % i) == 0)
            com = i;

    x.bunja /= com;
    x.bunmo /= com;
    return x;
}
