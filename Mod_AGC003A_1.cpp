#include <fstream>
#include <bits/stdc++.h>

using namespace std; fstream routeFile("route.txt", fstream::out|fstream::trunc);

int main(){ routeFile <<  to_string(__LINE__) << " ";
    string s; routeFile <<  to_string(__LINE__) << " ";
    cin >> s; routeFile <<  to_string(__LINE__) << " ";
    int Up = 0,Left = 0; routeFile <<  to_string(__LINE__) << " ";
    for(int i = 0; i < s.length(); i++){ routeFile <<  to_string(__LINE__) << " ";
        if(s[i]=='N'){ routeFile <<  to_string(__LINE__) << " ";
            Up++; routeFile <<  to_string(__LINE__) << " ";
        }else if(s[i]=='S'){ routeFile <<  to_string(__LINE__) << " ";
            Up--; routeFile <<  to_string(__LINE__) << " ";
        }else if(s[i]=='E'){ routeFile <<  to_string(__LINE__) << " ";
            Left++; routeFile <<  to_string(__LINE__) << " ";
        }else{  routeFile <<  to_string(__LINE__) << " ";
            Left--; routeFile <<  to_string(__LINE__) << " ";
        }
    }
    cout << ((Up==0&&Left==0)?"Yes":"No"); routeFile <<  to_string(__LINE__) << " ";
}
