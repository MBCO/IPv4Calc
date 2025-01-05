#include <iostream>
#include <bits/stdc++.h>
#include <regex>
#include <string>
#include <bitset>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


using namespace std;

bool isValidMaskOctect(const string& maskOctect)
{
    regex maskPattern("(^(255|254|252|248|240|224|192|128|0)$)");
    if (regex_match(maskOctect, maskPattern))
    {
        return true;
    }
    return false;
}

bool isValidIPv4(const string& ip, smatch& returnValue) {
    regex ipv4Pattern("^([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})$");

    if (regex_match(ip, returnValue, ipv4Pattern)) {
        for (int i = 1; i <= 4; ++i) {
            int num = stoi(returnValue[i]);
            if (num < 0 || num > 255) return false; // Controleer of elk octet binnen bereik is
        }
        return true;
    }
    return false;
}

bool isValidSubnetMask(const string& mask, smatch& returnValue)
{
    if (!isValidIPv4(mask, returnValue))
    {
        return false;
    }
    if (returnValue[1]=="0")
    {
        return false;

    }
    for (int i = 1; i<=4; ++i)
    {
        bool lastmask=false;
        if (!isValidMaskOctect(returnValue[i].str()))
        {
            return false;
        }
        if (lastmask && (returnValue[i]!="0"))
        {
            return false;
        }
        if (returnValue[i]!="255")
        {
            lastmask=true;
        }

    }
    return true;
}

int calculateMaskBitvalue(int mask) {
    return bitset<8>(mask).count();
}

int CalcIPv4BinairMask(smatch returnValue)
{
    int bitMask=0;

    for (int i=1; i<=4; ++i)
    {
        bitMask=bitMask+calculateMaskBitvalue(stoi(returnValue[i]));
    }
    return bitMask;
}

string calculateMaskDecvalue(int& IPv4BitMask)
{
    if (IPv4BitMask>8)
        {
            IPv4BitMask = IPv4BitMask -8;
            return"255";
        }
    if (IPv4BitMask<8 && IPv4BitMask >0)
        {
            switch (IPv4BitMask)
            {
            case 1: {IPv4BitMask=0; return "128"; break;}
            case 2: {IPv4BitMask=0; return "192"; break;}
            case 3: {IPv4BitMask=0; return "224";  break;}
            case 4: {IPv4BitMask=0; return "240"; break;}
            case 5: {IPv4BitMask=0; return "248"; break;}
            case 6: {IPv4BitMask=0; return "252"; break;}
            case 7: {IPv4BitMask=0; return "254"; break;}
            default: {IPv4BitMask=0; return "255"; break;}
            }
        }
    return "0";
}

int CalcNewMaskSubnets(int NumberOf, int IPv4BitMask, string& IPv4Maskstr, bool SubOrHost)
{

    double result = log(NumberOf) / log(2);
    if (SubOrHost) //true ist about subnets
    {
        IPv4BitMask = IPv4BitMask + (int)ceil(result);
    }
    else //false its about hotst
    {
        IPv4BitMask = 32 - (int)ceil(result);
    }

    if (IPv4BitMask > 32) {return 0;}

    int BitMasktoDecMask = IPv4BitMask;
    IPv4Maskstr="";
    for (int i=1; i<=4; ++i)
    {
        if (i==1)
        {
            IPv4Maskstr=calculateMaskDecvalue(BitMasktoDecMask);
        }
        else
        {
            IPv4Maskstr=IPv4Maskstr + "."+ calculateMaskDecvalue(BitMasktoDecMask);
        }


    }
    return IPv4BitMask;
}


int main(int argc, char* argv[])
{

    if (argc<2)
    {
        cout << "usage"<<endl;
        cout << "IPv4Calc -ip IPADRESS -m MASK"<<endl;
        return 1;
    }
    string IPv4;
    string IPv4Maskstr;
    smatch IPv4Address;
    smatch IPv4Mask;
    int NumberOfSubnets =0;
    int IPv4BitMask = 0;
    int NumberOfHosts = 0;

    for (int i=1; i< argc; ++i)
    {
        string arg=argv[i];
        if (arg == "-h")
        {
            cout << "usage"<<endl;
            cout << "IPv4Calc -ip IPADRESS -m MASK"<<endl;
            return 1;
        }
        if (arg=="-ip")
        {
            IPv4=argv[i+1];
            if (isValidIPv4(IPv4, IPv4Address))
            {
                //cout << IPv4 << " is a valid IPv4 address." << endl;
            }
            else
            {
                cout << "No valid ip address" << endl;
                return 1;
            }
        }
        if (arg=="-m")
        {
            IPv4Maskstr=argv[i+1];
            if (isValidSubnetMask(IPv4Maskstr, IPv4Mask))
            {
                IPv4BitMask= CalcIPv4BinairMask(IPv4Mask);
                //cout << IPv4Maskstr << "(/" << IPv4BitMask << ") is a valid IPv4 mask." << endl;

            }
            else
            {
                cout << "no valid mask" << endl;
                return 1;
            }
        }
        if (arg == "-subnets")
        {
            try
            {
                NumberOfSubnets = stoi(argv[i+1]);

            } catch (const std::invalid_argument& e) {
                cerr << "Error: invalid value-" << std::endl;
                return 1;
            } catch (const std::out_of_range& e) {
                std::cerr << "Error: out of range" << std::endl;
                return 1;
            }

             NumberOfSubnets=CalcNewMaskSubnets(NumberOfSubnets, IPv4BitMask, IPv4Maskstr, true);
             if (NumberOfSubnets <= 0)
             {
                 cout << "to many subnets" << endl;
                 return 1;
             } else
             {
                 cout <<endl;
                 cout << "new mask is " << IPv4Maskstr<< "(/" << NumberOfSubnets << ")"<<endl;;
             }
        }
        if (arg == "-hosts")
        {
            try
            {
                NumberOfHosts = stoi(argv[i+1]);

            } catch (const std::invalid_argument& e) {
                cerr << "Error: invalid value-" << std::endl;
                return 1;
            } catch (const std::out_of_range& e) {
                std::cerr << "Error: out of range" << std::endl;
                return 1;
            }
            NumberOfSubnets=CalcNewMaskSubnets(NumberOfSubnets, IPv4BitMask, IPv4Maskstr, false);
            if (NumberOfSubnets <= 0)
            {
                cout << "to many subnets" << endl;
                return 1;
            } else
            {
                cout <<endl;
                cout << "new mask is " << IPv4Maskstr<< "(/" << NumberOfSubnets << ")"<<endl;;
            }
        }

    }

    return 0;
}
