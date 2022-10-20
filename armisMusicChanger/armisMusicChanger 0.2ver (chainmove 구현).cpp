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
struct Notes {
    string piano="";
    Fraction bakja;
};
struct ChainedNotes {
    int stOct, stTempo, stVolume,stPointer;
    Fraction sbakja;
    string snote = "";
};
struct track {
    int v=12;
    int t=0;
    int oct = 4;
    Fraction bakja;
    string sbakja="4";
    Fraction sumbakja;
    int pointer=0,prePointer=0;
    Notes prenote;
    ifstream file;
    ofstream tofile;
    string out="";

};
Fraction addFraction(Fraction x, Fraction y);
Fraction subFraction(Fraction x, Fraction y);
Fraction multFraction(Fraction x, Fraction y);
Fraction divFraction(Fraction x, Fraction y);
Fraction trimFraction(Fraction x);
string pointerMove(int i);
Fraction Point(Fraction& bakja);
void AssignFraction(Fraction& into, Fraction backja);
int compare(Fraction x, Fraction y);
string OptimizeBakja(Fraction x,string piano);
ChainedNotes ChainedMove(int t);
int InsertToR(vector<int> TrackABC);
int ArrayTrack(vector<int> &TrackABC);
int CompressTrack(vector<int> TrackABC);

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
    }
}
void initAbcTrack(int size) {
    //포인터, 박자합 l박자 초기화
    for (int i = 0; i <size; i++) {
        abcTrack[i].pointer = 0;
        abcTrack[i].sumbakja.bunja = 0;
        abcTrack[i].sumbakja.bunmo = 4;

        abcTrack[i].bakja.bunja = 1;
        abcTrack[i].bakja.bunmo = 4;
        abcTrack[i].oct = 4;
        abcTrack[i].sbakja = "4";
    }
}

void testfuc() {
    Fraction t1;
    string temp = "aadda";
    t1.bunja = 15;
    t1.bunmo = 16;
    cout << OptimizeBakja(t1, "c");
    temp.replace(1, 1, "a16&a16");
    cout << temp;
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
    if (extrack > 0) {
        while (tracks[extrack].pointer < tracks[extrack].out.length()) {
            string temp = "";
            
            temp = pointerMove(extrack);
            if (temp.at(0) == 't') {
                tTempo.backja = tracks[extrack].sumbakja;
                tTempo.speed = tracks[extrack].t;
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
                    tTempo.speed = tracks[i].t;
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
    if (maxvolume < 15) {
        int volumeup = 15 - maxvolume;
        minvolume += volumeup;
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
                    temp = to_string(stoi(temp) + volumeup);
                    if (minvolume < 10) {
                        temp=to_string(stoi(temp) + (15 - stoi(temp)) / 2);
                    }
                    tracks[i].out.replace(tracks[i].pointer + 1, j, temp);
                    tracks[i].pointer += temp.length();
                    cout << to_string(tracks[i].pointer) + "번째 v의 음량이" + temp + "가 되었습니다\n";

                }
                tracks[i].pointer++;
            }
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
   
    CompressTrack(abcTrackNumber[0]);
    
    
    
    

    //읽기 전용으로 연 파일들 닫아주기.
    for(int i = 1; i <= trackcount; i++) {
        tracks[i].file.close();
    }

    //쓰기 전용으로 파일들 다시 열어서 개조한 문자열 대입시켜주기
    for (int i = 1; i <= trackcount; i++) {
        tracks[i].tofile.open("test" + to_string(i) + ".txt", ifstream::out);
        if (tracks[i].tofile.is_open())    //파일이 열렸는지 확인
        {
            tracks[i].tofile << tracks[i].out;
        }
        tracks[i].tofile.close();
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


int CompressTrack(vector<int> TrackABC)
{
    //if (TrackABC.size() <= 1)return 0;
    for (int i = 0; i < TrackABC.size(); i++) {
        cout << "track" << TrackABC[i]<<"\n";
        while (tracks[TrackABC[i]].out.length() > tracks[TrackABC[i]].pointer) {
            ChainedNotes temp;
            temp=ChainedMove(TrackABC[i]);
            cout << "\n" << temp.snote;

        }
        
    }
    return 0;

}
//메인트랙의 쉼표부근 삽입하기.
int InsertToR(vector<int> TrackABC) {
    if (TrackABC.size() <= 1)return 0;
    

}


ChainedNotes ChainedMove(int t) {
    ChainedNotes note;
    string temp = "", chaintext = "";
    int chained = 0;
    
    temp = pointerMove(t);
    note.stPointer = tracks[t].prePointer;
    note.stOct = tracks[t].oct;
    note.stVolume = tracks[t].v;
    note.stTempo = tracks[t].t;
    note.sbakja.bunmo = 4;
    note.sbakja.bunja = 0;
    note.snote = "";

    if (tracks[t].pointer>= tracks[t].out.length()) {
        cout << "end";
        return note;
    }



    string exp = "";
    switch (temp.at(0)) {
    case 'o':
    case '<':
    case '>':
    case 'l':
    case 't':
    case 'v':
        note.snote = temp;
        break;
    case 'r':
        note.sbakja = addFraction(note.sbakja, tracks[t].prenote.bakja);
        note.snote += temp;
        chained = 1;
        while (true) {
            if (tracks[t].pointer >= tracks[t].out.length())break;
            temp=pointerMove(t);   
            switch (temp.at(0)) {
            case 'o':
            case 'l':
            case 't':
            case 'v':
            case '<':
            case '>':
                exp += temp;
                if (tracks[t].out.at(tracks[t].pointer) == 'a' || tracks[t].out.at(tracks[t].pointer) == 'b' || tracks[t].out.at(tracks[t].pointer) == 'c' || tracks[t].out.at(tracks[t].pointer) == 'd' || tracks[t].out.at(tracks[t].pointer) == 'e' || tracks[t].out.at(tracks[t].pointer) == 'f' || tracks[t].out.at(tracks[t].pointer) == 'g' || tracks[t].out.at(tracks[t].pointer) == '*') {
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
                        exp += temp;

                    }
                    else {
                        chained = -1;
                        exp = "";
                    }
                    break;
                }
                else if(tracks[t].out.at(tracks[t].pointer) == '&'){
                    note.snote += exp;
                } 
                break;
            case '&':
                note.snote += temp;
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
        cout << "\nendp";
        break;
    default:
        cout << "ChainError :" << temp;
        note.snote = temp;
    }
    tracks[t].pointer = note.stPointer + note.snote.length();

    return note;
}

// i트랙의 포인터에 해당하는 음표에 j트랙의 포인터에 해당하는 음표를 덧붙일 수 있다면, 연결값을 반환한다.
string notearrange(int i, int j) {
    return "";
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


void testfunc() {
    Fraction test,test2,test3;
    test.bunmo = 5;
    test.bunja = 3;
    test2.bunmo = 6;
    test2.bunja= 2;
    test3 = addFraction(test, test2);
    cout << test3.bunja + "/" + test3.bunmo;

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
    return z;
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
