#include <iostream>
#include <typeinfo>

using namespace std;

class Card
{
    string nrCard, NumeDetinator, data_expirare;
    int CVV;
    double credit;
public:
    ///functie virtuala pura
    virtual void const f()=0;
    ///constructori initializare
    Card(){nrCard=NumeDetinator=data_expirare=""; CVV=credit=0;}
    Card(string, string, string, int, double);
    ///constructor copiere
    Card(const Card&);
    ///operator =
    Card& operator=(const Card&);
    ///destructor
    virtual ~Card(){}
    ///functie citire
    virtual void citire(istream& in){in>>nrCard>>NumeDetinator>>data_expirare>>CVV>>credit;}
    ///functie afisare
    virtual void afis(){cout<<nrCard<<" "<<NumeDetinator<<" "<<data_expirare<<" "<<CVV<<" "<<credit<<endl;}
    ///operator >> (cu functie membra de citire virtuala)
    friend istream& operator>>(istream&, Card&);
    ///operator << (fara functie membra de afisare virtuala)
    friend ostream& operator<<(ostream&, Card&);
    ///set si get
    string get_nr(){return nrCard;}
    double get_credit(){return credit;}
    void set_credit(double s){credit=s;}
    void set_credit1(double suma){credit+=suma;}
    ///functie extragere bani
    virtual void extrage(double);

    virtual double transfer(double suma){extrage(suma); return suma;}
};

Card::Card(string nr, string nd, string de, int c, double cr)
{
    nrCard=nr;
    NumeDetinator=nd;
    data_expirare=de;
    CVV=c;
    credit=cr;
}

Card::Card(const Card& c)
{
    nrCard=c.nrCard;
    NumeDetinator=c.NumeDetinator;
    data_expirare=c.data_expirare;
    CVV=c.CVV;
    credit=c.credit;
}

Card& Card::operator=(const Card& c)
{
    if(this!=&c)
    {
        nrCard=c.nrCard;
        NumeDetinator=c.NumeDetinator;
        data_expirare=c.data_expirare;
        CVV=c.CVV;
        credit=c.credit;
    }
    return *this;
}


ostream& operator<<(ostream& out, Card& c)
{
    out<<c.nrCard<<" "<<c.NumeDetinator<<" "<<c.data_expirare<<" "<<c.CVV<<" "<<c.credit<<endl;
    return out;
}

void Card::extrage(double suma)
{
    if(suma>credit)
    {
        cout<<"Fonduri insuficiente\n";
        throw(0);
    }
}


class Card_standard: public Card
{
    int limitaExtragere;
    static double comisionDepasireLimita;
public:
    void const f(){cout<<"Card standard\n";}
    ///constructori initializare
    Card_standard(): Card() {}
    Card_standard(string, string, string, int, double, int);
    ///constructor de copiere
    Card_standard(const Card_standard&);
    ///operator =
    Card_standard& operator=(const Card_standard&);
    ///destructor
    ~Card_standard(){}
    ///functie citire
    void citire(istream& in){Card::citire(in); in>>limitaExtragere;}
    ///functie afisare
    void afis(){cout<<"Tip card: standard\n"; Card::afis(); cout<<limitaExtragere<<" "<<comisionDepasireLimita<<endl;}
    ///operator <<
    friend ostream& operator<<(ostream&, Card_standard&);
    ///functie extragere bani
    void extrage(double suma);
    ///functie statica
    static void schimba_comision(double x){comisionDepasireLimita=x; cout<<"Comision modificat la "<<" "<<x<<endl;}

    double transfer(double suma){Card_standard::extrage(suma); return suma;}
};

Card_standard::Card_standard(string nr, string nd, string de, int c, double cr, int le):Card(nr, nd, de, c, cr)
{
    limitaExtragere=le;
}

Card_standard::Card_standard(const Card_standard& cs):Card(cs), limitaExtragere(3000)
{
    limitaExtragere=cs.limitaExtragere;
    comisionDepasireLimita=cs.comisionDepasireLimita;
}

Card_standard& Card_standard::operator=(const Card_standard& cs)
{
    if(this!=&cs)
    {
        this->Card::operator=(cs);
        limitaExtragere=cs.comisionDepasireLimita;
        comisionDepasireLimita=cs.comisionDepasireLimita;
    }
    return *this;
}

ostream& operator<<(ostream& out, Card_standard& cs)
{
    Card *c=&cs;
    out<<*c<<" ";
    out<<cs.limitaExtragere<<"\n"<<"Comision: "<<cs.comisionDepasireLimita<<endl;
}

