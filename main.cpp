#include <bits/stdc++.h>
using namespace std;
int numar_terminale, numar_neterminale, numar_productii;
set<char> terminale;
set<char> neterminale;
set<string> productii[128];
char start;

void citire(){

    cin >> start;

    cin >> numar_neterminale;
    for(int i = 0; i < numar_neterminale; i++)
    {
        char n;
        cin >> n;
        neterminale.insert(n);
    }

    cin >> numar_terminale;
    for(int i = 0; i < numar_terminale; i++)
    {
        char t;
        cin >> t;
        terminale.insert(t);
    }

    cin >> numar_productii;
    for (int i = 0; i < numar_productii; i++){
        int nr_tranzitii;
        cin >> nr_tranzitii;
        char e;
        cin >> e;
        for(int j = 0; j < nr_tranzitii; j++){
            string x;
            cin >> x;
            productii[int(e)].insert(x);
        }
    }
}

void afisare(){
    cout << "Productiile sunt: ";
    cout << '\n';
    for (int i = 0; i < 128; i++)
        if(!productii[i].empty()){
            cout << char(i) << "->";
            for(const auto& x : productii[i])
                cout << x << "|";
            cout <<'\n';
        }
}

//eliminare Productii Lambda
void eliminareProductiiLambda(){
    int ok = 0; //cat timp inca avem productii vide
    while (ok == 0){
        ok = 1;
        for (int i = 0; i < 128; i++)
            if (!productii[i].empty()) //pt productii nevide
                for(auto x : productii[i]){   //pentru fiecare string din productie
                    if(x.find('$') != string::npos) //gasesc lambda in productiile caracterului char(i)
                    {  ok = 0; //am gasit unul

                        if (productii[i].size() == 1) { //daca e unica productie
                            for(int j = 0; j < 128; j++)  //caut prin toate celelalte caractere
                                if(!productii[j].empty()) //daca e nevida
                                    for (auto y : productii[j]) //caut char(i) printre productii
                                        if(y.find(char(i)) != string::npos)
                                            if(y.size() >= 2) //daca are mai mult de doua simboluri
                                            {
                                                y.erase(y.find(char(i)), 1); //sterg caracterul din productie
                                            } else
                                            {   y.clear();  //MODIFICAT
                                                y = "$";} //daca are un simbol, adaug lambda
                        }
                        else { //daca sunt mai multe productii ale lui T
                                             //elimin productia
                            for (int j = 0; j < 128; j++)
                                if (!productii[j].empty())
                                    for (auto y : productii[j]) //caut T printre productii
                                        if(y.find(char(i)) != string::npos)
                                            if(y.size() >= 2){  //pentru productiile cu mai mult de 2 simboluri, scot T
                                                string temporar = y;
                                                temporar.erase(temporar.find(char(i)), 1);
                                                productii[j].insert(temporar);
                                            }
                        }
                        set<string> nou;
                        for (auto y : productii[i])
                            if(y.find('$') == string::npos)
                                nou.insert(y);
                        productii[i].clear(); //schimb cu copia
                        for (auto y : nou)
                            productii[i].insert(y);
                    }
        }
    }

}

void redenumire() {
    int ok = 0;
    while (ok == 0) {
        for (int i = 0; i < 128; i++)
            if (!productii[i].empty()) {
                set<string> nou;
                for (auto x : productii[i])
                    if(x.size() == 1)
                    {for (auto y : neterminale)
                        if (x[0] != y)
                            nou.insert(x);
                        }
                if (productii[i] == nou)
                    ok = 1;
                productii[i].clear();
                for (auto y : nou)
                    productii[i].insert(y);

            }
    }
}
//Pasul 2: Eliminam redenumirile
void eliminareaRedenumirilor(){
    //Elimin starile X->X
    for(int i = 0; i < 128; i++)
        if(!productii[i].empty()){
            set<string> nou;
            for (auto x: productii[i])
                if(x.size() != 1 || x[0]!=char(i))
                    nou.insert(x);
            productii[i].clear();
            for(auto x : nou)
                productii[i].insert(x);
        }

    set<string> copie[128];
    //Inlocuiesc X->Y
    bool redenumiri = false;
    while(!redenumiri)
    {   redenumiri = true;
        for(int i = 0; i < 128; i++) {
            copie[i] = productii[i];
            productii[i].clear();
        if (!copie[i].empty())
            for (auto x : copie[i])
                if(x.size() == 1 && neterminale.find(x[0])!= neterminale.end())
                    {
                        for(auto z : productii[int(x[0])])
                            productii[i].insert(z);
                        redenumiri = false;
                    }
                else
                    productii[i].insert(x);

    }

    }

}
int Infinite(string a, set<string> &folosite){
    queue<string> s;
    folosite.insert(a);
    for (auto x : productii[int(a[0])]){
        for (auto y : x){
            string temp;
            temp = y;
            if(folosite.find(temp) != folosite.end())
                {s.push(x);
                break;}
            for (auto z : neterminale)
                if (z == y){
                    string u;
                    u = y;
                    Infinite(u, folosite);}
    }
    }
}
//Pasul 3: Eliminarea productiilor inutile

