#include <fstream>
#include <bits/stdc++.h>
using namespace std; fstream routeFile("route.txt", fstream::out|fstream::trunc);

int main() { routeFile <<  to_string(__LINE__) << " ";
  int north=0, west=0; routeFile <<  to_string(__LINE__) << " ";
  string s; routeFile <<  to_string(__LINE__) << " ";
  cin >> s; routeFile <<  to_string(__LINE__) << " ";
  int n = s.size(); routeFile <<  to_string(__LINE__) << " ";
  for (int i = 0; i < n; i++) { routeFile <<  to_string(__LINE__) << " ";
    if (s.at(i) == 'N'){ routeFile <<  to_string(__LINE__) << " ";
      if (north < 2) north++; routeFile <<  to_string(__LINE__) << " ";
    }
    else if (s.at(i)=='S') { routeFile <<  to_string(__LINE__) << " ";
      if (north < 2) north++; routeFile <<  to_string(__LINE__) << " ";
    }
    else if (s.at(i) == 'W') { routeFile <<  to_string(__LINE__) << " ";
      if (west < 2) west++; routeFile <<  to_string(__LINE__) << " ";
    }
    else if (s.at(i) == 'E') { routeFile <<  to_string(__LINE__) << " ";
      if (west < 2) west++; routeFile <<  to_string(__LINE__) << " ";
    }
  }
  if (north%2==0 && west%2==0) { routeFile <<  to_string(__LINE__) << " ";
      cout << "Yes" << endl; routeFile <<  to_string(__LINE__) << " ";
  } else { routeFile <<  to_string(__LINE__) << " ";
      cout << "No" << endl; routeFile <<  to_string(__LINE__) << " ";
  }
}