void Card_standard::extrage(double suma)
{
    try
    {
        Card::extrage(suma);
    }
    catch(int x)
    {
        return;
    }

    if(suma>limitaExtragere)
    {
        double x=get_credit()-suma-(limitaExtragere-suma)*comisionDepasireLimita;
        set_credit(x);
    }
    else
    {
        double x=get_credit()-suma;
        set_credit(x);
    }
    if(typeid(*this)==typeid(Card_standard))
        cout<<"Suma "<<suma<<" extrasa. Credit curent: "<<this->get_credit()<<endl;;

}

class Card_premium: public Card_standard
{
    double cashback;
public:
    void const f(){cout<<"Card premium\n";}
    ///constructori initializare
    Card_premium() {}
    Card_premium(string, string, string, int, double, int, double);
    ///constructor de copiere
    Card_premium(const Card_premium&);
    ///operator =
    Card_premium& operator=(const Card_premium&);
    ///destructor
    ~Card_premium(){}
    ///functie citire
    void citire(istream& in){Card_standard::citire(in); in>>cashback;}
    ///functie afis()
    void afis(){cout<<"Tip card: premium\n"; Card::afis(); cout<<cashback;}
    ///operator <<
    friend ostream& operator<<(ostream&, Card_premium&);
    ///functie extragere
    void extrage(double);

    double transfer(double suma){Card_premium::extrage(suma); return suma;}

};
Card_premium::Card_premium(string nr, string nd, string de, int c, double cr, int le, double cb):
    Card_standard(nr, nd, de, c, cr, le), cashback(cb) {}

Card_premium::Card_premium(const Card_premium& cp):Card_standard(cp)
{
    cashback=cp.cashback;
}

Card_premium& Card_premium::operator=(const Card_premium& cp)
{
    if(this!=&cp)
    {
        this->Card_standard::operator=(cp);
        cashback=cp.cashback;

    }
    return *this;
}

ostream& operator<<(ostream& out, Card_premium& cp)
{
    Card_standard *cs=&cp;
    out<<*cs;
    out<<cp.cashback<<endl;
}

void Card_premium::extrage(double suma)
{
    try
    {
        Card::extrage(suma);
    }
    catch(int x)
    {
        return;
    }

    double x=get_credit()-suma+cashback/100*suma;
    set_credit(x);
    cout<<"Suma extrasa. Credit curent: "<<this->get_credit()<<endl;
}

///operatorul de citire
istream& operator>>(istream& in, Card& c)
{
    c.citire(in);
    return in;
}

///variabila statica
double Card_standard::comisionDepasireLimita=0.1;