void eliminareProductiiInutile(){
    set<string> folosite;
    string t;
    t = start;
    Infinite(t, folosite);
    set<char> Eliminare;
    for(int i = 0; i < 128; i++)
        if(!productii[i].empty()) {
            char c = i;
            string d;
            d = c;
            if (folosite.find(d)== folosite.end())
                    Eliminare.insert(d[0]);
        }

    for(int i = 0; i < 128; i++)
        if(!productii[i].empty())
    {
        int sters = 1;
        for(auto x : productii[i])
            if(x.find(char(i)))
                sters = 0;
        if (sters!= 0)
            Eliminare.insert(char(i));
    }

    for(auto x : Eliminare)
        productii[int(x)].clear();


    for(auto x : Eliminare)
        for(int i = 0; i < 128; i++)
        if(!productii[i].empty())
        {
            set<string> p;
            for (auto y : productii[i])
                if (y.find(x) == string::npos)
                    p.insert(y);
            productii[i].clear();
            for(auto l : p)
                productii[i].insert(l);
        }
}

void adaugaNeterminaleNoi(){
    set<char> schimbate;
    for(int i = 0; i < 128; i++) //caut starile finale care apar langa alte stari
        if(!productii[i].empty())
            for(auto y : productii[i])
                if(y.size() >=2)
                    for(auto z:y)
                        for(auto t : terminale)
                            if (z == t)
                                schimbate.insert(t);
    int cod = 65;
    for(auto x : schimbate){
        while(!productii[cod].empty())
            cod++;
        string t;
        t = x;
        neterminale.insert(char(cod));
        productii[cod].insert(t);
        for(int i = 0; i < 128; i++)
            if(i != cod && !productii[i].empty()) {
                set<string> nou;
                for (auto y : productii[i])
                    if ((y.size() < 2) || (y.find(x) == string::npos))
                        nou.insert(y);
                    else{
                        string m;
                        for(auto z : y)
                            if(z!=x)
                                m+=z;
                            else
                                m+=char(cod);
                            nou.insert(m);
                    }
                    productii[i].clear();
                    for(auto n : nou)
                        productii[i].insert(n);
            }

    }
}

int numarNeterminale(string a){
    int nr = 0;
    for(auto x : neterminale)
        for(auto y : a)
            if (x == y)
                nr++;
    return nr;
}


//Pasul 5
void adaugareNeterminaleNoi2(){
    set<string> modif;
    for(int i = 0; i < 128; i++)
        if(!productii[i].empty())
            for(auto x : productii[i])
                if(numarNeterminale(x) > 2 )
                    modif.insert(x);
    int cod = 65;
    for(auto sir : modif){
        while(!productii[cod].empty())
            cod++;
        neterminale.insert(cod);
        string nou;
        string b;
        b = char(cod);
        nou = sir[0] + b;
        string vechi;
        vechi = sir[1];
        for(int i = 2; i < sir.size(); i++)
            vechi+=sir[i];
        productii[cod].insert(vechi);
        for(int i = 0; i < 128; i++)
            if(!productii[i].empty())
            {  set<string> q;
                for(auto x : productii[i])
                {
                    if(x == sir)
                        q.insert(nou);
                    else
                        q.insert(x);
                }
                productii[i].clear();
                for(auto x : q)
                    productii[i].insert(x);
            }

    }
}

int main() {
    citire();
    afisare();
    cout << "Dupa eliminare productii lambda: \n";
    eliminareProductiiLambda();
    afisare();
    cout << "Dupa eliminarea redenumirilor: \n";
    eliminareaRedenumirilor();
    afisare();
    eliminareProductiiInutile();
    cout << "Dupa eliminarea productiilor inutile:\n";
    afisare();
    adaugaNeterminaleNoi();
    cout << "Dupa adaugarea terminalelor noi:\n";
    afisare();
    adaugareNeterminaleNoi2();
    cout << "Part2:\n";
    afisare();
    return 0;
}