int main()
{
    unsigned short int cerinta;
    cout<<"1.Deschidere card\n2.Folosire mai multe carduri\n3.Transformare tip card\n";
    cin>>cerinta;
    if(cerinta==1)
    {
        cout<<"Deschidere card:\nAlegeti tipul de card:\n";
        cout<<"1.Card standard\n2.Card premium\n";
        unsigned short int x;
        cin>>x;
        if(x==1)
        {
            cout<<"Introduceti date card standard: numar, nume detinator, data expirare, CVV, credit, limita extragere\n";
            string nr, nume, data;
            int cv, l;
            double cr;
            cin>>nr>>nume>>data>>cv>>cr>>l;
            Card_standard cs(nr, nume, data, cv, cr, l);
            cout<<cs;
            cout<<"1.Interogare sold\n2.Extragere numerar\n3.Modificare comision\n4.Oprire\n";
            short int y;
            cin>>y;
            switch(y)
            {
            case 1:
                cout<<"Credit: "<<cs.get_credit()<<endl;
                break;
            case 2:
                int suma;
                cout<<"Introduceti suma de extras: \n";
                cin>>suma;
                cs.extrage(suma);
                break;
            case 3:
                cout<<"Valoarea noului comision:\n";
                double d;
                cin>>d;
                cs.schimba_comision(d);
                break;
            case 4:
                break;
            }
        }

        if(x==2)
        {
            cout<<"Introduceti date card premium: numar, nume, data expirare, CVV, credit, limita extragere si cashback\n";
            Card_premium cp;
            cin>>cp;
            cout<<cp;
            cout<<"1.Interogare sold\n2.Extragere numerar\n3.Opririe\n";
            unsigned short int y;
            cin>>y;
            switch(y)
            {
            case 1:
                cout<<"Credit: "<<cp.get_credit()<<endl;
                break;
            case 2:
                int suma;
                cout<<"Introduceti suma de extras:\n";
                cin>>suma;
                cp.extrage(suma);
                break;
            case 3:
                cout<<"Valoarea noului comision:\n";
                double d;
                cin>>d;
                cp.schimba_comision(d);
                break;
            case 4:
                break;
            }
        }
    }

    if(cerinta==2)
    {
        bool ok=0;
        int n;
        do
        {
            ok=1;
            cout<<"Introduceti numarul de carduri (max 100):\n";
            cin>>n;
        try
        {
            if(n>100)
            {
                throw(0);
            }
        }

            catch(int x)
            {
                if(x==0)
                {
                    cout<<"Numarul de carduri este prea mare\nIntroduceti un nou numar\n";
                    ok=0;
                }

            }
        }while(ok==0);


        Card *v[101];
        for(int i=0; i<n; i++)
        {
            cout<<"Tip card (standard sau premium): \n";
            string s;
            cin>>s;
            if(s=="Standard" || s=="standard")
            {
                Card_standard cs1;
                cout<<"Introduceti date card standard: numar, nume detinator, data expirare, CVV, credit, limita extragere\n";
                cin>>cs1;
                v[i]=new Card_standard(cs1); //upcasting
            }

            else if(s=="premium" || s=="Premium")
            {
                Card_premium cp1;
                cout<<"Introduceti date card premium: numar, nume, data expirare, CVV, credit, limita extragere, cashback\n";
                cin>>cp1;
                v[i]=new Card_premium(cp1); //upcasting
            }

            else
            {
                cout<<"Tip de card invalid\n";
                return 0;
            }

           try
            {
                for(int j=0; j<i; j++)
                {
                    if(v[j]->get_nr()==v[i]->get_nr())
                    {
                        if((typeid(*v[i])==typeid(Card_standard)))
                            throw(1);
                        else
                            throw(2);
                    }

                }
            }
            catch(int x)
            {
                if(x==1)
                    cout<<"\nNu se poate deschide un card standard cu acelasi numar cu un alt card\n";
                else
                    cout<<"\nNu se poate deschide un card premium cu acelasi numar cu un alt card\n";
                return 0;

            }

        }

        for(int i=0; i<n; i++)
            v[i]->afis();

        cout<<"Alege un card. Indicele cardului:\n";
        int k;
        try
        {
            cin>>k;
            if(k>n-1)
                throw(k);
        }
        catch(int x)
        {
           cout<<k<<" nu este un indice valid\n";
            return 0;
        }
        cout<<"1.Extrage numerar\n2.Transfer bancar\n3.Interogare sold\n";
        unsigned short int x;
        cin>>x;
        switch(x)
        {
        case 1:
            {
                cout<<*v[k]<<endl<<"Introduceti suma\n";
                double suma;
                cin>>suma;
                v[k]->extrage(suma);
                break;
            }
        case 2:
            cout<<"Introduceti suma de transferat si destinatarul(indice card):\n";
            double suma;
            unsigned short int z;
            try
            {
                cin>>suma>>z;
                if(z>n)
                    throw(n);
            }
            catch(int x)
            {
                cout<<x<<" nu este un indice valid\n";
                return 0;
            }
            if(suma>v[k]->get_credit())
                    suma=0;
            v[z]->set_credit1(v[k]->transfer(suma));
            cout<<"Destinatarul:\n";
            cout<<*v[z]<<endl;
            break;

        case 3:
            cout<<"Destinatar:\n"<<v[k]->get_credit()<<endl;
            break;
        }
     }
    if(cerinta==3)
    {
        cout<<"1.Transformare card strandard in card premium (downcasting)\n2.Transformare card premium in card standard (upcasting)\n";
        unsigned short int z;
        cin>>z;
        if(z==1)
        {

            Card_standard *pcs=new Card_premium("124", "Ionescu", "09/2023", 456, 2000.50, 3000, 10);
            //downcasting
            Card_premium *pcp=dynamic_cast<Card_premium*>(pcs);
            cout<<"Noul card:\n";
            pcp->f();
            cout<<*pcp<<endl;
        }
        else
        {
            Card_standard *pcs=new Card_premium("124", "Ionescu", "09/2023", 456, 2000.50, 3000, 10);
            pcs->f();
            cout<<*pcs;
        }

    }

    return 0;
}
